#include "cVAOMeshUtils.h"



cVAOMeshUtils::cVAOMeshUtils()
{}


cVAOMeshUtils::~cVAOMeshUtils()
{}

cVAOMeshUtils* cVAOMeshUtils::pVAOMeshUtils = 0;

cVAOMeshUtils* cVAOMeshUtils::getInstance(void)
{
	if (pVAOMeshUtils == 0)
	{
		pVAOMeshUtils = new cVAOMeshUtils();
	}
	return pVAOMeshUtils;
}

