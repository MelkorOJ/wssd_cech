/**
 * file: Orthant.h
 * desc: Represents an orthant (generalization of quadrant) in a quadtree.
 *       This implementation can only handle 31-dimensional space.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _ORTHANT_H_
#define _ORTHANT_H_

#include "Assert.h"

/**
 * Identifies an orthant in a node. 
 */
template< int D >
struct Orthant
{
private:
	unsigned int orthant;

public:
    Orthant()
	: orthant(0)
	{
        ASSERT( D<32 );
    }

    Orthant(unsigned int orthant)
    : orthant(orthant)
    {}

    /**
     * Automatic cast to index
     */
    operator unsigned int() const { return orthant; }
	
    /**
     * Inequality operator.
     */
    bool operator!=(const Orthant<D>& rhs) const { return orthant != rhs.orthant; }

    /**
     * Prefix increment operator. Allowed to go 1 past the indexable children for
     * use with iterators.
     */
    Orthant& operator++() { ++orthant; ASSERT(orthant <= Max()); return *this; }

	/**
	 * Set the quadrant of dimension dim to high (true) or low (false).
	 */
	void Set(int dim, bool val)
	{
		orthant = (val ? orthant | (1<<dim) : orthant & ~(1<<dim) );
	}

	/**
	 * Returns the if the quadrant of dimension dim is set to high (true) or
	 * low (false)
	 */
	bool Get(int dim) const
	{
		return (orthant & (1<<dim)) != 0;
	}

    /**
     * Returns the maximal number of orthants.
     */
    static unsigned int Max()
    {
        ASSERT(D<32);
        return (1<<D);
    }
};

#endif //_ORTHANT_H_