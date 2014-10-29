// -*- c++ -*-
#ifndef EVENT_H
#define EVENT_H

#include "fastjet/PseudoJet.hh"

#include "IFinalStateSelector.h"

#include "RandomEngine.h"

#include <vector>

#define _compare( _x_ )                                                       \
  OrderContext oc = vtx.m_orderContext;                                       \
  vtx.setOrderContext(m_orderContext);                                        \
  bool flag(false);                                                           \
  switch ( m_orderContext )                                                   \
    {                                                                         \
    case PTSUM2:                                                              \
      flag = m_ptSum2 _x_ vtx.m_ptSum2;                                       \
      break;                                                                  \
    case PTSUM:                                                               \
      flag = m_ptSum _x_ vtx.m_ptSum;                                         \
      break;                                                                  \
    case PTSUM2CHARGED:                                                       \
      flag = m_ptSum2Charged _x_ vtx.m_ptSum2Charged;                         \
      break;                                                                  \
    case PTSUMCHARGED:                                                        \
      flag = m_ptSumCharged _x_ vtx.m_ptSumCharged;                           \
      break;                                                                  \
    case PTSUM2NEUTRAL:                                                       \
      flag = m_ptSum2-m_ptSum2Charged _x_ vtx.m_ptSum2-vtx.m_ptSum2Charged;   \
      break;                                                                  \
    case PTSUMNEUTRAL:                                                        \
      flag = m_ptSum-m_ptSumCharged _x_ vtx.m_ptSum-vtx.m_ptSumCharged;       \
      break;                                                                  \
    default:                                                                  \
      break;                                                                  \
    }                                                                         \
  vtx.setOrderContext(oc);						      \
  return flag
 
/*! @brief Collection of numerical and other constants */
namespace Constants
{
  /* @name Numerical constants */
  /*!@{*/
  static const double pi(2*asin(1.));
  static const double twoPi(2*pi); 
  /*!@}*/
}

class Vertex
{
public:

  /*! @name Ennumerators for states and contexts */
  /*!@{*/
  /*! @brief Interaction type
   * 
   *  Describes the source of a particle.
   */
  enum InteractionType { PILEUP  = 0x01,
			 SIGNAL  = 0x02,
			 MIXED   = 0x03,
			 UNKNOWN = 0x00 };
  /*! @brief Particle charge indicator */
  enum Type { NEUTRAL = 0,
	      CHARGED = 1, 
	      TOTAL   = 0 };
  /*! @brief Vertex ordering context */
  enum OrderContext { PTSUM = 0,
		      PTSUM2 = 1,
		      PTSUMCHARGED = 2,
		      PTSUM2CHARGED = 3,
		      PTSUMNEUTRAL = 4,
		      PTSUM2NEUTRAL = 5 };
  /*!@}*/

  /*! @name Constructor
   *  
   *  @param[in] oc order context used in comparators. This context can be
   *             changed even for a @c const @c Vertex (optional, defaults
   *             to @f$\sum{p_{\rm T}^{2}}@f$ ordering).
   * 
   *  @param[in] vtx client defined vertex identifier (number)
   *  
   */
  /*!@{*/
  Vertex(OrderContext oc=PTSUM2); /*!< Vertex identifier is @c 0 */
  Vertex(int vtx,OrderContext oc=PTSUM2); /*!< Vertex indentifier is client
					    defined */
  /*!@}*/
  /*! @brief Copy constructor
   *
   *  @param[in] vertex reference to non-modifiable @c Vertex object 
   */
  Vertex(const Vertex& vertex);
  /*! @brief Destructor */
  virtual ~Vertex();
  /*! @brief Assignment operator
   *
   *  @param[in] vertex reference to non-modifiable @c Vertex object 
   */
  Vertex& operator=(const Vertex& vertex);

  /*! @brief Add a particle or jet to the vertex 
   *  
   *  @param[in] pJet reference to non-modifiable @c fastjet::PseudoJet object
   *
   *  @note This method does not actually add a particle to a vertex, as
   *        @c Vertex does not hold references to particles. Rather, particles
   *        hold referencest to vertices. Here the various vertex 

   *        characteristics are updated according to the @c ParticleInfo 
   *        object attached to the @c fastjet::PseudoJet.
   *  
   */
  void addParticle(const fastjet::PseudoJet& pJet);

  /*! @brief Vertex id access */
  int id() const;

  /*! @name Vertex kinematics
   *
   *  @param vtype indicates all particles (@c Vertex::TOTAL), charged 
   *         (@c Vertex::CHARGED) or neutral (@c Vertex::NEUTRAL) particles.
   */
  /*!@{*/
  /*! @brief Sum of transverse momenta */
  double ptSum(Type vtype=TOTAL) const;
  /*! @brief Sum of squared tranverse momenta */
  double ptSum2(Type vtype=TOTAL ) const;
  /*! @brief Particle multiplicity */
  int multiplicity(Type vtype=TOTAL) const;
  /*!@}*/

  /*! @name Set order context
   *
   *  The order context is useful when applying comparators, or 
   *  building ordered lists of vertices.
   *
   *  @param[in] oc requested order context (optional, default is 
   *             @c Vertex::PTSUM2 
   *
   *  @note The order context can be changed even for a @c const reference to
   *        a @c Vertex. This requires two methods, one with a @c const 
   *        qualifier.
   */
  /*!@{*/
  void setOrderContext(OrderContext oc=PTSUM2); 
  void setOrderContext(OrderContext oc=PTSUM2) const;
  /*!@}*/

  /*! @brief Returns present order context */
  OrderContext orderContext() const;

  /*! @name Operators on @c Vertex
   *
   *  Comparators are defined in an order context. 
   *
   *  @param[in] vtx reference to non-modifiable @c Vertex to compare to.
   *
   *  For example, if order context is @c Vertex::PTSUM2, 
   *  `vtx0 < vtx1 == true` if `vtx0.ptSum2() < vtx1.ptSum2()`.
   *
   *  @note If the order context of @c this vertex is different from @c vtx, 
   *        the order context of @c vtx is temporarily set to the one of 
   *        @c this.
   *  
   */
  /*!@{*/
  bool operator==(const Vertex& vtx) const;
  bool operator!=(const Vertex& vtx) const;
  bool operator>=(const Vertex& vtx) const;
  bool operator>(const Vertex& vtx) const;
  bool operator<=(const Vertex& vtx) const;
  bool operator<(const Vertex& vtx) const;
  /*!@}*/
  
private:
  
  /*! @brief Vertex id */
  int m_id;
  
  /*! @name Kinematic properties */
  /*!@{*/
  /*! @brief @f$ \sum{p_{\rm T}} @f$ of all particles assigned to vertex */
  double m_ptSum;
  /*! @brief @f$ \sum{p_{\rm T}^{2}} @f$ of all particles assigned to vertex */
  double m_ptSum2;
  /*! @brief @f$ \sum{p_{\rm T}} @f$ of charged particles assigned to vertex */
  double m_ptSumCharged;
  /*! @brief @f$ \sum{p_{\rm T}^{2}} @f$ of charged particles assigned to vertex */  
  double m_ptSum2Charged;
  /*!@}*/

  /*! @name Multiplicities */
  /*!@{*/
  unsigned int m_nPart;        /*!< @brief All assigned particles */
  unsigned int m_nPartCharged; /*!< @brief Assigned charged particles */
  /*!@}*/

  /*! @brief Actual order context */
  mutable OrderContext m_orderContext;
};

inline void Vertex::setOrderContext(OrderContext oc)
{ m_orderContext = oc; }
inline void Vertex::setOrderContext(OrderContext oc) const
{ m_orderContext = oc; }
inline Vertex::OrderContext Vertex::orderContext() const
{ return m_orderContext; }

inline bool Vertex::operator==(const Vertex& vtx) const
{ return m_id == vtx.id(); }
inline bool Vertex::operator!=(const Vertex& vtx) const
{ return m_id != vtx.id(); }

/*! @class Vertex
 *
 *  @brief Container for vertex variables 
 *
 *  This data object holds kinematic and multiplicity  information for a given
 *  vertex, in addition to the identification (a number). The object features
 *  a @c Vertex::OrderContext state, which allows to compare two objects with
 *  respect to kinematic ordering and equality with respect to a client
 *  chosen metric. Available choices are @f$\sum{p_{\rm T}}@f$ of all
 *  or charged particles assigned to the vertex, and @f$\sum{p_{\rm T}^{2}}@f$
 *  with the same particle selections.   
 */

class Event;

class ParticleInfo : public fastjet::PseudoJet::UserInfoBase
{
public:

  /*! @brief Default constructor
   *
   *  This constructor generates an "emtpy" @c ParticleInfo object, which 
   *  is not of much use.
   *
   *  @note All data content for @c ParticleInfo can only be set at 
   *        construction! Once a @c ParticleInfo object is instantiated, 
   *        its content is frozen.
   */
  ParticleInfo();
  /*! @brief Constructor
   *
   *  @param[in] id     particle indentifier (PDG code) 
   *  @param[in] charge particle charge  
   *  @param[in] vtx    vertex indentifier (vertex number from @c
   *                    Vertex::id() )
   *  @param[in] type   specifies particle source like @c Vertex::SIGNAL or 
   *                    @c Vertex::PILEUP (optional, default is 
   *                    @c Vertex::UNKNOWN).
   */
  ParticleInfo(int id,double charge,int vtx,
	       Vertex::InteractionType type=Vertex::UNKNOWN);
  /*! @breif Copy constructor
   * 
   *  @param[in] pInfo reference to non-modifiable source @c ParticleInfo
   *                   object. 
   */
  ParticleInfo(const ParticleInfo& pInfo);
  /*! @brief Base class destructor 
   *
   *  @c ParticleInfo can be used as a base class for client implemented 
   *  extensions.
   */
  virtual ~ParticleInfo();
  
  /*! @name Data access */
  /*!@{*/
  int id()                                  const; /*!< @brief Particle id 
						     (PDG) */
  double charge()                           const; /*!< @brief Charge */
  int vertexId()                            const; /*!< @brief Vertex id */
  Vertex::InteractionType interactionType() const; /*!< @brief Source */
  /*!@}*/

  /*! @name Queries */
  /*!@{*/
  bool isPileup() const; /*!< @brief Returns @c true if particle is pile-up */
  bool isSignal() const; /*!< @brief Returns @c true if particle is signal */
  /*!@}*/

  /*! @brief Retrieve full vertex
   * 
   *  The full specification of the vertex associated with a particle is
   *  stored in the @c Event object. This method allows to retrieve this
   *  information for the particle described by the @c ParticleInfo object.
   * 
   *  @param[in]    thisEvt reference to @c Event object
   *  @param[inout] vertex  reference to the modifiable @c Vertex object to 
   *                        which the particle was added
   *
   *  @return @c true if @c Vertex information available. 
   */
  const bool vertex(const Event& thisEvt,Vertex& vertex) const; 
  
private:

  /*! @brief Particle id */
  int    m_id;
  /*! @brief Particle charge */
  double m_charge;
  /*! @brief Particle vertex id */
  int    m_vertexId;
  /*! @brief Particle source */
  Vertex::InteractionType m_interactionType;
};

/*! @class ParticeInfo
 *
 *  @brief Container for particle parameters and characteristics not 
 *         accommodated by @c fastjet::PseudoJet.
 *
 *  This object is intended to be attached to a @c fastjet::PseudoJet object
 *  using the @c user_info features (requires @c Fastjet version 3.X.X!)
 *  
 *      fastjet::PseudoJet pJet(...);
 *      ParticleInfo* pInfo = new ParticleInfo(...);
 *      pJet.set_user_info<ParticleInfo>(pInfo);
 *
 *  @c ParticleInfo can be safely accessed by
 * 
 *      if ( pJet.has_user_info<ParticleInfo>() ) 
 *        { const ParticleInfo& pInfo = pJet.user_info(); ... }
 *
 */

struct Converters
{
  /*! @name Converters
   *
   *  These methods have a template parameter which is the type of the
   *  @c ROOT raw data structure containing the particle information.
   *  While the requirements on the data structure are stringent, the 
   *  template is required to accmmodate the classes automatically
   *  generated by @c TTree::MakeClass(), which typically are named like
   *  the tree itself (class name is @c TTree::GetName()). 
   * 
   *  In any case, it is essential that the following arrays are
   *  available:
   *
   *      dataSource.Pt[]     // particle pt
   *      dataSource.Rap[]    // particle rapidity
   *      dataSource.Phi[]    // particle azimuth
   *      dataSource.Pm[]     // particle mass
   *      dataSource.ID[]     // particle id
   *      dataSource.Charge[] // particle charge
   *
   *  @param[in]    dataSource raw data structure (@c ROOT tuple)
   *  @param[in]    vtx        id of asscoiated vertex
   *  @param[in]    type       source of particle
   *  
   *  @return Number of converted particles.
   *
   */
  /*!@{*/
  /*! @brief Single object conversion 
   *
   *  @param[in]    index      index of particle in array 
   *  @param[inout] pJet       reference to modifiable @c fastjet::PseudoJet
   *                           object.
   *
   *  After invocation of this method the @c fastjet::PseudoJet referenced by
   *  @c pJet is updated and a @c ParticleInfo object is attached to it.
   */
  template<class T>
  static int convertToPJ(const T& dataSource,int index,int vtx,
			 Vertex::InteractionType type,
			 fastjet::PseudoJet& pJet)
  {
    pJet.reset(fastjet::PtYPhiM(dataSource.Pt[index],
				dataSource.Rap[index],
				dataSource.Phi[index],
				dataSource.Pm[index]));
    pJet.set_user_info(new ParticleInfo(dataSource.ID[index],
					dataSource.Charge[index],
					vtx,type));
    return 1;
  }
  /*! @brief Single object conversion 
   *
   *  @param[in]    findex     first index of particles in array
   *  @paran[in]    lindex     last index (+1) of particles in array 
   *  @param[inout] vJet       reference to modifiable container for 
   *                           @c fastjet::PseudoJet objects.
   *
   *  A total of `lindex - findex` particles are converted. 
   *
   *  After invocation of this method the @c vJet container is cleared and
   *  refilled with the requested number of @c fastjet::PseudoJet objects.
   */
  template<class T>
  static int convertToPJ(const T& dataSource,int findex,int lindex,int vtx,
			 Vertex::InteractionType type,
			 std::vector<fastjet::PseudoJet>& vJet)
  {
    vJet.clear();
    for ( int i=findex;i<lindex;++i )
      {
	vJet.push_back(fastjet::PseudoJet(0.,0.,0.,0.));
	convertToPJ(dataSource,i,vtx,type,vJet.back());
      }
    return (int)vJet.size();
  }
  /*!@{*/
};
/*! @struct Converters 
 *
 *  Collection of functions converting raw data to @c fastjet::PseudoJet
 */

struct Features
{
  /*! @name Check on particle charge 
   *
   *  @param[in] pJet reference to non-modifiable @c fastjet::PseudoJet
   *
   *  @return @c true if the corresponding particle is charged, @c false
   *          if it is neutral or has no @c ParticleInfo object attached.
   *
   *  @note `!isCharged(pJet) == isNeutral(pJet)` only if @c pJet is a 
   *        @c fastjet::PseudoJet with a @c ParticleInfo object attached.
   */
  /*!@{*/
  static bool isCharged(const fastjet::PseudoJet& pJet);
  static bool isNeutral(const fastjet::PseudoJet& pJet);
  /*!@}*/

  /*! @name Number of charged particles 
   *
   *  Calculates the number of charged/neutral particles in a list, optionally 
   *  restricted to a given vertex.
   *
   *  @param[in]    vJet reference to non-modifiable list of particles
   *  @param[inout] nJets number of charged/neutral particles found
   *  @param[in]    vtx consider only particles at vertex with this id 
   *                (optional, default -1 means all vertices)
   *  @param[in]    type consider only particles from this collision type
   */
  /*!@{*/
  static bool nCharged(const std::vector<fastjet::PseudoJet>& vJet,int& nJets,
		       int vtx=-1);
  static bool nCharged(const std::vector<fastjet::PseudoJet>& vJet,int& nJets,
		       Vertex::InteractionType type);
  static bool nNeutral(const std::vector<fastjet::PseudoJet>& vJet,int& nJets,
		       int vtx=-1);
  static bool nNeutral(const std::vector<fastjet::PseudoJet>& vJet,int& nJets,
		       Vertex::InteractionType type);
  /*!@}*/

  /*! @brief Checks on interaction type
   *
   *  @param[in] pJet reference to non-modifiable particle
   *  @param[in] type requested collision type (particle source)
   * 
   */
  static bool hasInteractionType(const fastjet::PseudoJet& pJet,
				 Vertex::InteractionType type);
  /*! @brief Checks on vertex id 
   *
   *  @param[in] pJet reference to non-modifiable particle
   *
   *  @return Valid vertex id @f$0\ldots N_{\rm vtx}@f$ or -1 if
   *          failure.
   */
  static int vertexId(const fastjet::PseudoJet& pJet);

  /*! @brief Get particle id
   *
   *  @param[in] pJet reference to non-modifiable particle
   *
   *  @return PDG code (or whatever was stored in the raw data), 0 if problem.
   */
  static int particleId(const fastjet::PseudoJet& pJet);
};
/*! @struct Features
 *
 *  Collection of helper functions to determine features of the particle or
 *  jet represented by a @c fastjet::PseudoJet with an attached 
 *  @c ParticleInfo object. 
 */


struct Utils
{
  /*! @name Copying @c fastjet::PseudoJet 
   *
   *  The following function suite provides deep copies of 
   *  @c fastjet::PseudoJet objects, including an actual copy of 
   *  @c ParticleInfo (instead of a sharing this one between the original
   *  and the copy).
   *
   *  Single object copy:
   *
   *  @param[in]    inJet reference to non-modifiable @c fastjet::PseudoJet to
   *                be copied
   *  @param[inout] outJet reference to modifiable @c fastjet::PseudoJet 
   *                object
   *
   *  Multiple object copies:
   *
   *  @param[in]    inJets reference to list of non-modifiable 
   *                @c fastjet::PseudoJet objects to be copied
   *  @param[inout] outJets reference to modifiable list of 
   *                @c fastjet::PseudoJet objects to be filled with copies
   *
   *  @warning These implementations should be stripped of the deep copy
   *           feature, as sharing the 
   *           @c ParticleInfo should be ok anyway. This is mainly 
   *           true because the @c ParticleInfo object can not be modified
   *           after its instantiation anyway - meaning its data content
   *           can not be overwritten/changed.
   *
   */
  /*!@{*/
  /*! @brief Full copy of single @c fastjet::PseudoJet */
  static void copyPJ(const fastjet::PseudoJet& inJet,
		     fastjet::PseudoJet& outJet);
  /*! @brief Full copy of list of @c fastjet::PseudoJet objects */
  static void copyPJ(const std::vector<fastjet::PseudoJet>& inJets,
		     std::vector<fastjet::PseudoJet>&       outJets);
  /*! @brief Conditional copying: exclusively copy only charged or neutral 
   *         particles from list of @c fastjet::PseudoJet objects
   * 
   *  @param[in] charged if @c true only charged particles are copied, 
   *             else only neutrals are copied.
   */
  static void copyPJ(const std::vector<fastjet::PseudoJet>& inJets,
		     std::vector<fastjet::PseudoJet>&       outJets,
		     bool charged);
  /*! @brief Conditional copying: exclusively copy only 
   *         particles from the same source from list of @c fastjet::PseudoJet 
   *         objects
   * 
   *  @param[in] type specification of requested source
   */
  static void copyPJ(const std::vector<fastjet::PseudoJet>& inJets,
		     std::vector<fastjet::PseudoJet>& outJets,
		     Vertex::InteractionType type);
  /*! @brief Conditional copying: exclusively copy only charged or neutral 
   *         particles from the same source from list of @c fastjet::PseudoJet 
   *         objects
   * 
   *  @param[in] type specification of requested source
   *  @param[in] charged if @c true only charged particles are copied, 
   *             else only neutrals are copied.
   */
  static void copyPJ(const std::vector<fastjet::PseudoJet>& inJets,
		     std::vector<fastjet::PseudoJet>& outJets,
		     Vertex::InteractionType type,
		     bool charged);
  /*!@}*/

  /*! @brief Setting the order context for a group of vertices
   *
   *  @param[in] vertices reference to list of modifiable @c Vertex objects
   */
  static void setOrderContext(std::vector<Vertex>& vertices,
			      Vertex::OrderContext oc=Vertex::PTSUM2)
  {
    std::vector<Vertex>::iterator fVtx(vertices.begin());
    std::vector<Vertex>::iterator lVtx(vertices.end());
    for ( ; fVtx != lVtx; ++fVtx ) { fVtx->setOrderContext(oc); }
  }

  /*! @brief Vertex sorting 
   *
   *  @param[inout] vertices reference to modifiable list of vertices 
   *                containing the vertices to be ordered on input and
   *                returning the ordered vertices on output.
   *  @param[in]    oc order context (optional, default is @c Vertex::PTSUM2
   *                (ordered descending in @f$\sum{p_{\rm T}^{2}}@f$) 
   */
  static void sortVtx(std::vector<Vertex>& vertices,
		      Vertex::OrderContext oc=Vertex::PTSUM2)
  { setOrderContext(vertices,oc);
    std::sort(vertices.begin(),vertices.end()); }
};
/*! @struct Utils
 *
 *  Collection of useful functions supporting object copying and others. 
 */

template<class T>
struct DataHandler
{
  /*! @brief Fill @c Event structure from raw data
   *
   *  This method fills the @c Event structure from the raw @c ROOT typle data.
   *  The assignment of the converted particles to vertices and vertex is 
   *  controlled, and the number of collisions to be merged can be specified.
   *
   *  @return @c true if @c Event is completely filled. If the input file 
   *          stream is exhausted, @c false is returned.
   * 
   *  @param[in]    dataSource reference to raw data container
   *  @param[inout] event      reference to modifiable @c Event to be filled 
   *                           from the data source
   *  @param[inout] nPtr       raw data index of next event
   *  @param[in]    mu         (average) number of collisions to be collected
   *  @param[in]    vtxOffset  integral offset of vertex id (optional, default
   *                           0)
   *  @param[in]    type       collision type to be assigned to all particles
   *                           in this event (optional, default is 
   *                           @c Vertex::UNKNOWN).
   *  @note @b nPtr should not be manipulated by the calling module, as it
   *           keeps track of the general I/O. The correct use is shown in 
   *           the example below
   * @code{.cpp}
   *       Event*     pEvt = new Event;       // get a new Event
   *       long long  nPtr(0);                // initialize record counter
   *       MyRawData* pRaw = new MyRawData(); //
   *       ...
   *       DataHandler<MyRawData> pHdlr = new DataHandler<MyRawData>();
   *     
   *       while ( !pHdlr->fill(*pRaw,*pEvt,nPtr,30,0,Vertex::PILEUP) )
   *         {
   *           .....            // your analysis
   *           pEvt->reset();   // do not forget!!!  
   *         }
   * @endcode
   *
   * The input file stream is exhausted once `nPtr + nEvts > nAllEvts`, 
   * meaning when the index @c nPtr to the next raw data structure after the 
   * last complete 
   * event plus the number of single collision events to be loaded @c nEvts 
   * (possibly
   * sampled from a Poisson distribution around @c mu ), is larger than the
   * total number of events @c nAllEvts available in the input stream.
   */
  bool fillEvent(T& dataSource,Event& event,long long& nPtr,int mu,
		 int vtxOffset=0,Vertex::InteractionType type=Vertex::UNKNOWN);
  /*! @brief Actual number of collisions 
   * 
   *  @param[in] mu (average) number of collisions 
   *
   *  @return If `mu < 0`, the absolute value @c |mu| is returned. 
   *          If `mu > 0`, a
   *          number sampled from a Poisson distribution with an average of
   *          @c mu is returned. 
   *
   *  @note This function is mostly useful for internal use in 
   *        @c DataHandler::fillEvent . 
   */
  int  getActVtx(int mu); 
};
/*! @struct DataHandler
 *
 *  Collection of functions filling the @c Event structure from the raw
 *  data. 
 */

class Event
{
public:

  /*! @brief Standard constructor
   * 
   *  @param[in] pSel pointer to final state selector (optional, default 
   *             @c NULL - no selection) 
   */
  Event(IFinalStateSelector* pSel=0);
  Event(Event* parent,IFinalStateSelector* pSel=0);
  /*! @brief Base class destructor */
  virtual ~Event();

  /*! @brief Set parent
   *
   */
  void   setParent(Event* pEvt);
  Event* parent();
  /*! @name Add particle(s)
   *
   *  All methods in this group return @c true if adding the particle was 
   *  successful. 
   */
  /*!@{*/
  /*! @brief Add particle using a set of variables
   *
   *  @param[in] ID particle id (PDG code)
   *  @param[in] charge particle charge
   *  @param[in] px particle momentum component @f$p_{x}@f$ 
   *  @param[in] py particle momentum component @f$p_{y}@f$ 
   *  @param[in] pz particle momentum component @f$p_{z}@f$
   *  @param[in] m  particle mass @f$m@f$
   *  @param[in] vtx associated vertex id (optional, default 0)
   *  @param[in] type associated vertex type (optional, default 
   *             @c Vertex::UNKNOWN) 
   *
   *  Internally creates a @c fastjet::PseudoJet object and invokes 
   *  `Event::add(const fastjet::PseudoJet& pJet)`. 
   */
  virtual bool add(int ID, double charge, 
		   double px,double py,double pz,double m,
		   int vtx=0,Vertex::InteractionType type=Vertex::UNKNOWN);
  /*! @brief Add a particle represented by a @c fastjet::PseudoJet 
   *
   *  @param[in] pJet reference to non-modifiable @c fastjet::PseudoJet 
   * 
   *  It is expected that the @c fastjet::PseudoJet has a @c ParticleInfo
   *  objects attached, otherwise the vertex allocation will fail.
   */
  virtual bool add(const fastjet::PseudoJet& pJet);
  /*! @brief Add a list of particles represented by @c fastjet::PseudoJet
   * 
   *  @param[in] reference to non-modifiable list of @c fastjet::PseudoJet
   *             objects
   * 
   *  It is expected that each @c fastjet::PseudoJet has a @c ParticleInfo
   *  objects attached, otherwise the vertex allocation will fail.
   */
  virtual bool add(const std::vector<fastjet::PseudoJet>& vJet);
  /**!@}*/

  /*! @name Check event content */
  /*!@{*/
  virtual bool haveParticleEvent(); /*!< @brief returns @c true if stable 
				      particles in event */
  virtual bool havePartonicEvent(); /*!< @brief returns @c true if partons
				     in event */ 
  /*!@}*/

  /*! @name Event content access
   * 
   *  All methods in this group return a list of @c fastjet::PseudoJet
   *  objects representing the requested part of the (particle/hadron level)
   *  event.
   *
   *  @note Particles may be subjected to general filters (final state
   *        selectors) if the @c Event object receives a valid pointer to one 
   *        of those at its construction. 
   * 
   *  @todo Access to partonic information not yet implemented. 
   */
  /*!@{*/
  /*! @brief Retrieve all particles
   * 
   *  @param[in] vtx specifies vertex id if only particles from a specific 
   *             vertex are to be retrieved (optional, default @c -1 means
   *             all particles from all vertices).  
   */
  virtual std::vector<fastjet::PseudoJet> pseudoJets(int vtx=-1);
  /*! @brief Retrieve all charged/neutral particles
   *
   *  @param[in] charged retrieve charged particles if @c true, else neutrals.
   *  @param[in] vtx specifies vertex id if only particles from a specific 
   *             vertex are to be retrieved (optional, default @c -1 means
   *             all particles from all vertices).  
   */
  virtual std::vector<fastjet::PseudoJet> pseudoJets(bool charged,int vtx=-1);
  /*! @brief Retrieve all particles from a given source  
   * 
   *  @param[in] type particle source specification
   *
   * An example: 
   * @code{.cpp}
   *     Event*       pEvt = new Event;            // get a new Event
   *     long long    nPtrPU(0);                   // initialize record counter
   *     long long    nPtrS(0);                    // initialize record counter
   *     MyRawPUData* pPURaw = new MyRawPUData();  // pile-up stream
   *     MyRawSData*  pSRaw  = new MyRawSData();   // signal stream
   *     ...
   *     DataHandler<MyRawPUData> puHdlr = new DataHandler<MyRawPUData>();
   *     DataHandler<MyRawSData>  psHdlr = new DataHandler<MyRawSData>();
   *     
   *     while ( !psHdlr->fill(*pSRaw,*pEvt,nPtrS,-1,0,Vertex::SIGNAL) &&
   *             !puHdlr->fill(*pPURaw,*pEvt,nPtrPU,30,1,Vertex::PILEUP) )
   *       {
   *         // get the full particle level event
   *         std::vector<fastjet::PseudoJet> fullEvt = pEvt->pseudoJets();
   *         // get the particle level pile-up
   *         std::vector<fastjet::PseudoJet> puEvt = pEvt->pseudoJets(Vertex::PILEUP);
   *         // get the particle level signal 
   *         std::vector<fastjet::PseudoJet> sigEvt = pEvt->pseudoJets(Vertex::SIGNAL);
   *         .....            // your analysis
   *         pEvt->reset();   // do not forget!!!  
   *       }
   * @endcode
   */
  virtual std::vector<fastjet::PseudoJet> pseudoJets(Vertex::InteractionType 
						     type);
  /*! @brief Retrieve all charged/neutral particles from a given source  
   * 
   *  Identical to @c Event::pseudoJets(Vertex::InteractionType type) , with
   *  an additional charge (or neutral) filter
   *
   *  @param[in] charged retrieve charged particles if @c true, else neutrals.
   *  @param[in] type particle source specification
   */
  virtual std::vector<fastjet::PseudoJet> pseudoJets(bool charged,
						     Vertex::InteractionType 
						     type);
  /*!@}*/


  /*! @name Event specifications */
  /*!@{*/
  /*! @brief Check if event is empty 
   *
   *  @param[in] vtxId checks only specific vertex (optional, default @c -1 
   *             checks all vertices)
   *
   *  @return @c true if any particles in event
    */
  virtual bool empty(int vtxId=-1) const; 
  /*! @brief Access number of particles
   *
   *  @param[in] vtxId return number of particles from given vertex (optional,
   *             default @c -1 means sum particles from all vertices)
   */
  virtual int  size(int vtxId=-1)  const;
  /*! @brief Return number of particles
   *
   *  @param[in] vtxId return number of particles from given vertex (optional,
   *             default @c -1 means sum particles from all vertices)
   */
  virtual int  nParticles(int vtxId=-1) const;
  /*! @brief Return number of partons
   *
   *  @param[in] vtx return number of partons from given vertex (optional,
   *             default @c -1 means sum partons from all vertices)
   *
   *  @warning Not yet implemented!
   *  @todo Provide missing implementation of parton access!  
   */
  virtual int  nPartons(int vtxId=-1)   const;
  /*! @brief Access number of vertices
   *
   *  @return Number of vertices in event. In case of a merged signal and 
   *          pile-up event, this number is given by the number of pile-up
   *          collisions @c mu merged with the signal, meaning this method
   *          returns `mu + 1`.
   */
  virtual int  nVertices()            const;
  /*!@}*/

  /*! @name Vertex retrieval */
  /*!@{*/
  /*! @brief Retrieve specific vertex
   *
   *  @param[0] vtx id of vertex to be retrieved (optional, default is
   *            first vertex in list, specified by `vtxId = -1`)
   *
   *  @note The list of vertices in the @c Event is not ordered per se. 
   *        The first vertex has only special meaning if in case of a
   *        signal event merged with pile-up, the vertex id offset was 
   *        chosen such that e.g. `vtxId = -1` means signal vertex. This 
   *        is strongly relying on a notation controlled by the client.  
   */
  virtual const Vertex& vertex(int vtxId=-1) const;
  /*! @brief Retrieve all vertices
   *
   *  @return Reference to non-modifiable list of vertices in @c Event.
   */
  virtual const std::vector<Vertex>& vertices() const;
  /*@}*/

  /*! @brief Reset all @c Event caches 
   *
   *  This method need to be invoked by the client before building a new
   *  event.
   */
  virtual void reset();

protected:

  /*! @brief Internal cache update */
  bool updateCache(const fastjet::PseudoJet& pJet);
//   /*! @brief Internal vertex list update */
//   void updateVtx(int vtx);
//   /*! @brief Internal function adding a particle to a vertex */
//   bool addToVtx(const fastjet::PseudoJet& pJet);

  /*! @brief Reference to dummy vertex for failed function returns */
  static const Vertex& dummyVtx();

private:

  /*! @brief Reference to final state selector */
  IFinalStateSelector* m_fsSel;

  /*! @brief Cache for particles in event
   *
   *  The external container is indexed by vertex id (>= 0). This means the
   *  cache cannot accommodate invalid vertices with id < 0.
   *
   *  For each vertex, a list of particles is attached.   
   */
  mutable std::vector<std::vector<fastjet::PseudoJet> > m_particles;

  /*! @brief Cache for vertices
   *
   *  This cache is index parallel to @c Event::m_particles. It contains the
   *  full specification for each vertex, as provided by the @c Vertex
   *  object.  
   */
  mutable std::vector<Vertex> m_vertices;
  /*! @brief Cache for partons in event
   *
   *  Has the same structure as @c Event::m_particles but is presently not
   *  filled. 
   */
  mutable std::vector<std::vector<fastjet::PseudoJet> > m_partons;

  Event* m_parent;

  static Vertex _dummyVertex;

  bool vtxIndex(int vtxId,size_t& idx) const;
};
/*! @class Event
 *
 *  Basic event container provides access to the full (signal+pile-up), 
 *  the signal,and the pile-up final state of a given event - if available.
 *  It accommodates simple filters on all particles.
 *
 *  @note While it is recommend to use only same @c Event object for 
 *        all full events to be analyzed, clients have to clear the cache
 *        in this object before loading a full new event!
 *                                          
 */

inline 
const std::vector<Vertex>& Event::vertices() const
{ return m_vertices; }

inline
bool Event::vtxIndex(int vtxId,size_t& idx) const
{ 
  std::vector<Vertex>::iterator fVtx(m_vertices.begin());
  std::vector<Vertex>::iterator lVtx(m_vertices.end());
  idx = 0;
  while ( fVtx != lVtx && vtxId != fVtx->id() ) { ++fVtx; ++idx; }
  return fVtx != lVtx; 
}

inline void Event::setParent(Event* parent)
{ m_parent = parent; }

inline Event* Event::parent()
{ return m_parent; }

inline
bool Features::isCharged(const fastjet::PseudoJet& pJet)
{ return pJet.has_user_info<ParticleInfo>() ?
    pJet.user_info<ParticleInfo>().charge() != 0. : false; }

inline 
bool Features::isNeutral(const fastjet::PseudoJet& pJet)
{ return pJet.has_user_info<ParticleInfo>() ?
    pJet.user_info<ParticleInfo>().charge() == 0. : false; }

inline
bool Features::hasInteractionType(const fastjet::PseudoJet& pJet,
				  Vertex::InteractionType type)
{ return pJet.has_user_info<ParticleInfo>() ? 
    (pJet.user_info<ParticleInfo>().interactionType() & type) == type : 
    false; }

inline
int Features::vertexId(const fastjet::PseudoJet& pJet)
{ return pJet.has_user_info<ParticleInfo>() ?
    pJet.user_info<ParticleInfo>().vertexId() : -1; }

inline
int Features::particleId(const fastjet::PseudoJet& pJet)
{ return pJet.has_user_info<ParticleInfo>() ?
    pJet.user_info<ParticleInfo>().id() : 0 ; }
#include "Event.icc"

#endif
