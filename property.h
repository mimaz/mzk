#ifndef __MZK_PROPERTY_H
#define __MZK_PROPERTY_H

#ifndef __MZK_NO_IMPL
# define __MZK_NO_IMPL
# include "object.h"
# include "signal.h"
# undef __MZK_NO_IMPL
#else
# include "object.h"
# include "signal.h"
#endif

namespace mzk
{
	template<typename value_type>
		class property
		{
		 public:
			class repeater;

			property();
			property(const property &other) = delete;
			property(property &&other) = delete;
			property(const value_type &value);

			~property();


			property &operator=(const property &other) = delete;
			property &operator=(property &&other) = delete;
			property &operator=(const value_type &value);



			const value_type &get_value() const;
			void set_value(const value_type &value);
		


			template<typename functor_type>
				void set_repeater(const functor_type &functor);

			void clear_repeater();


			template<typename ...bind_arg_types>
				slot bind(const bind_arg_types &...args);

			void unbind(mzk::slot slot);

		 private:
			value_type _value;
			ptr<repeater> _repeater;
			signal<value_type, value_type> _signal;
		};
}

#ifndef __MZK_NO_IMPL
# include "bits/property.inl"
#endif

#endif
