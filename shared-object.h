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

#ifndef __MZK_SHARED_OBJECT_H
#define __MZK_SHARED_OBJECT_H

#include <unordered_set>

#ifndef __MZK_NO_IMPL
# define __MZK_NO_IMPL
# include "pointer.h"
# undef __MZK_NO_IMPL
#else
# include "pointer.h"
#endif

namespace mzk
{
	class shared_object
	{
	  public:
		shared_object();
		shared_object(const shared_object &other) = delete;
		shared_object(shared_object &&other) = delete;

		virtual ~shared_object();

		void register_mzk_pointer(base_pointer *ptr, bool strong);
		void unregister_mzk_pointer(base_pointer *ptr, bool strong);

	  private:
		std::unordered_set<base_pointer *> _ptr_set;
		int _ref_count;
	};
}

#ifndef __MZK_NO_IMPL
# include "bits/shared-object.inl"
#endif

#endif
