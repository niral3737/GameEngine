#ifndef _CUE_RACK_HG_
#define _CUE_RACK_HG_

#include "iEquipment.h"
#include <vector>

#include "iCueRackBuilder.h"
#include "iMediatorEquipment.h"

class cCueRack : public iEquipment,
	public iCueRackBuilder
{
public:
	cCueRack();
	virtual ~cCueRack();

	virtual void move();
	virtual void addCue(iEquipment* cue);
	std::vector<iEquipment*> cues;

	virtual iMeshObject* getMesh();
	virtual void setMesh(iMeshObject* mesh);
	virtual void setMediator(iMediatorEquipment* mediator);

	iMeshObject* mesh;
private:
	iMediatorEquipment* mediator;
};

#endif // !_CUE_RACK_HG_



