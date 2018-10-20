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

      /** @brief Only prepopulate pages already in RAM
       *
       *  Specifying this option passes both MAP_POPULATE and MAP_NONBLOCK
       *  to the mmap() call.  See the mmap() man page for the exact effect
       *  of this option.
       */
      static const MMapOptions POPULATE_NO_READAHEAD;

      /** @brief Use huge pages to map the file
       *
       *  Specifying this option passes MAP_HUGETLB to the mmap() call.
       */
      static const MMapOptions USE_HUGE_PAGES;

      /** @brief Use 2 MB "huge" pages to map the file.
       *
       *  Specifying this option passes MAP_HUGETLB and MAP_HUGE_2MB to
       *  the mmap() call.
       */
      static const MMapOptions USE_2MB_PAGES;

      /** @brief Use 1 GB "huge" pages to map the file.
       *
       *  Specifying this option passes MAP_HUGETLB and MAP_HUGE_1GB to
       *  the mmap() call.
       */
      static const MMapOptions USE_1GB_PAGES;

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
