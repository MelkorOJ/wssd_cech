/**
 * file: WspdConstructor.h
 * desc: Well-separated pair decomposition constructor.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _WSPD_CONSTRUCTOR_H_
#define _WSPD_CONSTRUCTOR_H_

#include <limits>

#include "WellSeparatedTuple.h"

template< class T, int D >
class WspdConstructor
{
private:
    double eta;
    double maxMebDiameter;

public:
    WspdConstructor(double eta)
        : eta(eta)
        , maxMebDiameter(std::numeric_limits<double>::infinity())
    {}

    WspdConstructor(double eta, double maxMebDiameter)
        : eta(eta)
        , maxMebDiameter(maxMebDiameter)
    {}

public:
	/**
	 * Construct a eta-WSPD from a compressed quadtree. Parameter eta should be in (0,1)
	 */
	void ConstructWspd(Quadtree<T,D>* quadtree, KWSSD(T,D,1)& retWspd);

private:

    /**
     * Recursively find all well-separated pairs. Algorithm from Har-Peled
     */
    void wsPairs(Quadtree<T,D>* u, Quadtree<T,D>* v, KWSSD(T,D,1)& retWspd);

    /**
     * Test if two nodes are well-separated. Implemented as distance between bounding boxes.
     */
	bool WellSeparated(Quadtree<T,D>* u, Quadtree<T,D>* v);	
};

#endif //_WSPD_CONSTRUCTOR_H_
