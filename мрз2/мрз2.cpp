
#include "pch.h"
#include <iostream>
#include "Controller.h"

int main()
{
	auto *o = new Controller(6, 3, 5, 3);
	o->run();
	return 0;
}
