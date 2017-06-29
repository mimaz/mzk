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

#ifndef __MZK_ANIMATOR_INL
#define __MZK_ANIMATOR_INL

#include "../animator.h"

namespace mzk
{
	  template<typename value_type>
	animator<value_type>::animator()
	{
		prop_running.sig_changed.connect(
				&animator::_on_running_changed,
				this, arg1);

		prop_running = false;
		prop_duration = 1000;

		_timer.sig_triggered.connect(&animator::_on_tick, this);
		_timer.prop_ticks = -1;
		_timer.prop_period = 1000 / 60;
		_timer.prop_delay = 1000 / 60;
	}

	  template<typename value_type>
	void animator<value_type>::_on_running_changed(bool running)
	{
		if (running)
		{
			_start = prop_start_value;
			_end = prop_end_value;
			_start_time = timer::get_current_time();
			_end_time = _start_time + prop_duration;

			sig_started();
		}
		else
		{
			_set_progress(1.0f);

			sig_stopped();
		}

		_timer.prop_running = running;
	}

	  template<typename value_type>
	void animator<value_type>::_on_tick()
	{
		time_t time = timer::get_current_time();

		if (time >= _end_time)
		{
			prop_running = false;
			return;
		}

		time_t elapsed = time - _start_time;

		float progress = static_cast<float>(elapsed) 
			/ (_end_time - _start_time);

		_set_progress(progress);
	}

	  template<typename value_type>
	void animator<value_type>::_set_progress(float progress)
	{
		prop_value = _start + (_end - _start) * progress;
	}
}

#endif
