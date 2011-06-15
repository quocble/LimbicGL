/*******************************************************************************
    Copyright (c) 2011, Limbic Software, Inc.
    All rights reserved.
    This code is subject to the Google C++ Coding conventions:
         http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
 ******************************************************************************/
#ifndef LIMBICGL_CONFIG_H_
#define LIMBICGL_CONFIG_H_

// This can be used to run gamecenter in a separate GCD queue. This is for testing, and doesn't really seem to have any effect on the stuttering.
//#define GAMECENTER_WITH_GCD


#define SINGLETHREADEDDRIVER 0
#define GCDDRIVER 1
#define THREADEDDRIVER 2
// Specifies with Driver to use
#define DRIVER THREADEDDRIVER

// This can be used to get very verbose traces for debugging
//#define VERBOSE_LOG

#ifdef VERBOSE_LOG
#   define VerboseLog(fmt, ...) NSLog((@"%s " fmt), __PRETTY_FUNCTION__, ##__VA_ARGS__);
#else
#   define VerboseLog(...)
#endif


#endif  // LIMBICGL_CONFIG_H_
