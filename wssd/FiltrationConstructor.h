/**
 * file: FiltrationConstructor.h
 * desc: Constructs an approximate Cech Complex filtration based on the construction
 *       in "Approximate Cech Complexes in Low and High Dimensions" by Kerber and Sharathkumar.
 *
 * Copyright 2013 Okke Schrijvers.
 */

#ifndef _FILTRATION_CONSTRUCTOR_H_
#define _FILTRATION_CONSTRUCTOR_H_

#include "Simplex.h"
#include "WSSD.h"

#ifdef _WSSD_THREADS_
#include <thread>
#include <mutex>
#endif //_WSSD_THREADS_

#include <map>
#include <vector>
#include <set>


// Forward class declarations
template<typename T, int D>
class Filtration;

template<typename T, int D>
class FiltrationConstructor
{
// Fields
private:
    double                  epsilon;
    const int               minDelta;
    int                     maxDelta;

    int                     totalVertices;
    int                     collapsedVertices;

    // Needed to prevent duplications
    std::set<Simplex<T,D>*, typename Simplex<T,D>::Comparator>  simplices;

    // Needed to improve speed
    std::map<int, std::vector<std::set<Quadtree<T,D>*>>>        tuples;

	int				highestDelta;

#ifdef _WSSD_THREADS_
    std::mutex		hdMutex;
    std::mutex      tuplesMutex;
#endif //_WSSD_THREADS_

// Constructors
public:
    FiltrationConstructor(double eps, int minDelta, int maxDelta);

// Functions:
public:
    /**
     * Constructs the filtration for Delta in [minDelta, maxDelta]  =>  alpha = (1+eps)^Delta.
     * To conserve memory, it clears wssd.
     */
    void ConstructFiltration(WSSD<T,D>& wssd, Filtration<T,D>& retFiltration);

// Inline methods:
private:

    // These constants follow from the paper
    double GetTheta(int delta)                  const { return pow(1.0 + epsilon, delta); }
    double GetMaxQuadtreeCellDiam(double theta) const { return (2.0/5.0)*epsilon*theta; }
    double GetMaxTupleDiam(double theta)        const { return (1.0 + (2.0/3.0)*epsilon)*theta; }

// Functions:
private:

    /**
     * Add all quadtree nodes with small enough diameter as vertices to the filtration. This vertex set does not
     * change throughout the construction of the filtration. For each ancestor A of a vertex, set vertex that is
     * obtained by a 'first child'-order traversal of the subquadtree as a representative. This will be the vertex
     * that will be used whenever A participates in a simplex.
     */
    void AddAllVertices(Quadtree<T,D>* root, double maxNodeDiam, Filtration<T,D>& retFiltration);

	template<int K>
	void AddToMap(const WellSeparatedTuple<T, D, K>& tuple);

    template<int K>
    void PrepareTuples(WSSD<T,D>& wssd);

    /**
     * Finds the delta for which this tuple will be added to the filtration.
     */
    template<int K>
    int FindTimeToAdd(const WellSeparatedTuple<T,D,K>& tuple, std::set<Quadtree<T,D>*>& retNodes);

    /**
     * Update all the vertices to new theta. If two vertices now map to the same node in the quadtree, it collapses
     * one to the other.
     */
    void UpdateToNewTheta(double theta, Filtration<T,D>& retFiltration);

    /**
     * Adds a single vertex to the filtration if it doesn't exist.
     */
    void AddVertexToFiltration(Quadtree<T,D>* node, Filtration<T,D>& retFiltration);

    /**
     * Add a simplex to the filtration.
     */
    void AddSimplexToFiltration(const std::set<Quadtree<T,D>*>& nodes, double theta, Filtration<T,D>& retFiltration);


    /**
     * Collapse vertex v to vertex u.
     */
    void Collapse(Simplex<T,D>* u, Simplex<T,D>* v, double theta, Filtration<T,D>& retFiltration);




// Const helper functions:
private:
    /**
     * See if each element in the tuple has small enough diameter for being added to the filtration.
     */
    template<int K>
    bool HasSmallDiam(const WellSeparatedTuple<T,D,K>& tuple, const double& maxTupleDiam) const;

    /**
     * Get the diameter of the MEB of the nodes.
     */
    double GetDiam(const std::set<Quadtree<T,D>*>& nodes) const;

        /**
     * Check if the minimum enclosing ball radius is small enough for being added to the filtration.
     */
    bool ShouldAdd(const std::set<Quadtree<T,D>*>& nodes, const double& maxSimplexDiam) const;

    /**
     * Get the highest ancestor that has a diameter smaller than the one given.
     * Assumes that diam(node) < maxDiam.
     */
    Quadtree<T,D>* GetHighestAncestor(Quadtree<T,D>* node, const double& maxDiam) const;

    /**
     * Get the closure of the star of 'simplex'.
     */
    void GetStarClosure(Simplex<T,D>* simplex, const Filtration<T,D>& filtration, std::set<Simplex<T,D>*, typename Simplex<T,D>::Comparator>& retStarClosure) const;

    /**
     * Obtain the representatives for a set of nodes. Assumes that the representatives of all 'nodes' are unique.
     * Assumes that 'representatives' has the same size as 'nodes'.
     */
    void GetRepresentatives(const std::set<Quadtree<T,D>*>& nodes, std::vector<Quadtree<T,D>*>& representatives) const;
};


#endif //_FILTRATION_CONSTRUCTOR_H_
