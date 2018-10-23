#include "cRandomHelper.h"
#include <cstdlib>
#include <iostream>



cRandomHelper::cRandomHelper()
{}


cRandomHelper::~cRandomHelper()
{}

unsigned int cRandomHelper::generateRandomIntInRange(unsigned int start, unsigned int end)
{
	return start + (std::rand() % static_cast<int>(end - start + 1));
}
