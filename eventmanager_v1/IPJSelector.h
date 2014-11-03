// -*- c++ -*-
#ifndef IPJSELECTOR_H
#define IPJSELECTOR_H

#include "fastjet/PseudoJet.hh"

class IPJSelector
{
public:

  virtual ~IPJSelector() { }

  virtual bool accept(const fastjet::PseudoJet& pjet) const = 0;
};
#endif
