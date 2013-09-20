/**
 * file: FiltrationValidator.cpp
 *
 * Copyright 2013 Okke Schrijvers
 */

#include "Simplex.h"
#include "WSSD.h"
#include "FiltrationConstructor.h"

#include "FiltrationValidator.h"

template<typename T, int D>
bool FiltrationValidator<T,D>::ValidateFiltration(const Filtration<T,D>& filtration) const
{
    return TestPointers(filtration);
}


template<typename T, int D>
bool FiltrationValidator<T,D>::TestPointers(const Filtration<T,D>& filtration) const
{
    bool retVal = true;
    for(auto it = filtration.simplices.begin(); it != filtration.simplices.end(); ++it)
    {
        Simplex<T,D>* simplex = *it;
        for(auto it2 = simplex->GetBoundary().cbegin(); it2 != simplex->GetBoundary().cend(); ++it2)
        {
            Simplex<T,D>* face = *it2;
            if(face->GetParents().find(simplex) == face->GetParents().end())
            {
                retVal = false;
                printf("Parent of a boundary simplex not found.\n");
            }
        }
    }

    if(retVal)
    {
        printf("Pointers in the filtration are correct.\n");
    }

    return retVal;
}

template class FiltrationValidator<double,2>;