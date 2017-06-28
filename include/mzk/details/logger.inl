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

#ifndef __MZK_LOGGER_INL
#define __MZK_LOGGER_INL

namespace mzk
{
	namespace details
	{
		  template<typename arg_type>
		void build_string(
				std::ostringstream &os,
				const arg_type &arg)
		{ os << arg; }

		  template<typename arg_type, typename ...rest_types>
		void build_string(
				std::ostringstream &os, 
				const arg_type &arg, 
				const rest_types &...rest)
		{
			build_string(os, arg);
			build_string(os, rest...);
		}

		  template<typename ...arg_types>
		std::string get_string(
				const arg_types &...args)
		{
			std::ostringstream os;

			build_string(os, args...);

			return os.str();
		}
	}

	  template<typename ...arg_types>
	void logi(const arg_types &...args)
	{ logi_write(details::get_string("INFO: ", args...)); }

	  template<typename ...arg_types>
	void logd(const arg_types &...args)
	{ logd_write(details::get_string("DEBUG: ", args...)); }

	  template<typename ...arg_types>
	void loge(const arg_types &...args)
	{ loge_write(details::get_string("ERROR: ", args...)); }
}

#endif
