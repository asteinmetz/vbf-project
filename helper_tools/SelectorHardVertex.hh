// Copyright (c) 2013-, Gregory Soyez
//
// provides a selector to keep only particles coming from the hard
// interaction
//
// This uses Peter's UserInfo associated with the PseudoJet

#ifndef __SELECTOR_HARD_VERTEX_H__
#define __SELECTOR_HARD_VERTEX_H__

#include <fastjet/Selector.hh>
#include "../event_manager_v0/Event.h"  // probably needs to be checked...

FASTJET_BEGIN_NAMESPACE

/// \class SelectorWorkerHardVertex
/// the worker that really does the job in the shadow of the selector
class SelectorWorkerHardVertex : public SelectorWorker{
public:
  /// default ctor
  SelectorWorkerHardVertex(){}

  /// decide whether a particle passes the selection criterion
  /// (i.e. comes from the hard vertex)
  ///
  /// if the particle does not have the correct user info, just return
  /// false
  virtual bool pass(const PseudoJet &particle) const{
    return particle.has_user_info<ParticleInfo>() &&
      particle.user_info<ParticleInfo>().interactionType() == Vertex::SIGNAL;
  }

  /// the description of wht this selector does
  virtual std::string description() const{
    return "particle coming from the hard interaction";
  }
};

/// a Selector for particles coming from the hard interaction
Selector SelectorHardVertex(){
  return new SelectorWorkerHardVertex();
}

FASTJET_END_NAMESPACE


#endif // __SELECTOR_HARD_VERTEX_H__
