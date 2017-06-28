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

#include <unordered_set>
#include <thread>

#include <mzk/timer.h>

namespace mzk
{
	namespace
	{
		thread_local std::unordered_set<timer *> timer_set;
		thread_local bool running_loop;
		thread_local time_t next_time;
	}

	time_t timer::get_current_time()
	{
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
	}

	void timer::start_loop()
	{
		running_loop = true;
		next_time = get_current_time();

		std::vector<timer *> tmp_set;

		sig_loop_started();

		while (running_loop)
		{
			if (next_time > get_current_time())
			{
				std::this_thread::yield();
				continue;
			}

			next_time++;

			tmp_set.resize(timer_set.size());
			std::copy(timer_set.begin(), timer_set.end(), tmp_set.begin());

			for (timer *tim : tmp_set)
				if (timer_set.find(tim) != timer_set.end())
					tim->mzk_notify();
		}

		sig_loop_stopped();
	}

	void timer::stop_loop()
	{
		running_loop = false;
	}

	thread_local signal<> timer::sig_loop_started;
	thread_local signal<> timer::sig_loop_stopped;

	timer::timer()
	{
		timer_set.insert(this);

		prop_running.sig_changed.connect(
				&timer::_on_running_changed, this, arg1);

		prop_running = false;
		prop_ticks = -1;
		prop_delay = 0;
		prop_period = 1000;
	}

	timer::~timer()
	{
		timer_set.erase(this);
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
