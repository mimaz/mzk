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

#include <iostream>

#include <mzk/logger.h>

namespace mzk
{
	void default_logi_write(const std::string &str)
	{ std::cout << str << std::endl; }

	void default_logd_write(const std::string &str)
	{ std::cout << str << std::endl; }

	void default_loge_write(const std::string &str)
	{ std::cerr << str << std::endl; }

	void (*logi_write)(const std::string &str) = default_logi_write;
	void (*logd_write)(const std::string &str) = default_logd_write;
	void (*loge_write)(const std::string &str) = default_loge_write;
}
