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
		: _running(false)
		, _duration(1000)
	{
		_timer.sig_triggered.connect(&animator::_on_tick, this);
		_timer.set_ticks(-1);
		_timer.set_delay(1000 / 60);
		_timer.set_period(1000 / 60);
	}

	  template<typename value_type>
	bool animator<value_type>::is_running() const
	{ return _running; }

	  template<typename value_type>
	int animator<value_type>::get_duration() const
	{ return _duration; }

	  template<typename value_type>
	value_type animator<value_type>::get_start_value() const
	{ return _start_value; }

	  template<typename value_type>
	value_type animator<value_type>::get_end_value() const
	{ return _end_value; }

	  template<typename value_type>
	value_type animator<value_type>::get_value() const
	{ return _value; }

	  template<typename value_type>
	void animator<value_type>::start()
	{ set_running(true); }

	  template<typename value_type>
	void animator<value_type>::stop()
	{ set_running(false); }

	  template<typename value_type>
	void animator<value_type>::restart()
	{
		stop();
		start();
	}

	  template<typename value_type>
	void animator<value_type>::set_running(bool running)
	{
		if (running != _running)
		{
			_running = running;

			sig_running_changed(running);

			if (running)
			{
				sig_started();
				_start_time = timer::get_current_time();
			}
			else
			{
				sig_stopped();
			}

			_timer.set_running(running);
		}
	}

	  template<typename value_type>
	void animator<value_type>::set_duration(int duration)
	{
		if (duration != _duration)
		{
			_duration = duration;
			sig_duration_changed(duration);
		}
	}

	  template<typename value_type>
	void animator<value_type>::set_start_value(value_type value)
	{
		if (value != _start_value)
		{
			_start_value = value;
			sig_start_value_changed(value);
		}
	}

	  template<typename value_type>
	void animator<value_type>::set_end_value(value_type value)
	{
		if (value != _end_value)
		{
			_end_value = value;
			sig_end_value_changed(value);
		}
	}

	  template<typename value_type>
	void animator<value_type>::_set_progress(float progress)
	{
		_value = get_start_value() 
			+ (get_end_value() - get_start_value()) 
			* progress;
		
		sig_value_changed(get_value());
	}

	  template<typename value_type>
	void animator<value_type>::_on_tick()
	{
		int time = timer::get_current_time();

		int elapsed = time - _start_time;

		float progress = static_cast<float>(elapsed) / get_duration();

		if (progress < 1.0f)
		{
			_set_progress(progress);
		}
		else
		{
			_set_progress(1.0f);
			stop();
		}
	}
}

#endif
