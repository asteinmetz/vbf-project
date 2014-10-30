#ifndef HISTGROUPPARTICLECFG_H
#define HISTGROUPPARTICLECFG_H
#include <string>
#include <cmath>

namespace HistGroupParticleCfg
{
  static const int netaBins = 120;
  static const double etaMin = -6.;
  static const double etaMax =  6.;

  static const double detaMin = -12.;
  static const double detaMax = 12.;

  static const int nptBins = 100;
  static const double ptMin = 0.;
  static const double ptMax = 500.;

  static const int nmBins = 400;
  static const double mMin = 0.;
  static const double mMax = 4000.;

  static const int njBins = 30;
  static const double njMin = -0.5;
  static const double njMax = 29.5;

  static const int phiBins = 64;
  static const double phiMin = -3.2;
  static const double phiMax = 3.2;

  static const int ndisBins = 100;

  static const int ndecayBins = 31;
  static const double decayMin = -0.5;
  static const double decayMax = 30.5;

  static const int npidBins = 51;
  static const double pidMin = -25.5;
  static const double pidMax = 25.5;
}
#endif
