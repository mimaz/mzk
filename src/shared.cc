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

#include <mzk/shared.h>

namespace mzk
{
	shared_object::~shared_object()
	{
		for (base_ref *ptr : _ptr_set)
			ptr->on_mzk_object_delete();
	}

	void shared_object::register_mzk_ref(
			base_ref *ptr,
			bool strong)
	{ 
		if (_ptr_set.find(ptr) == _ptr_set.end())
		{
			_ptr_set.insert(ptr); 

			if (strong)
				_ref_count++;
		}
	}

	void shared_object::unregister_mzk_ref(
			base_ref *ptr,
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

	base_ref::base_ref()
		: _pointer(nullptr)
	{}
}
