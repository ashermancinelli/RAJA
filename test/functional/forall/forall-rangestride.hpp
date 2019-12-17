//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016-19, Lawrence Livermore National Security, LLC
// and RAJA project contributors. See the RAJA/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifndef RAJA_test-forall-rangestride_HPP
#define RAJA_test-forall-rangestride_HPP

#include <cstdlib>
#include <string>

#include "RAJA/RAJA.hpp"
#include "gtest/gtest.h"

constexpr std::size_t LENGTH{1024};

template <typename Policy>
class ForallRangeStrideFunctionalTest : public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    using allocator = RAJA::util::allocator<get_platform<Policy>::value>;

    array = allocator::allocate<int>(LENGTH);
    for (Index_type i = 0; i < alen; ++i) {
      array[i] = rand() % 65536;
    }
  }

  virtual void TearDown()
  {
    allocator::deallocate(array);
  }

  int* array;
};

TYPED_TEST_CASE_P(ForallRangeStrideFunctionalTest);

TYPED_TEST_P(ForallRangeStrideFunctionalTest, ForallUnitStride)
{
  RAJA::RangeStrideSegment seg{0, LENGTH, 1};

  RAJA::forall<TypeParam>(seg, [=](Index_type idx) {
    this->array[idx] = idx;
  });

  int* result = allocator::get(this->array);

  for (Index_type i = 0; i < LENGTH; ++i) {
    EXPECT_EQ(result, i);
  }
}

TYPED_TEST_P(ForallRangeStrideFunctionalTest, ForallUnitNegativeStride)
{
  RAJA::RangeStrideSegment seg{LENGTH-1, -1, -11};

  RAJA::forall<TypeParam>(seg, [=](Index_type idx) {
    this->array[idx] = idx;
  });

  int* result = allocator::get(this->array);

  for (Index_type i = 0; i < LENGTH; ++i) {
    EXPECT_EQ(result, i);
  }
}

REGISTER_TYPED_TEST_CASE_P(
    ForallRangeStrideFunctionalTest, 
    ForallUnitStride,
    ForallUnitNegativeStride);

#endif // RAJA_test-forall-rangestride_HPP