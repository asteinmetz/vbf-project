// -*- C++ -*-
//
// Copyright (c) 2005-2013, Gavin P. Salam and Gregory Soyez
//
/// little tool to count the net b contents from a PDGid
#ifndef __BTAG_HH__
#define __BTAG_HH__

#include <fastjet/FunctionOfPseudoJet.hh>

/// \class SimpleBTagger
/// simple class for b tagging
///
/// this class only works if the B mesions/hadrons are left stable. It
/// is constructed given a tag probability and a fake rate.
///
/// For a given jet, it looks at the net b count in that jet and return a weight for it to be b-tagger.
class SimpleBTagger : public fastjet::FunctionOfPseudoJet<double>{
public:
  /// ctor with initialisation
  ///   \param btag_proba  the probability to tag a b-jet as such
  ///   \param fake_proba  the probability to mistag a non-b jet (as a b jet)
  SimpleBTagger(double btag_proba=1.0, double fake_proba=0.0) : _btag_proba(btag_proba), _fake_proba(fake_proba){}

  /// get the b-tag probability for a given jet
  ///
  /// Note that if the jet has no particle, we assume we have a single
  /// particle and aplpy the b-tagging to it.
  double result(const fastjet::PseudoJet &jet) const;

  /// description of teh tagger
  std::string description() const;

private:
  /// given a PDGid, return the net b count in that particle
  int _b_count_from_particle_id(int id) const;

  double _btag_proba, _fake_proba;
};

#endif  // __BTAG_HH__
