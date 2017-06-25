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

class slot : public mzk::object
{
  public:
	void foo(int i, mzk::object *th)
	{
		std::cout << "slot: " << i << std::endl;
	}

  private:
};

class third : public mzk::object
{

};

int main()
{
	mzk::ptr<slot> sl = new slot;
	mzk::ptr<third> th = new third;
	mzk::signal<int> sig;


	auto connection = sig.connect(&slot::foo, sl, 123, th);


	sig(123);
	delete sl;
	sig(123);

	return 0;
}
