#pragma once

#include "Vector.h"
#include "CGame.h"

class CWorldPlayer
{
	Vector2f m_vfPosition;

	float m_fRotation;
	float m_fDrawRotation;

	int m_iTextureID;

	float m_fMoveSpeed;
	float m_fSprintMultiplier;
	float m_fSprintTime;
	float m_fMaxSprintTime;

	bool m_bWalkingForward;
	bool m_bWalkingBack;
	bool m_bLeftStrafe;
	bool m_bRightStrafe;
	bool m_bSprint;
	float m_fLastMoveTime;
	float m_fLastSprintTime;

public:
	void LookAt(Vector2i MousePosition, CGame* pGame);
	void Initialise(CGame* pGame);

	void Draw(CGame* pGame, int iDrawSize);

	void SetPosition(Vector2f vPos);
	void SetPosition(Vector2i vPos);
	Vector2f GetPosition();
	Vector2i GetPositionI();

	/* Movement */
	void SetWalkForward(CGame* pGame, bool bWalking);
	void SetWalkBack(CGame* pGame, bool bWalking);
	void SetLeftStrafe(CGame* pGame, bool bWalking);
	void SetRightStrafe(CGame* pGame, bool bWalking);
	void SetSprint(CGame* pGame, bool bSprint);
	void Update(CGame* pGame);

	bool CanSprint();
};