/**
 * file: Filtration.h
 * desc: A simplicial complex filtration class. The order of the simplices is the order
 *       in which they are inserted into the complex.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _FILTRATION_H_
#define _FILTRATION_H_

#include <vector>
#include <list>

template<typename T, int D>
class Simplex;

template<typename T, int D>
class Filtration
{
// Fields
public:
    std::vector<Simplex<T,D>*>              simplices;

// Destructor
public:
    ~Filtration()
    {
        for(auto it = simplices.begin(); it != simplices.end(); it++)
        {
            delete *it;
        }
    }
};


#endif //_SIM_COM_FILTRATION_H_
