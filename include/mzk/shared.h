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

#ifndef __MZK_SHARED_H
#define __MZK_SHARED_H

#include <unordered_set>

namespace mzk
{
	class base_ref;

	class shared_object
	{
	  public:
		shared_object() = default;
		shared_object(const shared_object &other) = delete;
		shared_object(shared_object &&other) = delete;

		virtual ~shared_object();

		void register_mzk_ref(base_ref *ptr, bool strong);
		void unregister_mzk_ref(base_ref *ptr, bool strong);

	  private:
		std::unordered_set<base_ref *> _ptr_set;
		int _ref_count;
	};

	class base_ref
	{
	  public:
		base_ref();

		virtual void on_mzk_object_delete() = 0;

		  template<typename object_type = shared_object>
		object_type *get_pointer() const;

		  template<typename object_type>
		void set_pointer(object_type *pointer);

	  private:
		shared_object *_pointer;
	};

	  template<typename object_type, bool strong = true>
	class ref : public base_ref
	{
	  public:
		ref() {}
		ref(const ref &other);
		ref(ref &&other);

		ref(object_type *obj);

		  template<typename other_object_type, bool other_strong>
		ref(const ref<other_object_type, other_strong> &other);

		  template<typename other_object_type>
		ref(other_object_type *obj);

		~ref();

		ref &operator=(const ref &other);
		ref &operator=(ref &&other);
		ref &operator=(object_type *obj);

		  template<typename other_object_type, bool other_strong>
		ref &operator=(const ref<other_object_type, other_strong> &other);

		  template<typename other_object_type>
		ref &operator=(other_object_type *obj);

		bool operator==(const ref &ptr) const;
		bool operator==(const object_type *obj) const;

		object_type *raw() const;
		operator object_type *() const;
		object_type *operator->() const;

		void on_mzk_object_delete() override;

		  template<typename other_object_type>
		inline other_object_type *cast() const;

		bool is_null() const;
	};

	  template<typename object_type>
	using weak_ref = ref<object_type, false>;
}

namespace std
{
	  template<typename object_type, bool strong>
	class hash<mzk::ref<object_type, strong>>
	{
	  public:
		size_t operator()(const mzk::ref<object_type, strong> &ptr) const;
	};
};

#ifndef __MZK_NO_IMPL
# include "details/shared.inl"
#endif

#endif
