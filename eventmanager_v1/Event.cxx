
#include "PrintDef.h"

#include "Services.h"
#include "Event.h"

#include <cstdio>

#include <algorithm> 
#include <iterator>

Vertex Event::_dummyVertex(-42);

const Vertex& Event::dummyVtx() { return _dummyVertex; }

////////////
// Vertex //
////////////

Vertex::Vertex(OrderContext oc) 
  : m_id(-42)
  , m_ptSum(0.)
  , m_ptSum2(0.)
  , m_ptSumCharged(0.)
  , m_ptSum2Charged(0.)
  , m_nPart(0)
  , m_nPartCharged(0)
  , m_orderContext(oc)
{ }

Vertex::Vertex(int vtx,OrderContext oc)
  : m_id(vtx)
  , m_ptSum(0.)
  , m_ptSum2(0.)
  , m_ptSumCharged(0.)
  , m_ptSum2Charged(0.)
  , m_nPart(0)
  , m_nPartCharged(0)
  , m_orderContext(oc)
{ }

Vertex::Vertex(const Vertex& vertex)
  : m_id(vertex.m_id)
  , m_ptSum(vertex.m_ptSum)
  , m_ptSum2(vertex.m_ptSum2)
  , m_ptSumCharged(vertex.m_ptSumCharged)
  , m_ptSum2Charged(vertex.m_ptSum2Charged)
  , m_nPart(vertex.m_nPart)
  , m_nPartCharged(vertex.m_nPartCharged)
  , m_orderContext(vertex.m_orderContext)
{ }

Vertex::~Vertex()
{ }

Vertex& Vertex::operator=(const Vertex& vertex)
{
  m_id            = vertex.m_id;
  m_ptSum         = vertex.m_ptSum;
  m_ptSum2        = vertex.m_ptSum2;
  m_ptSumCharged  = vertex.m_ptSumCharged;
  m_ptSum2Charged = vertex.m_ptSum2Charged;
  m_nPart         = vertex.m_nPart;
  m_nPartCharged  = vertex.m_nPartCharged;
  m_orderContext  = vertex.m_orderContext;
  return *this;
}

void Vertex::addParticle(const fastjet::PseudoJet& pJet)
{
  double pT(pJet.perp());
  double pT2(pT*pT);
  m_ptSum  += pT;
  m_ptSum2 += pT2;
  ++m_nPart;
  if ( Features::isCharged(pJet) )
    {
      m_ptSumCharged  += pT;
      m_ptSum2Charged += pT2;
      ++m_nPartCharged;
    }
}

double Vertex::ptSum(Type vtype) const
{ return vtype == TOTAL 
    ? m_ptSum :        vtype == CHARGED 
    ? m_ptSumCharged : vtype == NEUTRAL
    ? m_ptSum-m_ptSumCharged : 0.;
}

double Vertex::ptSum2(Type vtype) const
{ return vtype == TOTAL 
    ? m_ptSum2 :        vtype == CHARGED 
    ? m_ptSum2Charged : vtype == NEUTRAL
    ? m_ptSum2-m_ptSum2Charged : 0.;
}

int Vertex::id() const
{ return m_id; }

int Vertex::multiplicity(Type vtype) const
{ return vtype == TOTAL
    ? m_nPart :        vtype == CHARGED
    ? m_nPartCharged : vtype == NEUTRAL
    ? m_nPart-m_nPartCharged : 0; }
  
bool Vertex::operator>=(const Vertex& vtx) const
{ _compare( >= ); }
bool Vertex::operator>(const Vertex& vtx) const
{ _compare( > ); }
bool Vertex::operator<=(const Vertex& vtx) const
{ _compare( <= ); }
bool Vertex::operator<(const Vertex& vtx) const
{ _compare( < ); }

//////////////////////////
// Particle Information //
//////////////////////////

ParticleInfo::ParticleInfo()
  : fastjet::PseudoJet::UserInfoBase()
  , m_id(-9999)
  , m_charge(0.)
  , m_vertexId(-1)
  , m_interactionType(Vertex::UNKNOWN)
{ }

ParticleInfo::ParticleInfo(int id,double charge,int vtx,Vertex::InteractionType type)
  : fastjet::PseudoJet::UserInfoBase()
  , m_id(id)
  , m_charge(charge)
  , m_vertexId(vtx)
  , m_interactionType(type)
{ }

ParticleInfo::ParticleInfo(const ParticleInfo& pInfo)
  : fastjet::PseudoJet::UserInfoBase()
  , m_id(pInfo.m_id)
  , m_charge(pInfo.m_charge)
  , m_vertexId(pInfo.m_vertexId)
  , m_interactionType(pInfo.m_interactionType)
{ }

ParticleInfo::~ParticleInfo()
{ }

// void ParticleInfo::addParticleInfo(const fastjet::PseudoJet& pJet,
// 				   int id,double charge,int vtx)
// { 
//   // look for vertices
//   std::vector<Vertex>::iterator fVtx(m_vertices.begin());
//   std::vector<Vertex::iterator lVtx(m_vertices.end());
//   while ( fVtx != lVtx && vtx != (*fVtx).id() )
//     { ++fVtx; }
//   if ( fVtx != lVtx )
//     { fVtx->addParticle(pJet,charge); }
//   else
//     { m_vertices.push_back(Vertex(vtx));
//       m_vertices.back().addParticle(pJet,charge); }
//   // refer to first vertex
//   m_id       = id;
//   m_charge   = charge; 
//   m_vertexId = vtx;
// }

int ParticleInfo::id() const { return m_id; }
double ParticleInfo::charge() const { return m_charge; }
int ParticleInfo::vertexId() const { return m_vertexId; }
Vertex::InteractionType ParticleInfo::interactionType() const 
{ return m_interactionType; }
bool ParticleInfo::isSignal() const 
{ return ( m_interactionType & Vertex::SIGNAL ) == Vertex::SIGNAL; }
bool ParticleInfo::isPileup() const 
{ return ( m_interactionType & Vertex::PILEUP ) == Vertex::PILEUP; }

const bool ParticleInfo::vertex(const Event& thisEvt,Vertex& vertex) const
{
  int vtx(this->vertexId());
  if ( vtx >= 0 && vtx < thisEvt.vertices().size() )
    { vertex = thisEvt.vertices().at(vtx); return true; }
  else
    { return false; }
}

///////////
// Event //
///////////

Event::Event(IFinalStateSelector* pSel) 
  : m_fsSel(pSel)
  , m_parent(0)
{ }

Event::Event(Event* parent,IFinalStateSelector* pSel)
  : m_fsSel(pSel)
  , m_parent(parent)
{ }

Event::~Event()
{ }

bool Event::add(int id,double charge,
		double px,double py,double pz,double m,
		int vtx,Vertex::InteractionType type)
{
  double e(sqrt(px*px+py*py+pz*pz+m*m));
  // create PseudoJet
  fastjet::PseudoJet pJet(px,py,pz,e); 
  pJet.set_user_info(new ParticleInfo(id,charge,vtx,type));
  return this->add(pJet);
}

bool Event::add(const fastjet::PseudoJet& pJet)
{
  // check on selector
  if ( m_fsSel == 0 || (m_fsSel != 0 && m_fsSel->accept(pJet)) )
    { this->updateCache(pJet); return true; } 
  return false;
} 

bool Event::add(const std::vector<fastjet::PseudoJet>& vJet)
{
  std::vector<fastjet::PseudoJet>::const_iterator fJet(vJet.begin());
  std::vector<fastjet::PseudoJet>::const_iterator lJet(vJet.end());
  bool isOk(true);
  for ( ; fJet != lJet; ++fJet ) 
    { if ( isOk ) { isOk = this->add(*fJet); } else { this->add(*fJet); } }
  // catch empty events (this may happen in case of pile-up when all 
  // all particle are outside the detector acceptance!
  if ( !isOk && this->nVertices() == 0 )
    { 
      // find vertices
      std::vector<int> vids;
      std::vector<fastjet::PseudoJet>::const_iterator fJet(vJet.begin());
      std::vector<fastjet::PseudoJet>::const_iterator lJet(vJet.end());
      for ( ; fJet != lJet; ++fJet )
	{
	  int v = Features::vertexId(*fJet);
	  if ( std::find(vids.begin(),vids.end(),v) == vids.end() ) 
	    { vids.push_back(v); }
	}
      // add empty vertex
      size_t idx(size_t(-1));
      for ( size_t i(0); i<vids.size(); ++i )
	{ 
	  if ( !this->vtxIndex(vids.at(i),idx) ) 
	    {
	      // do not add particles == empty vertex!
	      m_vertices.push_back(Vertex(vids.at(i)));
	      m_particles.push_back(std::vector<fastjet::PseudoJet>());
	    } // new vertex index
	} // loop on vertices
    } // all particles not accepted
  return isOk;
}

bool Event::updateCache(const fastjet::PseudoJet& pJet)
{
  // check if the jet is composite
  int vid(0);
  if ( !pJet.has_constituents() )
    {
      // check if vertex is in list
      vid = Features::vertexId(pJet);
      //     std::cout << "Vertex id " << vid << std::endl; 
      size_t idx(size_t(-1));
      if ( !this->vtxIndex(vid,idx) )
	{
	  // new vertex
	  m_vertices.push_back(Vertex(vid));
	  m_vertices.back().addParticle(pJet);
	  // new particle at new vertex
	  m_particles.push_back(std::vector<fastjet::PseudoJet>());
	  m_particles.back().push_back(pJet);
	}
      // already registered vertex
      else
	{
	  m_vertices.at(idx).addParticle(pJet);
	  m_particles.at(idx).push_back(pJet);
	}
    }
  // composite jet
  else
    {
      if ( m_vertices.empty() )
	{ 
	  m_vertices.push_back(Vertex(0));
	  m_particles.push_back(std::vector<fastjet::PseudoJet>());
	}
      m_vertices.front().addParticle(pJet);
      m_particles.front().push_back(pJet);
      vid = static_cast<int>(m_particles.size());
    }
  return vid >= 0;
}

// bool Event::addToVtx(const fastjet::PseudoJet& pJet)
// {
//   if ( pJet.has_user_info<ParticleInfo>() )
//     {
//       // check on vertex id
//       int vtx(pJet.user_info<ParticleInfo>().vertexId());
//       if ( vtx < 0 )
// 	{
// 	  printf("Event::addToVtx(...) - ERROR - unknown vertex, index = %i\n",
// 		 vtx);
// 	  return false;
// 	}
//       m_vertices[vtx].addParticle(pJet);
//       return true;
//     }
//   printf("[Event::addToVtx(...)] - WARNING - invalid user information type\n");
//   return false;
// }

bool Event::haveParticleEvent() { return this->nParticles() > 0; }
bool Event::havePartonicEvent() { return this->nPartons()   > 0; }

bool Event::empty(int vtxId) const
{
  if ( vtxId == -1 ) return m_particles.empty();
  // check if known vertex
  int    vid(vtxId);
  size_t idx(size_t(-1));
  if ( !vtxIndex(vid,idx) ) return true;
  // 
  return m_particles.at(idx).empty();
}

int Event::size(int vtxId) const
{
  // all particles
  if ( vtxId == -1 ) 
    {
      size_t np(0);
      std::vector<std::vector<fastjet::PseudoJet> >::const_iterator 
	fJets(m_particles.begin());
      std::vector<std::vector<fastjet::PseudoJet> >::const_iterator 
	lJets(m_particles.end());
      for ( ;  fJets != lJets; ++fJets ) { np += fJets->size(); }
      return (int)np;
    }      
  // 
  size_t idx(size_t(-1));
  return this->vtxIndex(vtxId,idx) ? (int)m_particles.at(idx).size() : 0;
}

int Event::nParticles(int vtx) const
{ return this->size(vtx); }

int Event::nPartons(int /*vtx*/) const
{ return 0; }

int Event::nVertices() const
{ return (int)m_vertices.size(); }

void Event::reset() 
{
  m_particles.clear();
  m_partons.clear();
  m_vertices.clear();
  if ( m_fsSel != 0 ) m_fsSel->reset();
}

std::vector<fastjet::PseudoJet> Event::pseudoJets(int vtxId)
{
  std::vector<fastjet::PseudoJet> pVec;
  if ( vtxId == -1 )
    {
      std::vector<std::vector<fastjet::PseudoJet> >::const_iterator
	fVtx(m_particles.begin());
      std::vector<std::vector<fastjet::PseudoJet> >::const_iterator 
	lVtx(m_particles.end());
      for ( ; fVtx != lVtx; ++fVtx ) 
	{ std::copy(fVtx->begin(),fVtx->end(),
		    std::back_insert_iterator<std::vector<fastjet::PseudoJet> >
		    (pVec)); }
      //Utils::copyPJ(*fVtx,pVec); }
    } // all vertices
  else
    { 
      size_t idx(size_t(-1));
      if ( this->vtxIndex(vtxId,idx) )
	{ std::copy(m_particles.at(idx).begin(),
		    m_particles.at(idx).end(),
		    std::back_insert_iterator<std::vector<fastjet::PseudoJet> >
		    (pVec)); }
	  //Utils::copyPJ(m_particles.at(idx),pVec); }
    }
  return pVec;
}

std::vector<fastjet::PseudoJet> Event::pseudoJets(Vertex::InteractionType type)
{
  std::vector<fastjet::PseudoJet> pVec;
  std::vector<std::vector<fastjet::PseudoJet> >::const_iterator 
    fVtx(m_particles.begin());
  std::vector<std::vector<fastjet::PseudoJet> >::const_iterator 
    lVtx(m_particles.end());
  for ( ; fVtx != lVtx; ++fVtx ) { Utils::copyPJ(*fVtx,pVec,type); }
  return pVec;
}

std::vector<fastjet::PseudoJet> Event::pseudoJets(bool charged,int vtxId)
{
  std::vector<fastjet::PseudoJet> pVec;
  // check all vertices
  if ( vtxId == -1 )
    {
      std::vector<std::vector<fastjet::PseudoJet> >::const_iterator 
	fVtx(m_particles.begin());
      std::vector<std::vector<fastjet::PseudoJet> >::const_iterator 
	lVtx(m_particles.end());
      for ( ; fVtx != lVtx; ++fVtx )
	{ Utils::copyPJ(*fVtx,pVec,charged); }  // loop on all vertices
    } // all vertices request
  // check specific vertex
  else 
    {
      size_t idx(size_t(-1));
      if ( this->vtxIndex(vtxId,idx) ) 
	{ Utils::copyPJ(m_particles.at(idx),pVec,charged); }
    } 
  return pVec;
}

std::vector<fastjet::PseudoJet> Event::pseudoJets(bool charged,
						  Vertex::InteractionType type)
{
  std::vector<fastjet::PseudoJet> pVec;
  std::vector<std::vector<fastjet::PseudoJet> >::const_iterator 
    fVtx(m_particles.begin());
  std::vector<std::vector<fastjet::PseudoJet> >::const_iterator 
    lVtx(m_particles.end());
  for ( ; fVtx != lVtx; ++fVtx ) { Utils::copyPJ(*fVtx,pVec,type,charged); }
  return pVec;
}

const Vertex& Event::vertex(int vtxId) const
{ 
  if ( vtxId == -1 )
    { return !m_vertices.empty() ? m_vertices.at(0) : dummyVtx(); }
  else
    {
      size_t idx(size_t(-1));
      return this->vtxIndex(vtxId,idx) ? m_vertices.at(idx) : dummyVtx();
    }
}
