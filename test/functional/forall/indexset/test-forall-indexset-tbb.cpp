//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016-20, Lawrence Livermore National Security, LLC
// and RAJA project contributors. See the RAJA/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "tests/test-forall-indexset.hpp"

#if defined(RAJA_ENABLE_TBB)

#include "../test-forall-indexset-execpol.hpp"

// Cartesian product of types for TBB tests
using TBBForallIndexSetTypes =
  Test< camp::cartesian_product<IdxTypeList, 
                                HostResourceList, 
                                TBBForallIndexSetExecPols>>::Types;

INSTANTIATE_TYPED_TEST_SUITE_P(TBB,
                               ForallIndexSetTest,
                               TBBForallIndexSetTypes);

#endif
