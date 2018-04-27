#ifndef __CWORLDITEM_H__
#define __CWORLDITEM_H__

#include "Vector.h"
#include "CWorldEntity.h"

enum EWorldItemSpawnLocation {
	LOC_DONT_SPAWN,
	LOC_FOREST,
};

enum EWorldItemFlags {
	F_Pickup = 1 << 0,		/* Item can be picked up */
	F_Passable = 1 << 2,	/* Item can be walked over */
	F_Destructable = 1 << 1,/* Item can be destroyed */
	F_T = 1 << 3,
};

class CWorldItem : CWorldEntity {
	int m_iFlags;
	int m_iHP;

public:
	CWorldItem();
	void Damage(int dam);
	bool CanPickup();
	bool IsPassable();
	bool IsDestructable();
};

#endif