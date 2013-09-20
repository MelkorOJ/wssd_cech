/**
 * file: QuadtreeStats.h
 * desc: Reports stats of a (compressed) quadtree.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _QUADTREE_STATS_H_
#define _QUADTREE_STATS_H_

// Forward class declarations
template<typename T, int D>
class Quadtree;

template<typename T, int D>
class QuadtreeStats
{
public:
	/**
	 * Prints stats to the std out.
	 */
	void PrintStats(const Quadtree<T,D>* root) const;

    /**
     * Print the total number of internal nodes, the total number of leafNodes and the total number children.
     */
    void GetNodeStats(const Quadtree<T,D>* node, int& internalNodes, int& leafNodes, int& totalChildren) const;

    /**
     * Print the depth of the quadtree and the smallest side length.
     */
    void GetDepthStats(const Quadtree<T,D>* node, int& depth, int& maxDepth, T& minSideLength) const;

};

#endif //_QUADTREE_STATS_H_
