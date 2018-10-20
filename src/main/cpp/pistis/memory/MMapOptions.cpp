#include "MMapOptions.hpp"
#include <sstream>
#include <tuple>
#include <sys/mman.h>

using namespace pistis::memory;

namespace {
  const std::tuple<int, std::string> OPTION_TO_NAME_MAP[]{
    std::tuple<int, std::string>{ MAP_NORESERVE, "DONT_RESERVE_SWAP" },
    std::tuple<int, std::string>{ MAP_LOCKED, "LOCKED" },
    std::tuple<int, std::string>{ MAP_POPULATE, "POPULATE" },
  };

  const size_t OPTION_TO_NAME_MAP_SIZE =
      sizeof(OPTION_TO_NAME_MAP)/sizeof(std::tuple<int, std::string>);

  const std::tuple<int, std::string>* const OPTION_TO_NAME_MAP_END =
      OPTION_TO_NAME_MAP + OPTION_TO_NAME_MAP_SIZE;

}

const MMapOptions MMapOptions::NONE(0);
const MMapOptions MMapOptions::DONT_RESERVE_SWAP(MAP_NORESERVE);
const MMapOptions MMapOptions::LOCKED(MAP_LOCKED);
const MMapOptions MMapOptions::POPULATE(MAP_POPULATE);

std::string MMapOptions::name() const {
  if (!flags()) {
    return "NONE";
  } else {
    std::ostringstream out;
    int cnt = 0;
    for (const std::tuple<int, std::string>* p = OPTION_TO_NAME_MAP;
	 p != OPTION_TO_NAME_MAP_END;
	 ++p) {
      if (flags() & std::get<0>(*p)) {
	if (cnt > 0) {
	  out << "|";
	}
	out << std::get<1>(*p);
	++cnt;
      }
    }
		                                  
    return out.str();
  }
}

MMapOptions MMapOptions::operator~() const {
  static const int ALL_BITS = MAP_NORESERVE | MAP_LOCKED | MAP_POPULATE;
  return MMapOptions(~flags() & ALL_BITS);
}
