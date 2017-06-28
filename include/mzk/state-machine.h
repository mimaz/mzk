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
	  template<int state_count, typename id_type = int>
	class state_machine : public object
	{
	  public:
		class state;
		class transition;

		state_machine(id_type initial);

		state *get_state(id_type id);
		id_type get_initial_id() const;
		transition *get_transition(id_type source, id_type target);

		  template<id_type source, id_type target>
		void transit();

		property<bool> prop_active;
		property<id_type> prop_active_id;
	
	  private:
		id_type _repeat_id(id_type id) const;
		void _on_active_changed(bool);
		
		state _states[state_count];
		id_type _initial_id;
		id_type _active_state_id;
	};

	  template<int state_count, typename id_type>
	class state_machine<state_count, id_type>::state
	{
	  public:
		void assign(state_machine *machine, id_type id);

		state_machine *get_machine() const;
		transition *get_transition(id_type id);
		id_type get_id() const;

		signal<> sig_entered;
		signal<> sig_exited;

	  private:
		transition _transitions[state_count];
		state_machine *_machine;
		id_type _id;
	};

	  template<int state_count, typename id_type>
	class state_machine<state_count, id_type>::transition
	{
	  public:
		void assign(state *source, state *target);

		void transit();

		state_machine *get_machine() const;
		state *get_source() const;
		state *get_target() const;

	  private:
		state *_source;
		state *_target;
	};
}

#ifndef __MZK_NO_IMPL
# include "details/state-machine.inl"
#endif

#endif
