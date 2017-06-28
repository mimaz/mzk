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

#ifndef __MZK_STATE_MACHINE_INL
#define __MZK_STATE_MACHINE_INL

#include "../state-machine.h"

namespace mzk
{
	  template<typename state_type, state_type state_count>
	state_machine<state_type, state_count>::state_machine()
	{
		prop_state.sig_changed.connect(
				&state_machine::_on_state_changed, 
				this, arg1, arg2);
	}

	  template<typename state_type, state_type state_count>
	void state_machine<state_type, state_count>::transit(
			state_type from, 
			state_type to)
	{
		if (from == prop_state)
			prop_state = to;
	}

	  template<typename state_type, state_type state_count>
	void state_machine<state_type, state_count>::_on_state_changed(
			state_type new_id,
			state_type previous_id)
	{
		sigv_state_exited[previous_id](previous_id);
		sigv_state_entered[new_id](new_id);
	}
}

#endif
