//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016-20, Lawrence Livermore National Security, LLC
// and RAJA project contributors. See the RAJA/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// Source file containing basic functional tests for non-arithmetic atomic operations using forall
///

#ifndef __TEST_FORALL_ATOMIC_REF_OTHER_HPP__
#define __TEST_FORALL_ATOMIC_REF_OTHER_HPP__

#include <RAJA/RAJA.hpp>
#include "RAJA_gtest.hpp"
#include <type_traits>

template < typename T >
RAJA_INLINE
RAJA_HOST_DEVICE
typename std::enable_if<sizeof(T) == 1, T>::type np2m1(T val)
{
  val |= val >> 1  ;
  val |= val >> 2  ;
  val |= val >> 4  ;
  return val;
}

template < typename T >
RAJA_INLINE
RAJA_HOST_DEVICE
typename std::enable_if<sizeof(T) == 2, T>::type np2m1(T val)
{
  val |= val >> 1  ;
  val |= val >> 2  ;
  val |= val >> 4  ;
  val |= val >> 8  ;
  return val;
}

template < typename T >
RAJA_INLINE
RAJA_HOST_DEVICE
typename std::enable_if<sizeof(T) == 4, T>::type np2m1(T val)
{
  val |= val >> 1  ;
  val |= val >> 2  ;
  val |= val >> 4  ;
  val |= val >> 8  ;
  val |= val >> 16 ;
  return val;
}

template < typename T >
RAJA_INLINE
RAJA_HOST_DEVICE
typename std::enable_if<sizeof(T) == 8, T>::type np2m1(T val)
{
  val |= val >> 1  ;
  val |= val >> 2  ;
  val |= val >> 4  ;
  val |= val >> 8  ;
  val |= val >> 16 ;
  val |= val >> 32 ;
  return val;
}

template < typename T >
RAJA_INLINE
RAJA_HOST_DEVICE
typename std::enable_if<sizeof(T) == 16, T>::type np2m1(T val)
{
  val |= val >> 1  ;
  val |= val >> 2  ;
  val |= val >> 4  ;
  val |= val >> 8  ;
  val |= val >> 16 ;
  val |= val >> 32 ;
  val |= val >> 64 ;
  return val;
}

// Assist return type conditional overloading of testAtomicRefOtherOp
struct int_op {}; // represents underlying op type = integral

template < typename T, typename AtomicPolicy >
struct AndEqOtherOp : int_op {
  AndEqOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min(T(0)), max((T)seg.size()),
    final_min(min), final_max(min)
  { count[0] = np2m1((T)seg.size()); }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    { return other &= (T)i; }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct FetchAndOtherOp : int_op {
  FetchAndOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min(T(0)), max(np2m1((T)seg.size())),
    final_min(min), final_max(min)
  { count[0] = max; }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    { return other.fetch_and((T)i); }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct OrEqOtherOp : int_op {
  OrEqOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min(T(0)), max(np2m1((T)seg.size())),
    final_min(max), final_max(max)
  { count[0] = T(0); }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    { return other |= (T)i; }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct FetchOrOtherOp : int_op {
  FetchOrOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min(T(0)), max(np2m1((T)seg.size())),
    final_min(max), final_max(max)
  { count[0] = T(0); }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    { return other.fetch_or((T)i); }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct XorEqOtherOp : int_op {
  XorEqOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min(T(0)), max(np2m1((T)seg.size())),
    final_min(min), final_max(min)
  { count[0] = T(0);
    for (RAJA::Index_type i = 0; i < seg.size(); ++i) {
      final_min ^= (T)i; final_max ^= (T)i;
    } }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    { return other ^= (T)i; }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct FetchXorOtherOp : int_op {
  FetchXorOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min(T(0)), max(np2m1((T)seg.size())),
    final_min(min), final_max(min)
  { count[0] = T(0);
    for (RAJA::Index_type i = 0; i < seg.size(); ++i) {
      final_min ^= (T)i; final_max ^= (T)i;
    } }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    { return other.fetch_xor((T)i); }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

// Assist return type conditional overloading of testAtomicRefOtherOp
struct all_op {}; // these op types can accept integral or float

template < typename T, typename AtomicPolicy >
struct LoadOtherOp : all_op {
  LoadOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min((T)seg.size()), max(min),
    final_min(min), final_max(min)
  { count[0] = min; }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type RAJA_UNUSED_ARG(i)) const
    { return other.load(); }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct OperatorTOtherOp : all_op {
  OperatorTOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> RAJA_UNUSED_ARG(seg))
    : other(count), min(T(0)), max(min),
    final_min(min), final_max(min)
  { count[0] = min; }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type RAJA_UNUSED_ARG(i)) const
    { return other; }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct StoreOtherOp : all_op {
  StoreOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min((T)0), max((T)seg.size() - (T)1),
    final_min(min), final_max(max)
  { count[0] = (T)seg.size(); }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    { other.store((T)i); return (T)i; }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct AssignOtherOp : all_op {
  AssignOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min(T(0)), max((T)seg.size() - (T)1),
    final_min(min), final_max(max)
  { count[0] = (T)seg.size(); }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    { return (other = (T)i); }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct CASOtherOp : all_op {
  CASOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min((T)0), max((T)seg.size() - (T)1),
    final_min(min), final_max(max)
  { count[0] = (T)0; }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    {
      T received, expect = (T)0;
      while ((received = other.CAS(expect, (T)i)) != expect) {
        expect = received;
      }
      return received;
    }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct CompareExchangeWeakOtherOp : all_op {
  CompareExchangeWeakOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min((T)0), max((T)seg.size() - (T)1),
    final_min(min), final_max(max)
  { count[0] = (T)0; }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    {
      T expect = (T)0;
      while (!other.compare_exchange_weak(expect, (T)i)) {}
      return expect;
    }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct CompareExchangeStrongOtherOp : all_op {
  CompareExchangeStrongOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min((T)0), max((T)seg.size() - (T)1),
    final_min(min), final_max(max)
  { count[0] = (T)0; }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    {
      T expect = (T)0;
      while (!other.compare_exchange_strong(expect, (T)i)) {}
      return expect;
    }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct MaxEqOtherOp : all_op {
  MaxEqOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min(T(0)), max((T)seg.size() - (T)1),
    final_min(max), final_max(max)
  { count[0] = (T)0; }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    { return other.max((T)i); }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct FetchMaxOtherOp : all_op {
  FetchMaxOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min(T(0)), max((T)seg.size() - (T)1),
    final_min(max), final_max(max)
  { count[0] = (T)0; }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    { return other.fetch_max((T)i); }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct MinEqOtherOp : all_op {
  MinEqOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min(T(0)), max((T)seg.size() - (T)1),
    final_min(min), final_max(min)
  { count[0] = (T)seg.size(); }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    { return other.min((T)i); }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template < typename T, typename AtomicPolicy >
struct FetchMinOtherOp : all_op {
  FetchMinOtherOp(T* count, RAJA::TypedRangeSegment<RAJA::Index_type> seg)
    : other(count), min(T(0)), max((T)seg.size()),
    final_min(min), final_max(min)
  { count[0] = (T)seg.size(); }
  RAJA_HOST_DEVICE
    T operator()(RAJA::Index_type i) const
    { return other.fetch_min((T)i); }
  RAJA::AtomicRef<T, AtomicPolicy> other;
  T min, max, final_min, final_max;
};

template  < typename ExecPolicy,
            typename AtomicPolicy,
            typename T,
            template <typename, typename> class OtherOp>
// No test when underlying op type is int, and index type is float
typename std::enable_if<
            (std::is_floating_point<T>::value && std::is_base_of<int_op,OtherOp<T,AtomicPolicy>>::value)
         >::type
testAtomicRefOtherOp(RAJA::TypedRangeSegment<RAJA::Index_type> RAJA_UNUSED_ARG(seg), T* RAJA_UNUSED_ARG(count), T* RAJA_UNUSED_ARG(list))
{
}

template  < typename ExecPolicy,
            typename AtomicPolicy,
            typename T,
            template <typename, typename> class OtherOp>
// Run test if T is integral and operation is int_op, or for any all_op
typename std::enable_if<
            (std::is_integral<T>::value && std::is_base_of<int_op,OtherOp<T,AtomicPolicy>>::value)
            || (std::is_base_of<all_op,OtherOp<T,AtomicPolicy>>::value)
         >::type
testAtomicRefOtherOp(RAJA::TypedRangeSegment<RAJA::Index_type> seg, T* count, T* list)
{
  OtherOp<T, AtomicPolicy> otherop(count, seg);
  RAJA::forall<ExecPolicy>(seg, [=] RAJA_HOST_DEVICE(RAJA::Index_type i) {
      list[i] = otherop.max + (T)1;
      });
  RAJA::forall<ExecPolicy>(seg, [=] RAJA_HOST_DEVICE(RAJA::Index_type i) {
      T val = otherop(i);
      list[i] = val;
      });
#if defined(RAJA_ENABLE_CUDA)
  cudaErrchk(cudaDeviceSynchronize());
#endif
#if defined(RAJA_ENABLE_HIP)
  hipErrchk(hipDeviceSynchronize());
#endif
  EXPECT_LE(otherop.final_min, count[0]);
  EXPECT_GE(otherop.final_max, count[0]);
  for (RAJA::Index_type i = 0; i < seg.size(); i++) {
    EXPECT_LE(otherop.min, list[i]);
    EXPECT_GE(otherop.max, list[i]);
  }
}



TYPED_TEST_SUITE_P(ForallAtomicRefOtherFunctionalTest);

template <typename T>
class ForallAtomicRefOtherFunctionalTest : public ::testing::Test
{
};


template <typename ExecPolicy,
          typename AtomicPolicy,
          typename WORKINGRES,
          typename T>
void testAtomicFunctionRefOther( RAJA::Index_type N )
{
  RAJA::TypedRangeSegment<RAJA::Index_type> seg(0, N);

  camp::resources::Resource count_res{WORKINGRES()};
  camp::resources::Resource list_res{WORKINGRES()};

  T * count   = count_res.allocate<T>(1);
  T * list    = list_res.allocate<T>(N);

#if defined(RAJA_ENABLE_CUDA)
  cudaErrchk(cudaDeviceSynchronize());
#endif

#if defined(RAJA_ENABLE_HIP)
  hipErrchk(hipDeviceSynchronize());
#endif

  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, LoadOtherOp     >(seg, count, list);
  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, OperatorTOtherOp>(seg, count, list);
  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, StoreOtherOp    >(seg, count, list);
  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, AssignOtherOp   >(seg, count, list);

  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, CASOtherOp                  >(seg, count, list);
  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, CompareExchangeWeakOtherOp  >(seg, count, list);
  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, CompareExchangeStrongOtherOp>(seg, count, list);

  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, MaxEqOtherOp   >(seg, count, list);
  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, FetchMaxOtherOp>(seg, count, list);
  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, MinEqOtherOp   >(seg, count, list);
  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, FetchMinOtherOp>(seg, count, list);

  // Note: These integral tests require return type conditional overloading of testAtomicRefOtherOp
  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, AndEqOtherOp   >(seg, count, list);
  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, FetchAndOtherOp>(seg, count, list);
  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, OrEqOtherOp    >(seg, count, list);
  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, FetchOrOtherOp >(seg, count, list);
  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, XorEqOtherOp   >(seg, count, list);
  testAtomicRefOtherOp<ExecPolicy, AtomicPolicy, T, FetchXorOtherOp>(seg, count, list);

  count_res.deallocate( count );
  list_res.deallocate( list );
}

TYPED_TEST_P(ForallAtomicRefOtherFunctionalTest, AtomicRefOtherFunctionalForall)
{
  using AExec   = typename camp::at<TypeParam, camp::num<0>>::type;
  using APol    = typename camp::at<TypeParam, camp::num<1>>::type;
  using ResType = typename camp::at<TypeParam, camp::num<2>>::type;
  using DType   = typename camp::at<TypeParam, camp::num<3>>::type;
  testAtomicFunctionRefOther<AExec, APol, ResType, DType>( 10000 );
}

REGISTER_TYPED_TEST_SUITE_P( ForallAtomicRefOtherFunctionalTest,
                             AtomicRefOtherFunctionalForall
                           );

#endif  //__TEST_FORALL_ATOMIC_REF_OTHER_HPP__
