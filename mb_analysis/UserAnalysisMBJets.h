// -*- c++ -*-
#ifndef USERANALYSISMBJETS_H
#define USERANALYSISMBJETS_H

#include "UserAnalysisMB.h"

#include "HistGroup.h"
#include "HistGroupEvent.h"
#include "HistGroupContainer.h"

#include <vector>
#include <string>
#include <map>

#include <sstream>

class UserAnalysisMBJets : public UserAnalysisMB
{
public:

  UserAnalysisMBJets(const std::string& name,DataFrame* pileupServer);
  virtual ~UserAnalysisMBJets();

  virtual bool analyze(Event& pEvt);
  virtual bool book();

protected:

  virtual bool unpackConfig();
  virtual bool interpretCfg(const std::string& tag,std::string& vstr,
			    const std::string& cstr,double& rdat);
  virtual bool interpretCfg(const std::string& tag,std::string& vstr,
			    const std::string& cstr,
			    std::vector<double>& vdat);
private:

  typedef std::map<int,HistGroupContainer*> map_t;

  HistGroupEvent* _biasedPileupPlots;
  map_t           _jetPlots;
  map_t           _biasedJetPlots;

  typedef std::pair<int,int>        evtkey_t;
  typedef std::map<evtkey_t,Event*> evtmap_t;

  evtmap_t _eventStore;
  evtmap_t _biasedEventStore;
 
protected:

  /*! @name Jet event */
  /*!@{*/
  void fSetEventStore(Event* pEvt,
		      double rcat=0.,double pcat=0.,
		      double rprec=10.,double pprec=10.);
  void fSetEventStore(Event* pEvt,int rcat=0,int pcat=0);
  Event& fEventStore(double rcat=0.,double pcat=0.,
		     double rprec=10.,double pprec=10.);
  Event& fEventStore(int rcat=0,int pcat=0);
  void fSetEventParent(Event* parent);
  /*!@}*/

  /*! @name Biased jet event */
  /*!@{*/
  void fSetBiasedEventStore(Event* pEvt,
			    double rcat=0.,double pcat=0.,
			    double rprec=10.,double pprec=10.);
  void fSetBiasedEventStore(Event* pEvt,int rcat=0,int pcat=0);
  Event& fBiasedEventStore(double rcat=0.,double pcat=0.,
			   double rprec=10.,double pprec=10.);
  Event& fBiasedEventStore(int rcat=0,int pcat=0);
  void fSetBiasedEventParent(Event* parent);
  /*!@}*/

private:
  
  void   _fSetEventStore(evtmap_t& map,Event* pEvt,evtkey_t ekey);
  Event& _fEventStore(evtmap_t& map,evtkey_t ekey);
  void   _fSetParent(evtmap_t& map,Event* parent);

  /*! @name Particle level plots */
  /*!@{*/
  void fSetBiasedPileupPlots(HistGroupEvent* pGrp);
  HistGroupEvent& fBiasedPileupPlots();
  /*!@}*/

  void fSetJetPlots(HistGroupContainer* pGrp, double category=0.,
		    double prec=10.);
  void fSetJetPlots(HistGroupContainer* pGrpCont,int category=0);
 
  void fAddJetPlots(HistGroupBase* pGrp,double category=0.,
		    IFinalStateSelector* pSel=0,double prec=10.);
  void fAddJetPlots(HistGroupBase* pGrp,int category=0,
		    IFinalStateSelector* pSel=0);

  HistGroupContainer& fJetPlots(double category=0.,double prec=10.);
  HistGroupContainer& fJetPlots(int category=0);

  void fSetBiasedJetPlots(HistGroupContainer* pGrp, double category=0.,
			  double prec=10.);
  void fSetBiasedJetPlots(HistGroupContainer* pGrpCont,int category=0);
 
  void fAddBiasedJetPlots(HistGroupBase* pGrp,double category=0.,
			  IFinalStateSelector* pSel=0,double prec=10.);
  void fAddBiasedJetPlots(HistGroupBase* pGrp,int category=0,
			  IFinalStateSelector* pSel=0);

  HistGroupContainer& fBiasedJetPlots(double category=0.,double prec=10.);
  HistGroupContainer& fBiasedJetPlots(int category=0);

private:

  int cnvToInt(double val,double prec) { return (int)(val*prec); }

  static std::string s_rConeTag;
  static std::string s_rCoreTag;
  static std::string s_ptCutsTag;
  static std::string s_jvfThresholdTag;
  static std::string s_ptThresholdTag;

  std::string s_rCone;
  std::string s_rCore;
  std::string s_ptCuts;
  std::string s_jvfThreshold;
  std::string s_ptThreshold;

  std::vector<double> v_rCone;
  std::vector<double> v_rCore;
  std::vector<double> v_ptCuts;

  double _jvfThreshold;
  double _ptThreshold;

  void _fSetJetPlots(map_t& map,HistGroupContainer* pGrpCont,int category=0);
 
  void _fAddJetPlots(map_t& map,HistGroupBase* pGrp,int category=0,
		     IFinalStateSelector* pSel=0);

  HistGroupContainer& _fJetPlots(map_t& map,int category=0);

};

/////////////////
// Event Plots //
/////////////////

inline void UserAnalysisMBJets::fSetBiasedPileupPlots(HistGroupEvent* pGrp) 
{ _biasedPileupPlots = pGrp; }

inline HistGroupEvent& UserAnalysisMBJets::fBiasedPileupPlots() 
{ return *_biasedPileupPlots; }

//////////////////////////
// Unbiased event store //
//////////////////////////

inline void UserAnalysisMBJets::fSetEventStore(Event* pEvt,
					       double rcat,double pcat,
					       double rprec,double pprec)
{ fSetEventStore(pEvt,cnvToInt(rcat,rprec),cnvToInt(pcat,pprec)); }
inline void UserAnalysisMBJets::fSetEventStore(Event* pEvt,int rcat,int pcat)
{ _fSetEventStore(_eventStore,pEvt,evtkey_t(rcat,pcat)); }

inline Event& UserAnalysisMBJets::fEventStore(double rcat,double pcat,
					      double rprec,double pprec)
{ return fEventStore(cnvToInt(rcat,rprec),cnvToInt(pcat,pprec)); }
inline Event& UserAnalysisMBJets::fEventStore(int rcat,int pcat)
{ return _fEventStore(_eventStore,evtkey_t(rcat,pcat)); }

inline void UserAnalysisMBJets::fSetEventParent(Event* parent)
{ _fSetParent(_eventStore,parent); }

////////////////////////
// Biased event store //
////////////////////////

inline void UserAnalysisMBJets::fSetBiasedEventStore(Event* pEvt,
						     double rcat,double pcat,
						     double rprec,double pprec)
{ fSetBiasedEventStore(pEvt,cnvToInt(rcat,rprec),cnvToInt(pcat,pprec)); }
inline void UserAnalysisMBJets::fSetBiasedEventStore(Event* pEvt,int rcat,
						     int pcat)
{ _fSetEventStore(_biasedEventStore,pEvt,evtkey_t(rcat,pcat)); }

inline Event& UserAnalysisMBJets::fBiasedEventStore(double rcat,double pcat,
						    double rprec,double pprec)
{ return fBiasedEventStore(cnvToInt(rcat,rprec),cnvToInt(pcat,pprec)); }
inline Event& UserAnalysisMBJets::fBiasedEventStore(int rcat,int pcat)
{ return _fEventStore(_biasedEventStore,evtkey_t(rcat,pcat)); }

inline void UserAnalysisMBJets::fSetBiasedEventParent(Event* parent)
{ _fSetParent(_biasedEventStore,parent); }

//////////////////////////////////
// Unbiased jet plot management //
//////////////////////////////////

inline void UserAnalysisMBJets::fSetJetPlots(HistGroupContainer* pGrp,
					     double category,double prec)
{ fSetJetPlots(pGrp,cnvToInt(category,prec)); }
inline void UserAnalysisMBJets::fSetJetPlots(HistGroupContainer* pGrpCont,
					     int category)
{ _fSetJetPlots(_jetPlots,pGrpCont,category); }

inline void UserAnalysisMBJets::fAddJetPlots(HistGroupBase* pGrp,
					     double category,
					     IFinalStateSelector* pSel,
					     double prec)
{ fAddJetPlots(pGrp,cnvToInt(category,prec),pSel); }
inline void UserAnalysisMBJets::fAddJetPlots(HistGroupBase* pGrp,
					     int category,
					     IFinalStateSelector* pSel)
{ _fAddJetPlots(_jetPlots,pGrp,category,pSel); }

inline HistGroupContainer& UserAnalysisMBJets::fJetPlots(double category,
							 double prec) 
{ return fJetPlots(cnvToInt(category,prec)); }
inline HistGroupContainer& UserAnalysisMBJets::fJetPlots(int category)
{ return _fJetPlots(_jetPlots,category); }

////////////////////////////////
// Biased jet plot management //
////////////////////////////////

inline void UserAnalysisMBJets::fSetBiasedJetPlots(HistGroupContainer* pGrp,
					     double category,double prec)
{ fSetBiasedJetPlots(pGrp,cnvToInt(category,prec)); }
inline void 
UserAnalysisMBJets::fSetBiasedJetPlots(HistGroupContainer* pGrpCont,
				       int category)
{ _fSetJetPlots(_biasedJetPlots,pGrpCont,category); }

inline void UserAnalysisMBJets::fAddBiasedJetPlots(HistGroupBase* pGrp,
						   double category,
						   IFinalStateSelector* pSel,
						   double prec)
{ fAddBiasedJetPlots(pGrp,cnvToInt(category,prec),pSel); }
inline void UserAnalysisMBJets::fAddBiasedJetPlots(HistGroupBase* pGrp,
						   int category,
						   IFinalStateSelector* pSel)
{ _fAddJetPlots(_biasedJetPlots,pGrp,category,pSel); }

inline HistGroupContainer& UserAnalysisMBJets::fBiasedJetPlots(double category,
							       double prec) 
{ return fBiasedJetPlots(cnvToInt(category,prec)); }
inline HistGroupContainer& UserAnalysisMBJets::fBiasedJetPlots(int category)
{ return _fJetPlots(_biasedJetPlots,category); }

#endif
