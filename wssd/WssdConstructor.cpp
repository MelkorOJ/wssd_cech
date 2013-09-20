/**
 * file: WssdConstructor.cpp
 *
 * Copyright 2013 Okke Schrijvers
 */

#include "Quadtree.h"

#include "WssdConstructor.h"

template<typename T, int D, int K>
void WssdConstructor<T,D,K>::ConstructWssd(const KWSSD(T,D,K-1)& srcWssd, Quadtree<T,D>* root, KWSSD(T,D,K)& retWssd)
{
    vec<T,D> midPoint;
    double diameter, maxDiameter;
    double radiusFactor = (1.0 + 1.0/double(D))/sqrt(1.0 - 1.0/double(D*D)); // [OS] in the paper this is 2
    //double radiusFactor = 2.0;

    ASSERT_MSG(D>1, "The WSSD construction is not defined for D<2.\n");

    // Iterate over all tuples in the (K-1)realization
    for(KWSSD(T,D,K-1)::const_iterator it = srcWssd.cbegin(); it != srcWssd.cend(); ++it)
    {
        (*it).MidPointAndDiam(midPoint, diameter);
        maxDiameter = (eta/(1.0 + eta))*(diameter/2.0); // In the paper this is eta*diameter/4
        //maxDiameter = eta*(diameter/4.0); // In the paper this is eta*diameter/4

        // perform range query
        FindNewNodes((*it), root, midPoint, radiusFactor*(diameter/2.0), maxDiameter, retWssd);
    }
}

template<typename T, int D, int K>
void WssdConstructor<T,D,K>::FindNewNodes(const WellSeparatedTuple<T,D,K-1>& tuple, Quadtree<T,D>* node,
    const vec<T,D>& center, double radius, double maxDiameter, KWSSD(T,D,K)& retWssd)
{
    if(node->GetAabb().GetDiameter() > maxDiameter)
    {
        // Diameter of the node is too large, recurse on the children that are close enough
        for(Quadtree<T,D>::ChildIterator it = node->ChildBegin(); it != node->ChildEnd(); ++it)
        {
            double distance = (*it)->GetAabb().DistanceTo(center);
            if( distance < radius)
            {
                FindNewNodes(tuple, (*it), center, radius, maxDiameter, retWssd);
            }
        }
    }
    else // This is the first time the diameter is small enough, distance should be fulfilled by the other case in the function
    {
        ASSERT(node->GetAabb().DistanceTo(center) < radius);
        ASSERT(!node->GetParent() || node->GetParent()->GetAabb().GetDiameter() > maxDiameter);

        if(!tuple.Contains(node))
        {
            if(maxMebDiameter == std::numeric_limits<double>::infinity())
            {
                retWssd.push_back(WellSeparatedTuple<T,D,K>(tuple, node));
            }
            else
            {
                WellSeparatedTuple<T,D,K> tuple(tuple, node);
                // [OS] This should be MEB DIAMETER!!!!!
                //vec<T,D> mid;
                //double   diam;
                //tuple.MidPointAndDiam(mid, diam);
                //if(diam < maxMebDiameter)
                //{
                    retWssd.push_back(tuple);
//                }
            }
        }
    }
}

template class WssdConstructor<double,2,2>;