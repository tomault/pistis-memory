#include "MMapPtr.hpp"
#include <pistis/exceptions/IOError.hpp>
#include <sstream>
#include <sys/mman.h>

using namespace pistis::exceptions;
using namespace pistis::memory;

namespace {
  static std::string createErrorMessage(const std::string& action,
					const std::string& filename) {
    std::ostringstream msg;
    msg << "Failed to " << action << " from ";
    if (filename.empty()) {
      msg << "file";
    } else {
      msg << filename;
    }
    msg << ": #ERR#";
    return msg.str();
  }
}

void* pistis::memory::detail::mapMemoryFromFile(
    pistis::filesystem::File& file, size_t offset, size_t length,
    MMapPermissions permissions, MMapOptions options, MMapSharing sharing
) {
  void *p = ::mmap(nullptr, length, permissions.flags(),
		   sharing.flags() | options.flags(), file.fd(), offset);
  if (p == MAP_FAILED) {
    throw IOError::fromSystemError(createErrorMessage("map memory",
						      file.name()),
				   PISTIS_EX_HERE);
  }
  return p;
}

void pistis::memory::detil::unmapMemory(void* addr, size_t length) noexcept {
  // TODO: If this fails, log the failure somewhere...
  ::munmap(addr, length);
}
