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

#include <thread>

#include <mzk/timer-loop.h>
#include <mzk/timer.h>

#include "timer-thread.h"

namespace mzk
{
	void timer_loop::start()
	{
		_run_flag = true;

		sig_started();


		while (_run_flag)
		{
			bool noyield = false;

			for (timer *tm : this_timer_set)
				noyield = noyield || tm->mzk_notify();

			if (! noyield)
				std::this_thread::yield();
		}


		sig_stopped();
	}

	void timer_loop::stop()
	{
		_run_flag = false;
	}
}
