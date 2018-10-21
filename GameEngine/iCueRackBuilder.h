#ifndef _CUE_RACK_BUILDER
#define _CUE_RACK_BUILDER

#include "iEquipment.h"

class iCueRackBuilder
{
public:
	virtual ~iCueRackBuilder() {};

	virtual void addCue(iEquipment* cue) = 0;
};

#endif // !_CUE_RACK_BUILDER

