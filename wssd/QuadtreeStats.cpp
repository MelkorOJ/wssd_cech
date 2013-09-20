/**
 * file: QuadtreeStats.cpp
 *
 * Copyright 2013 Okke Schrijvers
 */

#include "Quadtree.h"

#include "QuadtreeStats.h"

template<typename T,int D>
void QuadtreeStats<T,D>::PrintStats(const Quadtree<T,D>* root) const
{
    // The stats we collect.
    int internalNodes = 0;
    int leafNodes = 0;
    int totalChildren = 0;
    int depth = 1;
    int maxDepth = 0;
    T   minSideLength = T(1<<30);

    // Retrieve the stats
    GetNodeStats(root, internalNodes, leafNodes, totalChildren);
    GetDepthStats(root, depth, maxDepth, minSideLength);

    // Print them
    printf("\n\nQuadtree stats:\n\nInt nodes:    %d\nLeaf nodes:   %d\nAvg branch:   %f\nDepth:        %d\nMin side len: %f\n\n",
        internalNodes, leafNodes, double(totalChildren)/internalNodes, maxDepth, minSideLength);
}

template<typename T,int D>
void QuadtreeStats<T,D>::GetNodeStats(const Quadtree<T,D>* node, int& internalNodes, int& leafNodes, int& totalChildren) const
{
    if(node->IsLeaf())
    {
        // We're a leaf node, inc counter
        ++leafNodes;
    }
    else
    {
        // We're an internal nod, inc counters
        ++internalNodes;
        totalChildren += node->GetNumChildren();

        // Recurse on children
        for(Quadtree<T,D>::ConstChildIterator it = node->CChildBegin(); it != node->CChildEnd(); ++it)
        {
            GetNodeStats(*it, internalNodes, leafNodes, totalChildren);
        }
    }
}

template<typename T,int D>
void QuadtreeStats<T,D>::GetDepthStats(const Quadtree<T,D>* node, int& depth, int& maxDepth, T& minSideLength) const
{
    // Maintain depth/sidelength
    minSideLength = std::min(minSideLength, node->GetSideLength());
    maxDepth = std::max(maxDepth, depth);

    // Recurse on children
    for(Quadtree<T,D>::ConstChildIterator it = node->CChildBegin(); it != node->CChildEnd(); ++it)
    {
        // Update depth
        ++depth;
        GetDepthStats(*it, depth, maxDepth, minSideLength);
        --depth;
    }
}

template class QuadtreeStats<double,2>;