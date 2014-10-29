// -*- C++ -*-
#ifndef __HZ_ANALYSIS_HH__
#define __HZ_ANALYSIS_HH__

#include "UserAnalysisBase.h"

#include <fastjet/Selector.hh>
#include <fastjet/tools/JetMedianBackgroundEstimator.hh>
#include "SubtractorWithMass.hh"  // for massless particles, use fastjet's Subtractor
#include "MDTaggerWithSub.hh"

#include <fastjet/tools/Filter.hh>

/// \class HZAnalysis
/// BDRS Boosted H analysis, here in association with a Z->ll
///
/// Analysis description:
/// ---------------------
///
///  1. get the leptons 
///     this includes e's & mu's [with pt and |y| cut]
///
///  2. reconstruct a Z->ll
///     find the 2 opposite-sign, same-flavour leptons. Among all
///     possible pairs, take the one closest to the Z mass. Require
///     that the reconstructed Z has a mass between 80 and 100 GeV
///
///  3. impose no extra hard lepton above 30 GeV with |y|<2.5
///
///  4. check for a hard/fat jet
///     neutrinos are excluded
///     leptons are included (except the 2 leptons from the Z)
///     require a hard/fat jet
///
///  5. potentially apply a pre-filtering step
///     [analysis run with and without that step]
///
///  6. apply a mass drop tagger (default parameters)
///
///  7. apply a filter
///
/// Parameters (from command line)
/// ------------------------------
///   jet_R      initial clustering (anti-kt) radius  [1.2]
///   jet_ptmin  pt cut on the jets               [225 GeV]
///   jet_rapmax rapidity cut on the jets               [4]
///   emu_ptmin  pt cut on the leptons             [25 GeV]
///   emu_rapmax rapidity cut on the leptons          [2.5]
///
/// 
class HZAnalysis : public UserAnalysisBase{
public:
  /// default ctor
  /// note that we force a PU server
  HZAnalysis(const std::string &name,
                     DataFrame *signalServer,
                     DataFrame *pileupServer)
    : UserAnalysisBase(name, signalServer, pileupServer){
    set_jet_R();
    set_jet_ptmin();
    set_jet_rapmax();
    set_emu_ptmin();
    set_emu_rapmax();
    set_Z_massrange();

    _do_btagging = true;
  }
  
  /// default dtor
  ~HZAnalysis();

  //--------------------------------------------------
  // external control
  void set_jet_R(const double R=1.2){
    _R = R;
  }
  void set_jet_ptmin(const double jet_ptmin=220.0){
    _jet_ptmin = jet_ptmin;
  }
  void set_jet_rapmax(const double jet_rapmax=4.0){
    _jet_rapmax = jet_rapmax;
  }
  void set_emu_ptmin(const double emu_ptmin=30.0){
    _emu_ptmin = emu_ptmin;
  }
  void set_emu_rapmax(const double emu_rapmax=2.5){
    _emu_rapmax = emu_rapmax;
  }
  void set_Z_massrange(const double Z_massmin =  80.0,
                       const double Z_massmax = 100.0){
    _sel_Zmass = fastjet::SelectorMassRange(Z_massmin, Z_massmax);
  }

  //--------------------------------------------------
  // things overloaded from the base class
  virtual bool book();
  virtual bool analyze(Event& pEvt);

protected:
  //--------------------------------------------------
  // "parameters"
  double _R;          ///< we'll cluster with anti-kt(R)
  double _jet_ptmin;  ///< minimal pt for the jets
  double _jet_rapmax; ///< maximal rapidity for the jets
  double _emu_ptmin;  ///< minimal pt for the jets
  double _emu_rapmax; ///< maximal rapidity for the jets

  bool _do_btagging;  ///< impose to b tags on the MD prongs

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

  // a bunch of cuts
  fastjet::Selector _sel_visible;    ///< gets rid of neutrinos
  fastjet::Selector _sel_emus;       ///< cuts applied on the leptons
  fastjet::Selector _sel_Zmass;      ///< cuts applied on the Z mass
  fastjet::Selector _sel_extra_emus; ///< cuts applied on the extra leptons
                                     ///< (after Z reconstruction)
  fastjet::Selector _sel_jets;       ///< cuts applied on the jets

  fastjet::AreaDefinition _area_def; ///< how jet area are ined
  fastjet::JetMedianBackgroundEstimator *_bge; ///< tool to facilitate rho&sigma estimation
  fastjet::SubtractorWithMass _subtractor;     ///< transformer to subtract PU from a jet
  fastjet::BackgroundRescalingYPolynomial _rescaling; ///< rapidity-dependent shape

  fastjet::MDTaggerWithSub _tagger; ///< the tagger
  fastjet::Filter _prefilter; ///< an extra "prefiltering" step before BDRS
  fastjet::Filter _filter;    ///< the "BDRS" filter (applied after tagging)

  /// reconstruct the Z boson in the event from a given set of leptons
  ///
  /// We will follow the description in the BDRS paper: reconstruct a
  /// Z with 80<mass<100 GeV and pT>200 GeV [We'll use 225].
  ///
  /// We further impose that there are no lepton left with pt>30 GeV
  /// and |y|<2.5
  ///
  /// QUESTION: do we also impose the 30 GeV cut on the original list
  ///           of leptons or only after the reconstruction of the Z??
  ///           The paper seems to indicate the latter... so the latter
  ///           it will be
  ///
  /// Note that this will be done on the full event only
  bool _tagZ(std::vector<fastjet::PseudoJet> &leptons, 
             fastjet::Selector &_sel_Z);

  /// Given a set of hadrons and a cluster sequence, check for a fat
  /// jet. If there is one, apply a mass drop tagger and if this
  /// succeeds, filter the resulting tagged PseudoJet.
  ///
  ///   cs            the cluster sequence from which to get the jets
  ///                 (avoids multiple clusterings)
  ///   do_areas      apply PU subtraction wherever it's applicable
  ///   do_prefilter  before applying teh tagger, pre-filtere the jet
  ///
  /// Note that we do not impose the presence of no extra jet
  bool _processHadronic(const fastjet::ClusterSequence &cs,
			const std::string &name,
			bool do_areas=false, bool do_prefilter=false);

};


#endif  // __HZ_ANALYSIS_HH__
