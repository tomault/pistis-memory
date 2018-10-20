#include <pistis/memory/MMapOptions.hpp>
#include <gtest/gtest.h>
#include <sstream>

#include <sys/mman.h>

using namespace pistis::memory;

TEST(MMapOptionsTests, FlagValues) {
  EXPECT_EQ(0, MMapOptions::NONE.flags());
  EXPECT_EQ(MAP_NORESERVE, MMapOptions::DONT_RESERVE_SWAP.flags());
  EXPECT_EQ(MAP_LOCKED, MMapOptions::LOCKED.flags());
  EXPECT_EQ(MAP_POPULATE, MMapOptions::POPULATE.flags());
}

TEST(MMapOptionsTests, Name) {
  EXPECT_EQ("NONE", MMapOptions::NONE.name());
  EXPECT_EQ("DONT_RESERVE_SWAP", MMapOptions::DONT_RESERVE_SWAP.name());
  EXPECT_EQ("LOCKED", MMapOptions::LOCKED.name());
  EXPECT_EQ("POPULATE", MMapOptions::POPULATE.name());
}

TEST(MMapOptionsTests, EqualityAndInequality) {
  EXPECT_TRUE(MMapOptions::LOCKED == MMapOptions::LOCKED);
  EXPECT_TRUE(MMapOptions::LOCKED != MMapOptions::POPULATE);
  EXPECT_FALSE(MMapOptions::LOCKED == MMapOptions::POPULATE);
  EXPECT_FALSE(MMapOptions::LOCKED != MMapOptions::LOCKED);
}

TEST(MMapOptionsTests, BitwiseOr) {
  MMapOptions options = MMapOptions::LOCKED | MMapOptions::POPULATE;
  EXPECT_EQ(MAP_LOCKED | MAP_POPULATE, options.flags());

  options |= MMapOptions::DONT_RESERVE_SWAP;
  EXPECT_EQ(MAP_LOCKED | MAP_POPULATE | MAP_NORESERVE, options.flags());
}

TEST(MMapOptionsTests, BitwiseAnd) {
  MMapOptions options = MMapOptions::LOCKED | MMapOptions::POPULATE;

  EXPECT_EQ(MMapOptions::LOCKED, options & MMapOptions::LOCKED);
  EXPECT_EQ(MMapOptions::POPULATE, options & MMapOptions::POPULATE);
  EXPECT_EQ(MMapOptions::NONE, options & MMapOptions::DONT_RESERVE_SWAP);

  options &= MMapOptions::LOCKED;
  EXPECT_EQ(MMapOptions::LOCKED, options);
}

TEST(MMapOptionsTests, BitwiseNot) {
  MMapOptions options = MMapOptions::LOCKED | MMapOptions::POPULATE;
  int truth = MAP_NORESERVE;

  EXPECT_EQ(truth, (~options).flags());
}

TEST(MMapOptionsTests, WriteToStream) {
  std::ostringstream out;
  MMapOptions options = MMapOptions::LOCKED | MMapOptions::POPULATE;

  out << options;
  EXPECT_EQ("LOCKED|POPULATE", out.str());
}

    
