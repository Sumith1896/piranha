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

#ifdef __linux__

extern "C"
{
#include <sys/sysinfo.h>
}

#endif

#include <mutex>
#include <thread>

#include "runtime_info.hpp"

namespace piranha
{

const std::thread::id runtime_info::m_main_thread_id = {std::this_thread::get_id()};
std::mutex runtime_info::m_mutex;

/// Main thread ID.
/**
 * @return ID of the main thread of execution.
 */
std::thread::id runtime_info::get_main_thread_id()
{
	return m_main_thread_id;
}

/// Hardware concurrency.
/**
 * @return number of detected hardware thread contexts (typically equal to the number of CPU cores), or 0 if
 * the detection fails.
 */
unsigned runtime_info::hardware_concurrency()
{
	std::lock_guard<std::mutex> lock(m_mutex);
#ifdef __linux__
	int candidate = ::get_nprocs();
	return (candidate <= 0) ? 0 : static_cast<unsigned>(candidate);
#else
#warning Cannot determine automatically hardware concurrency.
	return 0;
#endif
}

}
