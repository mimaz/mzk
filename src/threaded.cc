/*
 * Copyright (C) 2017 Mieszko Mazurek <mimaz@gmx.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <mzk/threaded.h>
#include <iostream>

#include "thread-data.h"

namespace mzk
{
	threaded::threaded()
	{ this_thread_data::register_object(this); }

	threaded::~threaded()
	{ this_thread_data::unregister_object(this); }

	std::thread::id threaded::get_thread_id() const
	{ return _thread_id; }

	void threaded::mzk_notify()
	{ std::cout << "mzk_notify!" << std::endl; }
}
