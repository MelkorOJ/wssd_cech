/**
 * file: main.cpp
 * desc: Generate test data.
 *
 * Copyright 2013 Okke Schrijvers
 */

#include "DataGen.h"

int main(void)
{
	// Generate a dataset
	DataGen<double> dg;
	dg.GenerateDataSet(kSmallTestCase, 2, 200, 0);

	return 0;
}
