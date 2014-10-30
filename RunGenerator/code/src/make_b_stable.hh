#ifndef __MAKE_STABLE_B_HH__
#define __MAKE_STABLE_B_HH__

#include "Pythia.h"

// make B-mesons/hadrons stable
int no_b_decay(Pythia8::Pythia &py8){
  py8.readString("511:onMode = off");
  py8.readString("521:onMode = off");
  py8.readString("531:onMode = off");
  py8.readString("541:onMode = off");
  py8.readString("551:onMode = off");
  py8.readString("5122:onMode = off");
  py8.readString("5132:onMode = off");
  py8.readString("5142:onMode = off");
  py8.readString("5232:onMode = off");
  py8.readString("5242:onMode = off");
  py8.readString("5332:onMode = off");
  py8.readString("5342:onMode = off");
  py8.readString("5412:onMode = off");
  py8.readString("5414:onMode = off");
  py8.readString("5422:onMode = off");
  py8.readString("5424:onMode = off");
  py8.readString("5432:onMode = off");
  py8.readString("5434:onMode = off");
  py8.readString("5442:onMode = off");
  py8.readString("5444:onMode = off");
  py8.readString("5514:onMode = off");
  py8.readString("5522:onMode = off");
  py8.readString("5524:onMode = off");
  py8.readString("5532:onMode = off");
  py8.readString("5534:onMode = off");
  py8.readString("5542:onMode = off");
  py8.readString("5544:onMode = off");
  py8.readString("5554:onMode = off");

  return 0;
}

#endif
