/**
 * file: Assert.h
 * desc: Debug asserts. This is a stub, expand if we want more advanced
 *       functionality like displaying debug messages.
 *
 * Copyright 2013 Okke Schrijvers
 */

#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <assert.h>

#define KILL_ON_ASSERT

// [OS] TODO: _DEBUG is microsoft-specific, we should include defines that handle the gcc
//      flags for debug builds.
#ifdef _DEBUG
    #ifdef KILL_ON_ASSERT
        #define ASSERT(exp) assert(exp)
        #define ASSERT_MSG(exp, msg) if(!(exp)) {printf(msg);} assert(exp)
    #else
        // Non-breaking versions
        #define ASSERT(exp) if(!(exp)) {printf("Error occurred in %s::%d.\n", __FILE__, __LINE__);}
        #define ASSERT_MSG(exp, msg) if(!(exp)) {printf(msg);} ASSERT(exp)
    #endif
#else //!_DEBUG
    #define ASSERT(exp) if(!(exp)) {printf("Error occurred in %s::%d.\n", __FILE__, __LINE__);}
    #define ASSERT_MSG(exp,msg)
#endif //_DEBUG

#endif //__ASSERT_H__
