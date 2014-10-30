// Copyright (c) 2005-2013, Matteo Cacciari, Gavin P. Salam and Gregory Soyez

#ifndef __MASS_DROP_TAGGER_WITH_SUBTRACTION_HH__
#define __MASS_DROP_TAGGER_WITH_SUBTRACTION_HH__

#include <fastjet/tools/Transformer.hh>
#include <fastjet/LimitedWarning.hh>
#include <fastjet/WrappedStructure.hh>

FASTJET_BEGIN_NAMESPACE

class MDTaggerWithSub;
class MDTaggerWithSubStructure;

//----------------------------------------------------------------------
/// \class MDTaggerWithSub
/// same as the mass drop tagger in FJ3 but with potential support for
/// a subtractor that subtracts the jets 4-momenta before applying the
/// MD conditions
class MDTaggerWithSub : public Transformer{
public:
  /// default ctor
  MDTaggerWithSub(const double mu=0.67, const double ycut=0.09) : _mu(mu), _ycut(ycut), _subtractor(0){};

  /// set the subtractor to be used
  void set_subtractor(Transformer *subtractor){ _subtractor = subtractor; }

  /// returns a textual description of the tagger
  virtual std::string description() const;

  /// runs the tagger on the given jet and
  /// returns the tagged PseudoJet if successful, a PseudoJet==0 otherwise
  /// (standard access is through operator()).
  ///  \param jet   the PseudoJet to tag
  virtual PseudoJet result(const PseudoJet & jet) const;

  /// the type of the associated structure
  typedef MDTaggerWithSubStructure StructureType;

protected:
  Transformer *_subtractor;
  double _mu, _ycut;
  static LimitedWarning _warnings_nonca;
};


//------------------------------------------------------------------------
/// @ingroup tools_taggers
/// \class MDTaggerWithSubStructure
/// the structure returned by the MDTaggerWithSub transformer.
///
/// See the MDTaggerWithSub class description for the details of what
/// is inside this structure
///
class MDTaggerWithSubStructure : public WrappedStructure{
public:
  /// ctor with initialisation
  ///  \param pieces  the pieces of the created jet
  ///  \param rec     the recombiner from the underlying cluster sequence
  MDTaggerWithSubStructure(const PseudoJet & result_jet) :
    WrappedStructure(result_jet.structure_shared_ptr()), _mu(0.0), _y(0.0){}

  /// returns the mass-drop ratio, pieces[0].m()/jet.m(), for the splitting
  /// that triggered the mass-drop condition
  inline double mu() const{return _mu;}

  /// returns the value of y = (squared kt distance) / (squared mass) for the
  /// splitting that triggered the mass-drop condition
  inline double y() const {return _y;}

protected:
  double _mu;              ///< the value of the mass-drop parameter
  double _y;               ///< the value of the asymmetry parameter

  // allow the tagger to set these
  friend class MDTaggerWithSub;
};



FASTJET_END_NAMESPACE

#endif  //  __MASS_DROP_TAGGER_WITH_SUBTRACTION_HH__
