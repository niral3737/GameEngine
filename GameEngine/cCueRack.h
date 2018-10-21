#ifndef _CUE_RACK_HG_
#define _CUE_RACK_HG_

#include "iEquipment.h"
#include <vector>

class cCueRack : public iEquipment
{
public:
	cCueRack();
	virtual ~cCueRack();

	virtual void move();
	virtual void addCue(iEquipment* cue);
	std::vector<iEquipment*> cues;
};

#endif // !_CUE_RACK_HG_



