/**
 * file: WssdStats.cpp
 *
 * Copyright 2013 Okke Schrijvers
 */

#include "WssdStats.h"

template<typename T, int D, int K>
void WssdStats<T,D,K>::PrintStats(const KWSSD(T,D,K)& wspd) const
{
    double maxAlpha = 1.5*double(1<<20);
    int steps = 1000;

    PrintAlphaStats(wspd, steps, maxAlpha);
}


template<typename T, int D, int K>
void WssdStats<T,D,K>::PrintAlphaStats(const KWSSD(T,D,K)& wspd, int steps, double maxAlpha) const
{
    int* counts = new int[steps];

    memset(counts, 0, steps*sizeof(int));

    vec<T,D> mid;
    double   diam;
    double   alpha;

    for(KWSSD(T,D,K)::const_iterator it = wspd.cbegin(); it != wspd.cend(); ++it)
    {
        it->MidPointAndDiam(mid, diam);

        for(int i=0; i < steps; ++i)
        {
            alpha = maxAlpha*(double(i+1)/steps);
            counts[i] += (diam <= alpha);
        }
    }

    for(int i=0; i < steps; ++i)
    {
        printf("%f, %d\n", maxAlpha*(double(i+1)/steps), counts[i]);
    }
}

template class WssdStats<double,2,1>;
template class WssdStats<double,2,2>;