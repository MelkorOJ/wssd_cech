/**
 * file: QuadtreeConstructor.h
 * desc: Compressed quadtree constructor. Uses simple incremental construction that runs
 *       in O(n log S) where S is the spread. Can be improved to O(sort n) time by using the
 *       algorithm of [Chan 08].
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _QUADTREE_CONSTRUCTOR_H_
#define _QUADTREE_CONSTRUCTOR_H_

// Forward class declarations
template<typename T, int D>
class Quadtree;

template<typename T, int D>
class QuadtreeConstructor
{
public:
	/**
	 * Construct a quadtree from the given point set. The point set
	 * is assumed to consist of point in [0, 2^L-1)^D for some L.
	 *
	 * This implementation runs in O(n log S) where S is the spread.
     *
     * NOTE: does NOT compress the quadtree. This needs to be done manually.
	 */
	Quadtree<T,D>* ConstructQuadtree(std::vector<vec<T,D>>& pointSet) ;

	/**
	 * Compress the quadtree at root to get a linear sized tree.
	 */
	void CompressQuadtree(Quadtree<T,D>* root) const;

private:
    /**
	 * Insert a point into the *uncompressed* quadtree. Behavior is undefined if the quadtree
	 * was already compressed.
	 */
	void Insert(Quadtree<T,D>* root, vec<T,D>* point) const;

    /**
     * Prepares a point set to lie in [0, 2^L-1)^D for some L.
     */
    void PreparePointSet(std::vector<vec<T,D>>& retPointSet, T& retSideLength);
};

#endif //_QUADTREE_CONSTRUCTOR_H_
