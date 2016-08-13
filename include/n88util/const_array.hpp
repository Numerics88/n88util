// Copyright (c) Eric Nodwell
// See LICENSE for details.

#ifndef N88UTIL_const_array_hpp_INCLUDED
#define N88UTIL_const_array_hpp_INCLUDED

#include "array.hpp"


namespace n88
{

  /**
    * A base class for const_array.
    *
    * The purpose of this base class is to provide a bunch of common methods
    * that can be inherited in partial specializations of const_array.  Please
    * refer to const_array for more details.
    */
  template <int N, typename TValue, typename TIndex=size_t>
  class const_array_base
  {

    protected:

      const TValue*   m_base;
      size_t          m_size;   // Intentionally size_t and not TIndex,
                                // as it may be desireable on occasion
                                // to create an array with dims in
                                // a smaller type, whose product however
                                // exceeds the representation of the smaller
                                // type.
      const TValue*   m_end;
      tuplet<N,TIndex> m_dims;

    public:

      enum {dimension = N};
      typedef TValue value_type;
      typedef TIndex index_type;

      /** Empty constructor.
        * You must subsequently call construct or construct_reference explicitly.
        */
      const_array_base()
        :
        m_base             (NULL),
        m_size             (0),
        m_end              (NULL),
        m_dims             (tuplet<N,TIndex>::zeros())
      {}

      /** Constructor to create reference to existing data defined by a pointer.
        * The referenced data will never be freed by this object.  You
        * must ensure that the referenced memory remains valid, and perform
        * appropriate clean up outside this object.
        *
        * @param data  A pointer to the data.
        * @param dims  The dimensions of the array.
        */
      explicit const_array_base(const TValue* data, tuplet<N,TIndex> dims)
        :
        m_base             (data),
        m_size             (long_product(dims)),
        m_end              (data + long_product(dims)),
        m_dims             (dims)
      {}

      /** Constructor to create reference to existing data in const_array object.
        * No automatic reference counting is performed.  You
        * must ensure that the referenced const_array remains in existence for as
        * long as you want to use this reference.
        *
        * @param source  An existing const_array object.
        */
      explicit const_array_base(const const_array_base<N,TValue,TIndex>& source)
        :
        m_base             (source.data()),
        m_size             (source.size()),
        m_end              (source.end()),
        m_dims             (source.dims())
      {}

      /** Constructor to create reference to existing data in array object.
        * No automatic reference counting is performed.  You
        * must ensure that the referenced array remains in existence for as
        * long as you want to use this reference.
        *
        * @param source  An existing const_array object.
        */
      const_array_base(const array_base<N,TValue,TIndex>& source)
        :
        m_base             (source.data()),
        m_size             (source.size()),
        m_end              (source.end()),
        m_dims             (source.dims())
      {}

      /** Create a reference to existing data defined by a pointer.
        * The referenced data will never be freed by this object.  You
        * must ensure that the referenced memory remains valid, and perform
        * appropriate clean up outside this object.
        *
        * @param data  A pointer to the data.
        * @param dims  The dimensions of the array.
        */
      void construct_reference(const TValue* data, tuplet<N,TIndex> dims)
      {
        if (this->m_base)
        { throw_n88_exception("const_array is already constructed."); }
        this->m_base = data;
        this->m_size = long_product(dims);
        this->m_end = this->m_base + this->m_size;
        this->m_dims = dims;
      }

      /** Create a reference to to existing data in const_array object.
        * No automatic reference counting is performed.  You
        * must ensure that the referenced const_array remains in existence for as
        * long as you want to use this reference.
        *
        * @param source  An existing const_array object.
        */
      void construct_reference(const const_array_base<N,TValue,TIndex>& source)
      { this->construct_reference (source.data(), source.dims()); }

      /** Create a reference to to existing data in array object.
        * No automatic reference counting is performed.  You
        * must ensure that the referenced array remains in existence for as
        * long as you want to use this reference.
        *
        * @param source  An existing array object.
        */
      void construct_reference(const array_base<N,TValue,TIndex>& source)
      { this->construct_reference (source.data(), source.dims()); }

      void destruct()
      {
        this->m_base = NULL;
        this->m_size = 0;
        this->m_end = NULL;
        this->m_dims = tuplet<N,TIndex>::zeros();
      }

      /** Returns true if the const_array has been constructed. */
      inline bool is_constructed() const
      { return (this->m_base != 0); }

      /** Returns the flattened (1D) size of the array. */
      inline size_t size() const
      { return this->m_size; }

      /** Returns the dimensions of the const_array. */
      inline tuplet<N,TIndex> dims() const
      { return this->m_dims; }

      /** Returns a pointer to the const_array data. */
      inline const TValue* data() const
        {
#ifdef RANGE_CHECKING
        if (!this->m_base)
        { throw_n88_exception("const_array is not constructed."); }
#endif
        return this->m_base;
      }

      /** A utility function that performs a run-time check of a pointer to
        * ensure that it points to an entry in the const_array data.  Note
        * that high-performance code often must use raw pointers; this provides
        * a mechanism to do some verification and debugging of such code.
        * This check is performed only if compiled with RANGE_CHECKING
        * defined; naturally there is a substantial performance penalty.
        */
      inline const TValue* verify_data(const TValue* p) const
      {
#ifdef RANGE_CHECKING
        if (!this->m_base)
        { throw_n88_exception("const_array is not constructed."); }
        if ((p < this->m_base) || (p >= this->m_end))
        { throw_n88_exception("const_array index out of bounds."); }
        if (((p - this->m_base) % sizeof(TValue)) != 0)
        { throw_n88_exception("const_array pointer has incorrect offset."); }
#endif
        return p;
      }
      inline TValue* verify_data(TValue* p) const
      { return const_cast<TValue*>(this->verify_data(static_cast<const TValue*>(p))); }

      /** Pointer to the last element plus one of the array data.  This
        * may be used in loops various STL algorithms that require an end
        * value.
        */
      inline const TValue* end() const
      {
#ifdef RANGE_CHECKING
        if (!this->m_base)
        { throw_n88_exception("const_array is not constructed."); }
#endif
        return this->m_end;
      }

      /** Converts a tuple index to the flattened 1D equivalent index. */
      inline size_t flat_index(tuplet<N,TIndex> indices) const
      {
        size_t index = indices[0];
        for (int i=1; i<N; i++)
        {
          index = index*static_cast<size_t>(this->m_dims[i]) + static_cast<size_t>(indices[i]);
        }
        return index;
      }

      /** Flat (1D) indexing of the array data. */
      inline const TValue& operator[](size_t i) const
      {
#ifdef RANGE_CHECKING
        if (!this->m_base)
        { throw_n88_exception("const_array is not constructed."); }
        if (i >= this->m_size)
        { throw_n88_exception("const_array index out of bounds."); }
#endif
        return this->m_base[i];
      }

      /** Indexing of the const_array data using N-dimensional tuples. */
      inline const TValue& operator()(tuplet<N,TIndex> indices) const
      { return const_array_base::operator[](this->flat_index(indices)); }

  }; // class const_array_base

  // ---------------------------------------------------------------------

 /**
  * const_array is a variation of the n88util class array that references
  * constant data.  Please refer to the documentation for array.
  *
  * "const array<1,float> x" is equivalent to a simple C style array declared as
  * "float * const x" while "const_array<1,float> x" is equivalent to a simple
  * C style array declared as "float const * x".
  *
  * Note that const_array has explicit constructors, EXCEPT for the
  * constructor taking an existing array as argument.  This allows arrays
  * to be passed to functions requiring const_array as argument.
  */
  template <int N, typename TValue, typename TIndex=size_t>
  class const_array : public const_array_base<N,TValue, TIndex>
  {
    public:

      /** Empty constructor.
        * You must subsequently call construct_reference explicitly.
        */
      const_array() : const_array_base<N,TValue,TIndex>() {}

      /** Constructor to create reference to existing data defined by a pointer.
        * The referenced data will never be freed by this object.  You
        * must ensure that the referenced memory remains valid, and perform
        * appropriate clean up outside this object.
        *
        * @param data  A pointer to the data.
        * @param dims  The dimensions of the array.
        */
      explicit const_array(const TValue* data, tuplet<N,TIndex> dims) : const_array_base<N,TValue,TIndex>(data, dims) {}

      /** Constructor to create reference to existing data in const_array object.
        * No automatic reference counting is performed.  You
        * must ensure that the referenced const_array remains in existence for as
        * long as you want to use this reference.
        *
        * @param source  An existing const_array object.
        */
      explicit const_array(const const_array_base<N,TValue,TIndex>& source) : const_array_base<N,TValue,TIndex>(source) {}

      /** Constructor to create reference to existing data in array object.
        * No automatic reference counting is performed.  You
        * must ensure that the referenced array remains in existence for as
        * long as you want to use this reference.
        *
        * @param source  An existing const_array object.
        */
      const_array(const array_base<N,TValue,TIndex>& source) : const_array_base<N,TValue,TIndex>(source) {}

  };

  // ---------------------------------------------------------------------

  template <typename TValue, typename TIndex>
  class const_array<1,TValue,TIndex> : public const_array_base<1,TValue,TIndex>
  {
    public:

      const_array() : const_array_base<1,TValue,TIndex>() {}

      explicit const_array(const TValue* data, tuplet<1,TIndex> dims) : const_array_base<1,TValue,TIndex>(data, dims) {}
      explicit const_array(const TValue* data, TIndex size) : const_array_base<1,TValue,TIndex>(data, tuplet<1,TIndex>(size)) {}

      explicit const_array(const const_array_base<1,TValue,TIndex>& source) : const_array_base<1,TValue,TIndex>(source) {}

      const_array(const array_base<1,TValue,TIndex>& source) : const_array_base<1,TValue,TIndex>(source) {}

      inline void construct_reference(const TValue* data, TIndex dim)
      { const_array_base<1,TValue,TIndex>::construct_reference(data, tuplet<1,TIndex>(dim)); }
      inline void construct_reference(const const_array_base<1,TValue,TIndex>& source)
      { const_array_base<1,TValue,TIndex>::construct_reference(source); }
      inline void construct_reference(const array_base<1,TValue,TIndex>& source)
      { const_array_base<1,TValue,TIndex>::construct_reference(source); }

      inline size_t flat_index(tuplet<1,TIndex> indices) const
      {
        // Implied static cast from TIndex to size_t
        return indices[0];
      }

      inline const TValue& operator()(TIndex i) const
      { return const_array_base<1,TValue,TIndex>::operator[](static_cast<size_t>(i)); }

      inline const TValue& operator()(tuplet<1,TIndex> indices) const
      { return const_array_base<1,TValue,TIndex>::operator[](this->flat_index(indices)); }

  };

  // ---------------------------------------------------------------------

  template <typename TValue, typename TIndex>
  class const_array<2,TValue,TIndex> : public const_array_base<2,TValue,TIndex>
  {
    public:

      const_array() : const_array_base<2,TValue,TIndex>() {}

      explicit const_array(const TValue* data, tuplet<2,TIndex> dims) : const_array_base<2,TValue,TIndex>(data, dims) {}
      explicit const_array(const TValue* data, TIndex dim0, TIndex dim1)
          : const_array_base<2,TValue,TIndex>(data, tuplet<2,TIndex>(dim0, dim1)) {}

      explicit const_array(const const_array_base<2,TValue,TIndex>& source) : const_array_base<2,TValue,TIndex>(source) {}

      const_array(const array_base<2,TValue,TIndex>& source) : const_array_base<2,TValue,TIndex>(source) {}

      inline void construct_reference(const TValue* data, TIndex dim0, TIndex dim1)
      { const_array_base<2,TValue,TIndex>::construct_reference(data, tuplet<2,TIndex>(dim0,dim1)); }
      inline void construct_reference(const const_array_base<2,TValue,TIndex>& source)
      { const_array_base<2,TValue,TIndex>::construct_reference(source); }
      inline void construct_reference(const array_base<2,TValue,TIndex>& source)
      { const_array_base<2,TValue,TIndex>::construct_reference(source); }

      inline size_t flat_index(TIndex i, TIndex j) const
      {
        return static_cast<size_t>(i)*static_cast<size_t>(this->m_dims[1])
            + static_cast<size_t>(j);
      }

      inline size_t flat_index(tuplet<2,TIndex> indices) const
      {
        return static_cast<size_t>(indices[0])*static_cast<size_t>(this->m_dims[1])
            + static_cast<size_t>(indices[1]);
      }

      inline const TValue& operator()(TIndex i, TIndex j) const
      { return const_array_base<2,TValue,TIndex>::operator[](this->flat_index(i,j)); }

      inline const TValue& operator()(tuplet<2,TIndex> indices) const
      { return const_array_base<2,TValue,TIndex>::operator[](this->flat_index(indices)); }

  };

  // ---------------------------------------------------------------------

  template <typename TValue, typename TIndex>
  class const_array<3,TValue,TIndex> : public const_array_base<3,TValue,TIndex>
  {
    public:

      const_array() : const_array_base<3,TValue,TIndex>() {}

      explicit const_array(const TValue* data, tuplet<3,TIndex> dims) : const_array_base<3,TValue,TIndex>(data, dims) {}
      explicit const_array(const TValue* data, TIndex dim0, TIndex dim1, TIndex dim2)
          : const_array_base<3,TValue,TIndex>(data, tuplet<3,TIndex>(dim0, dim1, dim2)) {}

      explicit const_array(const const_array_base<3,TValue,TIndex>& source) : const_array_base<3,TValue,TIndex>(source) {}

      const_array(const array_base<3,TValue,TIndex>& source) : const_array_base<3,TValue,TIndex>(source) {}

      inline void construct_reference(const TValue* data, TIndex dim0, TIndex dim1, TIndex dim2)
      { const_array_base<3,TValue,TIndex>::construct_reference(data, tuplet<3,TIndex>(dim0,dim1,dim2)); }
      inline void construct_reference(const const_array_base<3,TValue,TIndex>& source)
      { const_array_base<3,TValue,TIndex>::construct_reference(source); }
      inline void construct_reference(const array_base<3,TValue,TIndex>& source)
      { const_array_base<3,TValue,TIndex>::construct_reference(source); }

      inline size_t flat_index(TIndex i, TIndex j, TIndex k) const
      {
        return (static_cast<size_t>(i)*static_cast<size_t>(this->m_dims[1])
                + static_cast<size_t>(j))*static_cast<size_t>(this->m_dims[2])
               + static_cast<size_t>(k);
      }

      inline size_t flat_index(tuplet<3,TIndex> indices) const
      {
        return (static_cast<size_t>(indices[0])*static_cast<size_t>(this->m_dims[1])
                + static_cast<size_t>(indices[1]))*static_cast<size_t>(this->m_dims[2])
               + static_cast<size_t>(indices[2]);
      }

      inline const TValue& operator()(TIndex i, TIndex j, TIndex k) const
      { return const_array_base<3,TValue,TIndex>::operator[](this->flat_index(i,j,k)); }

      inline const TValue& operator()(tuplet<3,TIndex> indices) const
      { return const_array_base<3,TValue,TIndex>::operator[](this->flat_index(indices)); }

  };

  // ---------------------------------------------------------------------

  template <typename TValue, typename TIndex>
  class const_array<4,TValue,TIndex> : public const_array_base<4,TValue,TIndex>
  {
    public:

      const_array() : const_array_base<4,TValue,TIndex>() {}

      explicit const_array(const TValue* data, tuplet<4,TIndex> dims) : const_array_base<4,TValue,TIndex>(data, dims) {}
      explicit const_array(const TValue* data, TIndex dim0, TIndex dim1, TIndex dim2, TIndex dim3)
          : const_array_base<4,TValue,TIndex>(data, tuplet<4,TIndex>(dim0, dim1, dim2, dim3)) {}

      explicit const_array(const const_array_base<4,TValue,TIndex>& source) : const_array_base<4,TValue,TIndex>(source) {}

      const_array(const array_base<4,TValue,TIndex>& source) : const_array_base<4,TValue,TIndex>(source) {}

      inline void construct_reference(const TValue* data, TIndex dim0, TIndex dim1, TIndex dim2, TIndex dim3)
      { const_array_base<4,TValue,TIndex>::construct_reference(data, tuplet<4,TIndex>(dim0,dim1,dim2,dim3)); }
      inline void construct_reference(const const_array_base<4,TValue,TIndex>& source)
      { const_array_base<4,TValue,TIndex>::construct_reference(source); }
      inline void construct_reference(const array_base<4,TValue,TIndex>& source)
      { const_array_base<4,TValue,TIndex>::construct_reference(source); }

      inline size_t flat_index(TIndex i, TIndex j, TIndex k, TIndex l) const
      {
        return ((static_cast<size_t>(i)*static_cast<size_t>(this->m_dims[1])
               + static_cast<size_t>(j))*static_cast<size_t>(this->m_dims[2])
               + static_cast<size_t>(k))*static_cast<size_t>(this->m_dims[3])
               + static_cast<size_t>(l);
      }

      inline size_t flat_index(tuplet<4,TIndex> indices) const
      {
        return ((static_cast<size_t>(indices[0])*static_cast<size_t>(this->m_dims[1])
               + static_cast<size_t>(indices[1]))*static_cast<size_t>(this->m_dims[2])
               + static_cast<size_t>(indices[2]))*static_cast<size_t>(this->m_dims[3])
               + static_cast<size_t>(indices[3]);
      }

      inline const TValue& operator()(TIndex i, TIndex j, TIndex k, TIndex l) const
      { return const_array_base<4,TValue,TIndex>::operator[](this->flat_index(i,j,k,l)); }

      inline const TValue& operator()(tuplet<4,TIndex> indices) const
      { return const_array_base<4,TValue,TIndex>::operator[](this->flat_index(indices)); }

  };

} // namespace n88

#endif
