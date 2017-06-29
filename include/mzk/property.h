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

#ifndef __MZK_PROPERTY_H
#define __MZK_PROPERTY_H

#ifndef __MZK_NO_IMPL
# define __MZK_NO_IMPL
# include "signal.h"
# include "shared.h"
# undef __MZK_NO_IMPL
#else
# include "signal.h"
# include "shared.h"
#endif

namespace mzk
{
	  template<typename value_type>
	class repeater_interface;
	
	  template<typename value_type>
	using repeater = ref<repeater_interface<value_type>>;
	
	  template<typename value_type, typename functor_type>
	repeater<value_type> make_repeater(functor_type functor);



	  template<typename value_type>
	class property : public slot_object
	{
	  public:
		property();
		property(const property &other) = delete;
		property(property &&other) = delete;
		property(const value_type &value);

		~property();

		property &operator=(const property &other) = delete;
		property &operator=(property &&other) = delete;
		property &operator=(const value_type &value);

		  template<typename other_type>
		property &operator+=(const other_type &value);

		  template<typename other_type>
		property &operator-=(const other_type &value);

		  template<typename other_type>
		property &operator*=(const other_type &value);

		  template<typename other_type>
		property &operator/=(const other_type &value);

		property &operator--();

		value_type operator--(int);

		property &operator++();

		value_type operator++(int);

		operator value_type() const;

		const value_type &get_value() const;
		void set_value(const value_type &value);
	
		void set_repeater(repeater<value_type> rep);

		  template<typename functor_type>
		repeater<value_type> set_repeater(functor_type functor);

		void clear_repeater();

		signal<value_type, value_type> sig_changed;

	  private:
		repeater<value_type> _repeater;
		value_type _value;
	};
}

#ifndef __MZK_NO_IMPL
# include "details/property.inl"
#endif

#endif
