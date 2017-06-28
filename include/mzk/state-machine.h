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

#ifndef __MZK_STATE_MACHINE_H
#define __MZK_STATE_MACHINE_H

#include "object.h"
#include "property.h"

namespace mzk
{
	  template<typename state_type, state_type state_count>
	class state_machine : public object
	{
	  public:
		state_machine();

		void transit(state_type from, state_type to);

		property<state_type> prop_state;

		signal<state_type> sigv_state_entered[state_count];
		signal<state_type> sigv_state_exited[state_count];

	  private:
		void _on_state_changed(state_type, state_type);
	};
}

#ifndef __MZK_NO_IMPL
# include "details/state-machine.inl"
#endif

#endif
