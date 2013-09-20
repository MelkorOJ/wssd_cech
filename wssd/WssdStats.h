/**
 * file: WssdStats.h
 * desc: Reports stats of a WSSD. In particular print how many tuples would
 *       appear in the approximation up to a maxAlpha.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _WSSD_STATS_H_
#define _WSSD_STATS_H_

#include "WellSeparatedTuple.h"

template<typename T, int D, int K>
class WssdStats
{
public:
	/**
	 * Prints stats to the std out.
	 */
	void PrintStats(const KWSSD(T,D,K)& wspd) const;

    /**
     * Prints how many tuples have a diameter lower than a given alpha.
     */
    void PrintAlphaStats(const KWSSD(T,D,K)& wspd, int steps, double maxAlpha) const;


};

#endif //_WSSD_STATS_H_
