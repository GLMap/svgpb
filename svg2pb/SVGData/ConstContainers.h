//
//  ConstContainers.h
//  GLMap
//
//  Created by Arkadiy Tolkun on 8/21/20.
//  Copyright © 2020 Evgen Bodunov. All rights reserved.
//

#ifndef ConstContainers_h
#define ConstContainers_h

#include <initializer_list>
#include "FastHash.h"

template<int Size, class Type>
class ConstArray
{
public:
    Type _vals[Size];

    explicit constexpr ConstArray(const std::initializer_list<Type> &list) noexcept
    {
        for(int i=0; i<Size; ++i)
            _vals[i] = *(list.begin()+i);
    }

    template<int Size1, int Size2>
    constexpr ConstArray(const ConstArray<Size1, Type> &array1, const ConstArray<Size2, Type> &array2) noexcept
    {
        static_assert(Size == Size1+Size2, "Wrong size of array");
        for(int i=0; i<Size1; ++i)
            _vals[i] = array1._vals[i];
        for(int i=0; i<Size2; ++i)
            _vals[i+Size1] = array2._vals[i];
    }

    template<class OtherType>
    explicit constexpr ConstArray(const ConstArray<Size, OtherType> &otherArray) noexcept
    {
        for(int i=0; i<Size; ++i)
            _vals[i] = Type(otherArray._vals[i]);
    }

    constexpr int size() const { return Size; }
    constexpr const Type *begin() const { return _vals; }
    constexpr const Type *end() const { return _vals + Size; }
    constexpr const Type operator[] (int index) const { return _vals[index]; }

    template<int OtherSize>
    constexpr ConstArray<Size+OtherSize, Type> operator + (const ConstArray<OtherSize, Type> &other)const
    {
        return ConstArray<Size+OtherSize, Type>(*this, other);
    }
};

template<class T>
constexpr void const_swap(T& l, T& r)
{
    T tmp = std::move(l);
    l = std::move(r);
    r = std::move(tmp);
}

template<int Size, class Hash = FastHash>
class ConstHashMap
{
private:
    Hash _keys[Size];
    const char * _vals[Size];

    constexpr void const_sort_impl(size_t left, size_t right)
    {
        if (left < right)
        {
            size_t m = left;
            for (size_t i = left + 1; i<right; i++)
                if (this->_keys[i]<this->_keys[left]) {
                    const_swap(this->_vals[++m], this->_vals[i]);
                    const_swap(this->_keys[m], this->_keys[i]);
                }

            const_swap(this->_vals[left], this->_vals[m]);
            const_swap(this->_keys[left], this->_keys[m]);
            const_sort_impl(left, m);
            const_sort_impl(m + 1, right);
        }
    }
public:
    constexpr ConstHashMap(const ConstArray<Size, const char *> &array) noexcept
    {
        for(int i=0; i<Size; ++i) {
            _vals[i] = *(array.begin()+i);
            _keys[i] = Hash(_vals[i]);
        }
        const_sort_impl(0, Size);
    }

    const char *valueForKey(Hash key) const
    {
        auto it = std::lower_bound(_keys, _keys + Size, key);
        if(it == _keys + Size || *it != key)
            return nullptr;
        auto index = std::distance(_keys, it);
        return _vals[index];
    }
};

template<class Key, class Value>
struct ConstPair {
    Key key;
    Value value;

    inline constexpr ConstPair() noexcept {}
    inline constexpr ConstPair(Key key_in, Value value_in) noexcept:
    key(key_in),
    value(value_in)
    {
    }

    constexpr bool operator< (Key key_in) const {
        return key < key_in;
    }
    constexpr bool operator< (ConstPair other) const {
        return key < other.key;
    }
};

template<int Size, class Type>
class SortedConstArray : public ConstArray<Size, Type>
{
private:
    constexpr void const_sort_impl(size_t left, size_t right)
    {
        if (left < right)
        {
            size_t m = left;

            for (size_t i = left + 1; i<right; i++)
                if (this->_vals[i] < this->_vals[left])
                    const_swap(this->_vals[++m], this->_vals[i]);

            const_swap(this->_vals[left], this->_vals[m]);
            const_sort_impl(left, m);
            const_sort_impl(m + 1, right);
        }
    }
public:
    constexpr SortedConstArray(const std::initializer_list<Type> &list) noexcept:
    ConstArray<Size, Type>(list)
    {
        const_sort_impl(0, Size);
    }

    constexpr SortedConstArray(const ConstArray<Size, Type> &array) noexcept:
    ConstArray<Size, Type>(array)
    {
        const_sort_impl(0, Size);
    }

    bool contains(Type o) const
    {
        return std::binary_search(this->begin(), this->end(), o);
    }
};

template<int Size, class Key, class Value>
class ConstMap : public SortedConstArray<Size, ConstPair<Key, Value>> {
public:
    constexpr ConstMap(const std::initializer_list<ConstPair<Key, Value>> &list) noexcept:
    SortedConstArray<Size, ConstPair<Key, Value>>(list){}

    const ConstPair<Key, Value>* find(Key key) const
    {
        auto begin = ConstArray<Size, ConstPair<Key, Value>>::begin();
        auto end = ConstArray<Size, ConstPair<Key, Value>>::end();
        auto it = std::lower_bound(begin, end, key);
        if(it == end || it->key != key)
            return end;
        return it;
    }
};

template<typename... T>
constexpr auto FastHashSetMake(T&&... t)
{
    return SortedConstArray<sizeof...(T), FastHash>({t...});
}

template<int Size>
constexpr auto FastHashSetMake(const ConstArray<Size, const char *> &values)
{
    return SortedConstArray<Size, FastHash>(ConstArray<Size, FastHash>(values));
}

template<int Size>
constexpr auto ConstHashMapMake(const ConstArray<Size, const char *> &values)
{
    return ConstHashMap<Size>(values);
}

template<class Type, typename... T>
constexpr auto ConstArrayMake(Type first, T&&... t)
{
    return ConstArray<sizeof...(T) + 1, Type>({first, t... });
}

template<class Key, class Value, typename... T>
constexpr auto ConstMapMake(ConstPair<Key, Value> first, T&&... t)
{
    return ConstMap<sizeof...(T) + 1, Key, Value>({first, t... });
}

#endif /* ConstContainers_h */
