
#include "HistGroup.h"

HistGroupBase::HistGroupBase(const std::string& name)
  : m_name(name)
  , f_handler(Services::HistHandler::instance())
{ }

HistGroupBase::~HistGroupBase()
{ }

const std::string& HistGroupBase::name() const
{ return m_name; }


void HistGroupBase::write()
{  f_handler->writeHist(this->name()); }

void HistGroupBase::print()
{ Services::Print::info("%s::print()","Number of histograms in group: %i\n",
			this->name().c_str(),f_handler->numberRegHist(name()));
}

void HistGroupBase::book()
{ }

void HistGroupBase::fill()
{ }

void HistGroupBase::fill(const std::vector<fastjet::PseudoJet>& /*vJet*/)
{ }

void HistGroupBase::fill(Event& /*rEvt*/)
{ }

int HistGroupBase::numberRegHist()
{ return Services::Histogramming::numberRegHist(); }

int HistGroupBase::numberRegHist(const std::string& group)
{ return Services::Histogramming::numberRegHist(group); }

void HistGroupBase::cacheEvent(Event* pEvt)
{ m_cachedEvent = pEvt; }

Event& HistGroupBase::eventCache()
{  return *m_cachedEvent; }
