#ifndef __PISTIS__MEMORY__MMAPPTR_HPP__
#define __PISTIS__MEMORY__MMAPPTR_HPP__

/** @file MMapPtr.hpp 
 *
 *  Declaration and implementation of smart pointers to memory-mapped
 *  regions.
 */

#include <pistis/filesystem/File.hpp>
#include <pistis/memory/MMapPermissions.hpp>
#include <pistis/memory/MMapOptions.hpp>
#include <pistis/memory/MMapSharing.hpp>
#include <atomic>
#include <string>
#include <unistd.h>

namespace pistis {
  namespace memory {
    namespace detail {
      void* mapMemoryFromFile(pistis::filesystem::File& file, size_t length,
			      MMapPermissions permissions, MMapOptions options,
			      MMapSharing sharing);
      void unmapMemory(void* addr, size_t length) noexcept;
    }

    template <typename Data>
    class MMapPtr {
    public:
      typedef Data DataType;

    public:
      MMapPtr(): p_(nullptr) { }
      MMapPtr(Data* p, size_t length, int fd):
	  p_(new MMappedData(p, length, pistis::filesystem::File(fd))) {
      }
      MMapPtr(Data* p, size_t length, int fd, const std::string& filename):
	  p_(new MMappedData(p, length,
			     pistis::filesystem::File(fd, filename))) {
      }
      MMapPtr(Data* p, size_t length, pistis::filesystem::File&& file):
	  p_(new MMappedData(p, length, std::move(file))) {
      }
      MMapPtr(const MMapPtr& other): p_(addRef_(other.p_)) { }
      MMapPtr(MMapPtr&& other): p_(other.p_) { other.p_ = nullptr; }
      ~MMapPtr() { removeRef_(); }

      Data* get() const { return p_->data; }
      Data* end() const { return p_->data + p_->length; }
      size_t size() const { return p_->length; }
      const std::string& fileName() const { return p_->file.name(); }
      size_t refCnt() const { return p_->refCnt; }
      
      MMapPtr& operator=(const MMapPtr& other) {
	if (p_ != other.p_) {
	  removeRef_();
	  p_ = addRef_(other.p_);
	}
	return *this;
      }

      MMapPtr& operator=(MMapPtr&& other) {
	if (p_ != other.p_) {
	  removeRef_();
	  p_ = other.p_;
	  other.p_ = nullptr;
	}
	return *this;
      }

      operator bool() const { return (bool)p_; }
      Data& operator[](size_t n) const { return p_->data[n]; }

      static MMapPtr ofFile(
	  pistis::filesystem::File&& file, size_t offset, size_t length,
	  MMapPermissions access = MMapPermissions::READ_WRITE,
	  MMapOptions options = MMapOptions::NONE,
	  MMapSharing sharing = MMapSharing::SHARED) {
	void *mapped = detail::MapMemoryFromFile(file, offset, length,
						 access, options, sharing);
	return MMapPtr(mapped, length, std::move(file));
      }
			    
      static MMapPtr ofFile(
	  int fd, size_t offset, size_t length,
	  MMapPermissions access = MMapPermissions::READ_WRITE,
	  MMapOptions options = MMapOptions::NONE,
	  MMapSharing sharing = MMapSharing::SHARED) {
	return MMapPtr::ofFile(pistis::filesystem::File(fd), offset, length,
			       access, options, sharing);
      }
      
      static MMapPtr ofFile(
	  int fd, const std::string& name, size_t offset, size_t length,
	  MMapPermissions access = MMapPermissions::READ_WRITE,
	  MMapOptions options = MMapOptions::NONE,
	  MMapSharing sharing = MMapSharing::SHARED) {
	return MMapPtr::ofFile(pistis::filesystem::File(fd, name), offset,
			       length, access, options, sharing);
      }

      static MMapPtr<Data> ofFile(
	  const std::string& name, size_t offset, size_t length,
	  MMapPermissions access = MMapPermissions::READ_WRITE,
	  MMapOptions options = MMapOptions::NONE,
	  MMapSharing sharing = MMapSharing::SHARED) {
	pistis::filesystem::File file =
	    pistis::filesystem::File::open(
	       name, pistis::filesystem::FileCreationMode::OPEN_ONLY,
	       access.fileAccess());
	return MMapPtr::ofFile(std::move(file), offset, length, access,
			       options, sharing);
      }

    private:
      struct MMappedData {
	Data *data;
	size_t length;
	pistis::filesystem::File file;
	std::atomic_uint32_t refCnt;

	MMappedData(Data* d, size_t l, pistis::filesystem::File&& f):
	    data(d), length(l), file(std::move(f)), refCnt(1) {
	}

	~MMapppedData() {
	  detail::unmapMemory((void*)data, length);
	}	
      };

    private:
      MMappedData* p_;

      static MMappedData* addRef_(MMappedData* p) {
	if (p) {
	  ++(p->refCnt);
	}
	return p;
      }

      static void removeRef_(MMappedData* p) {
	if (p && !--(p->refCnt)) {
	  delete p;
	}
      }
      
    };
  }
}

#endif
