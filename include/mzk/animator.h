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

#ifndef __MZK_ANIMATOR_H
#define __MZK_ANIMATOR_H

#include "timer.h"

namespace mzk
{
	  template<typename value_type>
	class animator
	{
	  public:
		animator();

		bool is_running() const;
		int get_duration() const;
		value_type get_start_value() const;
		value_type get_end_value() const;
		value_type get_value() const;

		void start();
		void stop();
		void restart();

		void set_running(bool);
		void set_duration(int);
		void set_start_value(value_type);
		void set_end_value(value_type);

		signal<> sig_started;
		signal<> sig_stopped;
		signal<bool> sig_running_changed;
		signal<int> sig_duration_changed;
		signal<value_type> sig_start_value_changed;
		signal<value_type> sig_end_value_changed;
		signal<value_type> sig_value_changed;

	  private:
		void _set_progress(float);
		void _on_tick();

		bool _running;
		int _duration;
		value_type _start_value;
		value_type _end_value;
		value_type _value;

		timer _timer;
		int _start_time;
	};
	
	extern template class animator<short>;
	extern template class animator<unsigned short>;
	extern template class animator<int>;
	extern template class animator<unsigned int>;
	extern template class animator<long>;
	extern template class animator<unsigned long>;
	extern template class animator<float>;
	extern template class animator<double>;
	extern template class animator<long double>;
}

#ifndef __MZK_NO_IMPL
# include "details/animator.inl"
#endif

#endif
