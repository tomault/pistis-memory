#include "MMapOptions.hpp"
#include <sstream>
#include <tuple>
#include <mman.h>

using namespace pistis::memory;

namespace {
  static inline int addOption(std::ostringstream& out, const std::string& opt,
			      int cnt) {
    if (cnt) {
      out << "|";
    }
    out << opt;
    return cnt + 1;
  }
}

const MMapOptions MMapOptions::NONE(0);
const MMapOptions MMapOptions::DONT_RESERVE_SWAP(MAP_NORESERVE);
const MMapOptions MMapOptions::LOCKED(MAP_LOCKED);
const MMapOptions MMapOptions::POPULATE(MAP_POPULATE);
const MMapOptions MMapOptions::POPULATE_NO_READAHEAD(MAP_POPULATE|MAP_NONBLOCK);
const MMapOptions MMapOptions::USE_HUGE_PAGES(MAP_HUGETLB);
const MMapOptions MMapOptions::USE_2MB_PAGES(MAP_HUGETLB|MAP_HUGE_2MB);
const MMapOptions MMapOptions::USE_1GB_PAGES(MAP_HUGETLB|MAP_HUGE_1GB);

std::string MMapOptions::name() const {
  if (!flags()) {
    return "NONE";
  } else {
    std::ostringstream out;
    int cnt = 0;
    if (flags() & MAP_NORESERVE) {
      cnt = addOption(out, "DONT_RESERVE_SWAP", cnt);
    }
    if (flags() & MAP_LOCKED) {
      cnt = addOption(out, "LOCKED", cnt);
    }
    if (flags() & MAP_POPULATE) {
      const char* opt = flags() & MAP_NONBLOCK ? "POPULATE_NO_READAHEAD"
			                       : "POPULATE";
      cnt = addOption(out, opt, cnt);
    }
    if (flags & MAP_HUGETLB) {
      if (flags() & MAP_HUGE_1GB) {
	cnt = addOption(out, "USE_1GB_PAGES", cnt);
      } else if (flags() & MAP_HUGE_2MB) {
	cnt = addOption(out, "USE_2MB_PAGES", cnt);
      } else {
	cnt = addOption(out, "USE_HUGE_PAGES", cnt);
      }
    }
		                                  
    return out.str();
  }
}

MMapOptions MMapOptions::operator~() const {
  static const int ALL_BITS = MMAP_NORESERVE | MMAP_LOCKED | MMAP_POPULATE |
                              MMAP_NONBLOCK | MMAP_HUGETLB | MMAP_HUGE_2MB |
                              MMAP_HUGE_1GB;
  return MMapOptions(~flags() & ALL_BITS);
}
