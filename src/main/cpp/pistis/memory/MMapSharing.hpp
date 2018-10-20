#ifndef __PISTIS__MEMORY__MMAPSHARING_HPP__
#define __PISTIS__MEMORY__MMAPSHARING_HPP__

#include <ostream>
#include <string>

namespace pistis {
  namespace memory {

    class MMapSharing {
    public:
      static const MMapSharing PRIVATE;
      static const MMapSharing SHARED;

    public:
      MMapSharing(): ordinal_(0) { }
      int flags() const;
      const std::string& name() const;

      bool operator==(MMapSharing other) const {
	return ordinal_ == other.ordinal_;
      }
      bool operator!=(MMapSharing other) const {
	return ordinal_ != other.ordinal_;
      }

    private:
      int ordinal_;

      MMapSharing(int o): ordinal_(o) { }
    };

    inline std::ostream& operator<<(std::ostream& out, MMapSharing sharing) {
      return out << sharing.name();
    }
    
  }
}
#endif
