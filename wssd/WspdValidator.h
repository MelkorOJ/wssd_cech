/**
 * file: WspdValidator.h
 * desc: Validates WSPD is correct for a point set.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _WSPD_VALIDATOR_H_
#define _WSPD_VALIDATOR_H_

#include <vector>

#include "Vec.h"
#include "WellSeparatedTuple.h"

template<typename T, int D>
class WspdValidator
{
public:
	/**
	 * Validates that the given WSPD on the point set is correct. Used for debugging purposes.
	 */
	bool ValidateWspd(const KWSSD(T,D,1)& wspd, std::vector<vec<T,D>>& pointSet) const;

private:

#ifdef _WSSD_VALIDATION_
    /**
     * Test if all the pairs in the realization are well-separated.
     */
    bool TestWellSeparated(const KWSSD(T,D,1)& wspd) const;

    /**
     * Test if all pairs in the point set appear in the realization. Takes quadratic time and space.
     */
    bool TestAllPairs(const KWSSD(T,D,1)& wspd, std::vector<vec<T,D>>& pointSet) const;
#endif //_WSSD_VALIDATION_
};

#endif //_WSPD_VALIDATOR_H_
