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
	class base_pointer
	{
	  public:
		virtual void on_mzk_object_delete() = 0;
	};

	class shared_object
	{
	  public:
		shared_object() = default;
		shared_object(const shared_object &other) = delete;
		shared_object(shared_object &&other) = delete;

		virtual ~shared_object();

		void register_mzk_pointer(base_pointer *ptr, bool strong);
		void unregister_mzk_pointer(base_pointer *ptr, bool strong);

	  private:
		std::unordered_set<base_pointer *> _ptr_set;
		int _ref_count;
	};

	  template<typename object_type, bool strong>
	class pointer : public base_pointer
	{
	  public:
		pointer();
		pointer(const pointer &other);
		pointer(pointer &&other);

		pointer(object_type *obj);

		  template<typename other_object_type, bool other_strong>
		pointer(const pointer<other_object_type, other_strong> &other);

		  template<typename other_object_type>
		pointer(other_object_type *obj);

		~pointer();

		pointer &operator=(const pointer &other);
		pointer &operator=(pointer &&other);
		pointer &operator=(object_type *obj);

		  template<typename other_object_type, bool other_strong>
		pointer &operator=(const pointer<other_object_type, other_strong> &other);

		  template<typename other_object_type>
		pointer &operator=(other_object_type *obj);

		bool operator==(const pointer &ptr) const;
		bool operator==(const object_type *obj) const;

		object_type *raw() const;
		operator object_type *() const;
		object_type *operator->() const;

		  template<typename other_object_type>
		inline other_object_type *cast() const;

		bool is_null() const;

		void on_mzk_object_delete() override;

	 private:
		object_type *_pointer;
	};

	  template<typename object_type>
	using ptr = pointer<object_type, true>;

	  template<typename object_type>
	using weak_ptr = pointer<object_type, false>;
}

namespace std
{
	  template<typename object_type, bool strong>
	class hash<mzk::pointer<object_type, strong>>
	{
	  public:
		size_t operator()(const mzk::pointer<object_type, strong> &ptr) const;
	};
};

#ifndef __MZK_NO_IMPL
# include "details/shared.inl"
#endif

#endif
