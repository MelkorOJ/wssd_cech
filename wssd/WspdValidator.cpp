/**
 * file: WspdValidator.cpp
 *
 * Copyright 2013 Okke Schrijvers
 */
#include <iostream>
#include <map>

#include "Quadtree.h"

#include "WspdValidator.h"

template<class T, int D>
bool WspdValidator<T,D>::ValidateWspd(const KWSSD(T,D,1)& wspd, std::vector<vec<T,D>>& pointSet) const
{
#ifdef _WSSD_VALIDATION_
    TestWellSeparated(wspd);

    return TestAllPairs(wspd, pointSet);
#else // !_WSSD_VALIDATION_
    printf("compile with _WSSD_VALIDATION_ to include the code needed for validation.\n");
    return false;
#endif // _WSSD_VALIDATION_
}

#ifdef _WSSD_VALIDATION_
template< class T, int D >
bool WspdValidator<T,D>::TestWellSeparated(const KWSSD(T,D,1)& wspd) const
{
    // Should be good by construction, but implement if bored :P.
    printf("Didn't test for well-separateness.\n");
    return true;
}

template< class T, int D >
bool WspdValidator<T,D>::TestAllPairs(const KWSSD(T,D,1)& wspd, std::vector<vec<T,D>>& pointSet) const
{
    // Get the root of the quadtree
    Quadtree<T,D>* root = const_cast<Quadtree<T,D>*>((*wspd.cbegin())[0]);
    while( root->GetParent() )
    {
        root = root->GetParent();
    }
    // Make sure to store all points in each node.
    root->UpdateAllPoints();

    int size = int(pointSet.size());

    // pairs should contain all 'true' is all pairs are present
    int* pairs = new int[size*size]; //[OS] size could be halved
    memset(pairs, 0, sizeof(int)*size*size);
    std::map<vec<T,D>*, int> pointMap;
    for(int i = 0; i < size; ++i)
    {
        // create a mapping between points and indices
        pointMap.insert(std::make_pair(&(pointSet[i]), i));
    }

    // Go over all pairs in the realization and count all pairs they represent
    for(KWSSD(T,D,1)::const_iterator it = wspd.cbegin(); it != wspd.cend(); ++it)
    {
        for(unsigned int j=0; j < (*it)[0]->allPoints.size(); ++j)
        {
            for(unsigned int k=0; k < (*it)[1]->allPoints.size(); ++k)
            {
                int i1 = pointMap[(*it)[0]->allPoints[j]];
                int i2 = pointMap[(*it)[1]->allPoints[k]];

                pairs[i1*size+i2] += 1;
                pairs[i2*size+i1] += 1;
            }
        }
    }

    // Check if all pairs appear and collect stats about how often
    bool res = true;
    int excess = 0;
    int max = 0;
    int index = 0;
    for(int i=0; i < size; ++i)
    {
        for(int j=i+1; j < size; ++j)
        {
            index = i*size+j;
            if(pairs[index]>1)
            {
                excess += (pairs[index] - 1);
            }
            max = std::max(max, pairs[index]-1);
            
            res &= (pairs[index]>0);
        }
    }

    // Print the stats.
    if(res)
    {
        std::cout << "None missing" << std::endl;
    }
    else
    {
        std::cout << "There were pairs missing." << std::endl;
    }
    printf("There were %d duplicates and the largest one was %d.\n", excess, max);

    return res;
}
#endif //_WSSD_VALIDATION_

template class WspdValidator<double,2>;