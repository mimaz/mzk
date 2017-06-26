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

#ifndef __MZK_SIGNAL_H
#define __MZK_SIGNAL_H

#include <vector>
#include <unordered_set>

#ifndef __MZK_NO_IMPL
# define __MZK_NO_IMPL
# include "shared.h"
# undef __MZK_NO_IMPL
#else
# include "shared.h"
#endif

namespace mzk
{
	class slot_object;

	class connection : public shared_object
	{
	  public:
		virtual void disconnect() = 0;
	};

	class slot_object
	{
	  public:
		slot_object() = default;
		slot_object(const slot_object &other) = delete;
		slot_object(slot_object &&other) = delete;
		virtual ~slot_object();

		void register_mzk_connection(connection *conn);
		void unregister_mzk_connection(connection *conn);
	 
	  private:
		std::unordered_set<ptr<connection>> _connection_set;

		  template<typename ...>
		friend class signal;
	};

	  template<typename ...arg_types>
	class specific_connection;

	  template<typename ...arg_types>
	class signal : public slot_object
	{
	  public:
		  template<typename method_type,
			  	   typename slot_type,
				   typename ...bind_arg_types>
		ptr<connection> connect(method_type method, 
								slot_type *slot,
								const bind_arg_types &...bind_args);

		void operator()(const arg_types &...args);
	};
}

#ifndef __MZK_NO_IMPL
# include "details/signal.inl"
#endif

#endif
