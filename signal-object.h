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

#ifndef __MZK_BASE_OBJECT_H
#define __MZK_BASE_OBJECT_H

#ifndef __MZK_NO_IMPL
# define __MZK_NO_IMPL
# include "pointer.h"
# undef __MZK_NO_IMPL
#else
# include "pointer.h"
#endif

namespace mzk
{
	class signal_connection
	{
	 public:
		 virtual void disconnect() = 0;
	};

	class signal_object
	{
	 public:
		signal_object();
		signal_object(const signal_object &other) = delete;
		signal_object(signal_object &&other) = delete;

		virtual ~signal_object();

	 private:
	};
}

#ifndef __MZK_NO_IMPL
# include "bits/signal-object.inl"
#endif

#endif
