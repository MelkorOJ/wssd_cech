/**
 * file: WellSeparatedTuple.h
 * desc: Represents a well-separated tuple.
 *
 *       [OS] we could change the representation to a recursive one where we store
 *            one Quadtree node and a pointer to a K-1 WST.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _WELL_SEPARATED_TUPLE_H_
#define _WELL_SEPARATED_TUPLE_H_

#include <vector>
#include <iostream>

#include "Vec.h"


// Forward class declarations
template<typename T, int D>
class Quadtree;

/**
 * Holds the k+1 elements of a (e,k)-well separated tuple.
 *
 * Pointers stored in ascending order so tuples can be stored in a set.
 */
template<typename T, int D, int K>
class WellSeparatedTuple
{
// Fields
private:
	Quadtree<T,D>*      el[K+1];
    bool                handled;
    double              maxDiam;

// Constructors
public:
	WellSeparatedTuple();

    /**
     * Constructor that should only be used for K=1. No implementation is given
     * otherwise.
     */
    WellSeparatedTuple(Quadtree<T,D>* u, Quadtree<T,D>* v);

    /**
     * Constructor to create a K-WST from a (K-1) tuple.
     */
    WellSeparatedTuple(const WellSeparatedTuple<T,D,K-1>& tuple, Quadtree<T,D>* u);

// Operators
public:
    //Quadtree<T,D>*&   operator[](int index) { return el[index]; }
    Quadtree<T,D>*    operator[](int index) const { return el[index]; }

// Functions:
public:
    /**
     * Returns f the tuple contains a particular quadtree node.
     */
    bool Contains(const Quadtree<T,D>* u) const;

    /**
     * Get the mid point of the bounding box containing all points.
     * Only works of UpdateBoundingBoxes has been called on the quadtree.
     * [OS] TODO: For ints we might have to do something else as the sphere centered
     *      at the midPoint with the diameter may not include all points (due to
     *      rounding).
     */
    void MidPointAndDiam(vec<T,D>& retMidPoint, double& retDiameter) const;

    /**
     * Compares pointer addresses in ascending order.
     */
    bool operator<(const WellSeparatedTuple<T,D,K>& r) const;

    void SetHandled() { handled = true; }
    bool IsHandled() const { return handled; }


    double GetMaxDiam() const
    { 
        //if(maxDiam < 0)
        //{
        //    std::cout << "Wrong maxDiam!!!!" << std::endl;
        //    exit(-1);
        //}
        return maxDiam;
    }

private:
    void SetMaxDiam()
    {
        maxDiam = 0;
        for(int i=0; i < K+1; ++i)
        {
            maxDiam = std::max(maxDiam, el[i]->GetAabb().GetDiameter());
        }
    }
};


/**
 * Define the WSSD.
 */
template<typename T, int D, int K>
struct WSSDType
{
    typedef std::vector<WellSeparatedTuple<T,D,K>> type;
};
#define KWSSD(T,D,K) typename WSSDType<T,D,K>::type
// [OS] TODO: the above is an ugly workaround. Once MSVC supports alias declarations
//      change to the following:
//template<typename T, int D, int K>
//using WSSD<T,D,K> = std::set<WellSeparatedTuple<T,D,K>>;
//      this allows you to use WSSD<T,D,K> instead of WSSD(T,D,K)

#endif //_WELL_SEPARATED_TUPLE_H_
