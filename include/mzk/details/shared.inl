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

#ifndef __MZK_SHARED_INL
#define __MZK_SHARED_INL

#include "../shared.h"

namespace mzk
{
	  template<typename object_type>
	object_type *base_ref::get_pointer() const
	{ return static_cast<object_type *>(_pointer); }

	  template<typename object_type>
	void base_ref::set_pointer(object_type *pointer)
	{ _pointer = pointer; }

	  template<typename object_type, bool strong>
	ref<object_type, strong>::ref(
			const ref &other)
	{ operator=(other); }

	  template<typename object_type, bool strong>
	ref<object_type, strong>::ref(
			ref &&other)
	{ operator=(other); }

	  template<typename object_type, bool strong>
	ref<object_type, strong>::ref(
			object_type *obj)
	{ operator=(obj); }

	  template<typename object_type, bool strong>
		template<typename other_object_type, bool other_strong>
	ref<object_type, strong>::ref(
			const ref<other_object_type, other_strong> &other)
	{ operator=(other); }

	  template<typename object_type, bool strong>
		template<typename other_object_type>
	ref<object_type, strong>::ref(
			other_object_type *obj)
	{ operator=(obj); }

	  template<typename object_type, bool strong>
	ref<object_type, strong>::~ref()
	{ operator=(nullptr); }

	  template<typename object_type, bool strong>
	ref<object_type, strong> &ref<object_type, strong>::operator=(
			const ref &other)
	{ return operator=(static_cast<object_type *>(other)); }

	  template<typename object_type, bool strong>
	ref<object_type, strong> &ref<object_type, strong>::operator=(
			ref &&other)
	{ return operator=(static_cast<object_type *>(other)); }

	  template<typename object_type, bool strong>
	ref<object_type, strong> &ref<object_type, strong>::operator=(
			object_type *obj)
	{
		if (reinterpret_cast<shared_object *>(obj) != get_pointer())
		{
			if (get_pointer())
				get_pointer()->unregister_mzk_ref(this, strong);

			set_pointer(reinterpret_cast<shared_object *>(obj));

			if (get_pointer())
				get_pointer()->register_mzk_ref(this, strong);
		}

		return *this;
	}

	  template<typename object_type, bool strong>
		template<typename other_object_type, bool other_strong>
	ref<object_type, strong> &ref<object_type, strong>::operator=(
			const ref<other_object_type, other_strong> &other)
	{ return operator=(static_cast<object_type *>(other)); }

	  template<typename object_type, bool strong>
		template<typename other_object_type>
	ref<object_type, strong> &ref<object_type, strong>::operator=(
			other_object_type *obj)
	{ return operator=(static_cast<object_type *>(obj)); }

	  template<typename object_type, bool strong>
	bool ref<object_type, strong>::operator==(const ref &ptr) const
	{ return operator==(ptr.raw()); }

	  template<typename object_type, bool strong>
	bool ref<object_type, strong>::operator==(const object_type *obj) const
	{ return raw() == obj; }

	  template<typename object_type, bool strong>
	object_type *ref<object_type, strong>::raw() const
	{ return get_pointer<object_type>(); }

	  template<typename object_type, bool strong>
	ref<object_type, strong>::operator object_type *() const
	{ return raw(); }

	  template<typename object_type, bool strong>
	object_type *ref<object_type, strong>::operator->() const
	{ return raw(); }

	  template<typename object_type, bool strong>
	void ref<object_type, strong>::on_mzk_object_delete()
	{ set_pointer(static_cast<shared_object *>(nullptr)); }

	  template<typename object_type, bool strong>
		template<typename other_object_type>
	other_object_type *ref<object_type, strong>::cast() const
	{ return static_cast<other_object_type *>(raw()); }

	  template<typename object_type, bool strong>
	bool ref<object_type, strong>::is_null() const
	{ return raw() == nullptr; }
}

namespace std
{
	  template<typename object_type, bool strong>
	size_t hash<mzk::ref<object_type, strong>>::operator()(
			const mzk::ref<object_type, strong> &ptr) const
	{ return hash<object_type *>()(ptr.raw()); }
}

#endif
