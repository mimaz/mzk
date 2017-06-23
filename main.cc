#include <iostream>

#include "object.h"
#include "signal.h"
#include "property.h"

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

	ptr.as<test>()->foo(543);

	std::cout << "ptr: " << ptr << std::endl;

	delete ptr;

	std::cout << "ptr: " << ptr << std::endl;



	mzk::signal<int> sig;

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

	return 0;
}
