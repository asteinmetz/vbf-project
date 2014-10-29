// -*- c++ -*-
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "PrintDef.h"

#include <string>
#include <vector>
#include <map>
//#include <sstream>

#include <cstdio>
#include <cstdlib>
#include <typeinfo>

#ifndef _HAVE_NO_BOOST_
#include "boost/lexical_cast.hpp"
#else
#include <sstream>
#endif

namespace Configuration
{
  /*! @brief Configuration data container
   * 
   * Singleton instance shares configuration data across all modules.This
   * container maintains a keyed store of configuration keys and the 
   * associated data (usually given in the command line and/or an input
   * file). The interpreter can only recognize fairly simple syntax of the
   * form 
   * @code{.cpp} 
   * --<key>=<value>
   * --<key>
   * @endcode
   *
   * No spaces are allowed between the @c -- sign and the keyword @c <key>,
   * and around the assignment symbol (if any). Also, any quotes or double
   * quotes around the @c value are likely removed. The option without 
   * @c value indicates a boolean switch, with the interpretation that 
   * the associated data word is @c true of the switch is set. Full 
   * control on boolean switches can also be achieved by 
   * @code{.cpp}
   * --<key>=true
   * --<key>=false
   * @endcode
   * Note that in this case the test on the @c true or @false strings is 
   * case insensitive, meaning <tt>--<key>=True</tt> yields the same
   * result as <tt>--<key>=true</tt>.
   *
   * Keywords need to be sufficiently different. E.g., 
   * @code{.cpp}
   * --outputFile=test.root
   * --outoutFileOpts=recreate
   * @endcode
   * will not work, as the system will overwrite the @c outputFile when 
   * encountering @c outputFileOpts. This should probably be fixed soon.
   * 
   * 
   * The data is internally stored as @c std::string, there is no particular 
   * attempt made to guess the data value type. On the fly conversion in the
   * query to retrieve the data associated with a @c key is available for 
   * simple data types like @c int, `long int`, `long long int`, @c float,
   * @c double, and @c bool. In addition to these POD types the stored value
   * can always be accessed in its internal representation @c std::string. 
   * 
   *
   * In addition, client defined configuration data can be stored.
   * The data has to be provided with same syntax as discussed above, with
   * a client chosen keyword and, if appropriate, value.
   */
  class ConfigData
  {
  public:

    /*! @brief Status flag */
    bool isActive;

    /*! @brief Number of interactions */
    int mu;
    /*! @brief Number of requested signal events
     * 
     *  Total number of interactions per event is @c nEvents @c * @c mu . 
     * 
     * @note This the number of requested events. The number of analyzed
     *       events is controlled by the number of events avaliable for 
     *       signal events and pile-up events in the inout stream(s) such that
     *       the total number of analyzed events is equal to, or lower than,
     *       @c nEvents . 
     */
    int nEvents;

    /*! @brief Application name
     *
     * Constructed automatically from @c argv[0] for command line. Can be set
     * in steering file using the keyword "mainName" (not yet implemented). 
     */
    std::string mainName;
    /*! @brief Name of file containing list of files with pile-up events */
    std::string pileupFileList;
    /*! @brief Name of file containing list of files with signal events */
    std::string signalFileList;
    /*! @brief Output file name
     *
     * This can be the complete name in case of @c useMu=true . Else,
     * this variable is used as a template for form the output file name,
     * e.g.,
     * @code
     *   ConfigData::mu = 20;
     *   ConfigData::outputFile = "test.root";  
     * @endcode  
     * on input creates file name @c outputFile="test20.root" . 
     */
    std::string outputFile;

    /*! @brief Flags use of automatic output file name creation 
     * 
     * @c useMu=true generates outout file name explicitely containing the
     * @c mu value.
     */
    bool useMu;

    /*! @brief @c ROOT tree/chain name for signal events */
    std::string signalTreeName;

    /*! @brief @c ROOT tree/chain name for pile-up events */
    std::string pileupTreeName;

    /*! @brief @c Name of the analysis we'll use */
    std::string analysisName;

    /*! @name Internal state control
     *
     * @brief Flags controlling the configuration state of individual
     *        variables and the configuration itself
     * 
     * The variables are publicly accessible but of limited use for clients. 
     * Typically, a value of @c false indicates a default setting for the 
     * corresponding data.
     */
    /*!@{ */
    bool muTag;
    bool nEventsTag;
    bool mainNameTag;
    bool pileupFileListTag;
    bool signalFileListTag;
    bool outputFileTag;
    bool useMuTag;
    bool signalTreeNameTag;
    bool pileupTreeNameTag;
    bool analysisNameTag;
    bool isActiveTag; 
    /*!@}*/

    /*! @name Query client-defined configurations */
    /*!@{*/
    /*! @brief General data look-up
     *
     * @param[in] key key for data
     * @param[in] data reference to modifiable data word containing data found
     *            in look-up table on return
     *
     * @return @c true if @c key exists in map. If @c key does not exist in 
     *         map, @c false is returned and the content of @c data is 
     *         unchanged.
     */
    template<class C> 
    bool getConfig(const std::string& key,C& data);
    /*! @brief Add configuration data with a  given key 
     *
     *  @param[in] key key for data
     *  @param[in] data string representing data word
     *  @param[in] cmt optional comment for configuration
     *
     *  @return @c true if everything went well (no real error mode anyway).
     *
     *  @note If data already exists for a given key, it is overwritten.
     */
    bool addConfig(const std::string& key,const std::string& data,
		   const std::string& cmt="client configuraton");
    /*! @brief Get comment associated with configuration
     *
     *  @param[in] key key for comment (same as for data)
     *  @param[inout] cmt comment string filled on output
     *
     *  @return @c true if comment string is available. 
     */
    bool getConfigComment(const std::string& key,std::string& cmt);
    /*!@}*/

    /*! @name Internal types
     * 
     *  Only published for helpers. Don't use!
     */
    /*@{*/
    typedef std::string                        key_t;
    typedef std::pair<std::string,std::string> data_t;
    typedef std::map<key_t,data_t>             map_t;
    /*!@}*/

  private:

    /*! @brief Singleton instance constructor */
    ConfigData();
 
    /*! @brief Pointer to only instance */
    static ConfigData* m_instance;

    /*! @name Default strings
     * 
     *  @brief Internally used for invalid, default, and empty string tests. 
     */
    /*!@{ */
    static std::string m_invalidString; /*!< Invalid string tag */
    static std::string m_defaultString; /*!< Default string tag */
    static std::string m_nullString;    /*!< Empty string tag */
    /*!@} */

    /*! @name Map of client defined configurations */
    /*!@{ */
  public:
    static const map_t& configMap();
  private: 
    static map_t m_configMap;
    /*!@} */
 
  public:

    /*! @name Keywords for command line tags */
    /*!@{ */
    static std::string muLineTag;
    static std::string nEventsLineTag;
    static std::string pileupFileListLineTag;
    static std::string signalFileListLineTag;
    static std::string outputFileLineTag;
    static std::string useMuLineTag;
    static std::string helpLineTag;
    static std::string signalTreeNameLineTag;
    static std::string pileupTreeNameLineTag;
    static std::string analysisLineTag;
    /*!@} */

  public:

    /*! @brief Destructor */
    ~ConfigData();

    /*! @brief Picking up pointer to singleton instance */
    static ConfigData* instance();

    /*! @name Message/string content tests 
     * All functions in this group test a given string for qualifiers 
     * indicating "invalid", "default", or "empty".
     *
     * @return @c true if string contains respective qualifier.
     *
     * @param[in] str reference to non-modifiable string to be tested
     */
    /*!@{ */
    bool isInvalid(const std::string& str); 
    /*!< @brief Invalid string if @c true */
    bool isDefault(const std::string& str); 
    /*!< @brief Default string if @c true */
    bool isEmpty(const std::string& str);   
    /*!< @brief Empty string if @c true */
    /*!@} */

    /*! @name Access to qualifier notation
     *  All functions in this group return a reference to a 
     *  non-modifiable string containg the respective qualifier used to 
     *  tag string contents.
     */

    /*!@{*/
    /*! @brief String "invalid" qualification */
    const std::string& stringTagInvalid() const;
    /*! @brief String "default" qualification */
    const std::string& stringTagDefault() const;
    /*! @brief String "empty" qualification */
    const std::string& stringTagEmpty() const;
    /*!@}*/
  };

  /*! @name Extractor template and specializations */
  /*!@{*/
  /*! @brief Basic generalization 
   *
   *  Works for all types which can be constructed from a @c std::string.
   *
   *  @param[in] tagStr string to be inspected
   *  @param[out] value  value to be extracted from string
   *  @param[in] delimiter character before value (optional, default "=")
   *
   *  @return @c true if value extraction worked. 
   */
  template<class T>
  bool extractTag(const std::string& tagStr,T& value,
                  const std::string& delimiter="=");

  /*! @brief Specialization for type @c int 
   *
   *  @param[in] tagStr string to be inspected
   *  @param[out] value  value to be extracted from string
   *  @param[in] delimiter character before value (optional, default "=")
   *
   *  @return @c true if value extraction worked. 
   */
  template<>
  bool extractTag(const std::string& tagStr,int& value,
                  const std::string& delimiter);

  /*! @brief Specialization for type `long int` 
   *
   *  @param[in] tagStr string to be inspected
   *  @param[out] value  value to be extracted from string
   *  @param[in] delimiter character before value (optional, default "=")
   *
   *  @return @c true if value extraction worked. 
   */
  template<>
  bool extractTag(const std::string& tagStr,long int& value,
                  const std::string& delimiter);

  /*! @brief Specialization for type `long long int` 
   *
   *  @param[in] tagStr string to be inspected
   *  @param[out] value  value to be extracted from string
   *  @param[in] delimiter character before value (optional, default "=")
   *
   *  @return @c true if value extraction worked. 
   */
  template<>
  bool extractTag(const std::string& tagStr,long long int& value,
                  const std::string& delimiter);

  /*! @brief Specialization for type `unsigned int` 
   *
   *  @param[in] tagStr string to be inspected
   *  @param[out] value  value to be extracted from string
   *  @param[in] delimiter character before value (optional, default "=")
   *
   *  @return @c true if value extraction worked. 
   */
  template<>
  bool extractTag(const std::string& tagStr,unsigned int& value,
                  const std::string& delimiter);
  
  /*! @brief Specialization for type `float` 
   *
   *  @param[in] tagStr string to be inspected
   *  @param[out] value  value to be extracted from string
   *  @param[in] delimiter character before value (optional, default "=")
   *
   *  @return @c true if value extraction worked. 
   */
  template<>
  bool extractTag(const std::string& tagStr,float& value, 
                  const std::string& delimiter);

  /*! @brief Specialization for type `double` 
   *
   *  @param[in]  tagStr string to be inspected
   *  @param[out] value  value to be extracted from string
   *  @param[in]  delimiter character before value (optional, default "=")
   *
   *  @return @c true if value extraction worked. 
   */
  template<>
  bool extractTag(const std::string& tagStr,double& value, 
                  const std::string& delimiter);
  /*!@}*/

  /*! @name Client configuration data management */
  /*!@{ */
  bool addConfig(const std::string& key,const std::string& data,
		 const std::string& cmt="client data");
  bool getConfigComment(const std::string& key,std::string& cmt);
  template<class C>
  bool getConfig(const std::string& key,C& data);
  /*!@} */

  /*! @brief Interpret configuration from command line arguments 
   *
   *  Extracted command line arguments are stored in @c ConfigData object.
   * 
   *  @note Only syntax supported right now `--argname=<argument>` - no 
   *        spaces allowed around "=".
   * 
   *  @param[in]  argc number of command line words 
   *  @param[in]  argv list of command line words/arguments
   *  @param[out] config reference to modifiable @C ConfigData object 
   */
  void interpretConfig(int argc,char** argv,ConfigData& config);

  /*! @brief Print configuration
   *
   *  @param[in] config reference to non-modifiable @c ConfigData object 
   */
  void printConfig(const ConfigData& config);
  
  /*! @brief Command line help */
  void printHelp();

  /*! @brief Case insensitive string comparator
   *
   *  This function compares two strings without sensitivity to the
   *  case of the characters in them. It has two modes. The *strong* 
   *  comparison is successful if all characters are identical (except case)
   *  and the number of characters in each string is identical, too. 
   *  The weak comparison is successful if the the first @c n characters
   *  of a string are identical (except case). Here @c n is defined as the
   *  number of characters in the shorter string.
   * 
   *  @param[in] str1   first string
   *  @param[in] str2   second string
   *  @param[in] strong optional, default @c true means strong comparison
   */
  bool compareStrings(const std::string& str1,const std::string& str2,
		      bool strong=true);

  namespace Retrieve
  {
    template<class C>
    bool any_type(const std::string& payload,C& data);
    template<>
    bool any_type(const std::string& payload,bool& data);
  }
}
/*! @namespace Configuration 
 *
 *  Collects data structures and functions configuring the analysis job.
 */

inline bool Configuration::ConfigData::isInvalid(const std::string& str)
{ return str == m_invalidString; }
inline bool Configuration::ConfigData::isDefault(const std::string& str)
{ return str == m_defaultString; }
inline bool Configuration::ConfigData::isEmpty(const std::string& str)
{ return str == m_nullString; }
inline bool 
Configuration::addConfig(const std::string& key,const std::string& data,
			 const std::string& cmt)
{ ConfigData::instance()->addConfig(key,data,cmt); }
inline
bool Configuration::getConfigComment(const std::string& key,std::string& cmt)
{ ConfigData::instance()->getConfigComment(key,cmt); }


inline Configuration::ConfigData* Configuration::ConfigData::instance()
{ if ( m_instance == 0 ) {m_instance = new ConfigData();} return m_instance; }

inline const std::string& Configuration::ConfigData::stringTagInvalid() const
{ return m_invalidString; }
inline const std::string& Configuration::ConfigData::stringTagDefault() const
{ return m_defaultString; }
inline const std::string& Configuration::ConfigData::stringTagEmpty() const
{ return m_nullString; }
 
// inline
// bool Configuration::compareStrings(const std::string& str1,
// 				   const std::string& str2,
// 				   bool strong)
// {
//   size_t l1(str1.length());
//   size_t l2(str2.length());
//   if ( strong && ( l1 != l2 ) ) return false;

//   size_t ln = l1 < l2 ? l1 : l2;
//   size_t ii(0);
//   while ( ii < ln && ( toupper(str1[ii]) == toupper(str2[ii]) ) ) { ++ii; }
//   return ii == ln;
// }

template<class C>
bool 
Configuration::Retrieve::any_type(const std::string& payload,C& data)
{ data = Services::Utils::extract<C>(payload); return true; }
//std::stringstream sstr; sstr << payload; sstr >> data; return true; }

// template<>
// bool 
// Configuration::Retrieve::any_type<bool>(const std::string& payload,bool& data)
// {
//   if ( compareStrings(payload,"true",true) )
//     { data = true; }
//   else if ( compareStrings(payload,"false",true) )
//     { data = false; }
//   else 
//     {
//       PRINT_WARN("Configuration::Retrieve::any_type(...)",
// 		 "\042%s\042 is not \042true\042 or \042false\042!\n",
// 		 payload.c_str());
//       return false;
//     }
//   return true;
// }

#include "Configuration.icc"

#endif
