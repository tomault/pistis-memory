#include "MMapPermissions.hpp"
#include <sstream>

#include <sys/mman.h>

using namespace pistis::filesystem;
using namespace pistis::memory;

namespace {
  int addName(std::ostringstream& out, const char* name, int cnt) {
    if (cnt) {
      out << "|";
    }
    out << name;
    return cnt + 1;
  }
}

const MMapPermissions MMapPermissions::NONE(PROT_NONE);
const MMapPermissions MMapPermissions::READ(PROT_READ);
const MMapPermissions MMapPermissions::WRITE(PROT_WRITE);
const MMapPermissions MMapPermissions::EXECUTE(PROT_EXEC);
const MMapPermissions MMapPermissions::READ_WRITE(PROT_READ|PROT_WRITE);
const MMapPermissions MMapPermissions::READ_EXECUTE(PROT_READ|PROT_EXEC);
const MMapPermissions MMapPermissions::ALL(PROT_READ|PROT_WRITE|PROT_EXEC);

MMapPermissions::MMapPermissions(): flags_(PROT_NONE) { }

std::string MMapPermissions::name() const {
  std::ostringstream out;
  int cnt = 0;
  
  if (flags() & PROT_READ) {
    cnt = addName(out, "READ", cnt);
  }
  if (flags() & PROT_WRITE) {
    cnt = addName(out, "WRITE", cnt);
  }
  if (flags() & PROT_EXEC) {
    cnt = addName(out, "EXECUTE", cnt);
  }
  if (!cnt) {
    out << "NONE";
  }
  return out.str();
}

FileAccessMode MMapPermissions::fileAccess() const {
  return (flags() & PROT_WRITE) ? FileAccessMode::READ_WRITE
                                : FileAccessMode::READ_ONLY;
}

MMapPermissions MMapPermissions::operator~() const {
  static const int ALL_BITS = PROT_READ|PROT_WRITE|PROT_EXEC|PROT_NONE;
  return MMapPermissions(~flags() & ALL_BITS);
}
