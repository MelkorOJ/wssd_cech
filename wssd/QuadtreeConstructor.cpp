/**
 * file: CompressedQuadtreeConstructor.cpp
 *
 * Copyright 2013 Okke Schrijvers
 */
#include <iostream>

#include "Assert.h"
#include "Vec.h"
#include "Quadtree.h"

#include "QuadtreeConstructor.h"

template<typename T, int D>
Quadtree<T,D>* QuadtreeConstructor<T,D>::ConstructQuadtree(std::vector<vec<T,D>>& pointSet)
{
    T sideLength(0);
    PreparePointSet(pointSet, sideLength);

    std::cout << sideLength << std::endl;

    Quadtree<T,D>* root = new Quadtree<T,D>(NULL, vec<T,D>(), sideLength);
    
    // Insert all points
    for(std::vector<vec<T,D>>::iterator it = pointSet.begin(); it != pointSet.end(); ++it)
    {
        Insert(root, &(*it));
    }

    // Note: We don't compress at this point

	return root;
};



template<typename T, int D>
void QuadtreeConstructor<T,D>::CompressQuadtree(Quadtree<T,D>* root) const
{
    int numChildren = root->GetNumChildren();
    Quadtree<T,D>* child = root;

    // Compress a chain
    if( numChildren == 1 && root->parent != NULL )
    {
        Quadtree<T,D>* parent = root->parent;
        Orthant<D> orthant = parent->GetOrthant(&(root->minPoint + (root->sideLength/T(2))));
        
        while( numChildren == 1 )
        {
            // Find the child
            for(Quadtree<T,D>::ChildIterator it = child->ChildBegin(); it != child->ChildEnd(); ++it)
            {
                child = *it;
                child->parent->children[it.GetOrthant()] = NULL;
                break;
            }

            // Delete internal node
            delete child->parent;
            child->parent = NULL;

            // Continue along the chain
            numChildren = child->GetNumChildren();
        }

        parent->children[orthant] = child;
        child->parent             = parent;
    }

    // Recurse on children
    for(Quadtree<T,D>::ChildIterator it = child->ChildBegin(); it != child->ChildEnd(); ++it)
    {
        CompressQuadtree(*it);
    }
}

template<typename T, int D>
void QuadtreeConstructor<T,D>::Insert(Quadtree<T,D>* root, vec<T,D>* point) const
{
    // If this node does not contain the point we would wrongfully store it somewhere here.
    ASSERT(root->ContainsPoint(point));

    if(root->IsLeaf())
    {
        // The current node is a leaf
        if(root->point == NULL)
        {
            root->point = point;
        }
        else
        {
            // There are 2 points in this leaf, push them down
            Orthant<D> newPointOrthant = root->GetOrthant(point);
            Orthant<D> oldPointOrthant = root->GetOrthant(root->point);

            root->children[oldPointOrthant] =
                new Quadtree<T,D>(root, root->GetOrthantMinPoint(oldPointOrthant), root->sideLength/T(2));
            Insert(root->children[oldPointOrthant], root->point);

            if( oldPointOrthant!=newPointOrthant )
            {
                root->children[newPointOrthant] =
                    new Quadtree<T,D>(root, root->GetOrthantMinPoint(newPointOrthant), root->sideLength/T(2));
            }
            Insert(root->children[newPointOrthant], point);

            root->point = NULL;
        }
    }
    else
    {
        // Internal node, follow recurse on the child whose orthant contains the node.
        Orthant<D> orthant = root->GetOrthant(point);
        if(root->children[orthant] == NULL)
        {
            root->children[orthant] = new Quadtree<T,D>(root, root->GetOrthantMinPoint(orthant), root->sideLength/T(2));
        }

        Insert(root->children[orthant], point);
    }
};

template<typename T, int D>
void QuadtreeConstructor<T,D>::PreparePointSet(std::vector<vec<T,D>>& retPointSet, T& retSideLength)
{
    vec<T,D> min(std::numeric_limits<T>::infinity());
    vec<T,D> max(-std::numeric_limits<T>::infinity());
    for(auto it = retPointSet.begin(); it != retPointSet.end(); ++it)
    {
        min.MinExtend(*it);
        max.MaxExtend(*it);
    }

    for(auto it = retPointSet.begin(); it != retPointSet.end(); ++it)
    {
        *it -= min;
    }

    max -= min;

    retSideLength = 0;
    for(int i=0;i<D; ++i)
    {
        retSideLength = std::max(retSideLength, max[i]);
    }
}

template class QuadtreeConstructor<double,2>;