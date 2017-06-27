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

#include <mzk/signal.h>
#include <mzk/property.h>
#include <mzk/threaded.h>

class janusz : public mzk::shared, public mzk::signaled
{
  public:
	janusz()
	{
		std::cout << "constructor" << std::endl;
	}

	~janusz()
	{
		std::cout << "destructor" << std::endl;
	}

	void foo(int i, mzk::shared *th)
	{
		std::cout << "janusz: " << i << std::endl;
	}

  private:
};

class elephant : public mzk::shared, public mzk::signaled
{

};

int main()
{
	mzk::ptr<janusz> sl = new janusz;
	mzk::ptr<elephant> th = new elephant;
	mzk::signal<int> sig;


	auto scon = sig.connect_slot(&janusz::foo, sl.raw(), mzk::arg1, th.raw());


	sig(925);
	sig(653);


	mzk::property<int> prop;

	int addition = 10000;
	prop.set_repeater(mzk::make_repeater<int>([addition](int v) { return v + addition; }));
	prop.set_repeater([addition](int v) { return addition + v * 10; });

	auto pcon = prop.connect_lambda([](int n, int o) {
		std::cout << o << " -> " << n << std::endl;
	});


	prop = 123;
	prop = 543;


	sig.connect_slot(&mzk::property<int>::set_value, &prop, mzk::arg1);

	sig(999);



	mzk::threaded thr = mzk::threaded();


	return 0;
}
