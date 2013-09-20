/**
 * file: QuadtreeValidator.h
 * desc: Validates if a compressed quadtree is correct for a point set.
 *       Note: This is not very rigorous, it simply performs a number of sensible tests. 
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _QUADTREE_VALIDATOR_H_
#define _QUADTREE_VALIDATOR_H_

#include <vector>

#include "Vec.h"

// Forward class declarations
template<typename T, int D>
class Quadtree;

template<typename T, int D>
class QuadtreeValidator
{
public:
	/**
	 * Validates that the given compressed quadtree on the point set is correct. Used for debugging purposes.
	 */
	bool ValidateQuadtree(const Quadtree<T,D>* root, std::vector<vec<T,D>>& pointSet) const;

    /**
     * Returns true if the point is found in the quadtree.
     */
    bool FindPoint(const Quadtree<T,D>* root, const vec<T,D>* p) const;

    /**
     * Check if the pointers between nodes are correct.
     */
    bool ValidatePointers(const Quadtree<T,D>* root) const;

    /**
     * Check if a node is contained in it's parent node.
     */
    bool ValidateNodeContainment(const Quadtree<T,D>* node) const;
};

#endif //_QUADTREE_VALIDATOR_H_
