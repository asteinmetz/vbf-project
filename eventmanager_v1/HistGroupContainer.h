// -*- c++ -*- //
#ifndef HISTGROUPCONTAINER_H
#define HISTGROUPCONTAINER_H

#include "IFinalStateSelector.h"
#include "HistGroup.h"

class HistGroupContainer : public HistGroupBase
{
 public: 
  HistGroupContainer(const std::string& name="HistGroupContainer");
  virtual ~HistGroupContainer();

  virtual void fill();
  virtual void fill(Event& pEvt);
  virtual void fill(const std::vector<fastjet::PseudoJet>& vJet);

  bool addEventPlots(HistGroupBase* pGrp);
  bool addJetPlots(HistGroupBase* pGrp,IFinalStateSelector* pSel=0);

  virtual void write();
  virtual void print();
  virtual void book();

  virtual size_t size(Event& pEvt) const;
  virtual size_t size(const std::vector<fastjet::PseudoJet>& vJet) const;

 private:

  typedef std::pair<HistGroupBase*,IFinalStateSelector*> filler_t;
  std::vector<filler_t>       _jetPlots;
  std::vector<HistGroupBase*> _eventPlots;
};

inline bool HistGroupContainer::addEventPlots(HistGroupBase* pGrp)
{ _eventPlots.push_back(pGrp); return true; }

inline bool HistGroupContainer::addJetPlots(HistGroupBase* pGrp, 
					    IFinalStateSelector* pSel)
{ _jetPlots.push_back(filler_t(pGrp,pSel)); }
#endif
