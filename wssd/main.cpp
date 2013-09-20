/**
 * file: main.cpp
 * Copyright 2013 Okke Schrijvers
 */

#include "PointSetIO.h"

#include "Quadtree.h"
#include "QuadtreeConstructor.h"
#include "QuadtreeValidator.h"
#include "QuadtreeStats.h"

#include "WSSD.h"
#include "WspdConstructor.h"
#include "WspdValidator.h"

#include "WssdConstructor.h"
#include "WssdValidator.h"
#include "WssdStats.h"


#include "Filtration.h"
#include "FiltrationConstructor.h"
#include "FiltrationValidator.h"

#include "Exporter.h"

int main()
{
    // Test settings
    const int dimension = 2;
    typedef double T;
    const double eps = .1;
    const double eta = eps / 5.0;
    const double wspdEta = eta / 2.0;

    const int maxDelta = 200;
    const double maxAlpha = std::numeric_limits<double>::infinity();
    //const double maxAlpha = (1.0 + (2.0/3.0)*eps)*pow(1.0 + eps, maxDelta);


    // Filename and point set declaration
    //char* fileName = "D:/Downloads/equi_triangle.txt";
    char* fileName = "../data/testcase_2_200_0.bin";
    std::vector<vec<T,dimension>> points;
    
    // Read a dataset
	if( PointSetIO<T,dimension>().ReadFromFile(fileName, points) )
    {
        // Construct the quadtree
		QuadtreeConstructor<T, dimension> constructor;
		Quadtree<T, dimension>* quadtree = constructor.ConstructQuadtree(points);

        // Compress the quadtree
        constructor.CompressQuadtree(quadtree);

        // Validate quadtree
        //CompressedQuadtreeValidator<T,dimension> quadtreeValidator;
		//quadtreeValidator.ValidateQuadtree(quadtree, points);

        // Report quadtree stats
        //CompressedQuadtreeStats<T,dimension> quadtreeStats;
        //quadtreeStats.QuadtreeStats(quadtree);

        WSSD<T,dimension> wssd(eta);

        // Create WSPD
        WspdConstructor<T,dimension> wpsdConstructor(wspdEta, maxAlpha);
        wpsdConstructor.ConstructWspd(quadtree, wssd.GetKWssd<1>());

        printf("Number of 1-WSSD pairs: %d\n\n", wssd.GetKWssd<1>().size());

        //// Print stats of the (eta,1)-WSSD
        //WssdStats<T,dimension,1> wspdStats;
        //wspdStats.PrintStats(wspdTuples);

        //// Validate the (eta,1)-WSSD
        //printf("Starting (eta,1)-WSSD validation. Takes quadratic time/space, so might be slow.\n");
        //WspdValidator<T,dimension> wspdValidator;
        //wspdValidator.ValidateWspd(wssd.GetKWssd<1>(), points);
        

        // Construct (eta,2)-WSSD
        WssdConstructor<T,dimension,2> wssd2Constructor(eta, maxAlpha);
        wssd2Constructor.ConstructWssd(wssd.GetKWssd<1>(), quadtree, wssd.GetKWssd<2>());

        printf("Number of 2-WSSD tuples: %d\n\n", wssd.GetKWssd<2>().size());

        //// Print stats of the (eta,2)-WSSD
        //WssdStats<T,dimension,2> wssdStats;
        //wssdStats.PrintStats(wssd2Tuples);

        //// Validate (eta,2)-WSSD
        //printf("Starting (eta,2)-WSSD validation. Takes cubic time/space, so might be slow.\n");
        //WssdValidator<T,dimension,2> wssd2Validator;
        //wssd2Validator.ValidateWssd(wssd.GetKWssd<2>(), points);

        // Construct the filtration
        printf("Constructing filtration.\n");
        Filtration<T,dimension> filtration;
        FiltrationConstructor<T,dimension> filtrationConstructor(eps, 0, maxDelta);
        filtrationConstructor.ConstructFiltration(wssd, filtration);

        // Validate that the filtration has the correct setup
        //FiltrationValidator<T,dimension> filtValidator;
        //filtValidator.ValidateFiltration(filtration);

        printf("Writing to file.\n");
        Exporter<T,dimension> exporter;

        char outputFile[120];
        sprintf_s(outputFile, "filtration_%f.txt", eps);  
        exporter.Export(filtration, outputFile);

        printf("Press enter to continue...\n");
        getchar();

        delete quadtree;

        std::cout << "Leaving program." << std::endl;

		// everything's okay
		return 0;
	}

    // couldn't read input file
    printf("Couldn't read point set \"%s\"\n", fileName);
    printf("Press enter to continue...\n");
    getchar();

	return -1;
}
