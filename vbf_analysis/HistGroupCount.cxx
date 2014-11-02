
#include "Services.h"
#include "Event.h"
#include "fastjet/JetDefinition.hh"

#include "HistGroupParticleCfg.h"
#include "HistGroupCount.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TProfile2D.h"

#define BOOK_TH1 Services::Histogramming::book<TH1D>
#define BOOK_TH2 Services::Histogramming::book<TH2D>
#define BOOK_TP1 Services::Histogramming::book<TProfile>
#define BOOK_TP2 Services::Histogramming::book<TProfile2D>

#define PRINT_INFO Services::Print::info
#define PRINT_WARN Services::Print::warning
#define PRINT_ERROR Services::Print::error

using namespace HistGroupParticleCfg;

HistGroupCount::HistGroupCount(const std::string& name)
  : HistGroupBase(name)
  , h_njets((TH1D*)0)
{ }

HistGroupCount::~HistGroupCount()
{ }

void HistGroupCount::book()
{
  // counting histograms
  h_njets = BOOK_TH1(this->name(),"nJets","Number of jets per event",njBins,njMin,njMax,"N_{jets}","dN/dN_{jets}");
}

void HistGroupCount::fill(const std::vector<fastjet::PseudoJet>& rCount)
{ 
  double rnums=(double)rCount.size();
  h_njets->Fill(rnums);
}
