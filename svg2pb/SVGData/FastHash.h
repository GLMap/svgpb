//
//  FastHash.h
//  TapAlarmClock
//
//  Created by Arkadiy Tolkun on 02.03.12.
//  Copyright (c) 2014 Evgen Bodunov. All rights reserved.
//

#ifndef FastHash_h
#define FastHash_h

#include <stdint.h>
#include <string.h>

#include <unordered_map>
#include <algorithm>

constexpr const uint32_t StrLen(const char * vdata)
{
    uint32_t rv = 0;
    while(*vdata)
    {
        ++vdata;
        ++rv;
    }
    return rv;
}

constexpr uint32_t get16bits(const char *d)
{
    return ((uint32_t)d[1]<<8)+d[0];
}

constexpr uint32_t CalcFastHash (const char* data, size_t l)
{
    if (l <= 0 || data == NULL)
        return 0;
    
    uint32_t hash = (uint32_t)l;
    size_t rem = l & 3;
    l >>= 2;
    /* Main loop */
    for (;l > 0; l--) {
        hash  += get16bits(data);
        uint32_t tmp = (get16bits(data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        data  += 2*sizeof (uint16_t);
        hash  += hash >> 11;
    }
    
    /* Handle end cases */
    switch (rem)
    {
        case 3:
            hash += get16bits(data);
            hash ^= hash << 16;
            hash ^= ((uint32_t)data[2]) << 18;
            hash += hash >> 11;
            break;
        case 2:
            hash += get16bits (data);
            hash ^= hash << 11;
            hash += hash >> 17;
            break;
        case 1:
            hash += (signed char)*data;
            hash ^= hash << 10;
            hash += hash >> 1;
            break;
        default:
            break;
    }
    
    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    return hash;
}

class FastHash
{
private:
    uint32_t _hash;
public:
    inline constexpr FastHash() noexcept:
    _hash(0)
    {
    }
    
    inline constexpr explicit FastHash(uint32_t val) noexcept:
    _hash(val)
    {
    }

    inline explicit FastHash(intptr_t val) noexcept:
    _hash(CalcFastHash((const char *)&val, sizeof(val)))
    {
    }

    inline constexpr FastHash(const char* val) noexcept:
    _hash(val!=0 ? CalcFastHash(val, StrLen(val)) : 0)
    {
    }

    inline constexpr FastHash(const char* val, size_t len) noexcept:
    _hash(CalcFastHash(val, len))
    {
    }

    inline FastHash(float val) noexcept:
    _hash(CalcFastHash((const char *)&val, sizeof(float)))
    {
    }

    constexpr uint32_t getHash() const{return _hash;}
    
    inline constexpr bool operator<(FastHash key) const
    {
        return _hash<key._hash;
    }
    
    inline constexpr bool operator == (FastHash key) const
    {
        return _hash == key._hash;
    }
    
    inline constexpr bool operator != (FastHash key) const
    {
        return _hash != key._hash;
    }
    
    inline constexpr void operator += (const FastHash& hash){_hash += hash._hash;}
    inline constexpr void operator -= (const FastHash& hash){_hash -= hash._hash;}
    
    template<class Type>
    inline constexpr void operator += (const Type &val){_hash += CalcFastHash((const char *)&val, sizeof(Type));}
    
    template<class Type>
    inline constexpr FastHash operator + (const Type& val) const
    {
        FastHash rv(_hash);
        rv += val;
        return rv;
    }

    inline constexpr FastHash operator - (const FastHash& val) const
    {
        FastHash rv(_hash);
        rv -= val;
        return rv;
    }

    inline constexpr FastHash& operator = (FastHash key) {_hash = key._hash; return *this;}
};

namespace std {
    template <>
    struct hash<FastHash> {
    public:
        size_t operator()(const FastHash& x) const {
            return x.getHash();
        }
    };
}

#endif
