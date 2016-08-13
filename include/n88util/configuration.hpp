// Copyright (c) Eric Nodwell
// See LICENSE for details.

#ifndef N88UTIL_configuration_hpp_INCLUDED
#define N88UTIL_configuration_hpp_INCLUDED

#include "n88util/exception.hpp"
#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>
#include <vector>
#include <utility>
#include <string>

using boost::int64_t;

namespace n88
{

  /** An exception class for the bonelab utilities. */
  class configuration_exception : public n88::n88_exception
  {
    public:
      
      /** Constructor.
        *
        * @param what  Description of the exception.
        */
      explicit configuration_exception (const std::string& what) throw()
        :
        n88_exception (what)
      {}

  };

   /** A class to read, parse, store and write configuration data.
     *
     * This class contains a boost::property_tree::ptree object and largely
     * has a similar interface.  The boost ptree stores configuration data
     * as key/value pairs using the stream operators.  This is highly flexible,
     * as configuration parameters can be interpreted as different types as
     * required. Furthermore, values may themselves be ptrees, which leads to
     * a tree structure for the configuration.
     *
     * Note that it would be a misuse to store large amounts
     * of data, as it is neither particularly space-efficient or fast,
     * neither of which matter for configuration data.
     */
  class configuration
    :
    public boost::property_tree::ptree,
    private boost::noncopyable
  {
    public:

      /** Parse the command line.
        */
      virtual void ParseCommandLine (int argc, char **argv) = 0;

      /** Verify the configuration.
        * Ensures that all required values are present, and that they are
        * consistent.  Adds any required default values.
        */
      virtual void VerifyConfiguration() {}

      // /** Returns the value corresponding to path as type T.
      //   * path gives the complete nested key, with periods as delimiters
      //   * e.g. "SectionA.AnImportantValue".
      //   * Throws an exception if the specified key does not exist.
      //   */
      // template<class T> T get(const char* path) const
      //   { return this->m_PropertyTree.get<T>(path); }
      // 
      // /** Returns the value corresponding to path as type T.
      //   * path gives the complete nested key, with periods as delimiters
      //   * e.g. "SectionA.AnImportantValue".
      //   * If the key does not exist, then default_value is returned.
      //   */
      // template<class T> T get(const char* path, const T& default_value) const
      //   { return this->m_PropertyTree.get<T>(path, default_value); }
      // 
      // /** Returns the value corresponding to path as type T, wrapped in
      //   * boost::optional.
      //   * path gives the complete nested key, with periods as delimiters
      //   * e.g. "SectionA.AnImportantValue".
      //   * If the specified key does not exist, the returned value evaluates
      //   * to false; otherwise the configuration value can be obtained
      //   * with the value-of operator (*).
      //   */
      // template<class T> boost::optional<T> get_optional(const char* path) const
      //   { return this->m_PropertyTree.get_optional<T>(path); }

    protected:   

      // boost::property_tree::ptree m_PropertyTree;

      // template <typename T> T RequireParameter(const char* key);
      // 
      // template <typename T> T OptionalParameter (const char* key, T def);

      void Die(std::string message) const
      {
        throw configuration_exception (message);
      }

      void Die(const boost::format& message) const
      {
        this->Die(message.str());
      }

  };  // class

}  // namespace n88

#endif
