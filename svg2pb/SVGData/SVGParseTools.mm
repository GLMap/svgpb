//
//  SVGParseTools.m
//  SVGViewTest
//
//  Created by destman on 8/22/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "SVGParseTools.h"
#import <map>
#import <string>

#include "Matrix3x3.h"
using namespace std;

std::vector<double> parseNumbers(const char *data, const char **lastChar)
{
    std::vector<double> rv;
    while(*data)
    {
        char *nextNumber = nullptr;
        double nextVal = strtod(data, &nextNumber);
        if(nextNumber==data || nextNumber == nullptr) break;
        rv.push_back(nextVal);
        if(*nextNumber==',')
            ++nextNumber;
        data = nextNumber;
    }
    if(lastChar)
        *lastChar = data;
    return rv;
}


bool parseNumbersFromRow(const char *data, int count, double *result, const char **lastChar)
{
    char *nextNumber = NULL;
    while (count>0) 
    {
        *result = strtod(data, &nextNumber);
        if(nextNumber==data) break;
        if(nextNumber==0) break;        
        if(*nextNumber==',')
        {
            ++nextNumber;
        }        
        
        result++;
        data = nextNumber;
        count--;
    }
    if(lastChar)
        *lastChar = data;    
    return count==0;
}

bool parseNumbers(const char *data,int expectedNumbersCount, double *result, const char **lastChar)
{
    bool haveMinNumbers = parseNumbersFromRow(data, expectedNumbersCount, result, lastChar);
    if(!haveMinNumbers)
    {
        dbgLog(@"Error in SVG: expected %d params: %s",expectedNumbersCount,data);  
        return false;
    }
    
    double temp;
    bool haveMoreNumbers = parseNumbersFromRow(*lastChar, 1, &temp, 0);
    if (haveMoreNumbers) 
    {
        dbgLog(@"Error in SVG: expected %d params: %s",expectedNumbersCount,data);  
        return false;
    }
    return true;
}

bool parseMatrixString(ProtoAffineTransformMatrix *matrix, const char *data)
{
    Matrix3x3 r = identMatrix();
    bool isError = false;
    while(*data)
    {
        Matrix3x3 m = identMatrix();
        if(strncmp("matrix(", data, 7)==0)
        {
            double vals[6];
            if(parseNumbers(data+7, 6, vals, &data))
            {
                m.m[0][0] = vals[0];
                m.m[1][0] = vals[1];
                m.m[0][1] = vals[2];
                m.m[1][1] = vals[3];
                m.m[0][2] = vals[4];
                m.m[1][2] = vals[5];
            }else
            {
                isError = true;
                break;
            }
        }else if(strncmp("translate(", data, 10)==0)
        {
            auto vals = parseNumbers(data+10, &data);
            if(vals.size() == 1)
            {
                m.m[0][2] = vals[0];
            }else if(vals.size() == 2)
            {
                m.m[0][2] = vals[0];
                m.m[1][2] = vals[1];
            }else
            {
                isError = true;
                break;
            }
        }else if(strncmp("rotate(", data, 7)==0)
        {
            auto vals = parseNumbers(data+7, &data);
            if(vals.size()==1)
            {
                m = rotateMatrix(vals[0]);
            }else if(vals.size() == 3)
            {
                m = translateMatrix(vals[1], vals[2]) * rotateMatrix(vals[0]) * translateMatrix(-vals[1], -vals[2]);
            }else
            {
                isError = true;
                break;
            }
        }else if(strncmp("scale(", data, 6)==0)
        {
            auto vals = parseNumbers(data+6, &data);
            if(vals.size() == 1)
            {
                m.m[0][0] = vals[0];
                m.m[1][1] = vals[0];
            }else if(vals.size() == 2)
            {
                m.m[0][0] = vals[0];
                m.m[1][1] = vals[1];
            }else
            {
                isError = true;
                break;
            }
        }else
        {
            isError = true;
            break;
        }

        if(*data != ')')
        {
            isError = true;
            break;
        }
        ++data;
        if(*data == ' ')
            ++data;
        r = r * m;
    }
    if(isError)
    {
        dbgLog(@"Error loading svg: Transfrom not supported %s",data);
        return false;
    }
    matrix->set_a (r.m[0][0]);
    matrix->set_b (r.m[1][0]);
    matrix->set_c (r.m[0][1]);
    matrix->set_d (r.m[1][1]);
    matrix->set_tx(r.m[0][2]);
    matrix->set_ty(r.m[1][2]);
    return true;
}

void enumElements(TBXMLElement *element,EnumElementsBlock unknownElementBlock,...)
{
    map<string,EnumElementsBlock> searchMap;
    {
        va_list vl;
        va_start(vl,unknownElementBlock);
        while (true)
        {
            const char *nextName = va_arg(vl, const char *);
            if (!nextName)
                break;
            
            EnumElementsBlock attribBlock = (EnumElementsBlock)va_arg(vl, void *);
            if (!attribBlock)
                break;
            
            searchMap.insert(std::pair<string, EnumElementsBlock>(nextName,attribBlock));
        }
        va_end(vl);
    }        
    
    while (element) 
    {
        EnumElementsBlock block = nil;
        map<string, EnumElementsBlock>::iterator it = searchMap.find(element->name);
        if(it!=searchMap.end())
        {
            block = it->second;
        }            
        if(block)
        {
            block(element);
        }else
        {
            unknownElementBlock(element);
        }
        element = element->nextSibling;
    }
}


void enumAttributes(TBXMLElement *element,bool removeOnSuccess,EnumAttributesBlock unknownAttributeBlock,...)
{
    map<string,EnumAttributesBlock> attribMap;
    {
        va_list vl;
        va_start(vl,unknownAttributeBlock);
        while (true)
        {
            const char *nextName = va_arg(vl, const char *);
            if (!nextName)
                break;
            
            EnumAttributesBlock attribBlock = (EnumAttributesBlock)va_arg(vl, void *);
            if (!attribBlock)
                break;
            
            attribMap.insert(std::pair<string, EnumAttributesBlock>(nextName,attribBlock));
        }            
        va_end(vl);
    }
    
    TBXMLAttribute *attribure = element->firstAttribute;
    TBXMLAttribute *prevAttribute = 0;
    while (attribure) 
    {
        EnumAttributesBlock block = nil;
        map<string, EnumAttributesBlock>::iterator it = attribMap.find(attribure->name);
        if(it!=attribMap.end())
        {
            block = it->second;
        }
        bool continueSearch=true;
        if(block)
        {
            continueSearch = block(attribure);
            if(removeOnSuccess)
            {
                if(prevAttribute==nil)
                {
                    element->firstAttribute = attribure->next;
                }else
                {
                    prevAttribute->next = attribure->next;
                }
            }
        }else
        {
            if(unknownAttributeBlock)
            {
                continueSearch = unknownAttributeBlock(attribure);
            }
            prevAttribute = attribure;
        }
        if(!continueSearch)
            break;
        attribure = attribure->next;
    }
}

bool parseColorString(ProtoColor *color, const char *val)
{
    int len = strlen(val);
    if(*val=='#' && len==7)
    {
        long l = strtol(val+1, nil, 16);
        unsigned char *bgr = (unsigned char *)&l;
        color->set_r(bgr[2]);
        color->set_g(bgr[1]);
        color->set_b(bgr[0]);
        return true;
    }else if(*val=='#' && len==4)
    {
        long l = strtol(val+1, nil, 16);
        color->set_r(( (l >> 8) & 0xf ) * 0x11);
        color->set_g(( (l >> 4) & 0xf ) * 0x11);
        color->set_b(( l & 0xf ) * 0x11);
        return true;
    }
    return false;
}



bool parsePaintString(ProtoSVGPaint *paint, const char *val)
{
    int len = strlen(val);
    if(parseColorString(paint->mutable_color(), val))
    {
        return true;
    }else 
    {
        paint->clear_color();
        if(len>6 && strncmp("url(#", val, 5)==0)
        {
            if(val[len-1]==')')
            {
                paint->set_ref_id(val+5, len-6);
                return true;
            }
        }else if(len==4 && strncmp("none", val, 4)==0)
        {
            paint->set_paint_off(true);
            return true;
        }
    }
    dbgLog(@"Error in svg: Invalid paint %s",val);
    return false;
}    


