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

#include <mzk/object.h>
#include <mzk/signal.h>

class test : public mzk::object
{
 public:
	test()
	{
		std::cout << "constructor" << std::endl;
	}

	~test()
	{
		std::cout << "destructor" << std::endl;
	}

	void foo(int i)
	{
		std::cout << "i: " << i << std::endl;
	}
};

int main()
{
	mzk::ptr<mzk::object> ptr = new test;

	ptr.cast<test>()->foo(543);

	std::cout << "ptr: " << ptr << std::endl;

	delete ptr;

	std::cout << "ptr: " << ptr << std::endl;


	mzk::signal<int> sig;
	/*

	auto slot = sig.bind([](int i) {
		std::cout << "hello!" << i << std::endl;
	}, std::placeholders::_1);

	sig(4);
	slot->disconnect();
	sig(5);
	sig(6);



	mzk::property<int> prop;

	slot = prop.bind([](int value, int old) {
		std::cout << old << " -> " << value << std::endl;
	}, std::placeholders::_1, std::placeholders::_2);

	prop.set_repeater([](int i) { return i * 5; });

	prop = 5;
	prop = 6;
	prop = 6;

	prop.clear_repeater();


	prop = 12;
	*/

	return 0;
}
