// Copyright (c) 2005-2013, Matteo Cacciari, Gavin P. Salam and Gregory Soyez

#include "Recluster.hh"
#include "MDTaggerWithSub.hh"
#include <fastjet/ClusterSequence.hh>
#include <sstream>

FASTJET_BEGIN_NAMESPACE

LimitedWarning MDTaggerWithSub::_warnings_nonca;

using namespace std;

//----------------------------------------------------------------------
// MDTaggerWithSub class implementation
//----------------------------------------------------------------------

//------------------------------------------------------------------------
// description of the tagger
string MDTaggerWithSub::description() const{ 
  ostringstream oss;
  oss << "MDTaggerWithSub with mu=" << _mu << " and ycut=" << _ycut;
  return oss.str();
}

//------------------------------------------------------------------------
// returns the tagged PseudoJet if successful, 0 otherwise
//  - jet   the PseudoJet to tag
PseudoJet MDTaggerWithSub::result(const PseudoJet & jet) const{
  // recluster the jet with Cambridge/Aachen,
  //
  // Note that this returns a composite jet... but with our huge
  // radius, we'll assume there is a single piece
  PseudoJet j = Recluster(JetDefinition(cambridge_algorithm, 999.0))(jet).pieces()[0];

  // get its subtracted version if subtraction is switched on
  PseudoJet s = (_subtractor) ? (*_subtractor)(j) : j;
  
  PseudoJet j1, j2; // the raw version
  PseudoJet s1, s2; // the subtracted versions
  bool had_parents;

  // we just ask that we can "walk" in the cluster sequence.
  // appropriate errors will be thrown automatically if this is not
  // the case
  while ((had_parents = j.has_parents(j1,j2))) {
    s1 = (_subtractor) ? (*_subtractor)(j1) : j1;
    s2 = (_subtractor) ? (*_subtractor)(j2) : j2;

    // make parent1 the more massive jet
    if (s1.m2() < s2.m2()){ std::swap(s1,s2); std::swap(j1,j2);}

    // if we pass the conditions on the mass drop and its degree of
    // asymmetry (kt_dist/m^2 > rtycut [where kt_dist/m^2 \sim
    // z/(1-z)), then we've found something interesting, so exit the
    // loop
    if ( (s1.m2() < _mu*_mu*s.m2()) && (s1.kt_distance(s2) > _ycut*s.m2()) ){
      break;
    } else {
      j = j1; s = s1;
    }
  }
    
  if (!had_parents)
    // no Higgs found, return an empty PseudoJet
    return PseudoJet();

  // create the result and its structure
  PseudoJet result_local = s;
  MDTaggerWithSubStructure * st = new MDTaggerWithSubStructure(result_local);
//  s->_original_jet = jet;
  st->_mu = (s.m2()!=0.0) ? sqrt(s1.m2()/s.m2()) : 0.0;
  st->_y  = (s.m2()!=0.0) ? s1.kt_distance(s2)/s.m2() : 0.0;

  result_local.set_structure_shared_ptr(SharedPtr<PseudoJetStructureBase>(st));

  return result_local;
}

FASTJET_END_NAMESPACE

