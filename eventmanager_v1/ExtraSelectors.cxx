#include "ExtraSelectors.hh"
#include "Event.h"
#include <sstream>

using namespace fastjet;

//------------------------------------------------------------------------
/// \class SW_HardVertex
/// the worker that really does the job in the shadow of the selector
class SW_HardVertex : public SelectorWorker{
public:
  /// default ctor
  SW_HardVertex(){}

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
  return Selector(new SW_HardVertex());
}


//------------------------------------------------------------------------
/// \class SW_AbsId
/// the worker that really does the job in the shadow of the selector
class SW_AbsId : public SelectorWorker{
public:
  /// ctor with specification of
  ///   \param id    the id we'll match
  SW_AbsId(unsigned int id) : _id(id){}

  /// return true if the jet carries a large enough fraction of the reference.
  /// Throw an error if the reference is not initialised.
  virtual bool pass(const PseudoJet & particle) const {
    return particle.has_user_info<ParticleInfo>() &&
      abs(particle.user_info<ParticleInfo>().id()) == _id;
  }
  
  /// returns a description of the worker
  virtual std::string description() const {
    std::ostringstream oss;
    oss << "particles with |id|=" << _id;
    return oss.str();
  }

protected:
  unsigned int _id;
};

Selector SelectorAbsId(const unsigned int id){
  return Selector(new SW_AbsId(id));
}
