#include <pistis/memory/MMapSharing.hpp>
#include <gtest/gtest.h>
#include <sstream>

#include <sys/mman.h>

using namespace pistis::memory;

TEST(MMapSharingTests, FlagValues) {
  EXPECT_EQ(MAP_PRIVATE, MMapSharing::PRIVATE.flags());
  EXPECT_EQ(MAP_SHARED, MMapSharing::SHARED.flags());
}

TEST(MMapSharingTests, Name) {
  EXPECT_EQ("PRIVATE", MMapSharing::PRIVATE.name());
  EXPECT_EQ("SHARED", MMapSharing::SHARED.name());
}

TEST(MMapSharingTests, EqualityAndInequality) {
  EXPECT_TRUE(MMapSharing::PRIVATE == MMapSharing::PRIVATE);
  EXPECT_TRUE(MMapSharing::PRIVATE != MMapSharing::SHARED);
  EXPECT_FALSE(MMapSharing::PRIVATE != MMapSharing::PRIVATE);
  EXPECT_FALSE(MMapSharing::PRIVATE == MMapSharing::SHARED);
}

TEST(MMapSharingTests, WriteToStream) {
  std::ostringstream out;
  out << MMapSharing::PRIVATE << "|" << MMapSharing::SHARED;
  EXPECT_EQ("PRIVATE|SHARED", out.str());
}
