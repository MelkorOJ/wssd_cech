/**
 * file: Vec.h
 * desc: Class to hold D-dimensional point of type T.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _VEC_T_D_H_
#define _VEC_T_D_H_

#include <algorithm>
#include <string>
#include "Assert.h"

/**
 * Class to hold D-dimensional point of type T.
 */
template<typename T, int D> 
class vec
{
private:
	T el[D];

public:
    // Default constructor sets all elements to 0
                vec()      { memset(&el, 0, sizeof(el)); }
                vec(T val) { for(int d=0;d<D;++d) el[d]=val; }
                
    // Operator overloading
    vec<T,D>&   operator=(const vec<T,D>& rhs) { for(int d=0;d<D;++d) el[d]=rhs.el[d]; return *this; }
	T&          operator[](int index) { return el[index]; }
	const T&    operator[](int index) const { return el[index]; }

    vec<T,D>&   operator+=(const T& val);
    vec<T,D>    operator+(const T& val) const;
    vec<T,D>    operator+(const vec<T,D>& val) const;
    vec<T,D>&   operator-=(const vec<T,D>& val);
    vec<T,D>    operator-(const vec<T,D>& val) const;
    vec<T,D>    operator/(T val) const;

    // Functions
    double      Length() const;
    vec<T,D>    Clamped(const vec<T,D>& min, T range) const;
    vec<T,D>    Clamped(const vec<T,D>& min, const vec<T,D>& max) const;
    void        MinExtend(const vec<T,D>& v);
    void        MaxExtend(const vec<T,D>& v);
    std::string ToString() const;
};

#endif //_VEC_T_D_H_
