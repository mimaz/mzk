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

#ifndef __MZK_TIMER_H
#define __MZK_TIMER_H

#include "object.h"

namespace mzk
{
	class timer : public object
	{
	  public:
		static void start_loop();
		static void stop_loop();

		static thread_local signal<> sig_loop_started;
		static thread_local signal<> sig_loop_stopped;

		static int get_current_time();

		timer();
		~timer();

		void start();
		void stop();
		void restart();

		void set_running(bool);
		void set_ticks(int);
		void set_delay(int);
		void set_period(int);

		bool is_running() const;
		int get_ticks() const;
		int get_delay() const;
		int get_period() const;

		signal<> sig_started;
		signal<> sig_stopped;
		signal<bool> sig_running_changed;
		signal<int> sig_ticks_changed;
		signal<int> sig_delay_changed;
		signal<int> sig_period_changed;
		signal<> sig_triggered;

		void timer_tick();

	  private:
		bool _running;
		int _ticks;
		int _delay;
		int _period;

		int _ticks_left;
		int _next_time;
	};
}

#endif
