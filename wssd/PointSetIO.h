/**
 * file: PointSetIO.h
 * desc: Read and write point sets with parameterized dimension and data type.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _POINT_SET_IO_H_
#define _POINT_SET_IO_H_

#include <iostream>
#include <fstream>
#include <vector>
#include "Vec.h"

enum FileMode
{
	kBinary = 0,
	kAscii  = 1,
};


/**
 * Class that provides functions to read and write point sets to bin/txt files.
 */
template< class T, int D > 
class PointSetIO
{
public:
	/**
	 * Writes point set to disk. Overwrites file if it already existed. Returns if the operation was successful.
	 */
	bool WriteToFile(const std::string& strFilename, const std::vector<vec<T,D>>& vPoints, FileMode mode = kBinary, bool bVerbose = true);
	

	/**
	 * Writes point set to disk. Clears vRetPoints. Returns if the operation was successful.
	 */
	bool ReadFromFile(const std::string& strFilename, std::vector<vec<T,D>>& vRetPoints, FileMode mode = kBinary, bool bVerbose = true);
};




template< class T, int D >
bool PointSetIO<T,D>::WriteToFile(const std::string& strFilename, const std::vector<vec<T,D>>& vPoints, FileMode mode, bool bVerbose)
{
	// Open file output stream
	std::ofstream file(strFilename.c_str(), std::ios::out | (mode == kBinary ? std::ios::binary : 0) );

	// Check if this was successful
	if(file.is_open())
	{
		// Iterate over all points
		for( std::vector<vec<T,D>>::const_iterator it = vPoints.begin(); it != vPoints.end(); ++it )
		{
			if(mode == kBinary)
			{
				// For binary files, write entire block at once
				file.write(reinterpret_cast<const char*>(&(*it)), sizeof(vec<T,D>));
			}
			else //mode == kAscii
			{
				// For text files, write each component and add spacing
				for( unsigned int dim = 0; dim < D; ++dim )
				{
					file << it->el[dim] << ' ';
				}
				file << std::endl;
			}
		}

		// All point saved, report and return
		if(bVerbose)
		{
			std::cout << "Wrote " << vPoints.size() << " points to " << (mode == kBinary ? "binary" : "ascii")
				<< " file: \"" << strFilename.c_str() << "\"." << std::endl;
		}
		return true;
	}
	else
	{
		// File could not be created/opened. Report and return false
		if(bVerbose)
		{
			std::cout << "File \"" << strFilename.c_str() << "\" could not be opened for writing. No points saved." << std::endl;
		}
		return false;
	}
}

template< class T, int D>
bool PointSetIO<T,D>::ReadFromFile(const std::string& strFilename, std::vector<vec<T,D>>& vRetPoints, FileMode mode, bool bVerbose)
{
	// Clear ret point set
	vRetPoints.clear();

	// Open file input stream
	std::ifstream file(strFilename.c_str(), std::ios::in | (mode == kBinary ? std::ios::binary : 0) );

	// Check if this was successful
	if(file.is_open())
	{
		vec<T,D> point;

		if(mode == kBinary)
		{
			// Get the size of the file
			file.seekg (0, std::ios::end);
			unsigned int size = unsigned int( file.tellg() );
			file.seekg (0, std::ios::beg);

			// Compute the number of points
			unsigned int pointSize = sizeof(vec<T,D>);
			unsigned int numPoints = size / pointSize;

			// Read all points
			for(unsigned int i = 0; i < numPoints; ++i)
			{
				file.read(reinterpret_cast<char*>(&point), pointSize);
				vRetPoints.push_back(point);
			}
		}
		else // mode == kAscii
		{
			// Loop until eof reached
			while(file && !file.eof())
			{
				// Load each component of the point
				for( unsigned int dim = 0; dim < D; ++dim )
				{
					file >> point[dim];
				}

				// Add to return points
				vRetPoints.push_back(point);
			}

			if(!file)
			{
				// Error while reading points
				if(bVerbose)
				{
					std::cout << "Something went wrong while reading file: \"" << strFilename.c_str() << "\"." << std::endl;
				}
				return true;
			}

			// eof flag only set after first read of non-existing point, so last point was added twice.
			//vRetPoints.pop_back();
		}

		// All points read, report and return
		if(bVerbose)
		{
			std::cout << "Read " << vRetPoints.size() << " points from file: \"" << strFilename.c_str() << "\"." << std::endl;
		}
		return true;
	}
	else
	{
		// File could not be opened, report and return
		if(bVerbose)
		{
			std::cout << "File \"" << strFilename.c_str() << "\" could not be opened for reading. No points loaded." << std::endl;
		}
		return false;
	}
}

#endif //_POINT_SET_IO_H_