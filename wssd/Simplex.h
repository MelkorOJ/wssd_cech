/**
 * file: Simplex.h
 * desc: 
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _SIMPLEX_H_
#define _SIMPLEX_H_

#include <set>
#include <vector>

// Forward class declarations
template<typename T, int D>
class Quadtree;

template<typename T, int D>
class Simplex
{
// Comparator
public:
    struct Comparator
    {
        bool operator()(const Simplex<T,D>* lhs, const Simplex<T,D>* rhs) const
        {
            return (*lhs) < (*rhs);
        }
    };
// Fields
private:
    const unsigned int          index;
    const double                functionValue;
    std::vector<Quadtree<T,D>*> vertices;    // Size is K+1

    // [OS] perhaps at some point we want the order of boundary to match the order in vertices
    std::vector<Simplex<T,D>*>  boundary;    // size is K+1, there are no guarantees about the order
    std::set<Simplex<T,D>*>     parents;     // size can be [0,inf)

    bool                        collapsed;

	int							K;

public:
    bool                        fromCollapse;

// Constructors
public:
    /**
     * Create a new vertex. Specialized to prevent the creation of a set.
     */
    Simplex(Quadtree<T,D>* vertex, unsigned int index = -1, double functionValue = -1.0);

    /**
     * Create a new (K>=0)-simplex.
     */
    Simplex(const std::vector<Quadtree<T,D>*>& vertices, unsigned int index = -1, double functionValue = -1.0);
    
    /**
     * Create a copy of the simplex 'ref' adding vertex 'newVertex' and deleting vertex 'oldVertex'. If NULL is passed to either of them, they will be skipped.
     * The created simplex is NOT supposed to replace 'ref'.
     */
    Simplex(const Simplex<T,D>& ref, Quadtree<T,D>* newVertex, Quadtree<T,D>* oldVertex, int index = -1, double functionValue = -1);

// Operators
public:
    Quadtree<T,D>*& operator[](unsigned int index) { return vertices[index]; }
    Quadtree<T,D>*  operator[](unsigned int index) const { return vertices[index]; }
    bool            operator<(const Simplex<T,D>& rhs) const;

// Functions
public:
    int             GetK() const { return K; }
    unsigned int    GetIndex() const { return index; }
    double          GetFunctionValue() const { return functionValue; }
    std::vector<Quadtree<T,D>*>&  GetVertices() { return vertices; }
    std::vector<Simplex<T,D>*>&   GetBoundary() { return boundary; }
    std::set<Simplex<T,D>*>&      GetParents()  { return parents;  }

    bool            Contains(Quadtree<T,D>* node) const { for(auto it = vertices.cbegin(); it != vertices.cend(); ++it) { if ((*it) == node ) return true; } return false; }

    void            GetStar(std::set<Simplex<T,D>*, typename Simplex::Comparator>& star);
    void            AddParent(Simplex<T,D>* parent);

    //void            SetBoundary(const std::vector<Simplex<T,D>*>& b) { boundary = b; ASSERT(boundary.size() == vertices.size());}
    Simplex<T,D>*   GetBoundarySimplex(int i) const { return boundary[i]; }

    void            Collapse()
    { 
        collapsed=true;
        for(auto it = parents.begin(); it!=parents.end(); ++it)
        {
            (*it)->Collapse();
        }
        parents.clear();
    }
    bool            IsCollapsed() const { return collapsed; }
};


#endif //_SIMPLEX_H_
