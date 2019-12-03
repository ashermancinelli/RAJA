/*!
 ******************************************************************************
 *
 * \file
 *
 * \brief   RAJA header file defining a SIMD register abstraction.
 *
 ******************************************************************************
 */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016-19, Lawrence Livermore National Security, LLC
// and RAJA project contributors. See the RAJA/COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#ifdef __AVX2__

#ifndef RAJA_policy_simd_register_avx2_double4_HPP
#define RAJA_policy_simd_register_avx2_double4_HPP

#include "RAJA/config.hpp"
#include "RAJA/util/macros.hpp"

// Include SIMD intrinsics header file
#include <immintrin.h>
#include <cmath>


namespace RAJA
{


  template<>
  class Register<simd_avx2_register, double, 4>{
    public:
      using self_type = Register<simd_avx2_register, double, 4>;
      using element_type = double;

      static constexpr size_t s_num_elem = 4;
      static constexpr size_t s_byte_width = s_num_elem*sizeof(double);
      static constexpr size_t s_bit_width = s_byte_width*8;

      using simd_type = __m256d;

    private:
      simd_type m_value;

    public:

      /*!
       * @brief Default constructor, zeros register contents
       */
      RAJA_INLINE
      Register() : m_value(_mm256_setzero_pd()) {
      }

      /*!
       * @brief Copy constructor from underlying simd register
       */
      RAJA_INLINE
      explicit Register(simd_type const &c) : m_value(c) {}


      /*!
       * @brief Copy constructor
       */
      RAJA_INLINE
      Register(self_type const &c) : m_value(c.m_value) {}


      /*!
       * @brief Construct from scalar.
       * Sets all elements to same value (broadcast).
       */
      RAJA_INLINE
      Register(element_type const &c) : m_value(_mm256_set1_pd(c)) {}

      /*!
       * @brief Load constructor, assuming scalars are in consecutive memory
       * locations.
       */
      RAJA_INLINE
      void load(element_type const *ptr){
        m_value = _mm256_loadu_pd(ptr);
      }

      /*!
       * @brief Strided load constructor, when scalars are located in memory
       * locations ptr, ptr+stride, ptr+2*stride, etc.
       *
       *
       * Note: this could be done with "gather" instructions if they are
       * available. (like in avx2, but not in avx)
       */
      RAJA_INLINE
      void load(element_type const *ptr, size_t stride){
        m_value = _mm256_i64gather_pd(ptr,
                                      _mm256_set_epi64x(3*stride, 2*stride, stride, 0),
                                      sizeof(element_type));
      }



      /*!
       * @brief Store operation, assuming scalars are in consecutive memory
       * locations.
       */
      RAJA_INLINE
      void store(element_type *ptr) const{
        _mm256_storeu_pd(ptr, m_value);
      }

      /*!
       * @brief Strided store operation, where scalars are stored in memory
       * locations ptr, ptr+stride, ptr+2*stride, etc.
       *
       *
       * Note: this could be done with "scatter" instructions if they are
       * available.
       */
      RAJA_INLINE
      void store(element_type *ptr, size_t stride) const{
        for(size_t i = 0;i < s_num_elem;++ i){
          ptr[i*stride] = m_value[i];
        }
      }

      /*!
       * @brief Get scalar value from vector register
       * @param i Offset of scalar to get
       * @return Returns scalar value at i
       */
      template<typename IDX>
      constexpr
      RAJA_INLINE
      element_type operator[](IDX i) const
      {return m_value[i];}


      /*!
       * @brief Set scalar value in vector register
       * @param i Offset of scalar to set
       * @param value Value of scalar to set
       */
      template<typename IDX>
      RAJA_INLINE
      void set(IDX i, element_type value)
      {m_value[i] = value;}

      /*!
       * @brief Set entire vector to a single scalar value
       * @param value Value to set all vector elements to
       */
      RAJA_INLINE
      self_type const &operator=(element_type value)
      {
        m_value = _mm256_set1_pd(value);
        return *this;
      }

      /*!
       * @brief Assign one register to antoher
       * @param x Vector to copy
       * @return Value of (*this)
       */
      RAJA_INLINE
      self_type const &operator=(self_type const &x)
      {
        m_value = x.m_value;
        return *this;
      }


      /*!
       * @brief Add two vector registers
       * @param x Vector to add to this register
       * @return Value of (*this)+x
       */
      RAJA_INLINE
      self_type operator+(self_type const &x) const
      {
        return self_type(_mm256_add_pd(m_value, x.m_value));
      }

      /*!
       * @brief Add a vector to this vector
       * @param x Vector to add to this register
       * @return Value of (*this)+x
       */
      RAJA_INLINE
      self_type const &operator+=(self_type const &x)
      {
        m_value = _mm256_add_pd(m_value, x.m_value);
        return *this;
      }

      /*!
       * @brief Subtract two vector registers
       * @param x Vector to subctract from this register
       * @return Value of (*this)+x
       */
      RAJA_INLINE
      self_type operator-(self_type const &x) const
      {
        return self_type(_mm256_sub_pd(m_value, x.m_value));
      }

      /*!
       * @brief Subtract a vector from this vector
       * @param x Vector to subtract from this register
       * @return Value of (*this)+x
       */
      RAJA_INLINE
      self_type const &operator-=(self_type const &x)
      {
        m_value = _mm256_sub_pd(m_value, x.m_value);
        return *this;
      }

      /*!
       * @brief Multiply two vector registers, element wise
       * @param x Vector to subctract from this register
       * @return Value of (*this)+x
       */
      RAJA_INLINE
      self_type operator*(self_type const &x) const
      {
        return self_type(_mm256_mul_pd(m_value, x.m_value));
      }

      /*!
       * @brief Multiply a vector with this vector
       * @param x Vector to multiple with this register
       * @return Value of (*this)+x
       */
      RAJA_INLINE
      self_type const &operator*=(self_type const &x)
      {
        m_value = _mm256_mul_pd(m_value, x.m_value);
        return *this;
      }

      /*!
       * @brief Divide two vector registers, element wise
       * @param x Vector to subctract from this register
       * @return Value of (*this)+x
       */
      RAJA_INLINE
      self_type operator/(self_type const &x) const
      {
        return self_type(_mm256_div_pd(m_value, x.m_value));
      }

      /*!
       * @brief Divide this vector by another vector
       * @param x Vector to divide by
       * @return Value of (*this)+x
       */
      RAJA_INLINE
      self_type const &operator/=(self_type const &x)
      {
        m_value = _mm256_div_pd(m_value, x.m_value);
        return *this;
      }

      /*!
       * @brief Sum the elements of this vector
       * @return Sum of the values of the vectors scalar elements
       */
      RAJA_INLINE
      element_type sum() const
      {
        auto hsum = _mm256_hadd_pd(m_value, m_value);
        return hsum[0] + hsum[2];
      }

      /*!
       * @brief Dot product of two vectors
       * @param x Other vector to dot with this vector
       * @return Value of (*this) dot x
       */
      RAJA_INLINE
      element_type dot(self_type const &x) const
      {
        return self_type(_mm256_mul_pd(m_value, x.m_value)).sum();
      }

      /*!
       * @brief Returns the largest element
       * @return The largest scalar element in the register
       */
      RAJA_INLINE
      element_type max() const
      {
        // permute the first two and last two lanes of the register
        simd_type a = _mm256_shuffle_pd(m_value, m_value, 0x05);

        // take the minimum value of each lane
        // this gives us b=XXYY where
        // X = min(a[0], a[1])
        // Y = min(a[2], a[3])
        simd_type b = _mm256_max_pd(m_value, a);

        // now take the minimum of a lower and upper lane
        return std::max<double>(b[0], b[2]);
      }

      /*!
       * @brief Returns element-wise largest values
       * @return Vector of the element-wise max values
       */
      RAJA_INLINE
      self_type vmax(self_type a) const
      {
        return self_type(_mm256_max_pd(m_value, a.m_value));
      }

      /*!
       * @brief Returns the largest element
       * @return The largest scalar element in the register
       */
      RAJA_INLINE
      element_type min() const
      {
        // permute the first two and last two lanes of the register
        // m_value = ABCD
        // a = AACC
        simd_type a = _mm256_shuffle_pd(m_value, m_value, 0x05);

        // take the minimum value of each lane
        // this gives us b=XXYY where
        // X = min(a[0], a[1])
        // Y = min(a[2], a[3])
        simd_type b = _mm256_min_pd(m_value, a);

        // now take the minimum of a lower and upper lane
        return std::min<double>(b[0], b[2]);
      }

      /*!
       * @brief Returns element-wise largest values
       * @return Vector of the element-wise max values
       */
      RAJA_INLINE
      self_type vmin(self_type a) const
      {
        return self_type(_mm256_min_pd(m_value, a.m_value));
      }
  };



}  // namespace RAJA


#endif

#endif //__AVX2__