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

#ifndef __MZK_PROPERTY_INL
#define __MZK_PROPERTY_INL

#include <functional>

#include "../property.h"

#include "signal.inl"
#include "shared.inl"

namespace mzk
{
	  template<typename value_type, typename functor_type>
	inline repeater<value_type> make_repeater(functor_type functor)
	{
		class wrapper : public repeater_interface<value_type>
		{
		  public:
			inline wrapper(functor_type functor)
				: _functor(functor)
			{}

			value_type pass(const value_type &value) override
			{ return _functor(value); }

		  private:
			const functor_type _functor;
		};

		return new wrapper(functor);
	}



	  template<typename value_type>
	inline property<value_type>::property()
		: property(value_type())
	{}

	  template<typename value_type>
	inline property<value_type>::property(
			const value_type &value)
		: _value(value)
	{}

	  template<typename value_type>
	inline property<value_type>::~property()
	{}

	  template<typename value_type>
	inline property<value_type> &property<value_type>::operator=(
			const value_type &value)
	{
		set_value(value);
		return *this;
	}

	  template<typename value_type>
		template<typename other_type>
	inline property<value_type> &property<value_type>::operator+=(
			const other_type &value)
	{ return operator=(get_value() + value); }

	  template<typename value_type>
		template<typename other_type>
	inline property<value_type> &property<value_type>::operator-=(
			const other_type &value)
	{ return operator=(get_value() - value); }

	  template<typename value_type>
		template<typename other_type>
	inline property<value_type> &property<value_type>::operator*=(
			const other_type &value)
	{ return operator=(get_value() * value); }

	  template<typename value_type>
		template<typename other_type>
	inline property<value_type> &property<value_type>::operator/=(
			const other_type &value)
	{ return operator=(get_value() / value); }

	  template<typename value_type>
	inline property<value_type> &property<value_type>::operator--()
	{ 
		value_type value = get_value();
		return operator=(--value); 
	}

	  template<typename value_type>
	inline value_type property<value_type>::operator--(int)
	{ 
		value_type old = get_value();
		operator--();
		return old;
	}

	  template<typename value_type>
	inline property<value_type> &property<value_type>::operator++()
	{ 
		value_type value = get_value();
		return operator=(++value); 
	}

	  template<typename value_type>
	inline value_type property<value_type>::operator++(int)
	{ 
		value_type old = get_value();
		operator++();
		return old;
	}

	  template<typename value_type>
	inline property<value_type>::operator value_type() const
	{ return get_value(); }

	  template<typename value_type>
	inline const value_type &property<value_type>::get_value() const
	{ return _value; }

	  template<typename value_type>
	inline void property<value_type>::set_value(
			const value_type &value)
	{
		value_type new_value = _repeater.is_null() ? 
			value : _repeater->pass(value);

		if (new_value != get_value())
		{
			value_type old = get_value();
			_value = new_value;
			sig_changed(new_value, old);
		}
	}

	  template<typename value_type>
	inline void property<value_type>::set_repeater(
			repeater<value_type> rep)
	{ _repeater = rep; }

	  template<typename value_type>
		template<typename functor_type>
	inline repeater<value_type> property<value_type>::set_repeater(
			functor_type functor)
	{
		auto rep = make_repeater<value_type>(functor);
		set_repeater(rep);
		return rep;
	}

	  template<typename value_type>
	inline void property<value_type>::clear_repeater()
	{ _repeater = nullptr; }

	  template<typename value_type>
		template<typename method_type,
		  		 typename slot_type,
				 typename ...bind_arg_types>
	inline ptr<connection> property<value_type>::connect_slot(
			method_type method,
			slot_type slot,
			const bind_arg_types &...args)
	{ return sig_changed.connect_slot(method, slot, args...); }
	
	  template<typename value_type>
		template<typename functor_type>
	inline ptr<connection> property<value_type>::connect_lambda(
			functor_type functor)
	{ return sig_changed.connect_lambda(functor); }
}

#endif
