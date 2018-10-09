#ifndef _ENEMY_HG_
#define _ENEMY_HG_

class iEnemy
{
public:
	virtual ~iEnemy() {};
	virtual void kill(void) = 0;
};

#endif // !_ENEMY_HG_

