/**
 * file: Vec.cpp
 *
 * Copyright 2013 Okke Schrijvers
 */

#include "Vec.h"

template<typename T, int D>
vec<T,D>& vec<T,D>::operator+=(const T& val)
{
    for(int d=0; d<D; ++d)
    {
        el[d] += val;
    }

    return *this;
}

template<typename T, int D>
vec<T,D> vec<T,D>::operator+(const T& val) const
{
    vec<T,D> retVal(*this);
    for(int d=0; d<D; ++d)
    {
        retVal.el[d] += val;
    }
    return retVal;
}

template<typename T, int D>
vec<T,D> vec<T,D>::operator+(const vec<T,D>& val) const
{
    vec<T,D> retVal(*this);
    for(int d=0; d<D; ++d)
    {
        retVal.el[d] += val[d];
    }
    return retVal;
}

template<typename T, int D>
vec<T,D>& vec<T,D>::operator-=(const vec<T,D>& val)
{
    for(int d=0; d<D; ++d)
    {
        el[d] -= val[d];
    }
    return *this;
}

template<typename T, int D>
vec<T,D> vec<T,D>::operator-(const vec<T,D>& val) const
{
    vec<T,D> retVal(*this);
    for(int d=0; d<D; ++d)
    {
        retVal.el[d] -= val[d];
    }
    return retVal;
}

template<typename T, int D>
vec<T,D> vec<T,D>::operator/(T val) const
{
    vec<T,D> retVal(*this);
    for(int d=0; d<D; ++d)
    {
        retVal.el[d] /= val;
    }
    return retVal;
}

template<typename T, int D>
double vec<T,D>::Length() const
{
    double length=0.0;
    for(int d=0; d<D; ++d)
    {
        length += (el[d]*el[d]);
    }
    return sqrt(length);
}

template<typename T, int D>
vec<T,D> vec<T,D>::Clamped(const vec<T,D>& min, T range) const
{
    ASSERT(range >= T(0));
    vec<T,D> retVal;
    for(int d=0; d<D; ++d)
    {
        retVal[d] = std::min(std::max(el[d], min[d]), min[d]+range);
    }
    return retVal;
}

template<typename T, int D>
vec<T,D> vec<T,D>::Clamped(const vec<T,D>& min, const vec<T,D>& max) const
{
    vec<T,D> retVal;
    for(int d=0; d<D; ++d)
    {
        ASSERT(min[d]<=max[d]);
        retVal[d] = std::min(std::max(el[d], min[d]), max[d]);
        ASSERT(min[d] <= retVal[d] && retVal[d] <= max[d]);
    }
    return retVal;
}

template<typename T, int D>
void vec<T,D>::MinExtend(const vec<T,D>& v)
{
    for(int d=0; d<D; ++d)
    {
        el[d] = std::min(el[d], v[d]);
    }
}

template<typename T, int D>
void vec<T,D>::MaxExtend(const vec<T,D>& v)
{
    for(int d=0; d<D; ++d)
    {
        el[d] = std::max(el[d], v[d]);
    }
}

template<typename T, int D>
std::string vec<T,D>::ToString() const
{
    char buffer[32];
    std::string str = "(";
    for(int d=0; d<D; ++d)
    {
        sprintf_s(buffer, "%f, ", el[d]);
        str.append(buffer);
    }
    str = str.substr(0, str.length()-2);
    str.append(")");
    return str;
}

template class vec<double,2>;