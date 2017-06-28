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
#include <thread>
#include <initializer_list>

#include <mzk/signal.h>
#include <mzk/property.h>
#include <mzk/timer.h>
#include <mzk/state-machine.h>

class janusz : public mzk::shared_object, public mzk::slot_object
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

	void foo(int i, mzk::shared_object *th)
	{
		std::cout << "janusz: " << i << std::endl;
	}

  private:
};

class elephant : public mzk::shared_object, public mzk::slot_object
{

};


int main()
{
	mzk::ptr<janusz> sl = new janusz;
	mzk::ptr<elephant> th = new elephant;
	mzk::signal<int> sig;


	auto scon = sig.connect(&janusz::foo, sl.raw(), mzk::arg1, th.raw());


	sig(925);
	sig(653);


	mzk::property<int> prop;

	int addition = 10000;
	prop.set_repeater(mzk::make_repeater<int>([addition](int v) { return v + addition; }));
	prop.set_repeater([addition](int v) { return addition + v * 10; });

	auto pcon = prop.sig_changed.connect([](int n, int o) {
		std::cout << o << " -> " << n << std::endl;
	}, mzk::arg1, mzk::arg2);


	prop = 123;
	prop = 543;


	sig.connect(&mzk::property<int>::set_value, &prop, mzk::arg1);

	sig(999);



	mzk::timer tim;
	tim.prop_ticks = 5;
	tim.prop_period = 100;
	tim.prop_delay = 200;

	tim.sig_triggered.connect([]() {
		std::cout << "hello, world!" << std::endl;
	});

	tim.sig_stopped.connect(&mzk::timer::stop_loop);

	tim.prop_running = true;



	enum stateid 
	{
		MENU,
		GAME,
		ABOUT,
		LAST
	};

	mzk::state_machine<stateid, LAST> machine;

	machine.prop_state.sig_changed.connect([](int id, int old) {
		std::cout << "id: " << id << std::endl;
	}, mzk::arg1, mzk::arg2);

	machine.prop_state = GAME;

	machine.transit(GAME, ABOUT);
	mzk::timer::start_loop();

	return 0;
}

