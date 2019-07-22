#ifndef __PISTIS__MEMORY__ALIGNMENTUTILS_HPP__
#define __PISTIS__MEMORY__ALIGNMENTUTILS_HPP__

#include <algorithm>
#include <assert.h>
#include <stdint.h>

namespace pistis {
  namespace memory {

    /** @brief Compute the number of bytes to add to @c size to make it
     *         a multiple of @c alignment.
     *
     *  @param size       The value to align
     *  @param alignment  Boundary to align to.  Must be a power of two.
     *  @returns          The number of bytes to add to @c size to make
     *                      it a multiple of @c alignment.
     */
    template <typename S>
    constexpr S alignmentPadding(S size, size_t alignment) noexcept {
      return (-size) & (alignment - 1);
    }

    /** @brief Align @c size so it is a multiple of @c alignment
     *
     *  @param s           Size to align.  Its type must be an unsigned integer
     *  @param alignment   Boundary to align to.  Must be a power of two
     *                       or alignSize() produces undefined behavior
     *  @returns           The smallest multiple of @c alignment greater than
     *                       or equal to @c size, or 0 if this is not possible
     *                       because size is >= max(S) - alignment + 1
     *  @throws            Does not throw
     */
    template <typename S>
    constexpr S alignSize(S size, size_t alignment) noexcept {
      return size + alignmentPadding(size, alignment);
    }

    /** @brief Align pointer to the next "alignment" byte boundary
     *
     *  Unlike std::align, alignPtr does not perform any bounds checking.
     *  It just aligns the pointer.  The value of "alignment" should be
     *  a power of two, but alignPtr does not check this.
     *
     *  @param p          The pointer to align
     *  @param alignment  Boundary to align to.  Must be a power of two
     *                      or alignTo() produces undefined behavior
     *  @returns          The aligned pointer, or nullptr if
     *                      (p + alignment) < p because p is too large
     */
    template <typename T>
    constexpr T* alignTo(T* p, size_t alignment) noexcept {
      return reinterpret_cast<T*>(
	  alignSize(reinterpret_cast<uintptr_t>(p), alignment)
      );
    }

    /** @brief Write zeros to increase @c numWritten so it is a multiple
     *         of alignment
     *
     *  @param numWritten  Number of bytes written so far
     *  @param alignment   Boundary to align to.  Must be a power of two.
     *  @returns           The number of bytes written as padding
     */
    template <typename S, typename DataWriter>
    size_t writePadding(S numWritten, size_t alignment, DataWriter writeData) {
      static const uint8_t BUFFER[256] = {
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      };

      const size_t numToPad = alignmentPadding(numWritten, alignment);
      size_t n = 0;
      while (n < numToPad) {
	n += writeData(BUFFER, std::min(numToPad - n, sizeof(BUFFER)));
      }
      assert(n == numToPad);
      return n;
    }
    
  }
}
#endif
