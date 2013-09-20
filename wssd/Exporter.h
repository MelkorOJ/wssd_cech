/**
 * file: Exporter.h
 * desc: Exports a filtration to a text file.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef __EXPORTER_H__
#define __EXPORTER_H__

#include <string>

#include "Filtration.h"

template<typename T, int D>
class Exporter
{
public:
    /**
     * Saves to ASCII format. The format is:
     *
     *   alpha dimension vertex0 ... vertexd
     *
     * where 'alpha' is the alpha value at which the simplex is created, 'dimension' is
     * the dimension of the simplex, and vertex0, ..., vertexd are the d+1 0-simplices
     * that form the simplex. For dimension=0, there is not vertex0.
     */
    void Export(const Filtration<T,D>& filtration, const std::string& filename) const;

};


#endif //__EXPORTER_H__
