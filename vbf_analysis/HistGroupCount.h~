// -*- c++ -*-
#ifndef HISTGROUPPARTICLE_H
#define HISTGROUPPARTICLE_H
#include "HistGroup.h"
#include "fastjet/PseudoJet.hh"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include <vector>

class HistGroupParticle : public HistGroupBase
{
public:
  /*! @brief Standard constructor */
  HistGroupParticle(const std::string& name="HistGroupParticle");
  /*! @brief Baseclass destructor */
  virtual ~HistGroupParticle();

  virtual void book();
  virtual void fill(const std::vector<fastjet::PseudoJet>& rJet);

protected:

private:

  double _numberOfInteractions;

  /*! @brief Number of interactions */
  TH1D* h_eta;
  TH1D* h_phi;
  TH1D* h_pt;
};
#endif
