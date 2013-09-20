/**
 * file: Simplex.cpp
 *
 * Copyright 2013 Okke Schrijvers
 */

#include <algorithm>
#include <set>

#include "Assert.h"
#include "Simplex.h"

template<typename T, int D>
Simplex<T,D>::Simplex(Quadtree<T,D>* vertex, unsigned int index = -1, double functionValue = -1.0)
: index(index)
, functionValue(functionValue)
, vertices(1)
, collapsed(false)
, fromCollapse(false)
{
    vertices[0] = vertex;
	K = 0;
}

template<typename T, int D>
Simplex<T,D>::Simplex(const std::vector<Quadtree<T,D>*>& verts, unsigned int index = -1, double functionValue = -1.0)
: index(index)
, functionValue(functionValue)
, collapsed(false)
, fromCollapse(false)
{
    vertices.insert(vertices.end(), verts.begin(), verts.end());
    std::sort(vertices.begin(), vertices.end());
	K = vertices.size() - 1;
}

template<typename T, int D>
Simplex<T,D>::Simplex(const Simplex<T,D>& ref, Quadtree<T,D>* newVertex, Quadtree<T,D>* oldVertex, int index, double functionValue)
: index(index)
, functionValue(functionValue)
, collapsed(false)
, fromCollapse(false)
{
    std::set<Quadtree<T,D>*> verts;
    verts.insert(ref.vertices.begin(), ref.vertices.end());
    if(oldVertex)
    {
        verts.erase(oldVertex);
    }
    if(newVertex)
    {
        verts.insert(newVertex);
    }
	vertices.insert(vertices.end(), verts.begin(), verts.end());
	K = vertices.size() - 1;
}

template<typename T, int D>
bool Simplex<T,D>::operator<(const Simplex<T,D>& rhs) const
{
    // If the simplices have the same cardinality
    if(GetK() == rhs.GetK())
    {
        // Compare pointers in ascending order
        for(int i=0; i<GetK()+1; ++i)
        {
            if(vertices[i] < rhs.vertices[i])
            {
                // This tuple's pointers are lower
                return true;    
            }
            if(vertices[i] > rhs.vertices[i])
            {
                // This tuple's pointers are higher
                return false;
            }
            // The i^th pointers are equal: test i+1
        }
        // Tuples are equal
        return false;
    }

    // If different cardinality simplices, the lower is smaller
    return GetK() < rhs.GetK();
}

//template<typename T, int D>
//void Simplex<T,D>::GetStar(std::set<Simplex<T,D>*, typename Simplex<T,D>::Comparator>& retStar) const
//{
//    // Add all parents
//    retStar.insert(parents.begin(), parents.end());
//
//    // Recursively add all parents' parents.
//    for(auto it = parents.cbegin(); it != parents.cend(); ++it)
//    {
//        (*it)->GetStar(retStar);
//    }
//}


template<typename T, int D>
void Simplex<T,D>::AddParent(Simplex<T,D>* parent)
{
    parents.insert(parent);
}

template<typename T, int D>
void Simplex<T,D>::GetStar(std::set<Simplex<T,D>*, typename Simplex::Comparator>& star)
{
    if(!collapsed)
    {
        star.insert(this);

        for(auto it = parents.begin(); it != parents.end(); ++it)
        {
            (*it)->GetStar(star);
        }
    }
}


template class Simplex<double,2>;