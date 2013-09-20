/**
 * file: DataGen.h
 * desc: Generate some toy data to test with.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef __DATAGEN_H__
#define __DATAGEN_H__

#include <random>

enum Distribution
{
	kUniform=0,
	kNormal,
	//kKuzmin,
	//kPlane,
	//kCheckers,
    //kMoment,
    //kHelix,
    kSmallTestCase,

	kNumDistributions

};

static const char* names[kNumDistributions] =
{
	"uniform",
	"normal",
	//"kuzmin",
	//"plane",
	//"checkers",
    //"moment",
    //"helix",
    "testcase"
};

template<typename T>
class DataGen
{
private:

	// If available, this seeds the RNG with a truly random number
	std::random_device	rd;
	std::mt19937		randomEngine;

public:
		  DataGen();
	void  GenerateAllDataSets(int dim);
	void  GenerateDataSet(Distribution dist, int dim, int n, int iteration);
};

#endif //__DATAGEN_H__