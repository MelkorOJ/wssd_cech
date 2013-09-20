/**
 * file: WellSeparatedTuple.cpp
 *
 * Copyright 2013 Okke Schrijvers
 */

#include "Quadtree.h"

#include "WellSeparatedTuple.h"


template<typename T, int D, int K>
WellSeparatedTuple<T,D,K>::WellSeparatedTuple()
: handled(false)
, maxDiam(-1.0)
{
    // Initialize NULL pointers.
    memset(&el, 0, sizeof(el));
}

template<typename T, int D, int K>
WellSeparatedTuple<T,D,K>::WellSeparatedTuple(Quadtree<T,D>* u, Quadtree<T,D>* v)
: handled(false)
, maxDiam(-1.0)
{
    ASSERT_MSG(K==1, "This constructor is specialized for pairs (K=1) only.\n");
    
    // Store the pointers in ascending order
    // [OS] If we're not storing the WSTs in an std::set, we don't need to store them in ascending order.
    if(u < v)
    {
        el[0] = u;
        el[1] = v;
    }
    else
    {
        el[0] = v;
        el[1] = u;
    }

    SetMaxDiam();
}

template<typename T, int D, int K>
WellSeparatedTuple<T,D,K>::WellSeparatedTuple(const WellSeparatedTuple<T,D,K-1>& tuple, Quadtree<T,D>* u)
: handled(false)
, maxDiam(-1.0)
{
    // [OS] If we're not storing the WSTs in an std::set, we don't need to store them in ascending order
    // Stores the pointers in ascending order
    int i=0;

    // Store all pointers lower than u
    while(i<K && tuple[i] < u)
    {
        el[i] = tuple[i];
        ++i;
    }

    // Store u
    el[i] = u;
    
    // Store the remaining pointers
    while(i<K)
    {
        el[i+1] = tuple[i];
        ++i;
    }

    // Make sure that all pointers are in ascending order.
    for(int i=0;i<K;++i)
    {
        // [OS] it can happen that we create degenerate tuples (in which an quadtree node appears more than once).
        //      These cannot be discarded.
        ASSERT(el[i] <= el[i+1]);
    }
    SetMaxDiam();
}

template<typename T, int D, int K>
bool WellSeparatedTuple<T,D,K>::Contains(const Quadtree<T,D>* u) const
{
    //ASSERT_MSG(false, "Using this to discard tuples leads to a non-correct WSSD.\n");
    bool retVal = false;
    for(int i=0;i<K+1;++i)
    {
        retVal |= (el[i]==u);
    }
    return retVal;
}

template<typename T, int D, int K>
void WellSeparatedTuple<T,D,K>::MidPointAndDiam(vec<T,D>& retMidPoint, double& retDiameter) const
{
    AxisAlignedBoundingBox<T,D> aabb(el[0]->GetAabb());
    
    // Generate the AABB for the tuple
    for(int i=1; i<K+1;++i)
    {   
        aabb.Extend(el[i]->GetAabb());
    }

    // Compute midpoint and diameter of AABB
    retMidPoint = aabb.GetMidPoint();
    retDiameter = aabb.GetDiameter();
}

template<typename T, int D, int K>
bool WellSeparatedTuple<T,D,K>::operator<(const WellSeparatedTuple<T,D,K>& r) const
{
    // Compare pointers in ascending order
    for(int i=0; i<K+1; ++i)
    {
        if(el[i] < r.el[i])
        {
            // This tuple's pointers are lower
            return true;
        }
        if(el[i] > r.el[i])
        {
            // This tuple's pointers are higher
            return false;
        }
        // The i^th pointers are equal: test i+1
    }

    // Tuples are equal
    return false;
}

template class WellSeparatedTuple<double,2,1>;
template class WellSeparatedTuple<double,2,2>;