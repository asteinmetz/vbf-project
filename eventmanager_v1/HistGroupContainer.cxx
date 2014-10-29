
#include "PrintDef.h"

#include "HistGroupContainer.h"

HistGroupContainer::HistGroupContainer(const std::string& name)
  : HistGroupBase(name)
{ }

HistGroupContainer::~HistGroupContainer()
{ }

void HistGroupContainer::book()
{
  // event plots
  std::vector<HistGroupBase*>::iterator fHist(_eventPlots.begin());
  std::vector<HistGroupBase*>::iterator lHist(_eventPlots.begin());
  for ( ; fHist != lHist; ++fHist ) { (*fHist)->book(); }
  // jet plots
  std::vector<filler_t>::iterator fJet(_jetPlots.begin());
  std::vector<filler_t>::iterator lJet(_jetPlots.end());
  for ( ; fJet != lJet; ++fJet ) { fJet->first->book(); }
}

void HistGroupContainer::fill()
{
  std::vector<HistGroupBase*>::iterator fHist(_eventPlots.begin());
  std::vector<HistGroupBase*>::iterator lHist(_eventPlots.end());
  for ( ; fHist != lHist; ++fHist ) { (*fHist)->fill(); }  
}


void HistGroupContainer::fill(Event& pEvt)
{
  std::vector<HistGroupBase*>::iterator fHist(_eventPlots.begin());
  std::vector<HistGroupBase*>::iterator lHist(_eventPlots.end());
  for ( ; fHist != lHist; ++fHist ) { (*fHist)->fill(pEvt); }
}

void HistGroupContainer::fill(const std::vector<fastjet::PseudoJet>& vJet)
{
  std::vector<filler_t>::iterator fHist(_jetPlots.begin());
  std::vector<filler_t>::iterator lHist(_jetPlots.end());
  for ( ; fHist != lHist; ++fHist ) 
    {
      if ( fHist->second == 0 ) { fHist->first->fill(vJet); }
      else
	{ 
	  std::vector<fastjet::PseudoJet>::const_iterator fJet(vJet.begin());
	  std::vector<fastjet::PseudoJet>::const_iterator lJet(vJet.end());
	  std::vector<fastjet::PseudoJet> filtJet;
	  for ( ; fJet != lJet; ++fJet )
	    { if ( fHist->second->accept(*fJet) ) filtJet.push_back(*fJet); }
	  fHist->first->fill(filtJet);
	}
    }
}

void HistGroupContainer::print()
{
  PRINT_INFO("HistoGroupContainer_%s","JetPlots are filled with %i groups:\n",
	     this->name().c_str(),(int)_jetPlots.size());
  std::vector<filler_t>::iterator fjp(_jetPlots.begin());
  std::vector<filler_t>::iterator ljp(_jetPlots.end());
  for ( ; fjp != ljp; ++fjp ) 
    { PRINT_INFO("HistGroupContainer_%s","    <%s>",this->name().c_str(),
		 fjp->first->name().c_str()); }
  PRINT_INFO("HistoGroupContainer_%s",
	     "EventPlots are filled with %i groups:\n",
	     this->name().c_str(),(int)_eventPlots.size());
  std::vector<HistGroupBase*>::iterator fep(_eventPlots.begin());
  std::vector<HistGroupBase*>::iterator lep(_eventPlots.end());
  for ( ; fep != lep; ++fep )
    { PRINT_INFO("HistGroupContainer_%s","    <%s>",this->name().c_str(),
		 (*fep)->name().c_str()); }
}

void HistGroupContainer::write()
{ }
//   std::vector<HistGroupBase*>::iterator fHist(_eventtore.begin());
//   std::vector<HistGroupBase*>::iterator lHist(_store.end());
//   for ( ; fHist != lHist; ++fHist ) { (*fHist)->write(); }
// }

size_t HistGroupContainer::size(Event& /*pEvt*/) const
{ return _eventPlots.size(); }

size_t HistGroupContainer::size(const std::vector<fastjet::PseudoJet>& 
				/*vJet*/) const
{ return _jetPlots.size(); }

