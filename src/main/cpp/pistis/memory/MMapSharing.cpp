#include "MMapSharing.hpp"
#include <sys/mman.h>

using namespace pistis::memory;

namespace {
  static const int FLAGS[]{ MAP_PRIVATE, MAP_SHARED };
  static const std::string NAMES[]{ "PRIVATE", "SHARED" };
}

const MMapSharing MMapSharing::PRIVATE(0);
const MMapSharing MMapSharing::SHARED(1);

int MMapSharing::flags() const { return FLAGS[ordinal_]; }
const std::string& MMapSharing::name() const { return NAMES[ordinal_]; }
