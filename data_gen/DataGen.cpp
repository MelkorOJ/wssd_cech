/**
 * file: DataGen.cpp
 * desc: Generate some toy data to test with.
 *
 * Copyright 2013 Okke Schrijvers
 */

#define _USE_MATH_DEFINES
#include <math.h>


#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include "DataGen.h"

template< typename T >
DataGen<T>::DataGen()
:	rd()
,	randomEngine(rd())
{}

template< typename T >
void DataGen<T>::GenerateAllDataSets(int dim)
{
	for(int n = 1<<13; n <= 1<<22; n=n<<1)
    {
		for(int dist = 0; dist < kNumDistributions; ++dist)
		{
            for(int it = 0; it < 10; ++it)
			{
                GenerateDataSet(Distribution(dist), dim, n, it);
		    }
        }
	}
}

template< typename T >
void DataGen<T>::GenerateDataSet(Distribution dist, int dim, int n, int iteration)
{
	FILE* fout;
	char  filename[128];
	T     val;

    // Open file
    // [OS] I think _s is microsoft specific, if this is the case, we should aim to find
    //      platform independent safe way of doing this, or accepting that we're
    //      vulnerable to exploits.
    sprintf_s(filename, "%s_%i_%i_%i.bin", names[dist], dim, n, iteration);
    fopen_s(&fout, filename, "wb");
	
	if( fout )
	{
		printf("Generating: %s\n", filename);
		switch(dist)
		{
		case kUniform:
			{
				std::uniform_real_distribution<T> rng(0.0, T(1<<20));

				for(int i=0; i < n; ++i)
				{
					for(int d=0; d<dim; ++d)
					{
						val = rng(randomEngine); 
						fwrite(&val, 1, sizeof(T), fout);
					}
				}
			}
			break;
		case kNormal:
			{
				std::normal_distribution<T> rng(T(1<<19), T(1<<13));

				for(int i=0; i < n; ++i)
				{
					for(int d=0; d<dim; ++d)
					{
						val = rng(randomEngine); 
						fwrite(&val, 1, sizeof(T), fout);
					}
				}
			}
			break;
		//case kKuzmin:
		//	{
		//		float radius, random;
		//		float rsq;
		//		float fac;
		//		float maximum = 0.0f;
		//		float* xl = new float[n];
		//		float* yl = new float[n];
		//		float* zl = new float[n];

		//		for(int i=0; i < n; ++i)
		//		{
		//			// Determine radius at which to place point
		//			random = GetRandomFloat();
		//			radius = sqrtf(1/(random*random) - 1);

		//			// Find uniform dist point in sphere
		//			do {
		//				xl[i] = GetRandomFloatAroundZero();
		//				yl[i] = GetRandomFloatAroundZero();
		//				zl[i] = GetRandomFloatAroundZero();
		//				rsq = xl[i]*xl[i] + yl[i]*yl[i] + zl[i]*zl[i];
		//			} while(rsq >= 1.0f || rsq == 0.0f);
		//			
		//			// Rescale to put at radius
		//			fac = radius/sqrtf(rsq);
		//			xl[i] *= fac;
		//			yl[i] *= fac;
		//			zl[i] *= fac;

		//			maximum = std::max(maximum, fabs(xl[i]));
		//			maximum = std::max(maximum, fabs(yl[i]));
		//			maximum = std::max(maximum, fabs(zl[i]));
		//		}

		//		// Rescale all point so they fit in [-1,1)
		//		for(int i=0; i < n; ++i)
		//		{
		//			x = xl[i]/maximum;
		//			y = yl[i]/maximum;
		//			z = zl[i]/maximum;
		//			WriteToFile(&x, &y, &z, fout);
		//		}

		//		delete[] xl;
		//		delete[] yl;
		//		delete[] zl;
		//	}
		//	break;
		//case kPlane:
		//	{
		//		const float b = 0.001f;
		//		for(int i=0; i < n; ++i)
		//		{
		//			x = GetRandomFloatAroundZero();
		//			y = GetRandomFloatAroundZero();
		//			z = GetRandomFloat();
		//			z = -1.0f + (2*b)/(z - b*z + z);
		//			WriteToFile(&x, &y, &z, fout);
		//		}
		//	}
		//	break;
		//case kCheckers:
		//	{
		//		for(int i=0; i < n; ++i)
		//		{
		//			x = GetRandomFloatAroundZero();
		//			y = GetRandomFloatAroundZero();
		//			z = GetRandomFloat();

		//			if( ((int(x*8) + int(y*8)) % 2) != (int(z*4) % 2))
		//			{
		//				z = -z;
		//			}

		//			if(GetRandomFloat() < 0.125)
		//			{
		//				x = -x;
		//				y = -y;
		//				z = -z;
		//			}

		//			WriteToFile(&x, &y, &z, fout);
		//		}
		//	}
		//	break;
  //      case kMoment:
  //          {
  //              for(int i=0; i < n; ++i)
  //              {
  //                  t = GetRandomFloat();
  //                  x = float(t * 2.0 - 1.0);
  //                  y = float(t*t * 2.0 - 1.0);
  //                  z = float(t*t*t * 2.0 - 1.0);
  //                  WriteToFile(&x, &y, &z, fout);
  //              }
  //          }
  //          break;
  //      case kHelix:
  //          {
  //              float sqrtn = sqrtf(n);
  //              for(int i=0; i < n; ++i)
  //              {
  //                  x = float((2.0 * kPi * i)/n);
  //                  y = float(cos(2.0 * kPi * i / sqrtn) );
  //                  z = float(sin(2.0 * kPi * i / sqrtn) );
  //                  WriteToFile(&x, &y, &z, fout);
  //              }
  //          }
  //          break;
        case kSmallTestCase:
            {
                std::uniform_real_distribution<double> rng(-M_PI, M_PI);

                T x, y;

                for(int r = 0; r<2; r++)
                {
                    for(int i=0; i < n/2; ++i)
                    {
                        val = rng(randomEngine);
                        x = (1<<19) + (1<<18)*(0.9+r/10.0)*sin(val);
                        y = (1<<19) + (1<<18)*(0.9+r/10.0)*cos(val);
                        fwrite(&x, 1, sizeof(T), fout);
                        fwrite(&y, 1, sizeof(T), fout);
                    }
                }
            }
            break;
		case kNumDistributions:
		default:
			{
				printf("Invalid distribution.\n");
			}
		}
	}
	else
	{
		printf("Could not open or create: %s.\n", filename);
	}
	// Close file
	fclose(fout);

    printf("Done\n");
    getchar();
}

template class DataGen<double>;