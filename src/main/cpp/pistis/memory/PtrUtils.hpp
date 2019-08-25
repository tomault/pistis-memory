#ifndef __PISTIS__MEMORY__PTRUTILS_HPP__
#define __PISTIS__MEMORY__PTRUTILS_HPP__

#include <memory>
#include <utility>

namespace pistis {
  namespace memory {

    template <typename T>
    std::unique_ptr<T> wrapUnique(T* p) {
      return std::unique_ptr<T>(p);
    }

    template <typename T, typename D>
    std::unique_ptr<T, D> wrapUnique(T* p, const D& d) {
      return std::unique_ptr<T, D>(p, d);
    }

    template <typename T, typename D>
    std::unique_ptr<T, D> wrapUnique(T* p, D&& d) {
      return std::unique_ptr<T, D>(p, std::move(d));
    }

    template <typename T>
    std::shared_ptr<T> wrapShared(T* p) {
      return std::shared_ptr<T>(p);
    }

    template <typename T, typename D>
    std::shared_ptr<T> wrapShared(T* p, const D& d) {
      return std::shared_ptr<T>(p, d);
    }

    template <typename T, typename D>
    std::shared_ptr<T> wrapShared(T* p, D&& d) {
      return std::shared_ptr<T>(p, std::move(d));
    }
    
  }
}
#endif

