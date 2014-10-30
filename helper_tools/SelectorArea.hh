// Copyright (c) 2013-, Gregory Soyez

#ifndef __SELECTOR_AREA_HH__
#define __SELECTOR_AREA_HH__

#include <fastjet/Selector.hh>
#include <fastjet/tools/BackgroundEstimatorBase.hh>

FASTJET_BEGIN_NAMESPACE

/// select objects that carry at least a fraction "fraction" of the reference jet
/// (Note that this selectir takes a reference)
Selector SelectorAreaRhoThreshold(BackgroundEstimatorBase *bge, double crho, double csigma);

FASTJET_END_NAMESPACE

#endif // __SELECTOR_AREA_HH__
