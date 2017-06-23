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

#ifndef __MZK_SIGNAL_INL
#define __MZK_SIGNAL_INL

#include <functional>

#include "signal.h"

namespace mzk
{
	template<typename ...arg_types>
		class connection : public base_connection
		{
		 public:
			 using signal = mzk::signal<arg_types ...>;


			 inline connection(signal *sig)
				 : _sig(sig)
			 {}

			 inline ~connection()
			 { disconnect(); }

			 virtual void invoke(const arg_types &...args) = 0;

			 void disconnect() override
			 { _sig->unbind(this); }

		 private:
			 signal * const _sig;
		};



	template<typename ...arg_types>
		inline signal<arg_types ...>::signal()
		{}

	template<typename ...arg_types>
		inline signal<arg_types ...>::~signal()
		{}

	template<typename ...arg_types>
		template<typename ...bind_arg_types>
			inline slot signal<arg_types ...>::bind(
					const bind_arg_types &...args)
			{
				class wrapper : public mzk::connection<arg_types ...>
				{
				 public:
					inline wrapper(signal *sig, const bind_arg_types &...args)
						: mzk::connection<arg_types ...>(sig)
						, _binding(std::bind(args...))
					{}
					 
					void invoke(const arg_types &...args) override
					{ _binding(args...); }

				 private:
					 decltype(std::bind(args...)) _binding;
				};



				mzk::slot slot = new wrapper(this, args...);
				_slot_set.push_back(slot);


				return slot;
			}

	template<typename ...arg_types>
		inline void signal<arg_types ...>::unbind(
				connection *conn)
		{
			for (auto it = _slot_set.begin(); it != _slot_set.end(); it++)
				if (*it == conn)
					_slot_set.erase(it--);
		}

	template<typename ...arg_types>
		inline signal<arg_types ...> &signal<arg_types ...>::operator()(
				const arg_types &...args)
		{
			for (mzk::slot &slot : _slot_set)
				slot.as<connection>()->invoke(args...);

			return *this;
		}
}

#endif
