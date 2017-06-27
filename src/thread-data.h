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

#ifndef __MZK_THREAD_DATA_H
#define __MZK_THREAD_DATA_H

#include <unordered_set>

#include <mzk/threaded.h>
#include <mzk/shared.h>

namespace mzk
{
	class thread_data : public shared
	{
	  public:
		typedef std::unordered_set<threaded *> threaded_set;

		void register_object(threaded *obj);
		void unregister_object(threaded *obj);

		void notify_all();

		const threaded_set &get_object_set() const;
	
	  private:
		threaded_set _object_set;
	};

	namespace this_thread_data
	{
		thread_data *instance();

		void register_object(threaded *obj);

		void unregister_object(threaded *obj);

		void notify_all();

		const thread_data::threaded_set &get_object_set();
	}
}

#endif
