/**
 * file: FiltrationConstructor.cpp
 *
 * Copyright 2013 Okke Schrijvers
 */
#include <array>
#include <algorithm>
#include <iostream>
#include <functional>

#include "Miniball.hpp"

#include "Quadtree.h"
#include "Simplex.h"
#include "Filtration.h"
#include "FiltrationConstructor.h"





template<typename T, int D>
FiltrationConstructor<T,D>::FiltrationConstructor(double eps, int minDelta, int maxDelta)
: epsilon(eps)
, minDelta(minDelta)
, maxDelta(maxDelta)
, highestDelta(0)
, totalVertices(0)
, collapsedVertices(0)
{}

template<typename T, int D>
void FiltrationConstructor<T,D>::ConstructFiltration(WSSD<T,D>& wssd, Filtration<T,D>& retFiltration)
{
    totalVertices = 0;
    collapsedVertices = 0;


    // Create the initial vertices and simplices
    Quadtree<T,D>* root = GetHighestAncestor(wssd.GetKWssd<1>()[0][0], std::numeric_limits<double>::infinity());
    AddAllVertices(root, GetMaxQuadtreeCellDiam(GetTheta(minDelta)), retFiltration);

    std::cout << "Preparing WSSD" << std:: endl;
    PrepareTuples<1>(wssd);
    std::cout << "After 1-WSSD." << std:: endl;
    PrepareTuples<2>(wssd);
    std::cout << "After 2-WSSD." << std:: endl;
    maxDelta = highestDelta;
    wssd.clear();

    //for(int i = minDelta; i<=maxDelta; ++i)
    //{
    //    std::cout << tuples[i].size() << std::endl;
    //}

    // Update the simplices and add new ones
    double theta = GetTheta(minDelta);
    for(int i = minDelta; i<=maxDelta; ++i, theta*=(1.0+epsilon))
    {
        UpdateToNewTheta(theta, retFiltration);

        std::vector<std::set<Quadtree<T,D>*>>& nodes = tuples[i];
        for(auto it = nodes.begin(); it != nodes.end(); ++it)
        {
            AddSimplexToFiltration(*it, theta, retFiltration);
        }

        nodes.clear();

        std::cout << "Handled iteration " << i << " size of filtration: " << retFiltration.simplices.size() << std::endl;
        std::cout << "Collapsed vertices: " << collapsedVertices << "/" << totalVertices << std::endl;

        if( collapsedVertices + 1 == totalVertices)
            break;

        
    }
}

template<typename T, int D>
void FiltrationConstructor<T,D>::AddAllVertices(Quadtree<T,D>* root, double maxNodeDiam, Filtration<T,D>& retFiltration)
{
    if(root->GetAabb().GetDiameter() > maxNodeDiam)
    {
        // Node is to big, recurse
        for(Quadtree<T,D>::ChildIterator it = root->ChildBegin(); it != root->ChildEnd(); ++it)
        {
            AddAllVertices(*it, maxNodeDiam, retFiltration);
        }
        // And update representative based on 'first child'-traversal
        root->SetRepresentative((*root->ChildBegin())->GetRepresentative());
    }
    else
    {
        root->SetRepresentative(root);
        AddVertexToFiltration(root, retFiltration);
    }
}

template<typename T, int D>
template<int K>
int FiltrationConstructor<T, D>::FindTimeToAdd(const WellSeparatedTuple<T, D, K>& tuple, std::set<Quadtree<T, D>*>& retNodes)
{
	int delta = minDelta;
	double theta = GetTheta(delta);
	while (tuple.GetMaxDiam() > GetMaxQuadtreeCellDiam(theta))
	{
		delta++;
		theta *= (1.0 + epsilon);
	}

	// Get the correct ancestors of the quadtree nodes
	retNodes.clear();
	for (int j = 0; j < K + 1; ++j)
	{
		retNodes.insert(GetHighestAncestor(tuple[j], GetMaxQuadtreeCellDiam(theta)));
	}
	if (retNodes.size() < K + 1)
	{
		return -1;
	}

	double retNodesDiam = GetDiam(retNodes);

	while (retNodesDiam > GetMaxTupleDiam(theta))
	{
		double smallestParentDiam = std::numeric_limits<double>::infinity();
		for (auto it = retNodes.begin(); it != retNodes.end(); it++)
		{
			smallestParentDiam = std::min(smallestParentDiam, (*it)->GetParent()->GetAabb().GetDiameter());
		}

		while (GetMaxQuadtreeCellDiam(theta) < smallestParentDiam)
		{
			if (retNodesDiam <= GetMaxTupleDiam(theta))
			{
				return delta;
			}
			delta++;
			theta *= (1.0 + epsilon);
		}

		std::vector<Quadtree<T, D>*> newNodes;
		Quadtree<T, D>* quadtree = NULL;
		auto it = retNodes.begin();

		while (it != retNodes.end())
		{
			quadtree = GetHighestAncestor(*it, GetMaxQuadtreeCellDiam(theta));
			if ((*it) != quadtree)
			{
				Quadtree<T, D>* removeMe = *it;
				it++;
				retNodes.erase(removeMe);
				newNodes.push_back(quadtree);
			}
			else
			{
				it++;
			}
		}
		retNodes.insert(newNodes.begin(), newNodes.end());
		newNodes.clear();

		if (retNodes.size() < K + 1)
		{
			return -1;
		}

		retNodesDiam = GetDiam(retNodes);
	}

	return delta;
}

template<typename T, int D>
template<int K>
void FiltrationConstructor<T, D>::AddToMap(const WellSeparatedTuple<T, D, K>& tuple)
{
	std::set<Quadtree<T, D>*> nodes;
	int delta = FindTimeToAdd(tuple, nodes);
	if (delta > -1)
	{

#ifdef _WSSD_THREADS_
		hdMutex.lock();
#endif //_WSSD_THREADS_

		highestDelta = std::max(highestDelta, delta);

#ifdef _WSSD_THREADS_
        hdMutex.unlock();
        tuplesMutex.lock();
#endif //_WSSD_THREADS_

		tuples[delta].push_back(nodes);

#ifdef _WSSD_THREADS_
        tuplesMutex.unlock();
#endif //_WSSD_THREADS_
	}
}

template<typename T, int D>
template<int K>
void FiltrationConstructor<T,D>::PrepareTuples(WSSD<T,D>& wssd)
{
#ifdef _WSSD_THREADS_
	std::vector<std::thread> threads;
#endif //_WSSD_THREADS_

	int i = 0;

    // Add simplices for this level
    for(KWSSD(T,D,K)::iterator it = wssd.GetKWssd<K>().begin(); it != wssd.GetKWssd<K>().end(); ++it, ++i)
    {
#ifdef _WSSD_THREADS_
        threads.push_back(std::thread(std::bind(&FiltrationConstructor<T, D>::AddToMap<K>, std::ref(*this), std::ref(*it))));
#else // ~_WSSD_THREADS_
        AddToMap(*it);
#endif //_WSSD_THREADS_
    }

#ifdef _WSSD_THREADS_
	for (auto& th : threads) th.join();
#endif //_WSSD_THREADS_

    wssd.GetKWssd<K>().clear();
}





template<typename T, int D>
void FiltrationConstructor<T,D>::UpdateToNewTheta(double theta, Filtration<T,D>& retFiltration)
{
    // Iterate over all simplices in the filtration
    for(unsigned int i=0; i<retFiltration.simplices.size(); ++i)
    {
        Simplex<T,D>* simplex = retFiltration.simplices[i];

        // If it's a vertex, we might need to collapse it to another one
        if(simplex->GetK() == 0 && !simplex->IsCollapsed())
        {
            // Get largest ancestor that is small enough
            Quadtree<T,D>* node = GetHighestAncestor((*simplex)[0], GetMaxQuadtreeCellDiam(theta));

            // If we changed representatives
            if(node->GetRepresentative() != (*simplex)[0]->GetRepresentative())
            {
                Simplex<T,D>* temp = new Simplex<T,D>(node->GetRepresentative());
                auto foundSimplex = simplices.find(temp);
                delete temp;
                
                Simplex<T,D>* u = retFiltration.simplices[(*foundSimplex)->GetIndex()];
                Simplex<T,D>* v = simplex;

                ASSERT(u->GetVertices()[0] == u->GetVertices()[0]->GetRepresentative());
                ASSERT(v->GetVertices()[0] == v->GetVertices()[0]->GetRepresentative());

                Collapse(u, v, theta, retFiltration);
            }
        }
    }
}




template<typename T, int D>
void FiltrationConstructor<T,D>::AddVertexToFiltration(Quadtree<T,D>* node, Filtration<T,D>& retFiltration)
{
    // Create the vertex and check if we've already seen it.
    Simplex<T,D>* vertex = new Simplex<T,D>(node, retFiltration.simplices.size(), 0.0);

    // Add the vertex to the filtration and our search structure
    retFiltration.simplices.push_back(vertex);
    simplices.insert(vertex);

    totalVertices++;
}


template<typename T, int D>
void FiltrationConstructor<T,D>::AddSimplexToFiltration(const std::set<Quadtree<T,D>*>& nodes, double theta, Filtration<T,D>& retFiltration)
{
    // Get the representatives of the simplex
    std::vector<Quadtree<T,D>*> reps(nodes.size());
    GetRepresentatives(nodes, reps);

    Simplex<T,D>* simplex = new Simplex<T,D>(reps, retFiltration.simplices.size(), theta);
    if(simplices.find(simplex) == simplices.end())
    {
        // Build the boundary
        std::vector<Simplex<T,D>*>& boundary = simplex->GetBoundary();
        {
            // Find all faces
            for(auto it = reps.begin(); it != reps.end(); ++it)
            {
                // Creates the face of all nodes except for it
                Simplex<T,D>* s = new Simplex<T,D>(*simplex, NULL, *it);

                auto foundSimplex = simplices.find(s);
                boundary.push_back(retFiltration.simplices[(*foundSimplex)->GetIndex()]);
                delete s;
            }
        }
        
        // Add the simplex to the filtration and our search structure
        retFiltration.simplices.push_back(simplex);
        simplices.insert(simplex);

        // Update the 'parent' simplex information
        for(auto it = boundary.begin(); it != boundary.end(); ++it)
        {
            (*it)->AddParent(simplex);
        }
    }
    else
    {
        // Simplex already existed, clean up
        delete simplex;
    }
}

template<typename T, int D>
void FiltrationConstructor<T,D>::Collapse(Simplex<T,D>* u, Simplex<T,D>* v, double theta, Filtration<T,D>& retFiltration)
{
    // Get the star of v
    std::set<Simplex<T,D>*, typename Simplex<T,D>::Comparator> star;
    GetStarClosure(v, retFiltration, star);

    Simplex<T,D>* starSimplex;
    Simplex<T,D>* s;

    // Connect all of them to u
    for(auto it = star.begin(); it != star.end(); ++it)
    {
        starSimplex = *it;

        s = new Simplex<T,D>(*starSimplex, u->GetVertices()[0], NULL, retFiltration.simplices.size(), theta);
        if( simplices.find(s) == simplices.end() && s->GetK() < D+1)
        {
            // Now do the boundary
            std::vector<Simplex<T,D>*>& boundary = s->GetBoundary();
            if(starSimplex->GetK() == 0)
            {
                boundary.push_back(starSimplex);
                boundary.push_back(u);
            }
            else
            {
                for(auto it2 = s->GetVertices().cbegin(); it2 != s->GetVertices().cend(); ++it2)
                {
                    Simplex<T,D>* s2 = new Simplex<T,D>(*s, NULL, *it2);
                    auto foundSimplex = simplices.find(s2);
                    boundary.push_back(retFiltration.simplices[(*foundSimplex)->GetIndex()]);
                    delete s2;
                }
            }

            // Update parent information
            for(auto it2 = boundary.begin(); it2 != boundary.end(); ++it2)
            {
                (*it2)->AddParent(s);
            }

            // Add the simplex to the list
            s->fromCollapse = true;
            retFiltration.simplices.push_back(s);
            simplices.insert(s);
        }
        else
        {
            // We already had s in the filtration
            delete s;
            s = NULL;
        }
    }

    for(auto it = star.begin(); it != star.end(); ++it)
    {
        starSimplex = *it;
        if(starSimplex->Contains(v->GetVertices()[0]))
        {
            simplices.erase(starSimplex);
        }
    }

    v->Collapse();
    collapsedVertices++;
}






template<typename T, int D>
void FiltrationConstructor<T,D>::GetStarClosure(Simplex<T,D>* simplex,  const Filtration<T,D>& filtration, std::set<Simplex<T,D>*, typename Simplex<T,D>::Comparator>& retStarClosure) const
{
    std::set<Simplex<T,D>*, typename Simplex<T,D>::Comparator> star;
    simplex->GetStar(star);

    // Copy the container
    retStarClosure = star;
    
    // Get the closure
    for(auto it = star.cbegin(); it != star.cend(); ++it)
    {
        if((*it)->GetK()>0)
        {
            Simplex<T,D>* s = new Simplex<T,D>(**it, NULL, (*simplex)[0]);
            auto foundSimplex = simplices.find(s);
            retStarClosure.insert(filtration.simplices[(*foundSimplex)->GetIndex()]);

            delete s;
        }
    }
}

template<typename T, int D>
template<int K>
bool FiltrationConstructor<T,D>::HasSmallDiam(const WellSeparatedTuple<T,D,K>& tuple, const double& maxDiam) const
{
    return tuple.GetMaxDiam() < maxDiam;
}

template<typename T, int D>
double FiltrationConstructor<T,D>::GetDiam(const std::set<Quadtree<T,D>*>& nodes) const
{
    std::vector<std::vector<T>> points;
    std::vector<T> p(D);

    for(auto it = nodes.cbegin(); it != nodes.cend(); ++it)
    {
        const AxisAlignedBoundingBox<T,D>& aabb = (*it)->GetAabb();
        if(aabb.GetDiameter() > 0 )
        {
            for(std::size_t i=0; i < Orthant<D>::Max(); ++i)
            {
                aabb.GetExtremum(i, p);
                points.push_back(p);
            }
        }
        else
        {
            vec<T,D>& point = *(*it)->GetPoint();
            for(int j=0; j < D; ++j)
            {
                p[j] = point[j];
            }
            points.push_back(p);
        }
    }

    typedef std::vector<std::vector<T>>::const_iterator PointIterator; 
    typedef std::vector<T>::const_iterator CoordIterator;

    typedef Miniball::Miniball <Miniball::CoordAccessor<PointIterator, CoordIterator> > MB;
    MB mb(D, points.begin(), points.end());

    return 2.0 * sqrt(mb.squared_radius());
}

template<typename T, int D>
bool FiltrationConstructor<T,D>::ShouldAdd(const std::set<Quadtree<T,D>*>& nodes, const double& maxDiam) const
{
    // Create aabb for the set of quadtree nodes
    AxisAlignedBoundingBox<T,D> aabb((*nodes.cbegin())->GetAabb());
    for(auto it = ++(nodes.cbegin()); it != nodes.cend(); ++it)
    {
        aabb.Extend((*it)->GetAabb());
    }

    // And report if it's small enough
    return aabb.GetDiameter() <= maxDiam;
}


template<typename T, int D>
Quadtree<T,D>* FiltrationConstructor<T,D>::GetHighestAncestor(Quadtree<T,D>* node, const double& maxDiam) const
{
    while( node->GetParent() && node->GetParent()->GetAabb().GetDiameter() <= maxDiam )
    {
        node = node->GetParent();
    }

    return node;
}

template<typename T, int D>
void FiltrationConstructor<T,D>::GetRepresentatives(const std::set<Quadtree<T,D>*>& nodes, std::vector<Quadtree<T,D>*>& representatives) const
{
    int i=0;
    for(auto it = nodes.begin(); it!=nodes.end(); ++it)
    {
        representatives[i] = (*it)->GetRepresentative();
        ++i;
    }
}

template class FiltrationConstructor<double,2>;