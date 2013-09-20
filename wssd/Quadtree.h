/**
 * file: Quadtree.h
 * desc: Compressed quadtree implementation. It is assumed that the input set consists of points
 *       in [0, 2^L -1)^d for some L.
 *
 *       The paper also assumes that no 2 points fall in a unit grid cell, we don't make this
 *       assumption in the code.
 *
 *       Only supports up to 32 dimensions now. This is already infeasible since each quadtree
 *       element would have 2^32 > 1 Billion children = 16 GB for the pointers alone.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <vector>

#include "Assert.h"
#include "Vec.h"
#include "AxisAlignedBoundingBox.h"
#include "Orthant.h"

/**
 * Class to represent a (compressed) quadtree. Each element in the quad tree can be seen
 * as a quadtree in it's own right. The difference is that the root would not be NULL.
 */
template<typename T, int D>
class Quadtree
{
    template<typename TT, int DD> friend class QuadtreeConstructor;

private:
    /**
     * Iterator for traversing all children of a quadtree node.
     */
    template<typename P>
    class BaseChildIterator
    {
    private:
        P* parent;
        Orthant<D> orthant;
    public:
        BaseChildIterator(P* p, Orthant<D> o) : parent(p), orthant(o) {
            while(orthant < Orthant<D>::Max() && p->children[orthant] == NULL ) ++orthant;
        }

        bool                    operator!=(const BaseChildIterator<P>& rhs) const {return orthant != rhs.orthant || parent != rhs.parent; }
        P*                      operator*() const { ASSERT(parent->children[orthant]); return parent->children[orthant]; }
        BaseChildIterator<P>&   operator++() { do{++orthant;} while(orthant < Orthant<D>::Max() && !parent->children[orthant]); return *this; }
        Orthant<D>              GetOrthant() const { return orthant; }
    };

// Specialized iterator types for traversing the children
public:
    typedef BaseChildIterator<Quadtree<T,D>>       ChildIterator;
    typedef BaseChildIterator<const Quadtree<T,D>> ConstChildIterator;


private:
	// Quadtree fields
	Quadtree*	        parent;
	Quadtree*	        children[1<<D];  // [OS] this implementation becomes infeasible quickly for high dimensions.
    vec<T,D>*			point;

	const vec<T,D>		minPoint;
	const T				sideLength;
    AxisAlignedBoundingBox<T,D> aabb;

    Quadtree*           representative;

#ifdef _WSSD_VALIDATION_
public:
    // Only use for debugging purposes!
    std::vector<vec<T,D>*>  allPoints;
#endif //_WSSD_VALIDATION_
public:
	// Constructors
						Quadtree(Quadtree* parent, vec<T,D> minPoint, T sideLength, vec<T,D>* p = NULL);
						~Quadtree();

	// Functions
    const Quadtree*     FindLeafNode(const vec<T,D>* p) const;
	bool				ContainsPoint(const vec<T,D>* p) const;
	Orthant<D>			GetOrthant(const vec<T,D>* p) const;
    Orthant<D>          OrthantInParent() const;
    vec<T,D>            GetOrthantMinPoint(const Orthant<D>& orthant) const;
	bool			    IsLeaf() const;
    int                 GetNumChildren() const;

    void                UpdateBoundingBoxes();

    
    // Inline functions
    Quadtree*           GetParent() const { return parent; }
    vec<T,D>*           GetPoint() const { return point; }
    const vec<T,D>&     GetMinPoint() const { return minPoint; }
    const T&            GetSideLength() const { return sideLength; }
    const AxisAlignedBoundingBox<T,D>& GetAabb() const { return aabb; }

    // Iterator functions
    ChildIterator       ChildBegin() { return ChildIterator(this, 0); }
    ChildIterator       ChildEnd()   { return ChildIterator(this, Orthant<D>::Max()); }

    ConstChildIterator  CChildBegin() const { return ConstChildIterator(this, 0); }
    ConstChildIterator  CChildEnd()   const { return ConstChildIterator(this, Orthant<D>::Max()); }

    Quadtree*           GetRepresentative() const { return representative; }
    void                SetRepresentative(Quadtree* node) { representative = node; }

#ifdef _WSSD_VALIDATION_
    void                UpdateAllPoints();
#endif //_WSSD_VALIDATION_
};

#endif //_QUADTREE_H_
