/**
 * file: Quadtree.cpp
 *
 * Copyright 2013 Okke Schrijvers
 */

#include "Quadtree.h"

/**
 * Constructs and empty quadtree node.
 */
template<typename T, int D>
Quadtree<T,D>::Quadtree(Quadtree<T,D>* parent, vec<T,D> minPoint, T sideLength, vec<T,D>* p = NULL)
: parent(parent)
, minPoint(minPoint)
, sideLength(sideLength)
, point(p)
, representative(NULL)
{
	for(unsigned int i=0; i<Orthant<D>::Max(); ++i)
	{
		children[i]=NULL;
	}
}

/**
 * Deletes the children. Does not delete the points that this node contained.
 */
template<typename T, int D>
Quadtree<T,D>::~Quadtree()
{
	for(unsigned int i=0; i<Orthant<D>::Max(); ++i)
	{
		if(children[i])
		{
			delete children[i];
			children[i]=NULL;
		}
	}
}

/**
 * Returns if the node contains a given point.
 *
 * [OS] With a compressed quadtree it is not guaranteed that there is a leaf node that
 *      contains a point.
 */
template<typename T, int D>
const Quadtree<T,D>* Quadtree<T,D>::FindLeafNode(const vec<T,D>* p) const
{
	if(IsLeaf())
    {
        ASSERT(ContainsPoint(p));
        return this;
    }
    
    Orthant<D> orthant = GetOrthant(p);
    ASSERT_MSG(children[orthant] != NULL, "The point does not appear in a leaf node in the quadtree.\n");
    return children[orthant]->FindLeafNode(p);    
}

/**
 * Returns if the node contains a given point.
 */
template<typename T, int D>
bool Quadtree<T,D>::ContainsPoint(const vec<T,D>* p) const
{
	bool retVal = true;
	for(int i=0; i < D; ++i) {
		retVal &= (minPoint[i] < (*p)[i] || minPoint[i] == T(0.0));
		retVal &= (*p)[i] <= (minPoint[i] + sideLength);
	}
	return retVal;
}

/**
 * Returns the orthant in the quadtree node.
 */
template<typename T, int D>
Orthant<D> Quadtree<T,D>::GetOrthant(const vec<T,D>* p) const
{
    ASSERT_MSG(ContainsPoint(p), "The node does not contain the point for which we try to find the orthant.\n");

    Orthant<D> orthant;
    for(int d=0; d<D; ++d)
    {
        orthant.Set(d, minPoint[d] + (sideLength/T(2)) < (*p)[d]);
    }

    return orthant;
}

/**
 * Returns which orthant this quadtree node occupies in it's parent.
 */
template<typename T, int D>
Orthant<D> Quadtree<T,D>::OrthantInParent() const
{
    if(parent == NULL)
    {
        // Return 0 arbitrarily
        return Orthant<D>();
    }

    return parent->GetOrthant(&(minPoint + (sideLength/T(2))));
}


/**
 * Get the minimal point of the given orthant.
 */
template<typename T, int D>
vec<T,D> Quadtree<T,D>::GetOrthantMinPoint(const Orthant<D>& orthant) const
{
    vec<T,D> retVal = minPoint;
    for(int d=0; d<D; ++d)
    {
        if(orthant.Get(d))
        {
            retVal[d] += (sideLength/T(2));
        }
    }
    return retVal;
}

/**
 * Returns if the quadtree element is a leaf node.
 */
template<typename T, int D>
bool Quadtree<T,D>::IsLeaf() const
{
	for(unsigned int i=0; i<Orthant<D>::Max(); ++i)
    {
		if(children[i])
        {
			return false;
		}
	}
	return true;
}

/**
 * Returns the number of children that this node has.
 */
template<typename T, int D>
int Quadtree<T,D>::GetNumChildren() const
{
    int numChildren = 0;
	for(unsigned int i=0; i<Orthant<D>::Max(); ++i)
    {
		numChildren += (children[i] != NULL);
	}
	return numChildren;
}

/**
 * Create tight fitting bounding boxes for quadtree nodes.
 */
template<typename T, int D>
void Quadtree<T,D>::UpdateBoundingBoxes()
{
    if(point)
    {
        aabb.Set(*point, *point);
    }
    else
    {
        //aabb.Set(minPoint, minPoint + sideLength);
        bool first = true;
        for(ChildIterator it = ChildBegin(); it != ChildEnd(); ++it)
        {
            (*it)->UpdateBoundingBoxes();
            if(first)
            {
                aabb = (*it)->aabb;
                first = false;
            }
            else
            {
                aabb.Extend((*it)->aabb);
            }
        }
    }
}

#ifdef _WSSD_VALIDATION_
/**
 * Store all the points that are contained in (children of) the node, in allPoints.
 */
template<typename T, int D>
void Quadtree<T,D>::UpdateAllPoints()
{
    allPoints.clear();

    if(IsLeaf())
    {
        allPoints.push_back(point);
    }
    else
    {
        for(ChildIterator it = ChildBegin(); it != ChildEnd(); ++it)
        {
            (*it)->UpdateAllPoints();
            allPoints.insert(allPoints.end(), (*it)->allPoints.begin(), (*it)->allPoints.end());
        }
    }
}
#endif //_WSSD_VALIDATION_

template class Quadtree<double,2>;