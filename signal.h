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

#ifndef __MZK_SIGNAL_H
#define __MZK_SIGNAL_H

#include <vector>

#ifndef __MZK_NO_IMPL
# define __MZK_NO_IMPL
# include "signal-object.h"
# include "shared-object.h"
# undef __MZK_NO_IMPL
#else
# include "signal-object.h"
# include "shared-object.h"
#endif

namespace mzk
{
	class connection : public shared_object
	{
	  public:
		virtual void disconnect() = 0;
	};

	  template<typename ...arg_types>
	class signal
	{
	  public:
		signal();
		signal(const signal &other) = delete;
		signal(signal &&other) = delete;

		~signal();

		  template<typename ...bind_arg_types>
		ptr<connection> connect(const bind_arg_types &...args);

		void disconnect(connection *conn);

		signal &operator()(const arg_types &...args);

	 private:
		std::vector<ptr<connection>> _slot_set;
	};
}

#ifndef __MZK_NO_IMPL
# include "bits/signal.inl"
#endif

#endif
