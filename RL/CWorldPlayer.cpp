#include "stdafx.h"
#include "CWorldPlayer.h"
#include "Math.h"

#define PLAYER_TEXTURE ".\\Graphics\\Player.tga"
#define MIN_PLAYER_WALK_UPDATE_TIME 0.05f

/* Default movement params*/
#define PLAYER_MIN_SPRINT_PERCENTAGE 0.3f 
#define DEFAULT_PLAYER_SPRINT_TIME 9.0f
#define DEFAULT_PLAYER_MOVE_SPEED 10.0f
#define DEFAULT_PLAYER_SPRINT_MULT 1.4f
#define DEFAULT_PLAYER_BACKPEDDLE_SLOWDOWN 0.9f 

void CWorldPlayer::LookAt(Vector2i MousePosition, CGame* pGame)
{
	/* Player is centred at all times */
	Vector2i Delta = pGame->m_WindowManager.GetScreenCentre() - MousePosition;

	m_fDrawRotation = ((180.0f / 3.1415926f) * atan2pi((float)Delta.Y, (float)Delta.X)) - 90.0f;
	m_fRotation = atan2pi((float)Delta.X, (float)Delta.Y) + 3.14159f;
}
void CWorldPlayer::Initialise(CGame* pGame)
{
	/* Set everything to 0 */
	memset(this, 0, sizeof(CWorldPlayer));

	m_fMoveSpeed = DEFAULT_PLAYER_MOVE_SPEED;
	m_fSprintMultiplier = DEFAULT_PLAYER_SPRINT_MULT;
	m_iTextureID = pGame->m_TextureManager.GetID(PLAYER_TEXTURE);
	m_fSprintTime = DEFAULT_PLAYER_SPRINT_TIME;
	m_fMaxSprintTime = m_fSprintTime;
}
void CWorldPlayer::Draw(CGame* pGame, int iDrawSize)
{
	pGame->m_Drawing.DrawSpriteCentred(&pGame->m_WindowManager, (int)(m_vfPosition.X*iDrawSize), (int)(m_vfPosition.Y*iDrawSize),
		iDrawSize, iDrawSize, 1.0f, m_fDrawRotation, pGame->m_TextureManager.GetTexture(m_iTextureID));
}
void CWorldPlayer::SetPosition(Vector2f vPos)
{
	m_vfPosition = vPos;
}
void CWorldPlayer::SetPosition(Vector2i vPos)
{
	m_vfPosition.X = (float)vPos.X;
	m_vfPosition.Y = (float)vPos.Y;
}
Vector2f CWorldPlayer::GetPosition()
{
	return m_vfPosition;
}
Vector2i CWorldPlayer::GetPositionI()
{
	return Vector2i((int)m_vfPosition.X, (int)m_vfPosition.Y);
}
void CWorldPlayer::SetWalkForward(CGame* pGame, bool bWalking)
{
	if (bWalking && !m_bWalkingForward)
	{
		m_fLastMoveTime = pGame->m_Time.Time();
		m_bWalkingForward = true;
		return;
	}

	if (!bWalking && m_bWalkingForward)
	{
		m_bWalkingForward = false;
		return;
	}
}
void CWorldPlayer::SetLeftStrafe(CGame* pGame, bool bWalking)
{
	if (bWalking && !m_bLeftStrafe)
	{
		m_fLastMoveTime = pGame->m_Time.Time();
		m_bLeftStrafe = true;
		return;
	}

	if (!bWalking && m_bLeftStrafe)
	{
		m_bLeftStrafe = false;
		return;
	}
}
void CWorldPlayer::SetRightStrafe(CGame* pGame, bool bWalking)
{
	if (bWalking && !m_bRightStrafe)
	{
		m_fLastMoveTime = pGame->m_Time.Time();
		m_bRightStrafe = true;
		return;
	}

	if (!bWalking && m_bRightStrafe)
	{
		m_bRightStrafe = false;
		return;
	}
}
void CWorldPlayer::SetWalkBack(CGame* pGame, bool bWalking)
{
	if (bWalking && !m_bWalkingBack)
	{
		m_fLastMoveTime = pGame->m_Time.Time();
		m_bWalkingBack = true;
		return;
	}

	if (!bWalking && m_bWalkingBack)
	{
		m_bWalkingBack = false;
		return;
	}
}
bool CWorldPlayer::CanSprint()
{
	return (m_fSprintTime >= m_fMaxSprintTime * PLAYER_MIN_SPRINT_PERCENTAGE);
}
void CWorldPlayer::SetSprint(CGame* pGame, bool bSprint)
{
	if (!CanSprint())
		return;
	m_fLastSprintTime = pGame->m_Time.Time();
	m_bSprint = bSprint;
}
void CWorldPlayer::Update(CGame* pGame)
{
	if (!m_bWalkingForward && !m_bWalkingBack && !m_bLeftStrafe && !m_bRightStrafe)
		return;

	if (m_bWalkingForward && m_bWalkingBack)
	{
		m_bWalkingForward = m_bWalkingBack = false;
	}
	if (m_bLeftStrafe && m_bRightStrafe)
	{
		m_bRightStrafe = m_bLeftStrafe = false;
	}

	Vector2f MoveVector(0, 0);

	if (m_bWalkingForward)
	{
		MoveVector.X += sin(m_fRotation);
		MoveVector.Y += cos(m_fRotation);
	}
	if (m_bWalkingBack)
	{
		MoveVector.X -= sin(m_fRotation);
		MoveVector.Y -= cos(m_fRotation);
	}
	if (m_bLeftStrafe)
	{
		MoveVector.X -= sin(m_fRotation - 1.570795f);
		MoveVector.Y -= cos(m_fRotation - 1.570795f);
	}
	if (m_bRightStrafe)
	{
		MoveVector.X -= sin(m_fRotation + 1.570795f);
		MoveVector.Y -= cos(m_fRotation + 1.570795f);
	}

	float curtime = pGame->m_Time.Time();
	float deltatime = curtime - m_fLastMoveTime;
	/* Min time - tries to minimise rounding errors */
	if (deltatime < MIN_PLAYER_WALK_UPDATE_TIME)
		return;

	/* Regain sprint time */
	if (!m_bSprint)
		m_fSprintTime += deltatime;

	if (MoveVector.X != 0.0f || MoveVector.Y != 0.0f)
	{
		/* make unit .. */
		float Len = MoveVector.Length();

		MoveVector.X /= Len;
		MoveVector.Y /= Len;

		m_fLastMoveTime = curtime;

		/* Distance = speed*time */
		if (m_bWalkingBack)
			MoveVector = MoveVector * deltatime * m_fMoveSpeed * DEFAULT_PLAYER_BACKPEDDLE_SLOWDOWN;
		else
			MoveVector = MoveVector * deltatime * m_fMoveSpeed;

		if (m_bSprint) {
			MoveVector *= m_fSprintMultiplier;

			float deltasprint = curtime - m_fLastSprintTime;
			m_fLastSprintTime = curtime;
			m_fSprintTime -= deltasprint;
			if (m_fSprintTime <= 0) {
				m_fSprintTime = 0.0f;
				m_bSprint = false;
			}
		}

		m_vfPosition.X += MoveVector.X;
		m_vfPosition.Y += MoveVector.Y;
	}
	else {
		// Not moving, avoid wasting sprint time
		if (m_bSprint) {
			m_fSprintTime += deltatime;
			m_fLastSprintTime = curtime;
		}
	}
}