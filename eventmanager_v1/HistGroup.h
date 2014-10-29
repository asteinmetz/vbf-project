// -*- c++ -*-
#ifndef HISTGROUP_H
#define HISTGROUP_H

#include "fastjet/PseudoJet.hh"
#include "Event.h"

#include "Services.h"
#include "HistGroupCfg.h"

#include <vector>
#include <string>


/*! @brief Interface for @c HistGroup */
class IHistGroup
{
public:
  /*! @brief Pure abstract base class destructor */
  virtual ~IHistGroup() { }

  /*! @brief Interface to group name */
  virtual const std::string& name() const = 0;

  /*! @brief Interface to write out histograms */
  virtual void write() = 0;

  /*! @brief Interface to print out information concerning the @c HistGroup */
  virtual void print() = 0;  

  /*! @brief Booking interface */
  virtual void book() = 0;

  /*! @name Fill interfaces */
  /*!@{*/
  virtual void fill()                                            = 0;
  /*! @brief Fill from particle and/or jet final state
   *
   *  @param[in] vJet reference to non-modifiable list of particles and/or jets
   *             represented by @c fastjet::PseudoJet
   */
  virtual void fill(const std::vector<fastjet::PseudoJet>& vJet) = 0;
  /*! @brief Fill from @c Event
   *
   *  @param[in] rEvt reference to non-modifiable event data 
   */
  virtual void fill(Event& rEvt)                                 = 0;
  /*!@}*/

};

/*! @brief @c HistGroup implementations basec class */
class HistGroupBase : virtual public IHistGroup
{
public:
  /*! @brief Constructor
   *
   *  Constructs histogram group with given name.
   *
   *  @param[in] name reference to non-modifiable string containing group name
   *             (optional, default @c "HistGroupBase")
   */
  HistGroupBase(const std::string& name="HistGroupBase");
  /*! @brief Implementation base class destructor */
  virtual ~HistGroupBase();

  virtual const std::string& name() const;
  virtual void write();
  virtual void print();
  virtual void book();
  virtual void fill();
  virtual void fill(const std::vector<fastjet::PseudoJet>& vJet);
  virtual void fill(Event& rEvt);

  virtual void cacheEvent(Event* pEvt);
  virtual Event& eventCache();

private:

  /*! @brief Group name */
  std::string m_name;

  Event* m_cachedEvent;
  
protected:

  /*! @brief Pointer to histogram handler */
  Services::HistHandler* f_handler;

  /*! @brief Number of registered hists */
  int numberRegHist();
  int numberRegHist(const std::string& group);
};
#endif
