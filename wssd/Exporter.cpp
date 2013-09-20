/**
 * file: Exporter.cpp
 * 
 * Copyright 2013 Okke Schrijvers
 */

#include <iostream>
#include <fstream>

#include "Simplex.h"
#include "Exporter.h"

template<typename T, int D>
void Exporter<T,D>::Export(const Filtration<T,D>& filtration, const std::string& filename) const
{
    // Open ASCII file
    std::ofstream outputFile;
    outputFile.open(filename);

    if(outputFile.is_open())
    {
        Simplex<T,D>* simplex = NULL;

        // Go over all simplices in order
        for(unsigned int i=0; i < filtration.simplices.size(); ++i)
        {
            simplex = filtration.simplices[i];
            
            outputFile << (i+1) << " ";

            // Write function value
            outputFile << (simplex->GetFunctionValue()/2.0) << " ";

            if(simplex->GetK() == 0)
            {
                // Write dimension
                outputFile << "0";
            }
            else
            {
                // Write dimension
                outputFile << simplex->GetK();

                // Write vertices
                for(int j=0; j<simplex->GetK()+1; ++j)
                {
                    outputFile << " " << simplex->GetBoundarySimplex(j)->GetIndex();
                }
            }

            outputFile << std::endl;
        }

        outputFile.close();
    }
    else
    {
        printf("File couldn't be opened. Did not save.\n");
    }
}

template class Exporter<double,2>;