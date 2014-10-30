// -*- c++ -*-
#ifndef USERANALYSISMB_H
#define USERANALYSISMB_H

#include "UserAnalysisBase.h"

class UserAnalysisMB : public UserAnalysisBase
{
public:

  UserAnalysisMB(const std::string& name,DataFrame* pileupServer);
  virtual ~UserAnalysisMB();

  //  virtual bool process();
  virtual bool analyze(Event& pEvt);
  virtual bool book();
  //  virtual bool write();

protected:

  virtual bool fTicker();

private:
  
};
#endif
