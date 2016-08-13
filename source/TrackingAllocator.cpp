// Copyright (c) Eric Nodwell
// See LICENSE for details.

#include "n88util/TrackingAllocator.hpp"
#include "n88util/exception.hpp"
#include <iostream>

namespace n88
{

  boost::thread_specific_ptr<TrackingAllocatorValues> TrackingAllocator::allocated;

  void* TrackingAllocator::allocate(size_t size)
  {
    void* p = malloc(size);
    if (p)
    {
      if (allocated.get() == NULL)
      {
        allocated.reset(new TrackingAllocatorValues);
        allocated->current = 0;
        allocated->peak = 0;
      }
      allocated->current += size;
      if (allocated->current > allocated->peak)
      {
        allocated->peak = allocated->current;
      }
    }
    return p;
  }

  void TrackingAllocator::release (void* p, size_t size)
  {
    n88_assert (allocated.get());
    free (p);
    allocated->current -= size;
  }

  void TrackingAllocator::external_increase (size_t size)
  {
    allocated->current += size;
    if (allocated->current > allocated->peak)
    {
      allocated->peak = allocated->current;
    }    
  }

  void TrackingAllocator::external_decrease (size_t size)
  {
    allocated->current -= size;
  }

  size_t TrackingAllocator::get_current_allocated()
  {
    if (allocated.get() == NULL)
      { return 0; }
    return allocated->current;
  }

  size_t TrackingAllocator::get_peak_allocated()
  {
    if (allocated.get() == NULL)
      { return 0; }
    return allocated->peak;
  }

} // namespace n88
