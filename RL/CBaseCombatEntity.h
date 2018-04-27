#ifndef __CBASECOMBATENTITY_H__
#define __CBASECOMBATENTITY_H__

#include "CBaseEntity.h"

class CBaseCombatEntity : public CBaseEntity {
	int m_iHealth;
	int m_iMaxHealth;

public:

	void Hurt(int h) {
		m_iHealth -= h;
	}

	void Heal(int h, bool allowOverheal = false) {
		m_iHealth += h;
		if (!allowOverheal && m_iHealth > m_iMaxHealth) {
			m_iHealth = m_iMaxHealth;
		}
	}
};
#endif