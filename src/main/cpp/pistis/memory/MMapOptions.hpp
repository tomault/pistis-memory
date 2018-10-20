#ifndef __PISTIS__MEMORY__MMAPOPTIONS_HPP__
#define __PISTIS__MEMORY__MMAPOPTIONS_HPP__

#include <ostream>
#include <string>

namespace pistis {
  namespace memory {
 
    class MMapOptions {
    public:
      /** @brief No options specified */
      static const MMapOptions NONE;
      
      /** @brief Don't reserve swap space for the memory-mapped file
       *
       *  This option passes MAP_NORESERVE to the mmap() call.  If swap
       *  space is not reserved for the mapping, then writes may fail with
       *  a segmentation fault (SIGSEGV signal) if enough physical memory
       *  is not available.
       */
      static const MMapOptions DONT_RESERVE_SWAP;

      /** @brief Lock the memory-mapped region
       *
       *  Specifying this option passes MAP_LOCKED to the mmap() call.  The
       *  region will be locked and mmap() will try to prepopulate the
       *  region, but will not produce an error if the prepopulation fails.
       */
      static const MMapOptions LOCKED;

      /** @brief Prepopulate memory with data from the memory-mapped region
       *
       *  Specifying this option passes MAP_POPULATE to the mmap() call.
       *  The underlying implementation will attempt to prepopulate pages
       *  before they are needed.
       */
      static const MMapOptions POPULATE;

    public:
      MMapOptions(): flags_(0) { }
      int flags() const { return flags_; }
      std::string name() const;

      bool operator==(MMapOptions other) const {
	return flags() == other.flags();
      }
      bool operator!=(MMapOptions other) const {
	return flags() != other.flags();
      }

      MMapOptions operator|(MMapOptions other) const {
	return MMapOptions(flags() | other.flags());
      }

      MMapOptions& operator|=(MMapOptions other) {
	flags_ |= other.flags();
	return *this;
      }

      MMapOptions operator&(MMapOptions other) const {
	return MMapOptions(flags() & other.flags());
      }

      MMapOptions& operator&=(MMapOptions other) {
	flags_ &= other.flags();
	return *this;
      }

      MMapOptions operator~() const;
      
    private:
      int flags_;

      MMapOptions(int f): flags_(f) { }
    };

    inline std::ostream& operator<<(std::ostream& out, MMapOptions options) {
      return out << options.name();
    }
  }
}
#endif
