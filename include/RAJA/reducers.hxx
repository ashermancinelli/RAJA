/*
 * Copyright (c) 2016, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and
 * further review from Lawrence Livermore National Laboratory.
 */

/*!
 ******************************************************************************
 *
 * \file
 *
 * \brief   Header file providing RAJA reduction declarations.
 * 
 ******************************************************************************
 */

#ifndef RAJA_reducers_HXX
#define RAJA_reducers_HXX


#define RAJA_MAX_REDUCE_VARS (8)

namespace RAJA {

///
/// Macros for type agnostic reduction operations.
///
#define RAJA_MIN(a, b) (((b) < (a)) ? (b) : (a))
///
#define RAJA_MAX(a, b) (((b) > (a)) ? (b) : (a))


//
// Forward declarations for reduction templates. 
// Actual classes appear in forall_*.hxx header files. 
//
// IMPORTANT: reduction policy parameter must be consistent with loop 
//            execution policy type.
//
// Also, mutliple reductions using different reduction operations may be
// combined in a single RAJA forall() construct.
//

/*!
 ******************************************************************************
 *
 * \brief  Min reducer class template.
 *
 * Usage example:
 *
 * \verbatim

   Real_ptr data = ...;
   ReduceMin<reduce_policy, Real_type> my_min(init_val);

   forall<exec_policy>( ..., [=] (Index_type i) {
      my_min.min(data[i]); 
   }

   Real_type minval = my_min;
 
 * \endverbatim
 *
 ******************************************************************************
 */
template<typename REDUCE_POLICY_T,
         typename T>
class ReduceMin;


/*!
 ******************************************************************************
 *
 * \brief  Min-loc reducer class template.
 *
 * Usage example:
 *
 * \verbatim

   Real_ptr data = ...;
   ReduceMin<reduce_policy, Real_type> my_min(init_val, -1);

   forall<exec_policy>( ..., [=] (Index_type i) {
      my_min.minloc(data[i], i);
   }

   Real_type minval = my_min;
   Index_type minloc = my_min.getMinLoc();
 
 * \endverbatim
 *
 ******************************************************************************
 */
template<typename REDUCE_POLICY_T,
         typename T>
class ReduceMinLoc;


/*!
 ******************************************************************************
 *
 * \brief  Max reducer class template.
 *
 * Usage example:
 *
 * \verbatim

   Real_ptr data = ...;
   ReduceMax<reduce_policy, Real_type> my_max(init_val);

   forall<exec_policy>( ..., [=] (Index_type i) {
      my_max.max(data[i]); 
   }

   Real_type maxval = my_max;
 
 * \endverbatim
 *
 ******************************************************************************
 */
template<typename REDUCE_POLICY_T,
         typename T>
class ReduceMax;


/*!
 ******************************************************************************
 *
 * \brief  Max-loc reducer class template.
 *
 * Usage example:
 *
 * \verbatim

   Real_ptr data = ...;
   ReduceMax<reduce_policy, Real_type> my_max(init_val, -1);

   forall<exec_policy>( ..., [=] (Index_type i) {
      my_max.maxloc(data[i], i);
   }

   Real_type maxval = my_max;
   Index_type maxloc = my_max.getMaxLoc();

 * \endverbatim
 *
 ******************************************************************************
 */
template<typename REDUCE_POLICY_T,
         typename T>
class ReduceMaxLoc;


/*!
 ******************************************************************************
 *
 * \brief  Sum reducer class template.
 *
 * Usage example:
 *
 * \verbatim

   Real_ptr data = ...;
   ReduceSum<reduce_policy, Real_type> my_sum(init_val);

   forall<exec_policy>( ..., [=] (Index_type i) {
      my_sum += data[i];
   }

   Real_type sum = my_sum;

 * \endverbatim
 *
 ******************************************************************************
 */
template<typename REDUCE_POLICY_T,
         typename T>
class ReduceSum;


}  // closing brace for RAJA namespace


#endif  // closing endif for header file include guard