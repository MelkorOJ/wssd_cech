/**
 * file: WspdConstructor.cpp
 *
 * Copyright 2013 Okke Schrijvers
 */

#include "Quadtree.h"

#include "WspdConstructor.h"

template< class T, int D >
void WspdConstructor<T,D>::ConstructWspd(Quadtree<T,D>* quadtree, KWSSD(T,D,1)& retWspd)
{
    // TODO: we need to make sure that the eta here corresponds to the eta in the wssd
    ASSERT_MSG(0.0 < eta && eta < 1.0, "Eta should be in (0,1).\n");

    quadtree->UpdateBoundingBoxes();

    for(Quadtree<T,D>::ChildIterator it = quadtree->ChildBegin(); it != quadtree->ChildEnd(); ++it)
    {
        wsPairs(quadtree, *it, retWspd);
    }
}

template< class T, int D >
void WspdConstructor<T,D>::wsPairs(Quadtree<T,D>* u, Quadtree<T,D>* v, KWSSD(T,D,1)& retWspd)
{
    //detect symmetrical calls and avoid them
    if(u->GetParent() == v->GetParent() && u->OrthantInParent() > v->OrthantInParent() )
    {
        return;
    }

    if(WellSeparated(u,v))
    {
        if(maxMebDiameter == std::numeric_limits<double>::infinity())
        {
            retWspd.push_back(WellSeparatedTuple<T,D,1>(u, v));
        }
        else
        {
            WellSeparatedTuple<T,D,1> tuple(u, v);
            vec<T,D> mid;
            double   diam;
            tuple.MidPointAndDiam(mid, diam);
            if(diam < maxMebDiameter)
            {
                retWspd.push_back(tuple);
            }
        }
    }
    else
    {
        if( (u->GetSideLength() < v->GetSideLength() || u->IsLeaf()) && !v->IsLeaf() )
        {
            std::swap(u,v);
        }

        for(Quadtree<T,D>::ChildIterator it = u->ChildBegin(); it != u->ChildEnd(); ++it)
        {
            wsPairs(*it, v, retWspd);
        }
    }
}

/**
 * Compute the distance between the 2 quadtree boxes.
 */
template< class T, int D >
bool WspdConstructor<T,D>::WellSeparated(Quadtree<T,D>* u, Quadtree<T,D>* v)
{
    if(u==v)
    {
        // [OS] is this really necessary?
        return false;
    }

    double dist = u->GetAabb().DistanceTo(v->GetAabb());
    double maxDiam = std::max(u->GetAabb().GetDiameter(), v->GetAabb().GetDiameter());

    return (maxDiam <= eta*dist);
}

template class WspdConstructor<double,2>;