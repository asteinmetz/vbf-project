// -*- c++ -*-
#ifndef SERVICES_H
#define SERVICES_H

#include "TH1.h"

#include "PrintDef.h"

#include <map>
#include <vector>
#include <string> 

#include <cstdarg>
#include <cstdio>
#include <csignal>

#include <sstream>

#ifndef _PRINT_BUFFER_SIZE_
#define _PRINT_BUFFER_SIZE_ 2096
#endif

#ifndef _PRINT_PREFIX_WIDTH_
#define _PRINT_PREFIX_WIDTH_ 42
#endif

#define PFF_( _x_ ) \
  std::string nfmt = _prefixFormatter(module,_x_) + fmt;                \
  va_list args;						                \
  va_start(args,fmt);					                \
  vsprintf(Messaging::_printBuffer,nfmt.c_str(),args);			\
  va_end(args);                                                         \
  std::string mmm(Messaging::_formatBuffer());				\
  printf(mmm.c_str());
    

namespace Services
{
  namespace Messaging
  {
    /*! @name Message levels */
    /*!@{*/
    static const std::string info("INFO");
    static const std::string warning("WARNING");
    static const std::string error("ERROR");
    /*!@}*/
    /*! @name Terminal output control */
    /*!@{*/
    /*! @brief @b _Bold_ @b _on_ control sequence */
    static const std::string _boldOn("\033[1m");
    /*! @brief _Bold off_ control sequence  */
    static const std::string _boldOff("\033[0m");
    /*! @brief Buffer formmatter */
    static std::string _formatBuffer();
    /*! @brief Output buffer */
    static char _printBuffer[ _PRINT_BUFFER_SIZE_ ];
    /*! @brief Level tag */
    static const std::string _levelTag("-");
    /*!@}*/
  }
  /*! @namespace Services::Messaging
   *
   * @brief Format descriptors and message manipulators for terminal 
   *        output.
   */

  namespace Print
  {
    /*! @brief Prefix formatter
     *
     *  @return Formatted string with module name and message level.
     *
     *  @param[in] module name of printing module
     *  @param[in] level  message level
     */
    static std::string _prefixFormatter(const std::string& module,
					const std::string& level)
    { return module+std::string(" - ")+level+std::string(" - "); }

    /*! @brief General printer command
     *
     *  @param[in] module name of printing module
     *  @param[in] level  message level
     *  @param[in] fmt `C` style formatting string
     *  @param[in] ... variable argument list corresponding to `fmt`
     */
    static void message(const std::string& module,const std::string level,
			const std::string& fmt,...)
    { PFF_( level ) }

    /*! @name Specialized print commands
     * 
     *  All special print commands have the following arguments
     *
     *  @param[in] module name of printing module
     *  @param[in] fmt `C` style formatting string
     *  @param[in] ... variable argument list corresponding to `fmt`.
     */
    /*!@{*/
    /*! @brief Print info message */
    static void info(const std::string& module,const std::string& fmt,...)
    { PFF_( Messaging::info ) }
    static void warning(const std::string& module,const std::string& fmt,...)
    { PFF_( Messaging::warning ) }
    static void error(const std::string& module,const std::string& fmt,...)
    { PFF_( Messaging::error ) }
    /*!@}*/
  };
  /*! @namespace Services::Print 
   *
   * @brief Collects print-to-terminal commands.
   */

  class HistHandler
  {
  public:

    /*! @brief Destructor */
    ~HistHandler();

    /*! @brief Pointer allocator */
    static HistHandler* instance();

    /*! @name Booking methods 
     *
     *  1-dim histogram like `TH1D` and `TProfile`:
     *
     *  @param[in] name  histogram name
     *  @param[in] title histogram title
     *  @param[in] nbins number of bins (x axis)
     *  @param[in] xmin  lower boundary (x axis)
     *  @param[in] xmax  upper boundary (x axis)
     *  @param[in] xtitle x-axis title (optional, default `""`)
     *  @param[in] ytitle y-axis title (optional, default `""`)
     *
     *  Additional arguments for 2-dimensional histograms like `TH2D` and
     *  `TProfile2D`:
     *
     *  @param[in] mbins number of bins (y axis)
     *  @param[in] ymin  lower boundary (y axis)
     *  @param[in] ymax  upper boundary (y axis)
     *  @param[in] ztitle z-axis title (optional, default `""`)
     * 
     *  Additional arguments for 3-dimensional histograms like `TH3D`
     *
     *  @param[in] lbins number of bins (z axis)
     *  @param[in] zmin  lower boundary (z axis)
     *  @param[in] zmax  upper boundary (z axis)
     */ 
    /*!@{*/
    template<class H>
    H* bookHist(const std::string& name,
		const std::string& title,
		int nbins,double xmin,double xmax,
		const std::string& xtitle="",
		const std::string& ytitle="");
    template<class H>
    H* bookHist(const std::string& group,
		const std::string& name,
		const std::string& title,
		int nbins,double xmin,double xmax,
		const std::string& xtitle="",
		const std::string& ytitle="");
    template<class H>
    H* bookHist(const std::string& name,
		const std::string& title,
		int nbins,double xmin,double xmax,
		int mbins,double ymin,double ymax,
		const std::string& xtitle="",
		const std::string& ytitle="",
		const std::string& ztitle="");
    template<class H>
    H* bookHist(const std::string& group,
		const std::string& name,
		const std::string& title,
		int nbins,double xmin,double xmax,
		int mbins,double ymin,double ymax,
		const std::string& xtitle="",
		const std::string& ytitle="",
		const std::string& ztitle="");
    template<class H>
    H* bookHist(const std::string& name,
		const std::string& title,
		int nbins,double xmin,double xmax,
		int mbins,double ymin,double ymax,
		int lbins,double zmin,double zmax,
		const std::string& xtitle="",
		const std::string& ytitle="",
		const std::string& ztitle="");
    template<class H>
    H* bookHist(const std::string& group,
		const std::string& name,
		const std::string& title,
		int nbins,double xmin,double xmax,
		int mbins,double ymin,double ymax,
		int lbins,double zmin,double zmax,
		const std::string& xtitle="",
		const std::string& ytitle="",
		const std::string& ztitle="");
    /*!@}*/

    /*! @brief Write all histograms in registry
     *
     *  @return Number of written histograms 
     * 
     *  @param[in] noEmpties suppresses the writing of empty histograms if
     *             `true` (optional, default is `true`)
     */
    unsigned int writeHist(bool noEmpties=true);
    /*! @brief Write all histograms of a given group
     *
     *  @return Number of written histograms 
     * 
     *  @param[in] group name of histogram group. 
     *  @param[in] noEmpties suppresses the writing of empty histograms if
     *             `true` (optional, default is `true`)
     */
    unsigned int writeHist(const std::string& group,bool noEmpties=true);
    /*! @brief Write specific single histogram
     *
     *  @return Number of written histograms 
     * 
     *  @param[in] group name of histogram group.
     *  @param[in] name  histogram name 
     *  @param[in] noEmpties suppresses the writing of empty histograms if
     *             `true` (optional, default is `true`)
     */
    unsigned int writeHist(const std::string& group,const std::string& name,
			   bool noEmpties=true);

    /*! @brief Access default group name */
    const std::string& defaultGroupName() const;

    /*! @brief Get number of histograms in registry */
    unsigned int numberRegHist() const;

    /* @brief Get number of histograms of a given group in registry */
    unsigned int numberRegHist(const std::string& group) const;

  private:

    /*! @brief Default group name */
    static std::string m_defaultGroupName;
    
  protected:

    /*! @name Histogram name constructors
     *
     *  @return String with histogram name built from input.
     *
     *  Function arguments are:
     * 
     *   @param[in] group name of histogram group (if used)
     *   @param[in] name histogram name
    */
    /*!@{*/
    std::string getHistName(const std::string& name);
    std::string getHistName(const std::string& group,const std::string& name);
    /*!@}*/

    /*! @name Histogram registry types
     *
     *  Histograms are entered into the registry with a key which is 
     *  constructed from the group name and the histogram name.
     */
    /*@{*/
    typedef std::pair<std::string,std::string> key_t; /*!< @brief key type */
    typedef std::map<key_t,TH1*>               map_t; /*!< @brief registry 
							type */
    /*!@}*/

    /*! @brief Register function
     *
     *  The histogram is registered in the default group.
     * 
     *  @param[in] pHist pointer to histogram
     * 
     *  @note The histogram type `H` must have a base class `TH1` - this is the
     *         case for all `ROOT` histograms independent of their specific 
     *         dimensions.
     *
     */
    template<class H>
    void registerHist(H* pHist)
    { registerHist(m_defaultGroupName,pHist); }

    /*! @brief Register function

     *
     *  The histogram is registered in the specified group.
     *
     *  
     *  @param[in] group name of histogram group
     *  @param[in] pHist pointer to histogram
     * 
     *  @note The histogram type `H` must have a base class `TH1` - this is the
     *         case for all `ROOT` histograms independent of their specific 
     *         dimensions.
     */
    template<class H>
    void registerHist(const std::string& group,H* pHist)
    {
      if ( pHist != 0 )
	{
	  key_t key(group,std::string(pHist->GetName()));
	  if ( _histRegistry.find(key) == _histRegistry.end() )
	    { _histRegistry.insert(map_t::value_type(key,(TH1*)pHist)); }
	  else
	    { Print::warning("HistHandler::registerHist(%p)",
			     "Histogram <%s/%s> already registered.",
			     (void*)pHist,key.first.c_str(),
			     key.second.c_str());}
	}
      else
	{ 
	  Print::error("HistHandler::registerHist(%p)",
		       "Cannot register histogram, invalid pointer!",
		       (void*)pHist); 
	}
    }

  private:

    /*! @brief Private constructor of singleton */
    HistHandler();

    /*! @brief Histogram register */
    map_t _histRegistry;
    
    /*! @brief Pointer to self (`this`) */
    static HistHandler* m_instance;
  };
  /*! @class Services::HistHandler
   * 
   *  @brief Histgram mapper module 
   *
   *  Singleton instance.  
   */

  namespace Histogramming
  {
    /*! @name Booking functions
     *
     *  1-dim histogram like `TH1D` and `TProfile`:
     *
     *  @param[in] name  histogram name
     *  @param[in] title histogram title
     *  @param[in] nbins number of bins (x axis)
     *  @param[in] xmin  lower boundary (x axis)
     *  @param[in] xmax  upper boundary (x axis)
     *  @param[in] xtitle x-axis title (optional, default `""`)
     *  @param[in] ytitle y-axis title (optional, default `""`)
     *
     *  Additional arguments for 2-dimensional histograms like `TH2D` and
     *  `TProfile2D`:
     *
     *  @param[in] mbins number of bins (y axis)
     *  @param[in] ymin  lower boundary (y axis)
     *  @param[in] ymax  upper boundary (y axis)
     *  @param[in] ztitle z-axis title (optional, default `""`)
     * 
     *  Additional arguments for 3-dimensional histograms like `TH3D`
     *
     *  @param[in] lbins number of bins (z axis)
     *  @param[in] zmin  lower boundary (z axis)
     *  @param[in] zmax  upper boundary (z axis)
     */
    /*!@{*/
    template<class H>
    H* book(const std::string& name,
	    const std::string& title,
	    int nbins,double xmin,double xmax,
	    const std::string& xtitle="",
	    const std::string& ytitle="");
    template<class H>
    H* book(const std::string& group,
	    const std::string& name,
	    const std::string& title,
	    int nbins,double xmin,double xmax,
	    const std::string& xtitle="",
	    const std::string& ytitle="");
    template<class H>
    H* book(const std::string& name,
	    const std::string& title,
	    int nbins,double xmin,double xmax,
	    int mbins,double ymin,double ymax,
	    const std::string& xtitle="",
	    const std::string& ytitle="",
	    const std::string& ztitle="");
    template<class H>
    H* book(const std::string& group,
	    const std::string& name,
	    const std::string& title,
	    int nbins,double xmin,double xmax,
	    int mbins,double ymin,double ymax,
	    const std::string& xtitle="",
	    const std::string& ytitle="",
	    const std::string& ztitle="");
    template<class H>
    H* book(const std::string& name,
	    const std::string& title,
	    int nbins,double xmin,double xmax,
	    int mbins,double ymin,double ymax,
	    int lbins,double zmin,double zmax,
	    const std::string& xtitle="",
	    const std::string& ytitle="",
	    const std::string& ztitle="");
    template<class H>
    H* book(const std::string& group,
	    const std::string& name,
	    const std::string& title,
	    int nbins,double xmin,double xmax,
	    int mbins,double ymin,double ymax,
	    int lbins,double zmin,double zmax,
	    const std::string& xtitle="",
	    const std::string& ytitle="",
	    const std::string& ztitle="");
    /*!@}*/
    /*! @name Number of registered histograms 
     *
     *  @return Number of registered histograms
     */
    /*@{*/
    /*! @brief Count all histograms */
    unsigned int numberRegHist();
    /*! @brief Count only histogrmas in a group
     * 
     *  @param[in] group reference to non-modifiable string containing the 
     *             name of the group
     */
    unsigned int numberRegHist(const std::string& group);
    /*!@}*/
    /*! @name Write histograms to file 
     * 
     *  This group of functions requires the output @c ROOT file to be
     *  opened before invocation. None of the functions takes any control
     *  on this file or its status.
     *
     *  @param[in] noEmpties suppresses writing out empty histograms when set
     *                       to @c true (optional, default is @c true)
     *  @param[in] group reference to non-modifiable string containing the 
     *                   name of the histogram group to be written out.
     *  @param[in] name reference to non-modifiable string containing the name
     *                  of the histogram to be written out. 
     *
     *  @return Number of histograms written to file.
     */
    /*!@{*/
    /*! @brief Write all histograms */
    unsigned int writeHist(bool noEmpties=true);
    /*! @brief Write all histograms of a given group */
    unsigned int writeHist(const std::string& group,
			   bool noEmpties=true);
    /*! @brief Write single histogram with a given name in a given group */
    unsigned int writeHist(const std::string& group,
			   const std::string& name,
			   bool noEntries=true);
    /*!@}*/
    /*! @name Write histograms to specified file 
     * 
     *  This group of functions opens the output @c ROOT file with name
     *  and options specfied in the argument list, and closes the file
     *  before return.
     *
     *  @param[in] fileName reference to non-modifiable string containing
     *                      the file name
     *  @param[in] fileOPts options for the internally used @c TFile object
     *                      constructions  (optional argument, default is 
     *                      "RECREATE"):
     *                      - "WRITE" 
     *                        * Opens a new file for write access.
     *                        * Will fail if file already exists.
     *                      - "READ"
     *                        * Opens an existing file for read access.
     *                        * Not applicable in this context - write
     *                          will fail
     *                      - "RECREATE"
     *                        * Opens a new file for write access if file
     *                          not yet existing.
     *                        * Overwites the content of an existing file
     *                          with the new histograms to be written.
     *                      - "APPEND"
     *                        * Opens an existing file and appends the 
     *                          histograms to be written.
     *                        * If specified file is not yet existing,
     *                          creates a new one (TBC!)
     *                        * Histograms with the same name already in the
     *                          file will not be overwritten, rather a new
     *                          version is added.
      *  @param[in] noEmpties suppresses writing out empty histograms when set
     *                       to @c true (optional, default is @c true)
     *  @param[in] group reference to non-modifiable string containing the 
     *                   name of the histogram group to be written out.
     *  @param[in] name reference to non-modifiable string containing the name
     *                  of the histogram to be written out. 
     *
     *  @return Number of histograms written to file.
     */
    /*!@{*/
    /*! @brief Write all histograms */
    unsigned int write(const std::string& fileName,
		       const std::string& fileOpt="RECREATE",
		       bool noEmpties=true);
    /*! @brief Write all histograms of a given group */
    unsigned int write(const std::string& fileName,
		       const std::string& group,
		       const std::string& fileOpt="RECREATE",
		       bool noEmpties=true);
    /*! @brief Write single histogram with a given name in a given group */
    unsigned int write(const std::string& fileName,
		       const std::string& group,
		       const std::string& name,
		       const std::string& fileOpt="RECREATE",
		       bool  noEmpties=true);  
    /*!@}*/
  }
  /*! @namespace Services::Histogramming 
   *
   *  @brief Collection of functions to book and write histograms.
   *
   *  Presently only `ROOT` based. 
   */

  namespace Process
  {
    void terminate(const std::string& msg="");
  }
  /*! @namespace Services::Process  
   */

  namespace Utils
  {
    static const std::string _vbegin("{");
    static const std::string _vend("}");
    static const std::string _vsep(",");
    /*! @brief Extract value from string
     *
     *  @param[in] str reference to non-modifiable string
     *
     *  @return Extracted value.
     *
     *  @note The value type given by @c T needs to have the stream operator 
     *        @c >> implemented.
     */
    template<class T>
    T extract(const std::string& str);
    /*! @brief Convert string to value
     * 
     *  @param[in] value value to be converted
     *
     *  @return String representing the value in a default format
     * 
     *  @note The value type given by @c T needs to have the stream operator 
     *        @c << implemented.
     */
    template<class T>
    std::string convert(const T& value);
    template<class T>
    std::string convert(const T& value,const std::string& fmt);

    template<class T>
    bool unpackString(const std::string& str,typename std::vector<T>& vec,
		      const std::string& copen=_vbegin,
		      const std::string& cclose=_vend,
		      const std::string& sep=_vsep);

    bool isVector(const std::string& str);

  }
  /*! @namespace Utils */
}
/*! @namespace Services 
 *
 *  @brief Collection of useful services (singleton instances). 
 */
#include "Services.icc"

inline Services::HistHandler* Services::HistHandler::instance()
{ if ( m_instance == 0 ) { m_instance = new HistHandler(); } 
  return m_instance; }

inline const std::string& Services::HistHandler::defaultGroupName() const
{ return m_defaultGroupName; }

inline unsigned int Services::Histogramming::numberRegHist()
{ const Services::HistHandler* pHist = Services::HistHandler::instance(); 
  return pHist->numberRegHist(); }

inline unsigned int Services::Histogramming::numberRegHist(const std::string& 
							   group)
{ const Services::HistHandler* pHist = Services::HistHandler::instance(); 
  return pHist->numberRegHist(group); }

inline std::string Services::Messaging::_formatBuffer()
{
  // some constants
  static const std::string _strEllipse("...");
  static const std::string _strSpace(" ");
  static const std::string _strBracket(")");
  static const size_t      _maxPreLength(_PRINT_PREFIX_WIDTH_);
  static const size_t      _boldOnLength(_boldOn.length());
  static const size_t      _boldOffLength(_boldOff.length());

  // extract prefix
  std::string msg(_printBuffer);
  size_t nbrkt(msg.find_first_of(_strBracket));
  if ( nbrkt == std::string::npos ) nbrkt = 0;
  size_t npref(msg.substr(nbrkt,msg.length()).find_first_of(_levelTag));
  npref += nbrkt;
  std::string prf(msg.substr(0,npref));
  size_t prl(prf.length());

  // compress if too long
  if ( prl > _maxPreLength )
    { 
      prf = prf.substr(0,_maxPreLength-_strEllipse.length())+_strEllipse; 
      prf.append(_boldOff);
    }
  else
    { 
      prf.append(_boldOff);
      int nwrds(_maxPreLength-prl);
      if ( nwrds > 0 ) prf.append((size_t)nwrds,' ');
    }
//   printf("DEBUG: prelim  string length %i/%i (max)\n",
// 	 (int)prl,_maxPreLength); 
  msg = _boldOn + prf + msg.substr(npref);
  return msg;
}

inline void Services::Process::terminate(const std::string& msg)
{
  Print::info("Services::Process::terminate(...)",
	      "\033[1m%s - STOP!\033[0m\n",msg.c_str());
  kill(getpid(),SIGTERM); 
}

inline bool Services::Utils::isVector(const std::string& str)
{ 
  size_t frt(str.find_first_of(_vbegin));
  if ( frt == std::string::npos ) return false;
  size_t bck(str.find_last_of(_vend));
  if ( bck == std::string::npos ) return false;
  return bck > frt;
}
#endif
