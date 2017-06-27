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

#ifndef __MZK_STATE_H
#define __MZK_STATE_H

#include "object.h"

namespace mzk
{
	  template<typename id_type>
	class state;

	  template<typename id_type>
	class transition : public object
	{
	  public:
		transition(ptr<state<id_type>> from, ptr<state<id_type>> to);

		ptr<state<id_type>> get_source() const;
		ptr<state<id_type>> get_destination() const;

		void transit();

	  private:
		ptr<state<id_type>> _source;
		ptr<state<id_type>> _destination;
	};

	  template<typename id_type>
	class state : public object
	{
	  public:
		state(id_type id);

		id_type get_id() const;

	  private:
		id_type _id;
	};
}

#endif
