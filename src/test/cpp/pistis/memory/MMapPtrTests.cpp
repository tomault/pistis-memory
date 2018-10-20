#include <pistis/memory/MMapPtr.hpp>

#include <pistis/filesystem/Path.hpp>
#include <pistis/testing/Resources.hpp>

#include <gtest/gtest.h>
#include <sstream>

using namespace pistis::memory;
namespace fs = pistis::filesystem;
namespace pt = pistis::testing;

namespace {
  static const std::string TEST_FILE_1_CONTENT{
      "The text in this file is used by unit tests to verify the File "
      "implementation.\n"
      "This is the second line.\n"
      "This is the third line.\n"
  };
}

// TODO: Add tests to verify writing to mapped pages and so on,
//       but these will do for now.
TEST(MMapPtrTests, MapFile) {
  std::string fileName = pt::getResourcePath("test_file_1.txt");
  size_t fileSize = fs::path::fileSize(fileName);
  MMapPtr<char> p(MMapPtr<char>::ofFile(fileName, 0, fileSize));

  ASSERT_TRUE((bool)p);
  EXPECT_NE((void*)0, p.get());
  EXPECT_EQ(fileSize, p.size());
  EXPECT_EQ(fileSize, p.end() - p.get());
  EXPECT_EQ(fileName, p.fileName());
  EXPECT_EQ(1, p.refCnt());

  EXPECT_EQ(TEST_FILE_1_CONTENT, std::string(p.get(), p.end()));
}

TEST(MMapPtrTests, Copy) {
  std::string fileName = pt::getResourcePath("test_file_1.txt");
  size_t fileSize = fs::path::fileSize(fileName);
  MMapPtr<char> p(MMapPtr<char>::ofFile(fileName, 0, fileSize));

  ASSERT_TRUE((bool)p);
  ASSERT_EQ(1, p.refCnt());
  ASSERT_EQ(TEST_FILE_1_CONTENT, std::string(p.get(), p.end()));

  {
    MMapPtr<char> copy(p);

    EXPECT_TRUE((bool)copy);
    EXPECT_TRUE((bool)p);

    EXPECT_EQ(p.get(), copy.get());
    EXPECT_EQ(p.end(), copy.end());
    EXPECT_EQ(p.size(), copy.size());
    EXPECT_EQ(p.fileName(), copy.fileName());
    EXPECT_EQ(2, p.refCnt());
    EXPECT_EQ(2, copy.refCnt());

    EXPECT_EQ(TEST_FILE_1_CONTENT, std::string(copy.get(), copy.end()));
    EXPECT_EQ(TEST_FILE_1_CONTENT, std::string(p.get(), p.end()));
  }
  
  EXPECT_EQ(1, p.refCnt());
  EXPECT_EQ(TEST_FILE_1_CONTENT, std::string(p.get(), p.end()));
}

TEST(MMapPtrTests, Move) {
  std::string fileName = pt::getResourcePath("test_file_1.txt");
  size_t fileSize = fs::path::fileSize(fileName);
  MMapPtr<char> p(MMapPtr<char>::ofFile(fileName, 0, fileSize));

  ASSERT_TRUE((bool)p);
  ASSERT_EQ(1, p.refCnt());
  ASSERT_EQ(TEST_FILE_1_CONTENT, std::string(p.get(), p.end()));

  char* data= p.get();
  char* end = p.end();
  
  MMapPtr<char> moved(std::move(p));
  ASSERT_TRUE((bool)moved);
  EXPECT_FALSE((bool)p);
  EXPECT_EQ(1, moved.refCnt());
  EXPECT_EQ(data, moved.get());
  EXPECT_EQ(end, moved.end());
  EXPECT_EQ(fileSize, moved.size());
  EXPECT_EQ(fileName, moved.fileName());
  EXPECT_EQ(TEST_FILE_1_CONTENT, std::string(moved.get(), moved.end()));
}

TEST(MMapPtrTests, CopyAssignment) {
  std::string fileName = pt::getResourcePath("test_file_1.txt");
  size_t fileSize = fs::path::fileSize(fileName);
  MMapPtr<char> p(MMapPtr<char>::ofFile(fileName, 0, fileSize));
  MMapPtr<char> copy;

  ASSERT_TRUE((bool)p);
  ASSERT_FALSE((bool)copy);
  ASSERT_EQ(1, p.refCnt());
  ASSERT_EQ(TEST_FILE_1_CONTENT, std::string(p.get(), p.end()));

  char* data= p.get();
  char* end = p.end();

  copy = p;
  ASSERT_TRUE((bool)p);
  ASSERT_TRUE((bool)copy);
  EXPECT_EQ(p.get(), copy.get());
  EXPECT_EQ(data, copy.get());
  EXPECT_EQ(p.end(), copy.end());
  EXPECT_EQ(end, copy.end());
  EXPECT_EQ(p.size(), copy.size());
  EXPECT_EQ(p.fileName(), copy.fileName());
  EXPECT_EQ(2, p.refCnt());
  EXPECT_EQ(2, copy.refCnt());

  EXPECT_EQ(TEST_FILE_1_CONTENT, std::string(copy.get(), copy.end()));
  EXPECT_EQ(TEST_FILE_1_CONTENT, std::string(p.get(), p.end()));
}

TEST(MMapPtrTests, MoveAssignment) {
  std::string fileName = pt::getResourcePath("test_file_1.txt");
  size_t fileSize = fs::path::fileSize(fileName);
  MMapPtr<char> p(MMapPtr<char>::ofFile(fileName, 0, fileSize));
  MMapPtr<char> moved;

  ASSERT_TRUE((bool)p);
  ASSERT_FALSE((bool)moved);
  ASSERT_EQ(1, p.refCnt());
  ASSERT_EQ(TEST_FILE_1_CONTENT, std::string(p.get(), p.end()));

  char* data= p.get();
  char* end = p.end();

  moved = std::move(p);

  ASSERT_FALSE((bool)p);
  ASSERT_TRUE((bool)moved);
  EXPECT_EQ(1, moved.refCnt());
  EXPECT_EQ(data, moved.get());
  EXPECT_EQ(end, moved.end());
  EXPECT_EQ(fileSize, moved.size());
  EXPECT_EQ(fileName, moved.fileName());
  EXPECT_EQ(TEST_FILE_1_CONTENT, std::string(moved.get(), moved.end()));
}
