// -*- C++ -*-
#ifndef __TEST_FILTER_ANALYSIS_HH__
#define __TEST_FILTER_ANALYSIS_HH__

#include "UserAnalysisBase.h"

#include <fastjet/Selector.hh>
#include <fastjet/tools/JetMedianBackgroundEstimator.hh>
#include "SubtractorWithMass.hh"  // for massless particles, use fastjet's Subtractor

#include <fastjet/tools/Filter.hh>

#include <string>
#include <map>

/// \class TestFilterAnalysis
/// illustration of how to put together all the "jet" tools
class TestFilterAnalysis : public UserAnalysisBase{
public:
  /// default ctor
  /// note that we force a PU server
  TestFilterAnalysis(const std::string &name,
                     DataFrame *signalServer,
                     DataFrame *pileupServer)
    : UserAnalysisBase(name, signalServer, pileupServer),
      _R(0.4), _jet_ptmin(250.0), _jet_rapmax(4.0), 
      _bge(0){}
  
  /// default dtor
  ~TestFilterAnalysis();

  //--------------------------------------------------
  // external control
  void set_jet_R(const double &R){
    _R = R;
  }
  void set_jet_ptmin(const double &jet_ptmin){
    _jet_ptmin = jet_ptmin;
  }
  void set_jet_rapmax(const double &jet_rapmax){
    _jet_rapmax = jet_rapmax;
  }

  //--------------------------------------------------
  // things overloaded from the base class
  virtual bool book();
  virtual bool analyze(Event& pEvt);
  virtual bool write();

protected:
  //--------------------------------------------------
  // "parameters"
  double _R;          ///< we'll cluster with anti-kt(R)
  double _jet_ptmin;  ///< minimal pt for the jets
  double _jet_rapmax; ///< maximal rapidity for the jets

  //--------------------------------------------------
  // histograms
  //
  // Note that the names HAVE TO BE RIGHT otherwise you'll only see it
  // at runtime!
  std::map<std::string, TH1D*> hists;
  std::map<std::string, TProfile*> profiles;

  //--------------------------------------------------
  // additional (mostly internal) things
  fastjet::JetDefinition _jet_def; ///< how jets are defined/clustered
  fastjet::Selector _sel_jets; ///< handy tool to apply cut on teh jets
  fastjet::AreaDefinition _area_def; ///< how jet area are ined
  fastjet::JetMedianBackgroundEstimator *_bge; ///< tool to facilitate rho&sigma estimation
  fastjet::SubtractorWithMass _subtractor;     ///< transformer to subtract PU from a jet
  fastjet::BackgroundRescalingYPolynomial _rescaling; ///< rapidity-dependent shape
  fastjet::Filter _filter; ///< the one filter we'll test
};


#endif  // __TEST_FILTER_ANALYSIS_HH__
