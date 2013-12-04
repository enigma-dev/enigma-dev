/******************************************************************************

 ENIGMA
 Copyright (C) 2008-2013 Enigma Strike Force

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#ifndef ERT_MATH_HPP_
#define ERT_MATH_HPP_

#include "ert/real.hpp"
#include "ert/variant.hpp"

#include <array>
#include <numeric>
#include <functional>
#include <initializer_list>
#include <random>

namespace ert {
  extern const real_t pi;
  
  namespace internal {
    extern real_t epsilon;
  }
  
  real_t math_set_epsilon(real_t);
  
  namespace internal {
    template <int N>
    struct arch_random_t {
      using gen = std::mt19937;
    };
  
    template <>
    struct arch_random_t<8> {
      using gen = std::mt19937_64;
    };
  
    using gen_t = arch_random_t<sizeof(void*)>::gen;
    
    extern gen_t rand_gen;
    extern real_t rand_seed;
    extern std::random_device rand_rd;
  }
  
  // Random Functions
  real_t random(real_t);
  real_t random_range(real_t, real_t);
  real_t irandom(real_t);
  real_t irandom_range(real_t, real_t);
  real_t random_set_seed(real_t);
  real_t random_get_seed();
  real_t randomize();

  // Trigonometric Functions
  real_t arccos(real_t);
  real_t arcsin(real_t);
  real_t arctan(real_t);
  real_t arctan2(real_t, real_t);
  real_t sin(real_t);
  real_t tan(real_t);
  real_t cos(real_t);
  real_t degtorad(real_t);
  real_t radtodeg(real_t);
  real_t lengthdir_x(real_t, real_t);
  real_t lengthdir_y(real_t, real_t);
  
  // Rounding Functions
  real_t round(real_t);
  real_t floor(real_t);
  real_t frac(real_t);
  real_t abs(real_t);
  real_t sign(real_t);
  real_t ceil(real_t);
  real_t lerp(real_t, real_t, real_t);
  real_t clamp(real_t, real_t, real_t);

  // Miscellaneous Functions
  real_t is_real(const variant_t&);
  real_t is_string(const variant_t&);
  real_t exp(real_t);
  real_t ln(real_t);
  real_t power(real_t, real_t);
  real_t sqr(real_t);
  real_t sqrt(real_t);
  real_t log2(real_t);
  real_t log10(real_t);
  real_t logn(real_t, real_t);
  
  namespace templates {
    template <size_t N>
    variant_t choose_helper(std::array<variant_t, N> & vars) {
      std::uniform_int_distribution<> dis(0, N - 1);
      return vars[dis(internal::rand_gen)];
    }
    
    template <size_t N>
    real_t median_helper(std::array<real_t, N> & vars) {
      const unsigned n = N / 2;
      std::sort(vars.begin(), vars.end());
      if (N & 1) {
        return vars[n];
      }
      return (vars[n] + vars[n + 1]) * 0.5;
    }
    
    template <size_t N>
    real_t max_helper(std::array<real_t, N> & vars) {
      return std::max_element(vars.begin(), vars.end());
    }
    
    template <size_t N>
    real_t min_helper(std::array<real_t, N> & vars) {
      return std::min_element(vars.begin(), vars.end());
    }
    
    template <size_t N>
    real_t mean_helper(std::array<real_t, N> & vars) {
      return std::accumulate(vars.begin(), vars.end(), 0, std::plus<real_t>()) * (1 / vars.size());
    }
  }
  
  template <typename...T>
  variant_t choose(T const & ...args) {
    std::array<variant_t, sizeof...(args)> vars = {{ args... }};
    return templates::choose_helper(vars);
  }
  
  template <typename...T>
  real_t median(T const & ... args) {
    std::array<real_t, sizeof...(args)> vars = {args...};
    return templates::median_helper(vars);
  }
  
  template <typename...T>
  real_t max(T const & ... args) {
    std::array<real_t, sizeof...(args)> vars = {args...};
    return templates::max_helper(vars);
  }
  
  template <typename...T>
  real_t min(T const & ... args) {
    std::array<real_t, sizeof...(args)> vars = {args...};
    return templates::min_helper(vars);
  }
  
  template <typename...T>
  real_t mean(T const & ... args) {
    std::array<real_t, sizeof...(args)> vars = {args...};
    return templates::mean_helper(vars);
  }
}

#endif // ERT_MATH_HPP_
