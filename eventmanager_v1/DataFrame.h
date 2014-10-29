// -*- c++ -*-

#ifndef DATAFRAME_H
#define DATAFRAME_H

#include "Configuration.h"
#include "Event.h"

#include <TROOT.h>
#include <TChain.h>
#include <TBranch.h>

#include <string>

#ifndef _BASIC_CACHE_RESERVE_
#define _BASIC_CACHE_RESERVE_ 1000000
#endif

class HistGroupEvent;

class DataFrame
{
public :

  /*! @brief Pointer to chain of @c ROOT trees */
  TChain* fChain;
  /*! @brief Number of current file in chain */
  Int_t   fCurrent;

  /*! @name Data structure 
   *
   * These are the leaves in the @c ROOT tuple. The macro parameter 
   * `_BASIC_CACHE_RESERVE_` defines the size of _each_ data array. The 
   * actuall used number of words is given by `Nentry` (dynamic - changes with 
   * every event).
   *
   * @note The variables are defined as `ROOT` types, to assure correct 
   *       byte mapping to the `ROOT` I/O system.   
   *
   * The data structure is organized as follows. The first `Npartons` entries
   * in each array are partonic kinematics as filled by the generating 
   * software. The next `Nparticles` entries are the stable particles in the
   * final state. Unpacking follows the following example:
   *
   * @code 
   * // parton loop
   * for ( Int_t i=0; i<Npartons; ++i ) { ... }
   * // stable particle loop
   * for ( Int_t i=Npartons; i<Nentry; ++i ) { ... }
   * @endcode
   * 
   * This means that `Nentry = Npartons + Nparticles`.  
   */
  /*!@{ */
  Int_t           Nentry;     /*!< @brief Number of entries in arrays */
  Int_t           Npartons;   /*!< @brief Number of partons */
  Int_t           Nparticles; /*!< @brief Number of stable particles */
  /*! @brief Particle ID code (PDG code) */
  Int_t           ID[ _BASIC_CACHE_RESERVE_ ];
  /*! @brief Particle status word
   *
   *  Typical convention is `Stat[i] = -1` or `Stat[i] = -2` for partons and
   *  `Stat[i] = 2` for stable particles. 
   */    
  Int_t           Stat[ _BASIC_CACHE_RESERVE_ ];
  /*! @brief Electric charge charge */
  Float_t         Charge[ _BASIC_CACHE_RESERVE_ ];
  /*! @brief Parton/particle momentum component @f$p_x@f$ 
   *  @warning Not used anymore!
   *  @code{.cxx} 
   *  Float_t         Px[ _BASIC_CACHE_RESERVE_ ];
   *  @endcode
   */
  /*! @brief Parton/particle momentum component @f$p_y@f$
   *  @warning Not used anymore!
   *  @code{.cxx} 
   *   Float_t         Py[ _BASIC_CACHE_RESERVE_ ];
   *  @endcode
   */
  /*! @brief Parton/particle momentum component @f$p_z@f$
   *  @warning Not used anymore!
   *  @code{.cxx} 
   *   Float_t         Pz[ _BASIC_CACHE_RESERVE_ ];
   *  @endcode
   */
  /*! @brief Parton/particle momentum @f$p@f$
   *  @warning Not used anymore!
   *  @code{.cxx} 
   *   Float_t         P0[ _BASIC_CACHE_RESERVE_ ];
   *  @endcode
   */
  /*! @brief Parton/Particle mass @f$m@f$ */
  Float_t         Pm[ _BASIC_CACHE_RESERVE_ ];
  /*! @brief Parton/particle transverse momentum @f$p_{\rm T}@f$ */
  Float_t         Pt[ _BASIC_CACHE_RESERVE_ ];
  /*! @brief Parton/particle rapidity @f$y@f$ */
  Float_t         Rap[ _BASIC_CACHE_RESERVE_ ];
  /*! @brief Parton/particle azimuth @f$\phi@f$ */
  Float_t         Phi[ _BASIC_CACHE_RESERVE_ ];
  /*! @brief Parton/particle pseudorapidity @f$\eta@f$ 
   *  @warning Not used anymore!
   *  @code{.cxx} 
   *   Float_t         Eta[ _BASIC_CACHE_RESERVE_ ];
    *  @endcode
   */
  /*!@}*/

  /*! @name List of `ROOT` branches 
   *
   * Internal use only. Holds memory addresses for data structures. 
   */
  /*!@{*/
  TBranch* b_Nentry;
  TBranch* b_Npartons;
  TBranch* b_Nparticles;
  TBranch* b_ID;
  TBranch* b_Stat;
  TBranch* b_Charge;
//   TBranch* b_Px;
//   TBranch* b_Py;
//   TBranch* b_Pz;
//   TBranch* b_P0;
  TBranch* b_Pm;
  TBranch* b_Pt;
  TBranch* b_Rap;
  TBranch* b_Phi;
  //  TBranch* b_Eta;
  /*!@}*/

  /*! @brief Constructor
   * 
   *  Constructs data structure. 
   * 
   *  @param[in] pChain pointer to `TChain` object (default `NULL`) 
   */
  DataFrame(TChain* pChain=0);
  /*! @brief Base class destructor */
  virtual ~DataFrame();

  /*! @brief Initialization
   *
   *  @param[in] pChain pointer to valid `TChain` object 
   */
  virtual void init(TChain* pChain);

  /* @name Data access */
  /*!@{*/
  /*! @brief Get entry (event) at given index 
   *
   *  @return Number of bytes read for entry at index `entry`
   *
   *  @param[in] entry index of requested entry,
   */
  virtual Int_t    getEntry(Long64_t entry);
  /*! @brief Load tree
   *
   *  @return `> 0` indicates successful read of next entry (event); `<0` 
   *          indicates end-of-date on input or error.
   *
   *  @param[in] 
   */
  virtual Long64_t loadTree(Long64_t entry);
  /*!@}*/

  /*! @brief Implementing an entry based selection 
   *
   *  @param[in] entry index to valid data structure entry
   */
  virtual Int_t cut(Long64_t entry);

  /*! @brief Event loop manager
   *
   *  Most control of the event loop is provided by the @c Configuration module
   *  (number of events to be analyzed, number of pile-up interactions, output
   *  file name...).
   *
   *  @param[in] collType   interaction type for correct vertex assigments
   *  @param[in] vtxOffset  vertex index offset
   *
   */
  virtual void loop(Vertex::InteractionType collType=Vertex::SIGNAL,
		    Int_t vtxOffset=0);

  /*! @brief Notification/messaging invoked after each new tree is loaded */
  virtual Bool_t notify();
  /*! @brief Dump/show event data
   *
   *  @param[in] entry index of event data (default `-1` indicates nothing to
   *             show in the default implementation.  
   */
  virtual void show(Long64_t entry = -1);

  /*! @brief Retrieve one event
   *
   *  @param[inout] entry     index to next event
   *  @param[inout] rEvent    reference to modifiable @c Event object
   *  @param[in]    collType  collision vertex type (optional, default
   *                          @c Vertex::SIGNAL)
   *  @param[in]    vtxOffset index offset for first vertex in event (optional,
   *                          default `0`) 
   *
   *  @note @c entry should not be manipulated by the client. To access the
   *           first event, set `entry = 0` before calling this method. From
   *           then on @c entry will be incremented by the system at each
   *           invocation of this method. 
   */
  virtual Bool_t getEvent(Long64_t& entry,Event& rEvent,Int_t mu,
			  Vertex::InteractionType collType=Vertex::SIGNAL,
			  Int_t vtxOffset=0);

  /*! @brief Get number of events processed */
  virtual Int_t numberOfEvents();

  /*! @brief Set printout frequency 
   *
   *  @param[in] nEvtsFreq printout frequency (optional, default @c -1 means 
   *             no printouts)
   */
  virtual void setPrintFrequency(Int_t nEvtsFreq=-1);

  /*! @brief Write output
   *
   * @return Returns @c true if everything worked out.
   */
  virtual bool write();

  /*! @brief Total number of events in chain */
  virtual Long64_t getMaxEvts();

protected:

  /*! @brief Increment number of events
   *
   *  @param[in] nevts number of events to be added (optional, default @c 1)
   */
  void _incEvts(Int_t nevts=1);

private:
  
  /*! @brief Number of processed events */
  Long64_t fEvtsProc;
  /*! @brief Printout frequency */
  Long64_t fEvtsFreq;

  /*! @brief Pointer to @c DataHandler
   *
   *  This object provides all functionality to convert the "raw" data 
   *  structure of the @c ROOT tuple to the @c Event object. 
   */
  DataHandler<DataFrame>* fDataHandler;

  /*! @brief Pointer to configuration data object */
  Configuration::ConfigData* fConfig;

  /*! @brief Pointer to histogram group */
  HistGroupEvent* fHistGrp;

public:

  /*! @brief Book histograms
   *
   *  Does nothing by default. 
   */
  virtual Bool_t book();

  /*! @brief Analysis entry 
   *
   *  Interface to analysis implemented by clients.
   */
  virtual Bool_t analyze(Event& pEvt);

protected:

  /*! @brief Printouts */
  Bool_t ticker();
};

inline void DataFrame::_incEvts(Int_t nevts)
{ fEvtsProc += nevts; }
#endif
