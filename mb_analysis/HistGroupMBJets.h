// -*- c++ -*- 
#ifndef HISTGROUPMBJETS_H
#define HISTGROUPMBJETS_H

#include "HistGroup.h"

class TH1D;
class TH2D;
class TH3D;
class TProfile;

#include <string>

class HistGroupMBJets : public HistGroupBase
{
public:

  HistGroupMBJets();
  HistGroupMBJets(const std::string& name,double jvfThreshold=0.5,
		  double coreRadius=0.2);
  virtual ~HistGroupMBJets();

  virtual void fill();
  virtual void fill(Event& pEvt);
  virtual void book();

private:
 
  double _jvfThreshold;
  double _coreRadius;

  class _JetPlotter
  {
  public:
    _JetPlotter(const std::string& name);
    virtual ~_JetPlotter();
    
    TH1D* h_multi;
    TH1D* h_pt;
    TH1D* h_rap;
    TH1D* h_jvf;
    TH1D* h_nvtx;
    TH1D* h_core;

    TH2D* d_phi_vs_rap;
    TH2D* d_pt_vs_rap;
    TH2D* d_jvf_vs_rap;
    TH2D* d_core_vs_rap;
    TH2D* d_nvtx_vs_rap;

    TH2D* d_jvf_vs_pt;
    TH2D* d_core_vs_pt;

    TH2D* d_pt_vs_nvtx;
    TH2D* d_jvf_vs_nvtx;
    TH2D* d_core_vs_nvtx;

    TH2D* d_core_vs_jvf;

    TH3D* z_jvf_vs_pt_rap;
    TH3D* z_core_vs_pt_rap;
    TH3D* z_nvtx_vs_pt_rap;
    
    TH3D* z_pt_vs_jvf_nvtx;
    TH3D* z_core_vs_jvf_nvtx;

    const std::string& name();

    void reset();
    bool isOk();

  private:

    bool        _isOk;
    std::string _name;
  };

  class _EventPlotter
  {
  public:
    _EventPlotter(const std::string& name);
    virtual ~_EventPlotter();

    TProfile* d_ptsum_vs_rap;
    TProfile* d_multisum_vs_rap;

    const std::string& name();

    void reset(bool everything=true);
    bool isOk();
    bool transfer(); 
    bool transfer(TH1D* tmp,TProfile* prof);
    
    TH1D* d_cache_ptsum_vs_rap;
    TH1D* d_cache_multisum_vs_rap;

  private:
    
    bool        _isOk;
    std::string _name;
  };

  _JetPlotter   _jetPlotter;
  _EventPlotter _evtPlotter;

  bool _isBooked;
};
#endif
