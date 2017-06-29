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

		property<bool> prop_running;
		property<value_type> prop_start_value;
		property<value_type> prop_end_value;
		property<value_type> prop_value;
		property<int> prop_duration;

		signal<> sig_started;
		signal<> sig_stopped;

	  private:
		void _on_running_changed(bool);
		void _set_progress(float);
		void _on_tick();

		timer _timer;
		value_type _start;
		value_type _end;
		time_t _start_time;
		time_t _end_time;
	};
	
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
