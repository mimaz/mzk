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

#include <mzk/timer.h>

#include "timer-thread.h"

namespace mzk
{
	timer::timer()
	{
		this_timer_set.insert(this);

		prop_running.connect_slot(&timer::_on_running_changed, 
								  this, arg1);

		prop_running = false;
		prop_ticks = -1;
		prop_delay = 0;
		prop_period = 1000;
	}

	timer::~timer()
	{
		this_timer_set.erase(this);
	}

	bool timer::mzk_notify()
	{
		if (prop_running &&
				_next_time <= get_current_time())
		{
			sig_triggered();

			if (_ticks_left > 0)
				_ticks_left--;

			if (_ticks_left)
				_next_time += prop_period;
			else
				prop_running = false;

			return true;
		}

		return false;
	}

	void timer::_on_running_changed(bool running)
	{
		if (running)
		{
			_ticks_left = prop_ticks;
			_next_time = get_current_time() + prop_delay;

			sig_started();
		}
		else
		{
			sig_stopped();
		}
	}
}
