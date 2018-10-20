#ifndef __PISTIS__MEMORY__MMAPPERMISSIONS_HPP__
#define __PISTIS__MEMORY__MMAPPERMISSIONS_HPP__

#include <pistis/filesystem/FileAccessMode.hpp>
#include <ostream>
#include <string>

namespace pistis {
  namespace memory {

    class MMapPermissions {
    public:
      static const MMapPermissions NONE;
      static const MMapPermissions READ;
      static const MMapPermissions WRITE;
      static const MMapPermissions EXECUTE;
      static const MMapPermissions READ_WRITE;
      static const MMapPermissions READ_EXECUTE;
      static const MMapPermissions ALL;

    public:
      MMapPermissions();
      int flags() const { return flags_; }
      std::string name() const;
      pistis::filesystem::FileAccessMode fileAccess() const;

      bool operator==(MMapPermissions other) const {
	return flags() == other.flags();
      }
      bool operator!=(MMapPermissions other) const {
	return flags() != other.flags();
      }
      operator bool() const { return (bool)flags_; }
      MMapPermissions operator|(MMapPermissions other) const {
	return MMapPermissions(flags() | other.flags());
      }
      MMapPermissions& operator|=(MMapPermissions other) {
	flags_ |= other.flags();
	return *this;
      }
      MMapPermissions operator&(MMapPermissions other) const {
	return MMapPermissions(flags() & other.flags());
      }
      MMapPermissions& operator&=(MMapPermissions other) {
	flags_ &= other.flags();
      }
      MMapPermissions operator~() const;

    private:
      int flags_;

      MMapPermissions(int f): flags_(f) { }
    };
    
    inline std::ostream& operator<<(std::ostream& out,
				    MMapPermissions permissions) {
      return out << permissions.name();
    }
  }
}
#endif
