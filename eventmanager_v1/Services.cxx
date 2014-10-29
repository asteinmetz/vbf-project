
#include "TFile.h"

#include "Services.h"

#define _HDLR_WRITE1_( _x_ )			\
  HistHandler* pHdlr = HistHandler::instance(); \
  return pHdlr->writeHist( _x_ );

#define _HDLR_WRITE2_( _x_ , _y_ )		\
  HistHandler* pHdlr = HistHandler::instance(); \
  return pHdlr->writeHist( _x_ , _y_ );

#define _HDLR_WRITE3_( _x_ , _y_ , _z_ )	\
  HistHandler* pHdlr = HistHandler::instance(); \
  return pHdlr->writeHist( _x_ , _y_ , _z_ );



std::string Services::HistHandler::m_defaultGroupName = "DefaultGroup";

Services::HistHandler* Services::HistHandler::m_instance = 0;

Services::HistHandler::HistHandler()
{ }

Services::HistHandler::~HistHandler()
{ Print::info("HistHandler::~HistHandler","Found %i histograms in registry",
	      (int)_histRegistry.size()); }

unsigned int Services::HistHandler::writeHist(bool noEmpties)
{
  Services::Print::info("HistHandler::writeHist(...)","entered\n");
  map_t::iterator fHist(_histRegistry.begin());
  map_t::iterator lHist(_histRegistry.end());
  unsigned int i(0);
  for ( ; fHist != lHist; ++fHist )
    { if ( !noEmpties || (fHist->second)->GetEntries() > 0 ) 
	{ (fHist->second)->Write(); ++i; } }
  if ( i > 0 )
    { Print::info("HistHandler::writeHist(...)","Wrote %i histograms\n",
		  i); }
  else
    { Print::info("HistHandler::writeHist(...)","No histograms written\n"); }
  return i;
}

unsigned int Services::HistHandler::writeHist(const std::string& group, 
					      bool noEmpties)
{
  Services::Print::info("HistHandler::writeHist(\042%s\042,...)","entered\n",
			group.c_str());
  map_t::iterator fHist(_histRegistry.begin());
  map_t::iterator lHist(_histRegistry.end());
  unsigned int i(0);
  for ( ; fHist != lHist; ++fHist )
    {
      if ( fHist->first.first == group && 
	   ( !noEmpties || (fHist->second)->GetEntries() > 0 ) ) 
	{ (fHist->second)->Write(); ++i; } 
    }
  if ( i > 0 )
    { Print::info("HistHandler::writeHist(\042%s\042,...)",
		  "Wrote %i histograms\n",
		  group.c_str(),i); }
  else
    { Print::info("HistHandler::writeHist(\042%s\042,...)",
		  "No histograms written\n",group.c_str()); }
  return i;
}

unsigned int Services::HistHandler::writeHist(const std::string& group,
					      const std::string& name,
					      bool noEmpties)
{
  Services::Print::info("HistHandler::writeHist(\042%s\042,\042%s\042,...)",
			"entered\n",group.c_str(),name.c_str());
  key_t key(group,getHistName(group,name));
  map_t::iterator fHist(_histRegistry.find(key));
  if ( fHist != _histRegistry.end() && 
       ( ! noEmpties || (fHist->second)->GetEntries() > 0 ) )
    { 
      (fHist->second)->Write(); 
      Print::info("HistHandler::writeHist(\042%s\042,\042%s\042,...)",
		  "Wrote 1 histogram\n",
		  group.c_str(),name.c_str());
      return 1; 
    }
  else 
    { 
      Print::info("HistHandler::writeHist(\042%s\042,\042%s\042,...)",
		  "No histograms written\n",group.c_str(),name.c_str()); 
      return 0; 
    }
}

std::string Services::HistHandler::getHistName(const std::string& name)
{ return this->getHistName(m_defaultGroupName,name); }

std::string Services::HistHandler::getHistName(const std::string& group,
					       const std::string& name)
{ return std::string(group+std::string("_")+name); }

unsigned int Services::HistHandler::numberRegHist() const
{ return _histRegistry.size(); }

unsigned int 
Services::HistHandler::numberRegHist(const std::string& group) const
{ 
  unsigned int nHists(0);
  map_t::const_iterator fMap(_histRegistry.begin());
  map_t::const_iterator lMap(_histRegistry.end());
  for ( ; fMap != lMap; ++fMap )
    { if ( fMap->first.first == group ) ++nHists; }
  return nHists;
}

unsigned int Services::Histogramming::writeHist(bool noEmpties)
{ _HDLR_WRITE1_( noEmpties ) }

unsigned int Services::Histogramming::writeHist(const std::string& group,
						 bool noEmpties)
{ _HDLR_WRITE2_( group,noEmpties ) }

unsigned int Services::Histogramming::writeHist(const std::string& group,
						 const std::string& name,
						 bool noEmpties)
{ _HDLR_WRITE3_( group,name,noEmpties ) }

unsigned int Services::Histogramming::write(const std::string& fileName,
					    const std::string& fileOpts,
					    bool noEmpties)
{
  TFile* file = new TFile(fileName.c_str(),fileOpts.c_str());
  if ( file == 0 )
    {
      Print::error("Histogramming::write(\042%s\042,\042%s\042,...)",
		   "Problem opening root file \042%s\042, no output.\n",
		   fileName.c_str(),fileOpts.c_str(),fileName.c_str());
      return 0;
    }
  unsigned int nwrite(writeHist(noEmpties));
  file->Close();
  delete file;
  return nwrite;
}

unsigned int Services::Histogramming::write(const std::string& fileName,
					    const std::string& fileOpts,
					    const std::string& group,
					    bool noEmpties)
{
  TFile* file = new TFile(fileName.c_str(),fileOpts.c_str());
  if ( file == 0 )
    {
      Print::error("Histogramming::write(\042%s\042,\042%s\042,...)",
		   "Problem opening root file \042%s\042, no output.\n",
		   fileName.c_str(),fileOpts.c_str(),fileName.c_str());
      return 0;
    }
  unsigned int nwrite(writeHist(group,noEmpties));
  file->Close();
  delete file;
  return nwrite;
}

unsigned int Services::Histogramming::write(const std::string& fileName,
					    const std::string& fileOpts,
					    const std::string& group,
					    const std::string& name,
					    bool noEmpties)
{
  TFile* file = new TFile(fileName.c_str(),fileOpts.c_str());
  if ( file == 0 )
    {
      Print::error("Histogramming::write(\042%s\042,\042%s\042,...)",
		   "Problem opening root file \042%s\042, no output.\n",
		   fileName.c_str(),fileOpts.c_str(),fileName.c_str());
      return 0;
    }
  unsigned int nwrite(writeHist(group,name,noEmpties));
  file->Close();
  delete file;
  return nwrite;
}
