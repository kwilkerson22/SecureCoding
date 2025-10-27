// Test.cpp
// Name: Kenneth Wilkerson
// Course: CS 405

// Uncomment the next line to use precompiled headers
// #include "pch.h"
// uncomment the next line if you do not use precompiled headers
#include "gtest/gtest.h"
#include <stdexcept>
#include <vector>
#include <memory>
#include <cassert>
#include <cstdlib>
#include <ctime>

// the global test environment setup and tear down
class Environment : public ::testing::Environment
{
public:
  ~Environment() override {}

  // override this to define how to set up the environment
  void SetUp() override
  {
    // initialize random seed
    srand(time(nullptr));
  }

  // override this to define how to tear down the environment
  void TearDown() override {}
};

// create our test class to house shared data between tests
class CollectionTest : public ::testing::Test
{
protected:
  // create a smart point to hold our collection
  std::unique_ptr<std::vector<int>> collection;

  void SetUp() override
  {
    // create a new collection to be used in the test
    collection.reset(new std::vector<int>);
  }

  void TearDown() override
  {
    // erase all elements in the collection, if any remain
    collection->clear();
    // free the pointer
    collection.reset(nullptr);
  }

  // helper function to add random values from 0 to 99 count times to the collection
  void add_entries(int count)
  {
    assert(count > 0);
    for (auto i = 0; i < count; ++i)
      collection->push_back(rand() % 100);
  }
};

// Test #1: verify smart pointer is not null
TEST_F(CollectionTest, CollectionSmartPointerIsNotNull)
{
  ASSERT_TRUE(collection);
  ASSERT_NE(collection.get(), nullptr);
}

// Test #2: verify empty on create
TEST_F(CollectionTest, IsEmptyOnCreate)
{
  ASSERT_TRUE(collection->empty());
  ASSERT_EQ(collection->size(), 0);
}

// Test #3: negative test – always fail 
TEST_F(CollectionTest, AlwaysFail)
{
  FAIL() << "Intentional failure to verify test framework";
}

// Test # 4: verify adding one element to empty collection
TEST_F(CollectionTest, CanAddToEmptyVector)
{
  ASSERT_TRUE(collection->empty());
  add_entries(1);
  EXPECT_EQ(collection->size(), 1);
  EXPECT_FALSE(collection->empty());
}

// Test #5: verify adding five elements to collection
TEST_F(CollectionTest, CanAddFiveValuesToVector)
{
  add_entries(5);
  EXPECT_EQ(collection->size(), 5);
  EXPECT_FALSE(collection->empty());
}

// Test #6 verify max size >= size for multiple entry counts
TEST_F(CollectionTest, MaxSizeIsGreaterThanOrEqualToSize)
{
  int counts[] = {0, 1, 5, 10};
  for (int count : counts)
  {
    collection->clear();
    if (count > 0) add_entries(count);
    EXPECT_GE(collection->max_size(), collection->size());
  }
}

// Test #7: verify capacity >= size for multiple entry counts
TEST_F(CollectionTest, CapacityIsGreaterThanOrEqualToSize)
{
  int counts[] = {0, 1, 5, 10};
  for (int count : counts)
  {
    collection->clear();
    if (count > 0) add_entries(count);
    EXPECT_GE(collection->capacity(), collection->size());
  }
}

// Test #8: verify resizing increases the collection
TEST_F(CollectionTest, ResizeIncreasesCollection)
{
  add_entries(3);
  size_t oldSize = collection->size();
  collection->resize(10);
  EXPECT_GT(collection->size(), oldSize);
}

// Test #9: verify resizing decreases the collection
TEST_F(CollectionTest, ResizeDecreasesCollection)
{
  add_entries(10);
  collection->resize(5);
  EXPECT_EQ(collection->size(), 5);
}

// Test #10: verify resizing to zero empties collection
TEST_F(CollectionTest, ResizeToZeroClearsCollection)
{
  add_entries(5);
  collection->resize(0);
  EXPECT_TRUE(collection->empty());
}

// Test #11: verify clear() empties collection
TEST_F(CollectionTest, ClearErasesCollection)
{
  add_entries(5);
  collection->clear();
  EXPECT_TRUE(collection->empty());
  EXPECT_EQ(collection->size(), 0);
}

// Test #12: verify erase(begin, end) erases all elements
TEST_F(CollectionTest, EraseRangeClearsCollection)
{
  add_entries(7);
  collection->erase(collection->begin(), collection->end());
  EXPECT_TRUE(collection->empty());
  EXPECT_EQ(collection->size(), 0);
}

// Test #13: verify reserve increases capacity but not size
TEST_F(CollectionTest, ReserveIncreasesCapacityNotSize)
{
  size_t oldCapacity = collection->capacity();
  collection->reserve(oldCapacity + 50);
  EXPECT_GE(collection->capacity(), oldCapacity + 50);
  EXPECT_EQ(collection->size(), 0);
}

// Test #14: negative test – at() throws out_of_range
TEST_F(CollectionTest, ThrowsOutOfRangeWhenAccessingInvalidIndex)
{
  add_entries(3);
  EXPECT_THROW(collection->at(10), std::out_of_range);
}

// Test #15: custom positive test – verify values remain after resize up
TEST_F(CollectionTest, ValuesPersistAfterResizeUp)
{
  add_entries(3);
  int firstValue = (*collection)[0];
  collection->resize(10);
  EXPECT_EQ((*collection)[0], firstValue);
}

// Test #16: custom negative test – erase from empty collection doesn’t crash
TEST_F(CollectionTest, EraseOnEmptyCollectionIsSafe)
{
  EXPECT_NO_THROW(collection->erase(collection->begin(), collection->end()));
  EXPECT_TRUE(collection->empty());
}

