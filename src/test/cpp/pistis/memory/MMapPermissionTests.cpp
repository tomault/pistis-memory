#include <pistis/memory/MMapPermissions.hpp>
#include <gtest/gtest.h>
#include <sstream>

#include <sys/mman.h>

using namespace pistis::filesystem;
using namespace pistis::memory;

TEST(MMapPermissionsTests, FlagValues) {
  EXPECT_EQ(PROT_NONE, MMapPermissions::NONE.flags());
  EXPECT_EQ(PROT_READ, MMapPermissions::READ.flags());
  EXPECT_EQ(PROT_WRITE, MMapPermissions::WRITE.flags());
  EXPECT_EQ(PROT_EXEC, MMapPermissions::EXECUTE.flags());
  EXPECT_EQ(PROT_READ | PROT_WRITE, MMapPermissions::READ_WRITE.flags());
  EXPECT_EQ(PROT_READ | PROT_EXEC, MMapPermissions::READ_EXECUTE.flags());
  EXPECT_EQ(PROT_READ | PROT_WRITE | PROT_EXEC, MMapPermissions::ALL.flags());
}

TEST(MMapPermissionsTests, Name) {
  EXPECT_EQ("NONE", MMapPermissions::NONE.name());
  EXPECT_EQ("READ", MMapPermissions::READ.name());
  EXPECT_EQ("WRITE", MMapPermissions::WRITE.name());
  EXPECT_EQ("EXECUTE", MMapPermissions::EXECUTE.name());
  EXPECT_EQ("READ|WRITE", MMapPermissions::READ_WRITE.name());
  EXPECT_EQ("READ|EXECUTE", MMapPermissions::READ_EXECUTE.name());
  EXPECT_EQ("READ|WRITE|EXECUTE", MMapPermissions::ALL.name());
}

TEST(MMapPermissionsTests, FileAccess) {
  EXPECT_EQ(FileAccessMode::READ_ONLY, MMapPermissions::NONE.fileAccess());
  EXPECT_EQ(FileAccessMode::READ_ONLY, MMapPermissions::READ.fileAccess());
  EXPECT_EQ(FileAccessMode::READ_WRITE, MMapPermissions::WRITE.fileAccess());
  EXPECT_EQ(FileAccessMode::READ_ONLY, MMapPermissions::EXECUTE.fileAccess());
  EXPECT_EQ(FileAccessMode::READ_WRITE,
	    MMapPermissions::READ_WRITE.fileAccess());
  EXPECT_EQ(FileAccessMode::READ_ONLY,
	    MMapPermissions::READ_EXECUTE.fileAccess());
  EXPECT_EQ(FileAccessMode::READ_WRITE, MMapPermissions::ALL.fileAccess());
}

TEST(MMapPermissionsTests, EqualityAndInequality) {
  EXPECT_TRUE(MMapPermissions::READ == MMapPermissions::READ);
  EXPECT_TRUE(MMapPermissions::READ != MMapPermissions::WRITE);
  EXPECT_FALSE(MMapPermissions::READ == MMapPermissions::WRITE);
  EXPECT_FALSE(MMapPermissions::READ != MMapPermissions::READ);
}

TEST(MMapPermissionsTests, BitwiseOr) {
  MMapPermissions options = MMapPermissions::READ | MMapPermissions::WRITE;

  EXPECT_EQ(PROT_READ | PROT_WRITE, options.flags());

  options |= MMapPermissions::EXECUTE;
  EXPECT_EQ(PROT_READ | PROT_WRITE | PROT_EXEC, options.flags());
}

TEST(MMapPermissionsTests, BitwiseAnd) {
  MMapPermissions options = MMapPermissions::READ | MMapPermissions::WRITE;

  EXPECT_EQ(MMapPermissions::READ, options & MMapPermissions::READ);
  EXPECT_EQ(MMapPermissions::WRITE, options & MMapPermissions::WRITE);
  EXPECT_EQ(MMapPermissions::NONE, options & MMapPermissions::EXECUTE);

  options &= MMapPermissions::READ;
  EXPECT_EQ(MMapPermissions::READ, options);
}

TEST(MMapPermissionsTests, BitwiseNot) {
  MMapPermissions options = MMapPermissions::READ;
  EXPECT_EQ(PROT_WRITE | PROT_EXEC, (~options).flags());
}

TEST(MMapPermissionsTests, WriteToStream) {
  MMapPermissions options = MMapPermissions::READ | MMapPermissions::EXECUTE;
  std::ostringstream out;
  
  out << options;
  EXPECT_EQ("READ|EXECUTE", out.str());
}
