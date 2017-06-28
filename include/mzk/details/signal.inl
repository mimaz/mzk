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
		  template<typename functor_type, typename arg_type>
		void iterate_slots(functor_type functor, arg_type *arg)
		{
			if (std::is_base_of<slot_object, arg_type>::value)
				functor(reinterpret_cast<slot_object *>(arg));
		}

		  template<typename functor_type, typename arg_type>
		void iterate_slots(functor_type functor, ptr<arg_type> arg)
		{ iterate_slots(functor, arg.raw()); }

		  template<typename functor_type, typename arg_type>
		void iterate_slots(functor_type functor, arg_type arg)
		{}

		  template<typename functor_type>
		void iterate_slots(functor_type functor)
		{}

		  template<typename functor_type, typename arg_type, typename ...rest_types>
		void iterate_slots(functor_type functor, arg_type arg, rest_types ...rest)
		{
			iterate_slots(functor, arg);
			iterate_slots(functor, rest...);
		}

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
		inline specific_connection(const owner_types &...owners)
		{ 
			details::iterate_slots([this](slot_object *obj) {
				obj->register_mzk_connection(this);
				_owner_set.push_back(obj);
			}, owners...);
		}

		inline ~specific_connection()
		{ disconnect(); }
		 
		inline void disconnect() override
		{ 
			for (slot_object *owner : _owner_set)
				owner->unregister_mzk_connection(this);

			_owner_set.clear();
		}

		virtual void invoke(const arg_types &...args) const = 0;

	  private:
		std::vector<slot_object *> _owner_set;
	};
	
	  template<typename ...arg_types>
		template<typename ...bind_arg_types>
	ptr<connection> signal<arg_types ...>::connect(
			const bind_arg_types &...args)
	{
		class wrapper : public specific_connection<arg_types ...>
		{
		  public:
			wrapper(signal *sig, const bind_arg_types &...args)
				: specific_connection<arg_types ...>(sig, args...)
				, _binding(std::bind(args...))
			{}

			void invoke(const arg_types &...args) const override
			{ _binding(args...); }

		  private:
			const details::bind_type<bind_arg_types ...> _binding;
		};

		return new wrapper(this, args...);
	}

	  template<typename ...arg_types>
	inline void signal<arg_types ...>::send(
			const arg_types &...args) const
	{
		for (const ptr<connection> &conn : _connection_set)
			conn.cast<specific_connection<arg_types ...>>()->invoke(args...);
	}

	  template<typename ...arg_types>
	inline void signal<arg_types ...>::operator()(
			const arg_types &...args) const
	{ send(args...); }

	extern template class signal<>;
	extern template class signal<bool>;
	extern template class signal<bool, bool>;
	extern template class signal<char>;
	extern template class signal<char, char>;
	extern template class signal<unsigned char>;
	extern template class signal<unsigned char, unsigned char>;
	extern template class signal<short>;
	extern template class signal<short, short>;
	extern template class signal<unsigned short>;
	extern template class signal<unsigned short, unsigned short>;
	extern template class signal<int>;
	extern template class signal<int, int>;
	extern template class signal<unsigned int>;
	extern template class signal<unsigned int, unsigned int>;
	extern template class signal<long>;
	extern template class signal<long, long>;
	extern template class signal<unsigned long>;
	extern template class signal<unsigned long, unsigned long>;
}

#endif
