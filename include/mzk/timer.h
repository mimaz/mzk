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
#include "property.h"

namespace mzk
{
	class timer : public object
	{
	  public:
		static void start_loop();
		static void stop_loop();

		static thread_local signal<> sig_loop_started;
		static thread_local signal<> sig_loop_stopped;

		timer();
		~timer();

		property<bool> prop_running;
		property<int> prop_ticks;
		property<int> prop_delay;
		property<int> prop_period;

		signal<> sig_started;
		signal<> sig_stopped;
		signal<> sig_triggered;

		bool mzk_notify();

	  private:
		void _on_running_changed(bool);

		int _ticks_left;
		time_t _next_time;
	};
}

#endif
