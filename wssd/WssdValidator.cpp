/**
 * file: WssdValidator.cpp
 *
 * Copyright 2013 Okke Schrijvers
 */

#include <map>

#include "Quadtree.h"

#include "WssdValidator.h"

template<typename T, int D, int K>
bool WssdValidator<T,D,K>::ValidateWssd(const KWSSD(T,D,K)& wspd, std::vector<vec<T,D>>& pointSet) const
{
#ifdef _WSSD_VALIDATION_
    TestWellSeparated(wspd);

    return TestAllTuples(wspd, pointSet);
#else // !_WSSD_VALIDATION_
    printf("compile with _WSSD_VALIDATION_ to include the code needed for validation.\n");
    return false;
#endif // _WSSD_VALIDATION_
}

#ifdef _WSSD_VALIDATION_
template<typename T, int D, int K>
bool WssdValidator<T,D,K>::TestWellSeparated(const KWSSD(T,D,K)& wspd) const
{
    // Should be good by construction, but implement if bored :P.
    printf("Didn't test for well-separatedness.\n");
    return true;
}

template<class T, int D, int K>
bool WssdValidator<T,D,K>::TestAllTuples(const KWSSD(T,D,K)& wssd, std::vector<vec<T,D>>& pointSet) const
{
    ASSERT_MSG(K==2, "Only for K==2, for K==1 use WspdValidator, for K>2 write the code to validate ;). Running time is O(n^(K+1)).\n");

    // Get the root of the quadtree
    // [OS] performing const cast since it's just for validation anyway
    Quadtree<T,D>* root = const_cast<Quadtree<T,D>*>((*wssd.cbegin())[0]);
    while( root->GetParent() )
    {
        root = root->GetParent();
    }
    // Make sure to store all points in each node.
    root->UpdateAllPoints();

    int size = int(pointSet.size());

    // pairs should contain all 'true' is all pairs are present
    int* tuples = new int[size*size*size]; //[OS] size could be cut in 6
    memset(tuples, 0, sizeof(int)*size*size*size);
    std::map<vec<T,D>*, int> pointMap;
    for(int i = 0; i < size; ++i)
    {
        // create a mapping between points and indices
        pointMap.insert(std::make_pair(&(pointSet[i]), i));
    }

    // Add all triplets of the realization
    for(KWSSD(T,D,K)::const_iterator it = wssd.cbegin(); it != wssd.cend(); ++it)
    {
        for(unsigned int j=0; j < (*it)[0]->allPoints.size(); ++j)
        {
            for(unsigned int k=0; k < (*it)[1]->allPoints.size(); ++k)
            {
                for(unsigned int l=0; l < (*it)[2]->allPoints.size(); ++l)
                {
                    int i1 = pointMap[(*it)[0]->allPoints[j]];
                    int i2 = pointMap[(*it)[1]->allPoints[k]];
                    int i3 = pointMap[(*it)[2]->allPoints[l]];

                    tuples[i1*size*size + i2*size + i3] += 1;
                    tuples[i1*size*size + i3*size + i2] += 1;
                    tuples[i2*size*size + i1*size + i3] += 1;
                    tuples[i2*size*size + i3*size + i1] += 1;
                    tuples[i3*size*size + i1*size + i2] += 1;
                    tuples[i3*size*size + i2*size + i1] += 1;
                }
            }
        }
    }

    // Check if all triplets are accounted for
    bool res = true;
    int excess = 0;
    int max = 0;
    int missing = 0;
    int index = 0;
    for(int i=0; i < size; ++i)
    {
        for(int j=i+1; j < size; ++j)
        {
            for(int k=j+1; k < size; ++k)
            {
                index = i*size*size + j*size + k;
                if(tuples[index] > 1)
                {
                    excess += (tuples[index] - 1);
                }
                else if(tuples[index] == 0)
                {
                    missing++;
                }
                max = std::max(max, tuples[index]-1);

                res &= (tuples[index]>0);
            }
        }
    }

    // Report stats
    printf("There were %d duplicates and the largest one was %d.\n", excess, max);
    printf("Missing: %d.\n", missing);

    return res;
}
#endif //_WSSD_VALIDATION_

template class WssdValidator<double,2,2>;