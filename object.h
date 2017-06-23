#ifndef __MZK_OBJECT_H
#define __MZK_OBJECT_H

#include <unordered_set>

namespace mzk
{
	class base_pointer
	{
	 public:
		 virtual void on_mzk_object_delete() = 0;
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


			 object_type *raw() const;
			 operator object_type *() const;
			 object_type *operator->() const;

			 

			 template<typename other_object_type>
				 inline other_object_type *as() const;

			 bool is_null() const;


			 void on_mzk_object_delete() override;


		 private:
			 object_type *_pointer;
		};



	class object
	{
	 public:
		 object();
		 object(const object &other) = delete;
		 object(object &&other) = delete;

		 virtual ~object();


		 void register_mzk_pointer(base_pointer *ptr, bool strong);
		 void unregister_mzk_pointer(base_pointer *ptr, bool strong);

		private:
		 std::unordered_set<base_pointer *> _pointer_set;
		 int _ref_count;
	};


	template<typename object_type>
		using ptr = pointer<object_type, true>;

	template<typename object_type>
		using weak_ptr = pointer<object_type, false>;
}

#ifndef __MZK_NO_IMPL
# include "bits/object.inl"
#endif

#endif
