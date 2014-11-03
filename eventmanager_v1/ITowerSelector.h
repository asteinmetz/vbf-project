// -*- c++ -*-
#ifndef ITOWERSELECTOR_H
#define ITOWERSELECTOR_H

#include "TowerGrid.h"

class ITowerSelector
{
public:

  typedef TowerGrid::tower_t tower_t;

  virtual ~ITowerSelector() { }

  virtual bool accept(const tower_t& entry) const = 0;
};
#endif
