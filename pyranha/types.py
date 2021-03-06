# -*- coding: iso-8859-1 -*-
# Copyright (C) 2009-2011 by Francesco Biscani
# bluescarni@gmail.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the
# Free Software Foundation, Inc.,
# 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

""".. moduleauthor:: Francesco Biscani <bluescarni@gmail.com>"""

from __future__ import absolute_import as _ai

from ._core import types as _t

#: This generator represents the standard C++ type ``float``.
float = _t.float

#: This generator represents the standard C++ type ``double``.
double = _t.double

#: This generator represents the standard C++ type ``long double``.
long_double = _t.long_double

try:
	# NOTE: just a stub at the moment, to show that this works as expected with sphinx.
	#:
	float128 = _t.float128
except AttributeError:
	pass

#: This generator represents the standard C++ type ``signed char`` (a signed integer type
#: whose width is usually 8 bits).
signed_char = _t.signed_char

#: This generator represents the standard C++ type ``short`` (a signed integer type
#: whose width is usually 16 bits).
short = _t.short

#: This generator represents the arbitrary-precision integer type provided by the piranha C++ library.
integer = _t.integer

#: This generator represents the arbitrary-precision rational type provided by the piranha C++ library.
rational = _t.rational

#: This generator represents the multiprecision floating-point type provided by the piranha C++ library.
real = _t.real

#:
kronecker_monomial = _t.kronecker_monomial

#:
polynomial = _t.polynomial

#:
poisson_series = _t.poisson_series
