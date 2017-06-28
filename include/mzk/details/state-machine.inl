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
	/*
	 * state_machine
	 */

	  template<int state_count, typename id_type>
	inline state_machine<state_count, id_type>::state_machine(id_type initial)
		: _initial_id(initial)
	{
		prop_active.connect_slot(&state_machine::_on_active_changed, this, arg1);


		auto binding = std::bind(&state_machine::_repeat_id, this, arg1);
		auto repeater = make_repeater<id_type>(binding);

		prop_active_id.set_repeater(repeater);


		for (int i = 0; i < state_count; i++)
			get_state(static_cast<id_type>(i))->assign(this, static_cast<id_type>(i));

		prop_active = false;
		prop_active_id = initial;
	}

	  template<int state_count, typename id_type>
		inline typename state_machine<state_count, id_type>::state *
	state_machine<state_count, id_type>::get_state(id_type id)
	{ return &_states[id]; }

	  template<int state_count, typename id_type>
	inline id_type state_machine<state_count, id_type>::get_initial_id() const
	{ return _initial_id; }

	  template<int state_count, typename id_type>
		inline typename state_machine<state_count, id_type>::transition *
	state_machine<state_count, id_type>::get_transition(id_type source, id_type target)
	{ return get_state(source)->get_transition(target); }

	  template<int state_count, typename id_type>
		template<id_type source, id_type target>
	inline void state_machine<state_count, id_type>::transit()
	{ get_transition(source, target)->transit(); }

	  template<int state_count, typename id_type>
	inline id_type state_machine<state_count, id_type>::_repeat_id(id_type id) const
	{ return prop_active ? id : static_cast<id_type>(prop_active_id); }

	  template<int state_count, typename id_type>
	inline void state_machine<state_count, id_type>::_on_active_changed(bool active)
	{
		if (active)
			prop_active_id = get_initial_id();
	}

	/*
	 * state_machine::state
	 */

	  template<int state_count, typename id_type>
	inline void state_machine<state_count, id_type>::state::assign(
			state_machine *machine,
			id_type id)
	{
		_machine = machine;
		_id = id;

		for (int i = 0; i < state_count; i++)
			_transitions[i].assign(this, machine->get_state(static_cast<id_type>(i)));
	}

	  template<int state_count, typename id_type>
		inline state_machine<state_count, id_type> *
	state_machine<state_count, id_type>::state::get_machine() const
	{ return _machine; }

	  template<int state_count, typename id_type>
		inline typename state_machine<state_count, id_type>::transition *
	state_machine<state_count, id_type>::state::get_transition(id_type id)
	{ return &_transitions[id]; }

	  template<int state_count, typename id_type>
	inline id_type state_machine<state_count, id_type>::state::get_id() const
	{ return _id; }

	/*
	 * state_machine::transition
	 */

	  template<int state_count, typename id_type>
	inline void state_machine<state_count, id_type>::transition::assign(
			state *source,
			state *target)
	{
		_source = source;
		_target = target;
	}

	  template<int state_count, typename id_type>
	inline void state_machine<state_count, id_type>::transition::transit()
	{
		id_type active_id = static_cast<id_type>(get_machine()->prop_active_id);

		if (get_machine()->prop_active
				&& get_source()->get_id() == active_id)
		{
			get_machine()->prop_active_id = get_target()->get_id();
		}
	}

	  template<int state_count, typename id_type>
		inline state_machine<state_count, id_type> *
	state_machine<state_count, id_type>::transition::get_machine() const
	{ return get_source()->get_machine(); }

	  template<int state_count, typename id_type>
		inline typename state_machine<state_count, id_type>::state *
	state_machine<state_count, id_type>::transition::get_source() const
	{ return _source; }

	  template<int state_count, typename id_type>
		inline typename state_machine<state_count, id_type>::state *
	state_machine<state_count, id_type>::transition::get_target() const
	{ return _target; }
}

#endif
