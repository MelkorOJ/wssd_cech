/**
 * file: WssdValidator.h
 * desc: Validates WSSD is correct for a point set. Only works for K=2. Space and running time is O(n^(K+1)).
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _WSSD_VALIDATOR_H_
#define _WSSD_VALIDATOR_H_

#include <vector>

#include "WellSeparatedTuple.h"
#include "Vec.h"

template<typename T, int D, int K>
class WssdValidator
{
public:
	/**
	 * Validates that the given WSPD on the point set is correct. Used for debugging purposes.
	 */
	bool ValidateWssd(const KWSSD(T,D,K)& wssd, std::vector<vec<T,D>>& pointSet) const;

private:
#ifdef _WSSD_VALIDATION_
    /**
     * Test if the tuples in the realization are well-separated.
     */
    bool TestWellSeparated(const KWSSD(T,D,K)& wssd) const;

    /**
     * Test if all simplices appear in the realization.
     */
    bool TestAllTuples(const KWSSD(T,D,K)& wssd, std::vector<vec<T,D>>& pointSet) const;
#endif //_WSSD_VALIDATION_
};

#endif //_WSSD_VALIDATOR_H_
