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

#ifndef __MZK_SHARED_OBJECT_INL
#define __MZK_SHARED_OBJECT_INL

#include "../shared-object.h"

#include "pointer.inl"

namespace mzk
{
	inline shared_object::shared_object()
	{}

	inline shared_object::~shared_object()
	{}

	inline void shared_object::register_mzk_pointer(
			base_pointer *ptr,
			bool strong)
	{ 
		if (_ptr_set.find(ptr) == _ptr_set.end())
		{
			_ptr_set.insert(ptr); 

			if (strong)
				_ref_count++;
		}
	}

	inline void shared_object::unregister_mzk_pointer(
			base_pointer *ptr,
			bool strong)
	{ 
		if (_ptr_set.find(ptr) != _ptr_set.end())
		{
			_ptr_set.erase(ptr); 

			if (strong)
			{
				_ref_count--;

				if (_ref_count < 1)
					delete this;
			}
		}
	}
}

#endif
