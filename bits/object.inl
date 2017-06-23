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

#ifndef __MZK_OBJECT_INL
#define __MZK_OBJECT_INL

#include "../object.h"

namespace mzk
{
	template<typename object_type, bool strong>
		inline pointer<object_type, strong>::pointer()
			: _pointer(nullptr)
		{}

	template<typename object_type, bool strong>
		inline pointer<object_type, strong>::pointer(
				const pointer &other)
			: pointer()
		{ operator=(other); }

	template<typename object_type, bool strong>
		inline pointer<object_type, strong>::pointer(
				pointer &&other)
			: pointer()
		{ operator=(other); }

	template<typename object_type, bool strong>
		inline pointer<object_type, strong>::pointer(
				object_type *obj)
			: pointer()
		{ operator=(obj); }

	template<typename object_type, bool strong>
		template<typename other_object_type, bool other_strong>
			inline pointer<object_type, strong>::pointer(
					const pointer<other_object_type, other_strong> &other)
				: pointer()
			{ operator=(other); }

	template<typename object_type, bool strong>
		template<typename other_object_type>
			inline pointer<object_type, strong>::pointer(
					other_object_type *obj)
				: pointer()
			{ operator=(obj); }

	template<typename object_type, bool strong>
		inline pointer<object_type, strong>::~pointer()
		{ operator=(nullptr); }

	template<typename object_type, bool strong>
		inline pointer<object_type, strong> &
		pointer<object_type, strong>::operator=(
				const pointer &other)
		{ return operator=(static_cast<object_type *>(other)); }

	template<typename object_type, bool strong>
		inline pointer<object_type, strong> &
		pointer<object_type, strong>::operator=(
				pointer &&other)
		{ return operator=(static_cast<object_type *>(other)); }

	template<typename object_type, bool strong>
		inline pointer<object_type, strong> &
		pointer<object_type, strong>::operator=(
				object_type *obj)
		{
			if (obj)
				obj->register_mzk_pointer(this, strong);

			if (_pointer)
				_pointer->unregister_mzk_pointer(this, strong);

			_pointer = obj;
			return *this;
		}

	template<typename object_type, bool strong>
		template<typename other_object_type, bool other_strong>
			inline pointer<object_type, strong> &
			pointer<object_type, strong>::operator=(
					const pointer<other_object_type, other_strong> &other)
			{ return operator=(static_cast<object_type *>(other)); }

	template<typename object_type, bool strong>
		template<typename other_object_type>
			inline pointer<object_type, strong> &
			pointer<object_type, strong>::operator=(
					other_object_type *obj)
			{ return operator=(static_cast<object_type *>(obj)); }

	template<typename object_type, bool strong>
		inline object_type *pointer<object_type, strong>::raw() const
		{ return _pointer; }

	template<typename object_type, bool strong>
		inline pointer<object_type, strong>::operator object_type *() const
		{ return raw(); }

	template<typename object_type, bool strong>
		inline object_type *pointer<object_type, strong>::operator->() const
		{ return raw(); }

	template<typename object_type, bool strong>
		inline void pointer<object_type, strong>::on_mzk_object_delete()
		{ _pointer = nullptr; }

	template<typename object_type, bool strong>
		template<typename other_object_type>
		 inline other_object_type *pointer<object_type, strong>::as() const
		 { return static_cast<other_object_type *>(raw()); }

	template<typename object_type, bool strong>
		inline bool pointer<object_type, strong>::is_null() const
		{ return raw() == nullptr; }




	inline object::object()
		: _ref_count(0)
	{}

	inline void object::register_mzk_pointer(base_pointer *ptr, bool strong)
	{
		if (_pointer_set.find(ptr) == _pointer_set.end())
		{
			_pointer_set.insert(ptr);

			if (strong)
				_ref_count++;
		}
	}

	inline void object::unregister_mzk_pointer(base_pointer *ptr, bool strong)
	{
		if (_pointer_set.find(ptr) != _pointer_set.end())
		{
			_pointer_set.erase(ptr);

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
