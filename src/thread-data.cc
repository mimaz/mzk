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

#include <mutex>

#include "thread-data.h"

namespace mzk
{
	void thread_data::register_object(threaded *obj)
	{ _object_set.insert(obj); }

	void thread_data::unregister_object(threaded *obj)
	{ _object_set.erase(obj); }

	void thread_data::notify_all() 
	{
		for (threaded *obj : get_object_set())
			obj->mzk_notify();
	}

	const thread_data::threaded_set &thread_data::get_object_set() const
	{ return _object_set; }


	thread_data *this_thread_data::instance()
	{
		thread_local ptr<thread_data> data_ptr;

		if (data_ptr.is_null())
			data_ptr = new thread_data;

		return data_ptr;
	}

	void this_thread_data::register_object(threaded *obj)
	{ instance()->register_object(obj); }

	void this_thread_data::unregister_object(threaded *obj)
	{ instance()->unregister_object(obj); }

	void this_thread_data::notify_all()
	{ instance()->notify_all(); }

	const thread_data::threaded_set &this_thread_data::get_object_set()
	{ return instance()->get_object_set(); }
}
