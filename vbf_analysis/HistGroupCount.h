// -*- c++ -*-
#ifndef HISTGROUPCOUNT_H
#define HISTGROUPCOUNT_H
#include "HistGroup.h"
#include "fastjet/PseudoJet.hh"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include <vector>

class HistGroupCount : public HistGroupBase
{
public:
  /*! @brief Standard constructor */
  HistGroupCount(const std::string& name="HistGroupCount");
  /*! @brief Baseclass destructor */
  virtual ~HistGroupCount();

  virtual void book();
  virtual void fill(const std::vector<fastjet::PseudoJet>& rCount);

protected:

private:

  TH1D* h_njets;
};
#endif
