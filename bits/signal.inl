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

#include <functional>

#include "../signal.h"

#include "shared-object.inl"

namespace mzk
{
	namespace details
	{
		  template<typename functor_type>
		inline void for_each_slot(functor_type functor) 
		{}

		  template<typename functor_type,
			  typename arg_type>
		inline void for_each_slot(functor_type functor,
				arg_type *arg)
		{
			if (std::is_base_of<slot_object, arg_type>::value)
				functor(static_cast<slot_object *>(arg));
		}

		  template<typename functor_type,
			  typename arg_type>
		inline void for_each_slot(functor_type functor,
				arg_type arg)
		{}

		  template<typename functor_type, 
			  typename arg_type, 
			  typename ...rest_types>
		inline void for_each_slot(functor_type functor, 
				arg_type arg, 
				rest_types ...rest)
		{
			for_each_slot(functor, arg);
			for_each_slot(functor, rest...);
		}
	}

	  template<typename ...arg_types>
	class specific_connection : public connection
	{
	  public:
		  template<typename ...owner_types>
		inline specific_connection(
				signal<arg_types ...> *sig, 
				const owner_types &...owners)
			: _sig(sig)
		{ 
			details::for_each_slot([this](slot_object *owner) {
				_owner_set.push_back(owner);
				std::cout << "owner found!" << std::endl;
			}, owners...);

			_sig->register_mzk_connection(this); 

			for (slot_object *owner : _owner_set)
				owner->register_mzk_connection(this);
		}
		 
		inline void disconnect() override
		{ 
			_sig->unregister_mzk_connection(this); 

			for (slot_object *owner : _owner_set)
				owner->unregister_mzk_connection(this);
		}

		virtual void invoke(const arg_types &...args) = 0;

	  private:
		signal<arg_types ...> * const _sig;
		std::vector<slot_object *> _owner_set;
	};

  	/*
	 * slot_object class
	 */
	
	inline slot_object::slot_object()
	{}

	inline slot_object::~slot_object()
	{
		for (connection *conn : _connection_set)
			conn->disconnect();
	}

	inline void slot_object::register_mzk_connection(connection *conn)
	{ _connection_set.insert(conn); }

	inline void slot_object::unregister_mzk_connection(connection *conn)
	{ _connection_set.erase(conn); }

	/*
	 * signal class
	 */

	  template<typename ...arg_types>
	inline signal<arg_types ...>::signal()
	{}

	  template<typename ...arg_types>
	inline signal<arg_types ...>::~signal()
	{
		for (specific_connection<arg_types ...> *conn : _connection_set)
			conn->disconnect();
	}

	  template<typename ...arg_types>
		template<typename ...bind_arg_types>
	inline ptr<connection> signal<arg_types ...>::connect(
			const bind_arg_types &...args)
	{
		class wrapper : public specific_connection<arg_types ...>
		{
		  public:
			inline wrapper(signal *sig, const bind_arg_types &...args)
				: mzk::specific_connection<arg_types ...>(sig, args...)
				, _binding(std::bind(args...))
			{}
			 
			void invoke(const arg_types &...args) override
			{ _binding(args...); }

		  private:
			 decltype(std::bind(args...)) _binding;
		};

		return new wrapper(this, args...);
	}

	  template<typename ...arg_types>
	inline void signal<arg_types ...>::operator()(
			const arg_types &...args)
	{
		for (specific_connection<arg_types ...> *conn : _connection_set)
			conn->invoke(args...);
	}

	  template<typename ...arg_types>
	inline void signal<arg_types ...>::register_mzk_connection(
			specific_connection<arg_types ...> *conn)
	{ _connection_set.insert(conn); }

	  template<typename ...arg_types>
	inline void signal<arg_types ...>::unregister_mzk_connection(
			specific_connection<arg_types ...> *conn)
	{ _connection_set.erase(conn); }
}

#endif
