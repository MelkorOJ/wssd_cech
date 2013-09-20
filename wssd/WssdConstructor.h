/**
 * file: WssdConstructor.h
 * desc: Well-separated simplicial decomposition constructor. Constructs
 *       the (eta,k)-WSSD from the (eta,k-1)-WSSD.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _WSSD_CONSTRUCTOR_H_
#define _WSSD_CONSTRUCTOR_H_

#include <vector>

#include "WellSeparatedTuple.h"

template< typename T, int D, int K>
class WssdConstructor
{
private:
    double eta;
    double maxMebDiameter;

public:
    WssdConstructor(double eta)
    : eta(eta)
    , maxMebDiameter(std::numeric_limits<double>::infinity())
    {}

    WssdConstructor(double eta, double maxMebDiameter)
    : eta(eta)
    , maxMebDiameter(maxMebDiameter)
    {}

public:
	/**
	 * Construct a (eta,k)-WSSD from a (eta,k-1)-WSSD. Parameter eta should be in (0,1)
	 */
	void ConstructWssd(const KWSSD(T,D,K-1)& srcWssd, Quadtree<T,D>* root, KWSSD(T,D,K)& retWssd);

private:
    /**
     * Find all the quadtree nodes that should be added to the WST.
     */
    void WssdConstructor<T,D,K>::FindNewNodes(const WellSeparatedTuple<T,D,K-1>& tuple, Quadtree<T,D>* node,
        const vec<T,D>& center, double diameter, double maxDiameter, KWSSD(T,D,K)& retWssd);

};

#endif //_WSPD_CONSTRUCTOR_H_
