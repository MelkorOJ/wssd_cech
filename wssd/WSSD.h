/**
 * file: WSSD.h
 * desc: 
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef _WSSD_H_
#define _WSSD_H_

#include "WellSeparatedTuple.h"

/**
 * Recursive template meta programming implementation of WSSD.
 */
template<typename T, int D, int K=D>
class WSSD : public WSSD<T,D,K-1>
{
// Fields
protected:
    KWSSD(T,D,K) kWssd;

// Constructor
public:
    WSSD(double eta) : WSSD<T,D,1>(eta) {}

// Methods
public:
    template<int KK>
    KWSSD(T,D,KK)& GetKWssd() { return WSSD<T,D,KK>::kWssd; }
    template<int KK>
    const KWSSD(T,D,KK)& GetKWssd() const { return WSSD<T,D,KK>::kWssd; }

    void clear() { kWssd.clear(); WSSD<T,D,K-1>::clear(); }
};

/**
 * Base implementation of WSSD.
 */
template<typename T, int D>
class WSSD<T,D,1>
{
// Fields
protected:
    double       eta;
    KWSSD(T,D,1) kWssd;

// Constructor
public:
    WSSD(double eta) : eta(eta) {}

// Methods
public:
    // [OS] I don't know how to get rid of the duplication of this
    //      and the implementation in the base class.
    template<int KK>
    KWSSD(T,D,1)& GetKWssd() { return kWssd; }
    template<int KK>
    const KWSSD(T,D,1)& GetKWssd() const { return kWssd; }

    void clear() { kWssd.clear(); }
};

// [OS] find a way to not do this
template<typename T, int D>
class WSSD<T,D,0>
{};

#endif //_WSSD_H_
