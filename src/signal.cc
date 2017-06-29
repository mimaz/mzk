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

#include <mzk/signal.h>

namespace mzk
{
	slot_object::~slot_object()
	{
		std::vector<ref<connection>> tmpset(
				_connection_set.begin(), 
				_connection_set.end());

		for (connection *conn : tmpset)
			conn->disconnect();
	}

	void slot_object::register_mzk_connection(connection *conn)
	{ _connection_set.insert(conn); }

	void slot_object::unregister_mzk_connection(connection *conn)
	{ _connection_set.erase(conn); }

	template class signal<>;
	template class signal<bool>;
	template class signal<bool, bool>;
	template class signal<char>;
	template class signal<char, char>;
	template class signal<unsigned char>;
	template class signal<unsigned char, unsigned char>;
	template class signal<short>;
	template class signal<short, short>;
	template class signal<unsigned short>;
	template class signal<unsigned short, unsigned short>;
	template class signal<int>;
	template class signal<int, int>;
	template class signal<unsigned int>;
	template class signal<unsigned int, unsigned int>;
	template class signal<long>;
	template class signal<long, long>;
	template class signal<unsigned long>;
	template class signal<unsigned long, unsigned long>;
}
