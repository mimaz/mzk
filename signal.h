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

#include <unordered_set>
#include <functional>
#include <vector>

#ifndef __MZK_NO_IMPL
# define __MZK_NO_IMPL
# include "shared.h"
# undef __MZK_NO_IMPL
#else
# include "shared.h"
#endif

namespace mzk
{
	class signaled;

	class connection : public shared
	{
	  public:
		virtual void disconnect() = 0;
	};

	class signaled
	{
	  public:
		signaled() = default;
		signaled(const signaled &other) = delete;
		signaled(signaled &&other) = delete;
		virtual ~signaled();

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
	class signal : public signaled
	{
	  public:
		  template<typename method_type,
			  	   typename slot_type,
				   typename ...bind_arg_types>
		ptr<connection> connect_slot(method_type method, 
									 slot_type slot,
									 const bind_arg_types &...bind_args);

		  template<typename functor_type>
		ptr<connection> connect_lambda(functor_type functor);

		void operator()(const arg_types &...args) const;
	};

	constexpr auto arg1 = std::placeholders::_1;
	constexpr auto arg2 = std::placeholders::_2;
	constexpr auto arg3 = std::placeholders::_3;
	constexpr auto arg4 = std::placeholders::_4;
	constexpr auto arg5 = std::placeholders::_5;
	constexpr auto arg6 = std::placeholders::_6;
	constexpr auto arg7 = std::placeholders::_7;
	constexpr auto arg8 = std::placeholders::_8;
	constexpr auto arg9 = std::placeholders::_9;
}

#ifndef __MZK_NO_IMPL
# include "details/signal.inl"
#endif

#endif
