#ifndef __MZK_PROPERTY_INL
#define __MZK_PROPERTY_INL

#include <functional>

#include "../property.h"

#include "signal.inl"
#include "object.inl"

namespace mzk
{
	template<typename value_type>
		class property<value_type>::repeater : public object
		{
		 public:
			 virtual value_type pass(const value_type &value) = 0;
		};


	template<typename value_type>
		inline property<value_type>::property()
			: property(value_type())
		{}

	template<typename value_type>
		inline property<value_type>::property(
				const value_type &value)
			: _value(value)
		{}

	template<typename value_type>
		inline property<value_type>::~property()
		{}

	template<typename value_type>
		inline property<value_type> &property<value_type>::operator=(
				const value_type &value)
		{
			set_value(value);
			return * this;
		}

	template<typename value_type>
		inline const value_type &property<value_type>::get_value() const
		{ return _value; }

	template<typename value_type>
		inline void property<value_type>::set_value(const value_type &value)
		{
			value_type new_value = _repeater.is_null() ? 
				value : _repeater->pass(value);

			if (new_value != get_value())
			{
				value_type old = get_value();
				_value = new_value;
				_signal(new_value, old);
			}
		}

	template<typename value_type>
		template<typename functor_type>
			inline void property<value_type>::set_repeater(
					const functor_type &functor)
			{
				class wrapper : public repeater
				{
				 public:
					 inline wrapper(const functor_type &functor)
						 : _functor(functor)
					 {}

					 value_type pass(const value_type &value) override
					 { return _functor(value); }

				 private:
					 functor_type _functor;
				};

				_repeater = new wrapper(functor);
			}

	template<typename value_type>
		inline void property<value_type>::clear_repeater()
		{ _repeater = nullptr; }

	template<typename value_type>
		template<typename ...bind_arg_types>
			inline slot property<value_type>::bind(
					const bind_arg_types &...args)
			{ return _signal.bind(args...); }

	template<typename value_type>
		inline void property<value_type>::unbind(
				mzk::slot slot)
		{ _signal.unbind(slot); }
}

#endif
