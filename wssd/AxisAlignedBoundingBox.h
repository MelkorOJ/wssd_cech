/**
 * file: AxisAlignedBoundingBox.h
 * desc: 
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _AXIS_ALIGNED_BOUNDING_BOX_H_
#define _AXIS_ALIGNED_BOUNDING_BOX_H_

#include "Orthant.h"
#include "Vec.h"

/**
 * Class to hold D-dimensional point of type T.
 */
template<typename T, int D> 
class AxisAlignedBoundingBox
{
private:
	vec<T,D>    min;
    vec<T,D>    max;

public:

    AxisAlignedBoundingBox()
    {}

    AxisAlignedBoundingBox(const AxisAlignedBoundingBox& rhs)
    : min(rhs.min)
    , max(rhs.max)
    {}

    AxisAlignedBoundingBox(const vec<T,D>& min, const vec<T,D>& max)
    : min(min)
    , max(max)
    {}

    void Set(const vec<T,D>& min, const vec<T,D>& max)
    {
        this->min = min;
        this->max = max;
    }

    void Extend(const AxisAlignedBoundingBox<T,D>& rhs)
    {
        min.MinExtend(rhs.min);
        max.MaxExtend(rhs.max);
    }

    double DistanceTo(const vec<T,D>& rhs) const
    {
        return (rhs - rhs.Clamped(min, max)).Length();
    }

    double DistanceTo(const AxisAlignedBoundingBox<T,D>& rhs) const
    {
        // Initialize the distance vector to the 0 vector
        vec<T,D> distance;

        // Find the shortest distance in each dimension
        for(int d=0;d<D;++d)
        {
            if(max[d] < rhs.min[d])
            {
                distance[d] = rhs.min[d] - max[d];
            }
            else if(rhs.max[d] < min[d])
            {
                distance[d] = min[d] - rhs.max[d];
            }
            // else they overlap and distance of 0 is correct.
        }

        return distance.Length();
    }

    vec<T,D> GetMidPoint() const
    {
        return (min + max)/T(2);
    }

    double GetDiameter() const
    {
        // [OS] we might store the diameter at some point.
        //      If so, perform correctness assert here.
        return (max-min).Length();
    }

    /**
     * Assumes that 'extremum' is of size D
     */
    void GetExtremum(Orthant<D> orthant, std::vector<T>& extremum) const
    {
        // Find the shortest distance in each dimension
        for(int d=0;d<D;++d)
        {
            extremum[d] = (orthant.Get(d) ? max[d] : min[d]);
        }
    }

};

#endif //_AXIS_ALIGNED_BOUNDING_BOX_H_
