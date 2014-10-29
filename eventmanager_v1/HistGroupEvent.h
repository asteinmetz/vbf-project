// -*- c++ -*-
#ifndef HISTGROUPEVENT_H
#define HISTGROUPEVENT_H

#include "HistGroup.h"

#include "fastjet/PseudoJet.hh"

#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TProfile2D.h"

#include <vector>

class HistGroupEvent : public HistGroupBase
{
public:
  /*! @brief Standard constructor */
  HistGroupEvent(const std::string& name="HistGroupEvent");
  /*! @brief Baseclass destructor */
  virtual ~HistGroupEvent();

  virtual void book();
  virtual void fill(Event& rEvt);
  virtual void fill(const std::vector<fastjet::PseudoJet>& vJet);

protected:

  bool accumulateAtVtx(std::vector<int>&    list,int vtx,int& nvtx);
  bool accumulateAtVtx(std::vector<double>& list,int vtx,int& nvtx,double val);

private:

  double _numberOfInteractions;

  /*! @brief Number of interactions */
  TH1D* h_mu;

  /*! @brief Particle properties */
  struct _particle
  {
    /*!@}*/
    /*! @name Inclusive single particle kinematics */
    /*!@{*/
    TH1D* h_pt;           /*!< @brief particle @f$p_{\rm T}@f$ */
    TH1D* h_rap;          /*!< @brief particle rapidity @f$y@f$ */
    TH1D* h_phi;          /*!< @brief particle azimuth @f$\phi@f$ */
    TH2D* d_pt_vs_rap;    /*!< @brief @f$p_{\rm T}@f$ versus @f$y@f$ */
    TH2D* d_phi_vs_rap;   /*!< @brief @f$\phi@f$ versus @f$y@f$ */
    /*!@}*/
    _particle() : h_pt((TH1D*)0)
		, h_rap((TH1D*)0)
		, h_phi((TH1D*)0)
		, d_pt_vs_rap((TH2D*)0)
		, d_phi_vs_rap((TH2D*)0)
    { }
  };

  struct _event
  {
    /*! @name Inclusive single particle multiplicities */
    /*!@{*/
    TH1D* h_multi;         /*!< @brief Number of particles per event */
    TH1D* h_multi_at_vtx;  /*!< @brief Number of particles per vertex */
    TH1D* h_multi_at_hardest_vtx;
    /*!@}*/
    
    /*! @name Inclusive particle flow observables */
    /*@{*/
    TH1D* h_met;                /*!< @brief event @f$E_{\rm T}^{\rm miss}@f$ */
    TH1D* h_set;                /*!< @brief event @f$\sigma E_{\rm T}@f$ */
    TH1D* h_set_at_vtx;         /*!< @brief vertex @f$\sigma E_{\rm T}@f$ */
    TH1D* h_set_at_hardest_vtx; /*!< @brief hardest vertex 
				  @f$\sigma E_{\rm T}@f$ */
   
    TH2D* d_set_vs_vtx;         /*!< @brief @f$\sigma E_{\rm T}@f$ versus 
				  @f$N_{\rm vtx}@f$ */
    /*!@}*/
    TH2D* d_vtx_vs_mu;              /*!< @brief @f$N_{\rm vtx}@f$ versus 
				      @f$\mu@f$ */
    TH2D* d_vtx_over_mu_vs_mu;      /*!< @brief @f$N_{\rm vtx}/\mu@f$ versus 
				      @f$\mu@f$ */
    TH2D* d_ptsum_vs_rap;           /*!< @brief event @f$p_{\rm T}(y)@f$*/
    TH2D* d_multisum_vs_rap;        /*!< @brief event 
				      @f$\langle N\rangle_{\rm part}(y)@f$*/
    TProfile2D* p_multisum_vs_rap_phi;
    TProfile2D* p_ptsum_vs_rap_phi; /*!< @brief event
				      @f$\langle p_{\rm T}\rangle(y,\phi)@f$*/
    _event() : h_multi((TH1D*)0)
	     , h_multi_at_vtx((TH1D*)0)
	     , h_multi_at_hardest_vtx((TH1D*)0)
	     , h_met((TH1D*)0)
	     , h_set((TH1D*)0)
	     , h_set_at_vtx((TH1D*)0)
	     , h_set_at_hardest_vtx((TH1D*)0)
	     , d_set_vs_vtx((TH2D*)0)
	     , d_vtx_vs_mu((TH2D*)0)
	     , d_vtx_over_mu_vs_mu((TH2D*)0)
	     , d_ptsum_vs_rap((TH2D*)0)
	     , d_multisum_vs_rap((TH2D*)0)
	     , p_ptsum_vs_rap_phi((TProfile2D*)0)
	     , p_multisum_vs_rap_phi((TProfile2D*)0)
    { }
  };

  struct _accumulators
  {
    /*! @name Temporary binned stores */
    /*@{*/
    TH1D* h_pt_vs_rap;
    TH2D* d_pt_vs_rap_phi;
    TH1D* h_multi_vs_rap;
    TH2D* d_multi_vs_rap_phi;
    /*@}*/
    _accumulators() : h_pt_vs_rap((TH1D*)0)
		    , d_pt_vs_rap_phi((TH2D*)0)
		    , h_multi_vs_rap((TH1D*)0)
		    , d_multi_vs_rap_phi((TH2D*)0)
    { }
    void reset() 
    { 
      if ( h_pt_vs_rap != 0 )        h_pt_vs_rap->Reset();
      if ( d_pt_vs_rap_phi != 0 )    d_pt_vs_rap_phi->Reset();
      if ( h_multi_vs_rap != 0 )     h_multi_vs_rap->Reset();
      if ( d_multi_vs_rap_phi != 0 ) d_multi_vs_rap_phi->Reset();
    }
    static void transfer(TH1* accHist,TH1* fillHist);
    static void transfer(TH2* accHist,TH2* fillHist);
  };

  _event        fEvent;
  _particle     fParticle;
  _accumulators fAccu;
};
/*! @class HistGroupEvent
 * 
 *  Manager and container for event level histograms.
 */

#endif
