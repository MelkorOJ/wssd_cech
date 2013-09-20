/**
 * file: FiltrationValidator.h
 * desc: Validates if the filtration is correct.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _FILTRATION_VALIDATOR_H_
#define _FILTRATION_VALIDATOR_H_

#include "Filtration.h"

template<typename T, int D>
class FiltrationValidator
{
public:
	bool ValidateFiltration(const Filtration<T,D>& filtration) const;


private:
    
	/**
	 * Test if the pointers between parents and children are all correct.
	 */
    bool TestPointers(const Filtration<T,D>& filtration) const;
};

#endif //_FILTRATION_VALIDATOR_H_
