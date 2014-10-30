// $Id$
//
// Copyright (c) 2013-, Matteo Cacciari, Gavin P. Salam and Gregory Soyez

#ifndef __RECLUSTER_HH__
#define __RECLUSTER_HH__

#include <fastjet/tools/Transformer.hh>
#include <fastjet/CompositeJetStructure.hh>
#include <fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh>

FASTJET_BEGIN_NAMESPACE   // put in Fj for simplicity

/// \class Recluster
/// Recluster the jet into subjets, returning a composite jet with the
/// subjet as its pieces
class Recluster : public Transformer{
public:
  Recluster(){};
  Recluster(const JetDefinition &jet_def) : _jet_def(jet_def){}

  /// class description
  virtual std::string description() const{
    return std::string("reclustering the jet constituents with ")+_jet_def.description();
  }

  /// access to the jet def used
  const JetDefinition jet_def() const{ return _jet_def;}

  /// the reclustering itselt
  virtual PseudoJet result(const PseudoJet & jet) const{
    // make sure the jet has constituents
    if (! jet.has_constituents())
      throw Error("Recluster only applies on jets with constituents");

    // if we have explicit ghosts, do the clustering keeping area info
    // TODO: make that more robust
    ClusterSequence *cs;
    if (jet.has_area()){
      std::vector<PseudoJet> particles, ghosts;
      SelectorIsPureGhost().sift(jet.constituents(), ghosts, particles);
      if (ghosts.size()) // keep area info
	cs = new ClusterSequenceActiveAreaExplicitGhosts(particles, _jet_def, ghosts, ghosts[0].area());
      else  // non-ghosted area... use regular clustering
	cs = new ClusterSequence(particles, _jet_def);
    } else  // no area info, use regular clustering
      cs = new ClusterSequence(jet.constituents(), _jet_def);

    // keep clustering info available
    PseudoJet result = join(cs->inclusive_jets());
    cs->delete_self_when_unused();

    return result;
  }


  /// the type of the associated structure
  typedef CompositeJetStructure StructureType;

protected:
  JetDefinition _jet_def;
};

FASTJET_END_NAMESPACE

#endif  // __RECLUSTER_HH__
