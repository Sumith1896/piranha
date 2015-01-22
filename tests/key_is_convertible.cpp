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

#include "../src/key_is_convertible.hpp"

#define BOOST_TEST_MODULE key_is_convertible_test
#include <boost/test/unit_test.hpp>

#include <cstddef>
#include <functional>
#include <iostream>

#include "../src/environment.hpp"
#include "../src/symbol_set.hpp"

using namespace piranha;

// Mock key which is not convertible to other key types.
struct mock_key
{
	mock_key() = default;
	mock_key(const mock_key &) = default;
	mock_key(mock_key &&) noexcept;
	mock_key &operator=(const mock_key &) = default;
	mock_key &operator=(mock_key &&) noexcept;
	mock_key(const symbol_set &);
	bool operator==(const mock_key &) const;
	bool operator!=(const mock_key &) const;
	bool is_compatible(const symbol_set &) const noexcept;
	bool is_ignorable(const symbol_set &) const noexcept;
	mock_key merge_args(const symbol_set &, const symbol_set &) const;
	bool is_unitary(const symbol_set &) const;
	void print(std::ostream &, const symbol_set &) const;
	void print_tex(std::ostream &, const symbol_set &) const;
};

// Good one, depending on the other key type.
struct mock_key_00
{
	mock_key_00() = default;
	mock_key_00(const mock_key_00 &) = default;
	mock_key_00(mock_key_00 &&) noexcept;
	mock_key_00 &operator=(const mock_key_00 &) = default;
	mock_key_00 &operator=(mock_key_00 &&) noexcept;
	mock_key_00(const symbol_set &);
	mock_key_00(const mock_key &, const symbol_set &);
	bool operator==(const mock_key_00 &) const;
	bool operator!=(const mock_key_00 &) const;
	bool is_compatible(const symbol_set &) const noexcept;
	bool is_ignorable(const symbol_set &) const noexcept;
	mock_key_00 merge_args(const symbol_set &, const symbol_set &) const;
	bool is_unitary(const symbol_set &) const;
	void print(std::ostream &, const symbol_set &) const;
	void print_tex(std::ostream &, const symbol_set &) const;
};

namespace std
{

template <>
struct hash<mock_key>
{
	std::size_t operator()(const mock_key &) const;
};

template <>
struct hash<mock_key_00>
{
	std::size_t operator()(const mock_key_00 &) const;
};

}

BOOST_AUTO_TEST_CASE(key_is_convertible_test_00)
{
	environment env;
	BOOST_CHECK(is_key<mock_key>::value);
	BOOST_CHECK((!key_is_convertible<mock_key,mock_key>::value));
	BOOST_CHECK((!key_is_convertible<const mock_key,mock_key &>::value));
	BOOST_CHECK((!key_is_convertible<const mock_key &,mock_key &&>::value));
	BOOST_CHECK(is_key<mock_key_00>::value);
	BOOST_CHECK((!key_is_convertible<mock_key_00,mock_key_00>::value));
	BOOST_CHECK((!key_is_convertible<const mock_key_00,mock_key_00 &>::value));
	BOOST_CHECK((!key_is_convertible<const mock_key_00 &,mock_key_00 &&>::value));
	BOOST_CHECK((!key_is_convertible<mock_key,mock_key_00>::value));
	BOOST_CHECK((!key_is_convertible<const mock_key,mock_key_00 &>::value));
	BOOST_CHECK((!key_is_convertible<const mock_key &,mock_key_00 &&>::value));
	BOOST_CHECK((key_is_convertible<mock_key_00,mock_key>::value));
	BOOST_CHECK((key_is_convertible<const mock_key_00,mock_key &>::value));
	BOOST_CHECK((key_is_convertible<const mock_key_00 &,mock_key &&>::value));
}
