#ifndef _ENTITY_HG_
#define _ENTITY_HG_

#include <string>

class iEntity
{
public:
	virtual ~iEntity() {};
	virtual std::string getName() = 0;
};


#endif // !ENTITY_HG_
