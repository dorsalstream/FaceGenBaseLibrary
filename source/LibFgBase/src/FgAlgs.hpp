//
// Copyright (c) 2015 Singular Inversions Inc. (facegen.com)
// Use, modification and distribution is subject to the MIT License,
// see accompanying file LICENSE.txt or facegen.com/base_library_license.txt
//
// Authors:     Andrew Beatty
// Created:     Feb 21, 2005
//
// General purpose little algorithms that are used broadly and don't fit readily into
// any other class/file.
//

#ifndef FGALGS_HPP
#define FGALGS_HPP

#include "FgStdLibs.hpp"
#include "FgTypes.hpp"
#include "FgStdVector.hpp"

// Useful for min/max with different types:
inline
uint64
fgMin(uint64 a,uint b)
{return std::min(a,uint64(b)); }

inline
uint64
fgMin(uint a,uint64 b)
{return std::min(uint64(a),b); }

// Useful for recursive template stub, 3-arg min/max, and when windows.h is included (has min/max macros):

template<class T>
const T &   fgMax(const T &x1,const T& x2)
{return std::max(x1,x2); }

template<class T> 
const T &  fgMax(const T &x1,const T &x2,const T &x3)
{return std::max(std::max(x1,x2),x3); }

template<class T> 
const T &  fgMax(const T &x1,const T &x2,const T &x3,const T &x4)
{return std::max(std::max(x1,x2),std::max(x3,x4)); }

template<class T>
const T &  fgMin(const T &x1,const T &x2)
{return std::min(x1,x2); }

template<class T>
const T &  fgMin(const T &x1,const T &x2,const T &x3)
{return std::min(std::min(x1,x2),x3); }

template<class T>
T
fgClamp(T val,T lo,T hi)
{
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

template <typename T>
inline T
fgSqr(T a)
{return (a*a); }

inline
bool
fgIsPow2(uint xx)
{return ((xx != 0) && ((xx & (xx-1)) == 0)); }

uint
fgNumLeadingZeros(uint32 xx);                                               // 0 returns 32.

inline uint fgLog2Floor(uint32 xx) {return (31-fgNumLeadingZeros(xx)); }    // Not valid for 0.
uint        fgLog2Ceil(uint32 xx);                                          // Not valid for 0.
inline uint fgPower2Floor(uint32 xx) {return (1 << fgLog2Floor(xx)); }      // Not valid for 0.
inline uint fgPower2Ceil(uint32 xx) {return (1 << fgLog2Ceil(xx)); }        // Not valid for 0.

// 1D convolution with zero-value boundary handling (non-optimized):
std::vector<double>
fgConvolve(
    const vector<double> &     data,
    const vector<double> &     kernel);    // Must be odd size with convolution centre in middle.

// 1D Gaussian convolution for large kernels (direct sampled kernel)
// with zero-value boundary handling:
std::vector<double>
fgConvolveGauss(
    const vector<double> &     in,
    double                          stdev); // Kernel stdev relative to sample spacing.

// Gives a relative different for similar values, maxes out at +/- 2 for very different values:
inline
double
fgRelDiff(double a,double b)
{return (a*b == 0) ? 0 : (b-a) * 2.0 / (std::abs(b)+std::abs(a)); }

vector<double>
fgRelDiff(const vector<double> & a,const vector<double> & b);

#endif