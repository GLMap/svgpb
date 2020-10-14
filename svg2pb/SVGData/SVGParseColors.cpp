//
//  SVGParseColors.cpp
//  svg2pb
//
//  Created by Evgen Bodunov on 10/14/20.
//

#include "SVGParseColors.h"
#include "ConstContainers.h"
#include "FastHash.h"
#include "svg.pb.h"

constexpr auto cssColors = ConstMapMake(
    ConstPair<FastHash, uint32_t>("aliceblue",0xf0f8ff),
    ConstPair<FastHash, uint32_t>("antiquewhite",0xfaebd7),
    ConstPair<FastHash, uint32_t>("aqua",0x00ffff),
    ConstPair<FastHash, uint32_t>("aquamarine",0x7fffd4),
    ConstPair<FastHash, uint32_t>("azure",0xf0ffff),
    ConstPair<FastHash, uint32_t>("beige",0xf5f5dc),
    ConstPair<FastHash, uint32_t>("bisque",0xffe4c4),
    ConstPair<FastHash, uint32_t>("black",0x000000),
    ConstPair<FastHash, uint32_t>("blanchedalmond",0xffebcd),
    ConstPair<FastHash, uint32_t>("blue",0x0000ff),
    ConstPair<FastHash, uint32_t>("blueviolet",0x8a2be2),
    ConstPair<FastHash, uint32_t>("brown",0xa52a2a),
    ConstPair<FastHash, uint32_t>("burlywood",0xdeb887),
    ConstPair<FastHash, uint32_t>("cadetblue",0x5f9ea0),
    ConstPair<FastHash, uint32_t>("chartreuse",0x7fff00),
    ConstPair<FastHash, uint32_t>("chocolate",0xd2691e),
    ConstPair<FastHash, uint32_t>("coral",0xff7f50),
    ConstPair<FastHash, uint32_t>("cornflowerblue",0x6495ed),
    ConstPair<FastHash, uint32_t>("cornsilk",0xfff8dc),
    ConstPair<FastHash, uint32_t>("crimson",0xdc143c),
    ConstPair<FastHash, uint32_t>("cyan",0x00ffff),
    ConstPair<FastHash, uint32_t>("darkblue",0x00008b),
    ConstPair<FastHash, uint32_t>("darkcyan",0x008b8b),
    ConstPair<FastHash, uint32_t>("darkgoldenrod",0xb8860b),
    ConstPair<FastHash, uint32_t>("darkgray",0xa9a9a9),
    ConstPair<FastHash, uint32_t>("darkgrey",0xa9a9a9),
    ConstPair<FastHash, uint32_t>("darkgreen",0x006400),
    ConstPair<FastHash, uint32_t>("darkkhaki",0xbdb76b),
    ConstPair<FastHash, uint32_t>("darkmagenta",0x8b008b),
    ConstPair<FastHash, uint32_t>("darkolivegreen",0x556b2f),
    ConstPair<FastHash, uint32_t>("darkorange",0xff8c00),
    ConstPair<FastHash, uint32_t>("darkorchid",0x9932cc),
    ConstPair<FastHash, uint32_t>("darkred",0x8b0000),
    ConstPair<FastHash, uint32_t>("darksalmon",0xe9967a),
    ConstPair<FastHash, uint32_t>("darkseagreen",0x8fbc8f),
    ConstPair<FastHash, uint32_t>("darkslateblue",0x483d8b),
    ConstPair<FastHash, uint32_t>("darkslategray",0x2f4f4f),
    ConstPair<FastHash, uint32_t>("darkslategrey",0x2f4f4f),
    ConstPair<FastHash, uint32_t>("darkturquoise",0x00ced1),
    ConstPair<FastHash, uint32_t>("darkviolet",0x9400d3),
    ConstPair<FastHash, uint32_t>("deeppink",0xff1493),
    ConstPair<FastHash, uint32_t>("deepskyblue",0x00bfff),
    ConstPair<FastHash, uint32_t>("dimgray",0x696969),
    ConstPair<FastHash, uint32_t>("dimgrey",0x696969),
    ConstPair<FastHash, uint32_t>("dodgerblue",0x1e90ff),
    ConstPair<FastHash, uint32_t>("firebrick",0xb22222),
    ConstPair<FastHash, uint32_t>("floralwhite",0xfffaf0),
    ConstPair<FastHash, uint32_t>("forestgreen",0x228b22),
    ConstPair<FastHash, uint32_t>("fuchsia",0xff00ff),
    ConstPair<FastHash, uint32_t>("gainsboro",0xdcdcdc),
    ConstPair<FastHash, uint32_t>("ghostwhite",0xf8f8ff),
    ConstPair<FastHash, uint32_t>("gold",0xffd700),
    ConstPair<FastHash, uint32_t>("goldenrod",0xdaa520),
    ConstPair<FastHash, uint32_t>("gray",0x808080),
    ConstPair<FastHash, uint32_t>("grey",0x808080),
    ConstPair<FastHash, uint32_t>("green",0x008000),
    ConstPair<FastHash, uint32_t>("greenyellow",0xadff2f),
    ConstPair<FastHash, uint32_t>("honeydew",0xf0fff0),
    ConstPair<FastHash, uint32_t>("hotpink",0xff69b4),
    ConstPair<FastHash, uint32_t>("indianred",0xcd5c5c),
    ConstPair<FastHash, uint32_t>("indigo",0x4b0082),
    ConstPair<FastHash, uint32_t>("ivory",0xfffff0),
    ConstPair<FastHash, uint32_t>("khaki",0xf0e68c),
    ConstPair<FastHash, uint32_t>("lavender",0xe6e6fa),
    ConstPair<FastHash, uint32_t>("lavenderblush",0xfff0f5),
    ConstPair<FastHash, uint32_t>("lawngreen",0x7cfc00),
    ConstPair<FastHash, uint32_t>("lemonchiffon",0xfffacd),
    ConstPair<FastHash, uint32_t>("lightblue",0xadd8e6),
    ConstPair<FastHash, uint32_t>("lightcoral",0xf08080),
    ConstPair<FastHash, uint32_t>("lightcyan",0xe0ffff),
    ConstPair<FastHash, uint32_t>("lightgoldenrodyellow",0xfafad2),
    ConstPair<FastHash, uint32_t>("lightgray",0xd3d3d3),
    ConstPair<FastHash, uint32_t>("lightgrey",0xd3d3d3),
    ConstPair<FastHash, uint32_t>("lightgreen",0x90ee90),
    ConstPair<FastHash, uint32_t>("lightpink",0xffb6c1),
    ConstPair<FastHash, uint32_t>("lightsalmon",0xffa07a),
    ConstPair<FastHash, uint32_t>("lightseagreen",0x20b2aa),
    ConstPair<FastHash, uint32_t>("lightskyblue",0x87cefa),
    ConstPair<FastHash, uint32_t>("lightslategray",0x778899),
    ConstPair<FastHash, uint32_t>("lightslategrey",0x778899),
    ConstPair<FastHash, uint32_t>("lightsteelblue",0xb0c4de),
    ConstPair<FastHash, uint32_t>("lightyellow",0xffffe0),
    ConstPair<FastHash, uint32_t>("lime",0x00ff00),
    ConstPair<FastHash, uint32_t>("limegreen",0x32cd32),
    ConstPair<FastHash, uint32_t>("linen",0xfaf0e6),
    ConstPair<FastHash, uint32_t>("magenta",0xff00ff),
    ConstPair<FastHash, uint32_t>("maroon",0x800000),
    ConstPair<FastHash, uint32_t>("mediumaquamarine",0x66cdaa),
    ConstPair<FastHash, uint32_t>("mediumblue",0x0000cd),
    ConstPair<FastHash, uint32_t>("mediumorchid",0xba55d3),
    ConstPair<FastHash, uint32_t>("mediumpurple",0x9370db),
    ConstPair<FastHash, uint32_t>("mediumseagreen",0x3cb371),
    ConstPair<FastHash, uint32_t>("mediumslateblue",0x7b68ee),
    ConstPair<FastHash, uint32_t>("mediumspringgreen",0x00fa9a),
    ConstPair<FastHash, uint32_t>("mediumturquoise",0x48d1cc),
    ConstPair<FastHash, uint32_t>("mediumvioletred",0xc71585),
    ConstPair<FastHash, uint32_t>("midnightblue",0x191970),
    ConstPair<FastHash, uint32_t>("mintcream",0xf5fffa),
    ConstPair<FastHash, uint32_t>("mistyrose",0xffe4e1),
    ConstPair<FastHash, uint32_t>("moccasin",0xffe4b5),
    ConstPair<FastHash, uint32_t>("navajowhite",0xffdead),
    ConstPair<FastHash, uint32_t>("navy",0x000080),
    ConstPair<FastHash, uint32_t>("oldlace",0xfdf5e6),
    ConstPair<FastHash, uint32_t>("olive",0x808000),
    ConstPair<FastHash, uint32_t>("olivedrab",0x6b8e23),
    ConstPair<FastHash, uint32_t>("orange",0xffa500),
    ConstPair<FastHash, uint32_t>("orangered",0xff4500),
    ConstPair<FastHash, uint32_t>("orchid",0xda70d6),
    ConstPair<FastHash, uint32_t>("palegoldenrod",0xeee8aa),
    ConstPair<FastHash, uint32_t>("palegreen",0x98fb98),
    ConstPair<FastHash, uint32_t>("paleturquoise",0xafeeee),
    ConstPair<FastHash, uint32_t>("palevioletred",0xdb7093),
    ConstPair<FastHash, uint32_t>("papayawhip",0xffefd5),
    ConstPair<FastHash, uint32_t>("peachpuff",0xffdab9),
    ConstPair<FastHash, uint32_t>("peru",0xcd853f),
    ConstPair<FastHash, uint32_t>("pink",0xffc0cb),
    ConstPair<FastHash, uint32_t>("plum",0xdda0dd),
    ConstPair<FastHash, uint32_t>("powderblue",0xb0e0e6),
    ConstPair<FastHash, uint32_t>("purple",0x800080),
    ConstPair<FastHash, uint32_t>("red",0xff0000),
    ConstPair<FastHash, uint32_t>("rosybrown",0xbc8f8f),
    ConstPair<FastHash, uint32_t>("royalblue",0x4169e1),
    ConstPair<FastHash, uint32_t>("saddlebrown",0x8b4513),
    ConstPair<FastHash, uint32_t>("salmon",0xfa8072),
    ConstPair<FastHash, uint32_t>("sandybrown",0xf4a460),
    ConstPair<FastHash, uint32_t>("seagreen",0x2e8b57),
    ConstPair<FastHash, uint32_t>("seashell",0xfff5ee),
    ConstPair<FastHash, uint32_t>("sienna",0xa0522d),
    ConstPair<FastHash, uint32_t>("silver",0xc0c0c0),
    ConstPair<FastHash, uint32_t>("skyblue",0x87ceeb),
    ConstPair<FastHash, uint32_t>("slateblue",0x6a5acd),
    ConstPair<FastHash, uint32_t>("slategray",0x708090),
    ConstPair<FastHash, uint32_t>("slategrey",0x708090),
    ConstPair<FastHash, uint32_t>("snow",0xfffafa),
    ConstPair<FastHash, uint32_t>("springgreen",0x00ff7f),
    ConstPair<FastHash, uint32_t>("steelblue",0x4682b4),
    ConstPair<FastHash, uint32_t>("tan",0xd2b48c),
    ConstPair<FastHash, uint32_t>("teal",0x008080),
    ConstPair<FastHash, uint32_t>("thistle",0xd8bfd8),
    ConstPair<FastHash, uint32_t>("tomato",0xff6347),
    ConstPair<FastHash, uint32_t>("turquoise",0x40e0d0),
    ConstPair<FastHash, uint32_t>("violet",0xee82ee),
    ConstPair<FastHash, uint32_t>("wheat",0xf5deb3),
    ConstPair<FastHash, uint32_t>("white",0xffffff),
    ConstPair<FastHash, uint32_t>("whitesmoke",0xf5f5f5),
    ConstPair<FastHash, uint32_t>("yellow",0xffff00),
    ConstPair<FastHash, uint32_t>("yellowgreen",0x9acd3)
);

static ProtoColor ProtoColorMake(uint8_t r, uint8_t g, uint8_t b) {
    ProtoColor color;
    color.set_r(r);
    color.set_g(g);
    color.set_b(b);
    return color;
}

static bool isCSSColor(const char *str, size_t length, ProtoColor &result)
{
    char *tmp = (char *)alloca(length);
    for (size_t i=0; i<length; ++i)
    {
        char ch = str[i];
        if(ch>='a' && ch<='z')
            tmp[i] = ch;
        else if(ch>='A' && ch<='Z')
            tmp[i] = ch + 'a' - 'A';
        else
            return false;
    }
    FastHash hashLow(tmp, length);
    auto it = cssColors.find(hashLow);
    if(it != cssColors.end())
    {
        auto bgr = (uint8_t *)&it->value;
        result = ProtoColorMake(bgr[2], bgr[1], bgr[0]);
        return true;
    }
    return false;
}

static bool isNumberColor(const char *str, size_t length, ProtoColor &result)
{
    if(str[0]!='#')
        return false;
    const char *input = str+1;
    char *parsed;
    if(length==4)
    {
        long num = strtol(input, &parsed, 16);
        if((parsed-input)==3)
        {
            uint8_t b = ( num & 0xf) * 0x11;
            uint8_t g = ( (num >> 4) & 0xf ) * 0x11;
            uint8_t r = ( (num >> 8) & 0xf ) * 0x11;
            result = ProtoColorMake(r, g, b);
            return true;
        }
    }else if(length==7)
    {
        auto tmp =(uint32_t)strtoll(input, &parsed, 16);
        if((parsed-input)==6)
        {
            auto bgr = (uint8_t *)&tmp;
            result = ProtoColorMake(bgr[2], bgr[1], bgr[0]);
            return true;
        }
        return false;
    }else if(length==9)
    {
        auto tmp =(uint32_t)strtoll(input, &parsed, 16);
        if((parsed-input)==8)
        {
            auto bgr = (uint8_t *)&tmp;
            result = ProtoColorMake(bgr[3], bgr[2], bgr[1]);
            printf("Warning. We don't support alpha in colors!");
            return true;
        }
    }
    return false;
}


bool parseColorString(ProtoColor *result, const char *str) {
    if(!str || !result)
        return false;

    auto len = strlen(str);
    
    if(isNumberColor(str, len, *result))
        return true;

    return isCSSColor(str, len, *result);
}
