#ifndef __PISTIS__MEMORY__ALLOCATIONUTILS_HPP__
#define __PISTIS__MEMORY__ALLOCATIONUTILS_HPP__

#include <pistis/exceptions/OutOfRangeError.hpp>
#include <tuple>
#include <stddef.h>
#include <string.h>

namespace pistis {
  namespace memory {
    namespace detail {
      size_t computeNewAllocationSize(size_t currentSize, size_t minSize,
				      size_t maxSize) {
	size_t newSize = currentSize << 1;
	if ((newSize > maxSize) || (newSize < currentSize)) {
	  // If newSize < currentSize, the shift must have overflowed
	  return maxSize;
	} else if (newSize < minSize) {
	  return minSize;
	} else {
	  return newSize;
	}
      }
    }

    template <typename Allocator>
    std::tuple<typename Allocator::value_type*,
	       typename Allocator::value_type*,
	       typename Allocator::value_type*>
        increaseAllocation(Allocator& allocator, size_t minSize,
			   size_t maxSize, 
			   typename Allocator::value_type* const data,
			   typename Allocator::value_type* const end,
			   typename Allocator::value_type* const endOfStorage) {
      const size_t currentSize = endOfStorage - data;
      if (currentSize >= maxSize) {
	throw pistis::exceptions::OutOfRangeError(
	    "Maximum allocation size exceeded", PISTIS_EX_HERE
	);
      }

      const size_t newSize = detail::computeNewAllocationSize(currentSize,
							      minSize,
							      maxSize);
      char* newData = allocator.allocate(newSize);
      if (currentSize) {
	::memcpy(newData, data, currentSize);
	allocator.deallocate(data, currentSize);
      }

      return std::make_tuple(newData, newData + currentSize, newData + newSize);
    }

    
  }
}
#endif
