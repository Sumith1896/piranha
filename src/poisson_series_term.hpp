/***************************************************************************
 *   Copyright (C) 2009-2011 by Francesco Biscani                          *
 *   bluescarni@gmail.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef PIRANHA_POISSON_SERIES_TERM_HPP
#define PIRANHA_POISSON_SERIES_TERM_HPP

#include <boost/concept/assert.hpp>
#include <tuple>
#include <type_traits>
#include <vector>

#include "base_term.hpp"
#include "concepts/multipliable_term.hpp"
#include "concepts/poisson_series_coefficient.hpp"
#include "forwarding.hpp"
#include "math.hpp"
#include "power_series_term.hpp"
#include "real_trigonometric_kronecker_monomial.hpp"
#include "symbol_set.hpp"
#include "symbol.hpp"
#include "type_traits.hpp"

namespace piranha
{

/// Poisson series term.
/**
 * This class extends piranha::base_term for use in Poisson series. The coefficient type \p Cf is generic,
 * the key type is piranha::real_trigonometric_kronecker_monomial<>. A Poisson series term represents symbolic
 * expressions of the form
 * \f[
 * C \cdot
 * \begin{array}{c}
 * \sin \tabularnewline
 * \cos
 * \end{array}
 * \left(n_0x_0 + n_1x_1 + \ldots + n_mx_m\right),
 * \f]
 * where \f$ C \f$ is an arbitrary type parametrised by \p Cf.
 * 
 * This class is a model of the piranha::concept::MultipliableTerm concept.
 * 
 * \section type_requirements Type requirements
 * 
 * \p Cf must be a model of piranha::concept::PoissonSeriesCoefficient.
 * 
 * \section exception_safety Exception safety guarantee
 * 
 * This class provides the same guarantee as piranha::base_term.
 * 
 * \section move_semantics Move semantics
 * 
 * Move semantics is equivalent to piranha::base_term's move semantics.
 * 
 * @author Francesco Biscani (bluescarni@gmail.com)
 */
template <typename Cf>
class poisson_series_term: public power_series_term<base_term<Cf,real_trigonometric_kronecker_monomial<>,poisson_series_term<Cf>>>
{
		BOOST_CONCEPT_ASSERT((concept::PoissonSeriesCoefficient<Cf>));
		typedef power_series_term<base_term<Cf,real_trigonometric_kronecker_monomial<>,poisson_series_term<Cf>>> base;
	public:
		/// Result type for the multiplication by another term.
		typedef std::tuple<poisson_series_term,poisson_series_term> multiplication_result_type;
		/// Defaulted default constructor.
		poisson_series_term() = default;
		/// Defaulted copy constructor.
		poisson_series_term(const poisson_series_term &) = default;
		/// Defaulted move constructor.
		poisson_series_term(poisson_series_term &&) = default;
		PIRANHA_FORWARDING_CTOR(poisson_series_term,base)
		/// Trivial destructor.
		~poisson_series_term() noexcept(true)
		{
			BOOST_CONCEPT_ASSERT((concept::MultipliableTerm<poisson_series_term>));
		}
		/// Defaulted copy assignment operator.
		poisson_series_term &operator=(const poisson_series_term &) = default;
		/// Defaulted move assignment operator.
		poisson_series_term &operator=(poisson_series_term &&) = default;
		/// Term multiplication.
		/**
		 * Multiplication of \p this by \p other will produce two terms according to the prosthaphaeresis formulas.
		 * This method provides the basic exception safety guarantee: in face of exceptions, \p retval will be left in an
		 * undefined but valid state.
		 * 
		 * @param[out] retval return value of the multiplication.
		 * @param[in] other argument of the multiplication.
		 * @param[in] args reference set of arguments.
		 * 
		 * @throws unspecified any exception thrown by:
		 * - the assignment operators of the coefficient type,
		 * - the <tt>multiply()</tt> method of the key type,
		 * - the multiplication and division operators of the coefficient types.
		 */
		template <typename Cf2>
		void multiply(multiplication_result_type &retval, const poisson_series_term<Cf2> &other, const symbol_set &args) const
		{
			// NOTE: first divide by two here and then multiply, at least for series? Would reduce
			// quadratically the number of coefficient divisions.
			// Similarly, it might be worth to change the sign as needed before the multiplication.
			// NOTE: and if we divide everything by two in the end instead? Might be better overall.
			Cf res_cf(this->m_cf * other.m_cf);
			res_cf /= 2;
			std::get<0u>(retval).m_cf = res_cf;
			std::get<1u>(retval).m_cf = std::move(res_cf);
			const bool f1 = this->m_key.get_flavour(), f2 = other.m_key.get_flavour();
			if (f1 && f2) {
				// cos, cos: no change.
			} else if (!f1 && !f2) {
				// sin, sin: negate the plus.
				math::negate(std::get<0u>(retval).m_cf);
			} else if (!f1 && f2) {
				// sin, cos: no change.
			} else {
				// cos, sin: negate the minus.
				math::negate(std::get<1u>(retval).m_cf);
			}
			// Handle the keys.
			bool sign_plus, sign_minus;
			this->m_key.multiply(std::get<0u>(retval).m_key,std::get<1u>(retval).m_key,other.m_key,sign_plus,sign_minus,args);
			// If multiplier sign was changed and the result is a sine, negate the coefficient.
			if (sign_plus && !std::get<0u>(retval).m_key.get_flavour()) {
				math::negate(std::get<0u>(retval).m_cf);
			}
			if (sign_minus && !std::get<1u>(retval).m_key.get_flavour()) {
				math::negate(std::get<1u>(retval).m_cf);
			}
		}
		/// Partial derivative.
		/**
		 * Will return a vector of Poisson series terms representing the partial derivative of \p this with respect to
		 * symbol \p s. The partial derivative is computed via piranha::math::partial() and the differentiation method
		 * of the monomial type. This method requires the coefficient type to
		 * - be multipliable by piranha::integer,
		 * - satisfy the piranha::is_differentiable type trait.
		 * 
		 * @param[in] s piranha::symbol with respect to which the derivative will be calculated.
		 * @param[in] args reference set of arguments.
		 * 
		 * @return partial derivative of \p this with respect to \p s.
		 * 
		 * @throws unspecified any exception throw by:
		 * - piranha::math::partial() and piranha::math::is_zero(),
		 * - coefficient, term and key constructors,
		 * - memory allocation errors in standard containers,
		 * - the differentiation method of the monomial type,
		 * - the multiplication operator of the coefficient type.
		 */
		std::vector<poisson_series_term> partial(const symbol &s, const symbol_set &args) const
		{
			PIRANHA_TT_CHECK(is_differentiable,Cf);
			std::vector<poisson_series_term> retval;
			auto cf_partial = math::partial(this->m_cf,s.get_name());
			if (!math::is_zero(cf_partial)) {
				retval.push_back(poisson_series_term(std::move(cf_partial),this->m_key));
			}
			auto key_partial = this->m_key.partial(s,args);
			if (!math::is_zero(key_partial.first)) {
				retval.push_back(poisson_series_term(this->m_cf * key_partial.first,std::move(key_partial.second)));
			}
			return retval;
		}
};

}

#endif
