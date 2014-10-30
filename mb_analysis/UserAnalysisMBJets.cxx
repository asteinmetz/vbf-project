
#include "fastjet/JetDefinition.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequenceArea.hh"

#include "PrintDef.h"

#include "HistGroupMBJets.h"

#include "UserAnalysisMBJets.h"
#include "Selectors.h"

#include "TFile.h"

std::string UserAnalysisMBJets::s_rConeTag        = "mbj::rcone";
std::string UserAnalysisMBJets::s_rCoreTag        = "mbj::rcore";
std::string UserAnalysisMBJets::s_ptCutsTag       = "mbj::ptcuts";
std::string UserAnalysisMBJets::s_jvfThresholdTag = "mbj::jvfmin";
std::string UserAnalysisMBJets::s_ptThresholdTag  = "mbj::ptmin";
 
UserAnalysisMBJets::UserAnalysisMBJets(const std::string& name,
				       DataFrame* pileupServer)
  : UserAnalysisMB(name,pileupServer)
  , s_rCone("{0.4,0.6}")
  , s_rCore("{0.2,0.3}")
  , s_ptCuts("{10.,20.,30.,40.,50.}")
  , s_jvfThreshold("0.5")
  , s_ptThreshold("0.5")
  , v_rCone(std::vector<double>())
  , v_rCore(std::vector<double>())
  , v_ptCuts(std::vector<double>())
  , _biasedPileupPlots(0)
{
  // default configuration vectors
  Services::Utils::unpackString<double>(s_rCone,v_rCone);
  Services::Utils::unpackString<double>(s_rCore,v_rCore);
  Services::Utils::unpackString<double>(s_ptCuts,v_ptCuts);
  // default configuration scalars
  _jvfThreshold = Services::Utils::extract<double>(s_jvfThreshold);
  _ptThreshold  = Services::Utils::extract<double>(s_ptThreshold);
}

UserAnalysisMBJets::~UserAnalysisMBJets()
{ }

bool UserAnalysisMBJets::analyze(Event& pEvt)
{
  // standard particle level pile-up plots
  this->fPileupPlots().fill(pEvt); 
  this->fBiasedPileupPlots().fill(pEvt); 

  // find input
  std::vector<fastjet::PseudoJet> mbParts = pEvt.pseudoJets(Vertex::PILEUP);

  // set parent in all other events
  this->fSetEventParent(&pEvt);
  this->fSetBiasedEventParent(&pEvt);

  // find jets for all configurations
  std::vector<fastjet::ClusterSequenceArea*> clusSeqs;
  fastjet::AreaDefinition areaDef(fastjet::active_area,
				  fastjet::GhostedAreaSpec(-5.,5.));
  std::vector<double>::iterator fCone(v_rCone.begin());
  std::vector<double>::iterator lCone(v_rCone.end());

  for ( ; fCone != lCone; ++fCone )
    {
      // jet configurations
      fastjet::JetDefinition jetDef(fastjet::antikt_algorithm,(*fCone));
      clusSeqs.push_back(new 
			 fastjet::ClusterSequenceArea(mbParts,jetDef,areaDef));
      // fill jet plots (least biased cuts)
      std::vector<fastjet::PseudoJet> 
	pJet = clusSeqs.back()->inclusive_jets(v_ptCuts.front());
      // fill events with selections
      std::vector<double>::iterator fPt(v_ptCuts.begin());
      std::vector<double>::iterator lPt(v_ptCuts.end());
      for ( ; fPt != lPt; ++fPt )
	{
	  Event& pEvt = this->fEventStore(*fCone,*fPt);
	  pEvt.reset();
	  pEvt.add(pJet);
	  PRINT_INFO("UserAnalysisMBJets_%s:::analyze(...)",
		     "allocate event at %p (%3.1f,%3.1f) with %i vertices filled from %i particles\n",
		     this->name().c_str(),(void*)(&pEvt),*fCone,*fPt,
		     pEvt.nVertices(),(int)pJet.size());
	}
      // fill plots
      this->fJetPlots(*fCone).fill();
    }

  std::vector<fastjet::ClusterSequenceArea*>::iterator fS(clusSeqs.begin());
  while ( fS != clusSeqs.end() ) { delete *fS; fS = clusSeqs.erase(fS); }

  return true; 
}

bool UserAnalysisMBJets::book()
{ 
  // unpack configuration
  this->unpackConfig();
  printf("******* ptThreshold ******* %5.2f\n",_ptThreshold);

  ////////////////////
  // Particle plots //
  ////////////////////

  this->fSetPileupPlots(new HistGroupEvent("PileupEvent"));
  this->fPileupPlots().book();

  if ( _ptThreshold > 0. )
    { this->fSetBiasedPileupPlots(new HistGroupEvent("BiasedPileupEvent"));
      this->fBiasedPileupPlots().book();
    }

  ///////////////
  // Jet plots //
  ///////////////

  // figure out the configuration
  size_t nj(v_rCone.size());
  size_t np(v_ptCuts.size());
  
  // matching cores with jet size if same number FIXME!
  size_t ntot(nj*np);  
  PRINT_INFO("UserAnalysisMBJets_%s","configure %i selections\n",
	     this->name().c_str(),(int)ntot);

  // general acceptance selector
  Selectors::EtaRange* etaSel = new Selectors::EtaRange(-5.,5.);

  // put standard configuration together
  std::string tag("Akt");
  for ( size_t ij(0); ij<nj; ++ij ) // loop jet size
    {
      // parameters
      double rcone(v_rCone.at(ij));
      double rcore(rcone/2.);
      int irc(this->cnvToInt(rcone,100.));
      std::string jtg = tag+Services::Utils::convert<int>(irc,"%03i")+"Jet";
      // histogram groups for this configuration
      this->fSetJetPlots(new HistGroupContainer("EventJets"),rcone);
      // loop pt thresholds
      for ( size_t ip(0); ip<np; ++ip )
	{
	  int ipp(this->cnvToInt(v_ptCuts.at(ip),1));
	  std::string ptg=jtg+"_Pt"+Services::Utils::convert<int>(ipp,"%03i");
	  // set up selectors
	  Selectors::Handler* sHandle = new Selectors::Handler();
	  sHandle->registerSelector(etaSel);
	  sHandle->registerSelector(new 
				    Selectors::PtThreshold(v_ptCuts.at(ip)));
	  // new event
	  Event* pEvt = new Event(sHandle);
	  fSetEventStore(pEvt,rcone,v_ptCuts.at(ip));
	  // new histogram group
	  HistGroupMBJets* pGrp = new HistGroupMBJets(ptg,0.5,rcore);
	  pGrp->cacheEvent(pEvt);
	  PRINT_INFO("UserAnalysisMBJets_%s::book()",
		     "HistGroup \042%s\042 cached Event %p/%p\n",
		     this->name().c_str(),pGrp->name().c_str(),
		     (void*)(&(pGrp->eventCache())),(void*)pEvt);
	  pGrp->book();
	  this->fAddJetPlots(pGrp,rcone);
	}
      //      this->fJetPlots(rcone).book();
    }

  // put biased configuration together
//   if ( _ptThreshold > 0. )
//     {
//       tag = "BiasedAkt";
//       for ( size_t ij(0); ij<nj; ++ij )
// 	{
// 	  // parameters
// 	  double rcone(v_rCone.at(ij));
// 	  double rcore(rcone/2.);
// 	  int irc(this->cnvToInt(rcone,100.));
// 	  std::string jtg = tag+Services::Utils::convert<int>(irc,"%03i")+
// 	    "Jet";
// 	  // histogram groups for this configuration
// 	  this->fSetBiasedJetPlots(new HistGroupContainer("BiasedEventJets"),
// 				   rcone);
// 	  // loop  pt thresholds
// 	  for ( size_t ip(0); ip<np; ++ip )
// 	    {
// 	      int ipp(this->cnvToInt(v_ptCuts.at(ip),1));
// 	      std::string ptg=jtg+"_Pt"+Services::Utils::convert<int>(ipp,
// 								      "%03i");
// 	      // set up selectors
// 	      Selectors::Handler* sHandle = new Selectors::Handler();
// 	      sHandle->registerSelector(etaSel);
// 	      sHandle->registerSelector(new 
// 					Selectors::PtThreshold(v_ptCuts.at(ip)));
// 	      // new event
// 	      Event* pEvt = new Event(sHandle);
// 	      fSetBiasedEventStore(pEvt,rcone,v_ptCuts.at(ip));
// 	      // new histogram group
// 	      HistGroupMBJets* pGrp = new HistGroupMBJets(ptg,0.5,rcore);
// 	      pGrp->cacheEvent(pEvt);
// 	      PRINT_INFO("UserAnalysisMBJets_%s::book()",
// 			 "HistGroup \042%s\042 cached Event %p/%p\n",
// 			 this->name().c_str(),pGrp->name().c_str(),
// 			 (void*)(&(pGrp->eventCache())),(void*)pEvt);
// 	      this->fAddBiasedJetPlots(pGrp,rcone);
// 	      pGrp->book();
// 	    }
// 	  //	  this->fBiasedJetPlots(rcone).book();
// 	}
//     }

  return true; 
}

bool UserAnalysisMBJets::unpackConfig()
{

  ///////////////////////
  // List of jet sizes //
  ///////////////////////

  bool changeCfg(this->interpretCfg(s_rConeTag,s_rCone,"jet size",v_rCone)); 

  ////////////////////////////
  // List of jet core radii //
  ////////////////////////////

  changeCfg = changeCfg || this->interpretCfg(s_rCoreTag,s_rCore,
					      "jet core radius",v_rCore);

  ///////////////////////////////
  // List of jet pT thresholds //
  //////////////s////////////////

  changeCfg = changeCfg || this->interpretCfg(s_ptCutsTag,s_ptCuts,
					      "jet pT thresholds",v_ptCuts);

  ///////////////////
  // JVF threshold //
  ///////////////////

  changeCfg = changeCfg || this->interpretCfg(s_jvfThresholdTag,s_jvfThreshold,
					      "JVF threshold",_jvfThreshold);

  ///////////////////////////////
  // Pt threshold for particle //
  ///////////////////////////////

  changeCfg = changeCfg || this->interpretCfg(s_ptThresholdTag,s_ptThreshold,
					      "particle pT threshold",
					      _ptThreshold);

  if ( changeCfg ) Configuration::printConfig(fConfig());

  return changeCfg;
} 

bool UserAnalysisMBJets::interpretCfg(const std::string& tag,
				      std::string& vstr,
				      const std::string& cstr,
				      double&            rdat)
{
  bool changeCfg(false);
  if ( !Configuration::getConfig(tag,rdat) )
    { std::string tstr = std::string("[default] ")+cstr; 
      Configuration::addConfig(tag,vstr,tstr); 
      changeCfg = true;
    }
  else
    { vstr = Services::Utils::convert<double>(rdat); }
  return changeCfg;
}


bool UserAnalysisMBJets::interpretCfg(const std::string&   tag,
				      std::string&   vstr,
				      const std::string&   cstr,
				      std::vector<double>& vdat)
{
  bool changeCfg(false);
  if ( Configuration::getConfig(tag,vstr) )
    { 
      // check if it is a vector
      if ( !Services::Utils::unpackString<double>(vstr,vdat) )
	{
	  // try to get singular parameter
	  double rc(0.);
	  if ( Configuration::getConfig(tag,rc) )
	    {
	      PRINT_WARN("UserAnalysisMBJets_%s::unpackConfig()",
			 "retrieved parameter <%s> as double value %4.2f\n",
			 this->name().c_str(),
			 tag.c_str(),rc);
	      PRINT_WARN("UserAnalysisMBJets_%s::unpackConfig()",
			 ">>>>>> expected vector! <<<<<<\n",
			 this->name().c_str());
	      vdat.clear(); vdat.push_back(rc);
	      Configuration::addConfig(tag,
				       Services::Utils::convert<double>(rc),
				       cstr);
	      changeCfg = true;
	    }
	}
      // updated configuration vector
//       else
// 	{
// 	  Configuration::addConfig(tag,vstr,cstr);
// 	  changeCfg = true;
//	}
    }
  // use default
  else
    { std::string tstr = std::string("[default] ")+cstr;
      Configuration::addConfig(tag,vstr,tstr); }
  
  return changeCfg;
}

void UserAnalysisMBJets::_fSetJetPlots(map_t& map,HistGroupContainer* pGrpCont,
				       int category)
{
  map_t::iterator fMap(map.find(category)); 
  if ( fMap == map.end() ) 
    { map.insert(map_t::value_type(category,pGrpCont)); }
  else
    { fMap->second = pGrpCont; }
}

void UserAnalysisMBJets::_fAddJetPlots(map_t& map,HistGroupBase* pGrp,
				       int category,
				       IFinalStateSelector* pSel)
{ 
  map_t::iterator fMap(map.find(category));
  if ( fMap == map.end() ) 
    {
      PRINT_ERROR("UserAnalysisMBJets_%s::fAddJetPlots(...)",
		  "cannot find histogram group for category %i\n",
		  this->name().c_str(),category);
      Services::Process::terminate("Inconsistent/incomplete internal memory");
    }
  fMap->second->addEventPlots(pGrp); 
}

HistGroupContainer& UserAnalysisMBJets::_fJetPlots(map_t& map,int category)
{
  map_t::iterator fMap(map.find(category)); 
  if ( fMap == map.end() )
    {
      PRINT_ERROR("UserAnalysisMBJets_%s::fJetPlots(...)",
		  "cannot find histogram group for category %i\n",
		  this->name().c_str(),category);
      Services::Process::terminate("Inconsistent/incomplete internal memory");
    }
  return *(fMap->second); 
}

void UserAnalysisMBJets::_fSetEventStore(evtmap_t& map,Event* pEvt,
					 evtkey_t key)
{
  evtmap_t::iterator fMap(map.find(key));
  if ( fMap == map.end() ) { map.insert(evtmap_t::value_type(key,pEvt)); }
  else
    {
      PRINT_ERROR("UserAnalysisMBJets_%s::fSetEventStore(...)",
		  "event with key <%i,%i> already existing, fatal!\n",
		  this->name().c_str(),key.first,key.second);
      Services::Process::terminate("Overwriting event store");
    }
}

Event& UserAnalysisMBJets::_fEventStore(evtmap_t& map,evtkey_t key)
{
  evtmap_t::iterator fMap(map.find(key));
  if ( fMap ==   map.end() )
    {
      PRINT_ERROR("UserAnalysisMBJets_%s::fEventStore(...)",
		  "event with key <%i,%i> not existing, fatal!\n",
		  this->name().c_str(),key.first,key.second);
      Services::Process::terminate("Overwriting event store");
    }
  return *(fMap->second);
}

void UserAnalysisMBJets::_fSetParent(evtmap_t& map,Event* parent)
{
  evtmap_t::iterator fMap(map.begin());
  evtmap_t::iterator lMap(map.end());
  for ( ; fMap != lMap; ++fMap ) { fMap->second->setParent(parent); }
}
