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

#ifndef __MZK_SIGNAL_INL
#define __MZK_SIGNAL_INL

#include "../signal.h"

#include "shared.inl"

namespace mzk
{
	namespace details
	{
		  template<typename ...arg_types>
		struct bind_type_getter
		{
			auto operator()(arg_types ...args) 
				-> decltype(std::bind(args...)) {}
		};

		  template<typename ...arg_types>
		using bind_type = typename 
				std::result_of<bind_type_getter<arg_types ...>(arg_types ...)>::type;
	}

	  template<typename ...arg_types>
	class specific_connection : public connection
	{
	  public:
		  template<typename ...owner_types>
		inline specific_connection(
				const std::initializer_list<signaled *> &slot_list)
		{ 
			for (signaled *slot : slot_list)
			{
				slot->register_mzk_connection(this);
				_owner_set.push_back(slot);
			}
		}

		inline ~specific_connection()
		{ disconnect(); }
		 
		inline void disconnect() override
		{ 
			for (signaled *owner : _owner_set)
				owner->unregister_mzk_connection(this);

			_owner_set.clear();
		}

		virtual void invoke(const arg_types &...args) const = 0;

	  private:
		std::vector<signaled *> _owner_set;
	};

  	/*
	 * signaled class
	 */
	
	inline signaled::~signaled()
	{
		std::vector<ptr<connection>> tmpset(
				_connection_set.begin(), 
				_connection_set.end());

		for (connection *conn : tmpset)
			conn->disconnect();
	}

	inline void signaled::register_mzk_connection(connection *conn)
	{ _connection_set.insert(conn); }

	inline void signaled::unregister_mzk_connection(connection *conn)
	{ _connection_set.erase(conn); }

	/*
	 * signal class
	 */
	
	  template<typename ...arg_types>
		template<typename method_type, 
		  		 typename slot_type,
				 typename ...bind_arg_types>
	inline ptr<connection> signal<arg_types ...>::connect_slot(
			method_type method,
			slot_type slot,
			const bind_arg_types &...bind_args)
	{
		class wrapper : public specific_connection<arg_types ...>
		{
		  public:
			inline wrapper(signal *sig, 
						   method_type method, 
						   slot_type slot, 
						   const bind_arg_types &...bind_args)
				: specific_connection<arg_types ...>({ sig, slot })
				, _binding(std::bind(method, slot, bind_args...))
			{}

			void invoke(const arg_types &...args) const override
			{ _binding(args...); }

		  private:
			  const 
			details::bind_type<method_type, 
							   slot_type, 
							   bind_arg_types ...> _binding;
		};

		return new wrapper(this, method, slot, bind_args...);
	}

	  template<typename ...arg_types>
		template<typename functor_type>
	inline ptr<connection> signal<arg_types ...>::connect_lambda(
			functor_type functor)
	{
		class wrapper : public specific_connection<arg_types ...>
		{
		  public:
			inline wrapper(signal *sig, functor_type functor)
				: specific_connection<arg_types ...>({ sig })
				, _functor(functor)
			{}

			void invoke(const arg_types &...args) const override
			{ _functor(args...); }

		  private:
			const functor_type _functor;
		};

		return new wrapper(this, functor);
	}

	  template<typename ...arg_types>
	inline void signal<arg_types ...>::operator()(
			const arg_types &...args) const
	{
		for (const ptr<connection> &conn : _connection_set)
			conn.cast<specific_connection<arg_types ...>>()->invoke(args...);
	}
}

#endif
