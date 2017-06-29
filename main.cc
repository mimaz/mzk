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

#include <thread>
#include <initializer_list>

#include <mzk/signal.h>
#include <mzk/property.h>
#include <mzk/timer.h>
#include <mzk/state-machine.h>
#include <mzk/logger.h>
#include <mzk/animator.h>

class janusz : public mzk::shared_object, public mzk::slot_object
{
  public:
	janusz()
	{
		mzk::logi("constructor");
	}

	~janusz()
	{
		mzk::logi("destructor");
	}

	void foo(int i, mzk::shared_object *th)
	{
		mzk::logi("janusz");
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
		mzk::logi(o, " -> ", n, mzk::endl);
	}, mzk::arg1, mzk::arg2);


	prop = 123;
	prop = 543;


	sig.connect(&mzk::property<int>::set_value, &prop, mzk::arg1);

	sig(999);



	mzk::timer tim;
	tim.set_ticks(3);
	tim.set_period(1000);
	tim.set_delay(200);

	tim.sig_triggered.connect([]() {
		mzk::loge("hello, world!");
	});

	//tim.sig_stopped.connect(&mzk::timer::stop_loop);

	tim.start();



	enum stateid 
	{
		MENU,
		GAME,
		ABOUT,
		LAST
	};

	mzk::state_machine<stateid, LAST> machine;

	machine.prop_state.sig_changed.connect([](int id, int old) {
		mzk::logi("id: ", id);
	}, mzk::arg1, mzk::arg2);

	machine.prop_state = GAME;

	machine.transit(GAME, ABOUT);


	mzk::animator<float> anim;
	anim.sig_stopped.connect(&mzk::timer::stop_loop);

	anim.set_duration(5000);
	anim.set_start_value(100);
	anim.set_end_value(150);

	anim.start();

	anim.sig_value_changed.connect([](float v) { mzk::logi("v: ", v); }, mzk::arg1);




	mzk::timer::start_loop();


	return 0;
}

