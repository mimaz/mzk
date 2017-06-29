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
		thread_local time_t start_time;
	}

	int timer::get_current_time()
	{
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		time_t time = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

		if (start_time == 0)
			start_time = time;

		return static_cast<int>(time - start_time);
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
					tim->timer_tick();
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
		: _running(false)
		, _ticks(-1)
		, _delay(0)
		, _period(1000)
	{
		timer_set.insert(this);
	}

	timer::~timer()
	{
		timer_set.erase(this);
	}

	void timer::start()
	{ set_running(true); }

	void timer::stop()
	{ set_running(false); }

	void timer::restart()
	{
		stop();
		start();
	}

	void timer::set_running(bool running)
	{
		if (running != _running)
		{
			_running = running;
			sig_running_changed(running);

			if (running)
			{
				sig_started();

				_ticks_left = get_ticks();
				_next_time = get_current_time() + get_delay();
			}
			else
			{
				sig_stopped();
			}
		}
	}

	void timer::set_ticks(int ticks)
	{
		if (ticks != _ticks)
		{
			_ticks = ticks;
			sig_ticks_changed(ticks);
		}
	}

	void timer::set_delay(int delay)
	{
		if (delay != _delay)
		{
			_delay = delay;
			sig_delay_changed(delay);
		}
	}

	void timer::set_period(int period)
	{
		if (period != _period)
		{
			_period = period;
			sig_period_changed(period);
		}
	}

	bool timer::is_running() const
	{ return _running; }

	int timer::get_ticks() const
	{ return _ticks; }

	int timer::get_delay() const
	{ return _delay; }

	int timer::get_period() const
	{ return _period; }

	void timer::timer_tick()
	{
		if (is_running() &&
				_next_time <= get_current_time())
		{
			sig_triggered();

			if (_ticks_left > 0)
				_ticks_left--;

			if (_ticks_left)
				_next_time += get_period();
			else
				stop();
		}
	}
}
