#include <pistis/memory/AlignmentUtils.hpp>
#include <gtest/gtest.h>

using namespace pistis::memory;

TEST(AlignmentUtilsTests, AlignmentPadding) {
  EXPECT_EQ(0, alignmentPadding((uint32_t)0x1000, 4));
  EXPECT_EQ(3, alignmentPadding((uint32_t)0x1001, 4));
  EXPECT_EQ(2, alignmentPadding((uint32_t)0x1002, 4));
  EXPECT_EQ(1, alignmentPadding((uint32_t)0x1003, 4));
}

TEST(AlignmentUtilsTests, AlignmentSize) {
  EXPECT_EQ(0x1000, alignSize((uint32_t)0x1000, 4));
  EXPECT_EQ(0x1004, alignSize((uint32_t)0x1001, 4));
  EXPECT_EQ(0x1004, alignSize((uint32_t)0x1002, 4));
  EXPECT_EQ(0x1004, alignSize((uint32_t)0x1003, 4));
}

TEST(AlignmentUtilsTests, AlignTo) {
  const uint8_t* p = (const uint8_t*)0x40000000;

  EXPECT_EQ(p, alignTo(p, 8));
  EXPECT_EQ(p + 8, alignTo(p + 1, 8));
  EXPECT_EQ(p + 8, alignTo(p + 2, 8));
  EXPECT_EQ(p + 8, alignTo(p + 3, 8));
  EXPECT_EQ(p + 8, alignTo(p + 4, 8));
  EXPECT_EQ(p + 8, alignTo(p + 5, 8));
  EXPECT_EQ(p + 8, alignTo(p + 6, 8));
  EXPECT_EQ(p + 8, alignTo(p + 7, 8));
}

TEST(AlignmentUtilsTests, WritePadding) {
  uint8_t buffer[8] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF };
  uint32_t nInBuffer = 0xFFFFFFFF;
  uint32_t numWritten = writePadding(0x1003, 8,
      [&nInBuffer, &buffer](const uint8_t* data, uint32_t n) {
	   ::memcpy((void*)buffer, data, std::min(n, (uint32_t)sizeof(buffer)));
	   nInBuffer = n;
	   return n;
  });

  EXPECT_EQ(5, numWritten);
  EXPECT_EQ(5, nInBuffer);
  EXPECT_EQ(0, buffer[0]);
  EXPECT_EQ(0, buffer[1]);
  EXPECT_EQ(0, buffer[2]);
  EXPECT_EQ(0, buffer[3]);
  EXPECT_EQ(0, buffer[4]);
  EXPECT_EQ(0xAD, buffer[5]);
  EXPECT_EQ(0xBE, buffer[6]);
  EXPECT_EQ(0XEF, buffer[7]);
}
