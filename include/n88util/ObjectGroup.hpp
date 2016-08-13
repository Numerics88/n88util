// Copyright (c) Eric Nodwell
// See LICENSE for details.

#ifndef N88UTIL_ObjectGroup_hpp_INCLUDED
#define N88UTIL_ObjectGroup_hpp_INCLUDED

#include "exception.hpp"
#include <boost/noncopyable.hpp>
#include <vector>
#include <cstdlib>

namespace n88
{

  /** A collection of objects (no copy constructor required).
    *
    * This container is typically used for resource-managing objects, which
    * cannot be copied and therefore do not have copy constructors.  Note
    * that STL containers require copy constructors.  The objects
    * stored in ObjectGroup must however have a default constructor (one
    * that takes no arguments).
    */
  template <typename TObject>
  class ObjectGroup : private boost::noncopyable
  {

    protected:

      std::vector<TObject*> m_Objects;

    public:

      /** Default constructor.  Creates an empty ObjectGroup. */
      ObjectGroup() {}

      /** Constructor that sets size of ObjectGroup to n, creating n objects. */
      ObjectGroup(size_t n)
      {
        this->construct<TObject>(n);
      }

      ~ObjectGroup()
      {
        this->destroy();
      }

      /** Sets the size of ObjectGroup to n, creating n objects.
        * Throws an exception if existing size is non-zero.
        */
      template <class TDerivedObject>
      void construct(size_t n)
      {
        n88_assert(this->m_Objects.size() == 0);
        for (size_t i=0; i<n; ++i) {
          this->m_Objects.push_back(new TDerivedObject());
        }
      }

      /** Sets the size of ObjectGroup to 0, calling the destructors of
        * each object currently in the ObjectGroup.
        */
      void destroy()
      {
        while (!this->m_Objects.empty())
        {
          delete this->m_Objects.back();
          this->m_Objects.pop_back();
        }
      }

      /** Returns the size (number of objects) of ObjectGroup. */
      size_t size() const {
        return this->m_Objects.size(); }

      bool is_constructed() const {
        return !this->m_Objects.empty(); }
        
      /** Returns a reference to object i. */
      TObject& operator[](size_t n) const
      {
#ifdef RANGE_CHECKING
        n88_assert(n < m_Objects.size());
#endif
        return *this->m_Objects[n];
      }
//       const TObject& operator[](size_t n) const
//       {
// #ifdef RANGE_CHECKING
//         n88_assert(n < m_Objects.size());
// #endif
//         return *this->m_Objects[n];
//       }

  };

} // namespace n88

#endif
