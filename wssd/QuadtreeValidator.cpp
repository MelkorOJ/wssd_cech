/**
 * file: QuadtreeValidator.cpp
 *
 * Copyright 2013 Okke Schrijvers
 */

#include "Quadtree.h"

#include "QuadtreeValidator.h"

template<typename T, int D>
bool QuadtreeValidator<T,D>::ValidateQuadtree(const Quadtree<T,D>* root, std::vector<vec<T,D>>& pointSet) const
{
    bool valid = true;
    printf("\nChecking quadtree validity:\n");

    // Check if all points can be found
    for(std::vector<vec<T,D>>::iterator it = pointSet.begin(); it != pointSet.end(); ++it)
    {
        if(!FindPoint(root, &(*it)))
        {
            printf("Could not find point: %s\n", it->ToString().c_str());
            valid = false;
        }
    }

    // Print results
    if(valid)
    {
        printf("All points were found in the quadtree.\n");
    }
    else
    {
        printf("ERROR: Not all points were found in the quadtree.\n");
    }


    // Check pointers
    if( ValidatePointers(root) )
    {
        printf("All pointers in the quadtree are correct.\n");
    }
    else
    {
        printf("ERROR: There was a problem with the pointers between nodes in the quadtree.\n");
        valid = false;
    }

    // Check if nodes are contained in their parent.
    if( ValidateNodeContainment(root) )
    {
        printf("All quadtree boxes are contained in their parent.\n");
    }
    else
    {
        printf("ERROR: Not all quadtree boxes are contained in their parent.\n");
        valid = false;
    }

    // Print overall results
    if(valid)
    {
        printf("Quadtree is correct!\n\n");
    }
    else
    {
        printf("ERROR: Quadtree is not correct.\n\n");
    }

	return valid;
}

template<typename T, int D>
bool QuadtreeValidator<T,D>::FindPoint(const Quadtree<T,D>* root, const vec<T,D>* p) const
{
    bool retVal = true;

    // Find the leaf node that contains this point
    // This may fail (assert) if the point is not in the tree
    const Quadtree<T,D>* leaf = root->FindLeafNode(p);
    
    // Check if the leaf node's orthant contains the point
    if(!leaf->ContainsPoint(p))
    {
        printf("The point %s was not found in the reported leaf node.\n", p->ToString().c_str());
        retVal = false;
    }

    // Check if the point stored in the lead node is the same point
    if(leaf->GetPoint() != p)
    {
        printf("The leaf node contains a different point.\nSearching for: %s\nFound:         %s\n",
            p->ToString().c_str(), (leaf->GetPoint() ? leaf->GetPoint()->ToString().c_str() : "NULL"));
        retVal = false;
    }

    return retVal;
}

template<typename T, int D>
bool QuadtreeValidator<T,D>::ValidatePointers(const Quadtree<T,D>* root) const
{
    bool retVal = true;

    // Check that all present children point to root
    for(Quadtree<T,D>::ConstChildIterator it = root->CChildBegin(); it != root->CChildEnd(); ++it)
    {
        if((*it)->GetParent() != root)
        {
            printf("Child is not pointing to it's parent.\n");
            retVal = false;
        }
    }

    // Recurse
    for(Quadtree<T,D>::ConstChildIterator it = root->CChildBegin(); it != root->CChildEnd(); ++it)
    {
        retVal &= ValidatePointers(*it);
    }

    return retVal;
}

template<typename T, int D>
bool QuadtreeValidator<T,D>::ValidateNodeContainment(const Quadtree<T,D>* node) const
{
    bool retVal = true;

    // Check that this node is contained in it's parent
    if(node->GetParent())
    {
        for(int d=0; d<D; ++d)
        {
            retVal &= node->GetParent()->GetMinPoint()[d]            <= node->GetMinPoint()[d];
            retVal &= node->GetMinPoint()[d] + node->GetSideLength() <= node->GetParent()->GetMinPoint()[d] + node->GetParent()->GetSideLength();
        }
    }

    if(!retVal)
    {
        printf("Node's bounding box is not contained in it's parent.\n");
    }

    // Recurse
    for(Quadtree<T,D>::ConstChildIterator it = node->CChildBegin(); it != node->CChildEnd(); ++it)
    {
        retVal &= ValidateNodeContainment(*it);
    }

    return retVal;
}

template class QuadtreeValidator<double,2>;