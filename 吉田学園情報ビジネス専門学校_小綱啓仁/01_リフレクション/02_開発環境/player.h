//==================================================
// player.h
// Author: Buriya Kota
//==================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//**************************************************
// インクルード
//**************************************************
#include "object2D.h"

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************

//**************************************************
// 定数定義
//**************************************************

//**************************************************
// 構造体定義
//**************************************************

//**************************************************
// クラス
//**************************************************
class CPlayer : public CObject2D
{
public:
	enum PLAYER_STATE
	{
		PLAYER_STATE_NONE = 0,
		PLAYER_STATE_UP,
		PLAYER_STATE_DOWN,
		PLAYER_STATE_RIGHT,
		PLAYER_STATE_LEFT,
		PLAYER_STATE_MAX
	};

	static const int INVINCIBLE_ITEM = 300;

	explicit CPlayer(int nPriority = PRIORITY_PLAYER);
	~CPlayer();

	HRESULT Init() override;
	void Update() override;

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	void SetInvincible(bool bInvincible) { m_bInvincible = bInvincible; }
	bool GetInvincible() { return m_bInvincible; }

private:
	void StartMove_();
	void Reflection_();
	void Control_();
	void ReflectionEffect_0(const D3DXVECTOR3& pos, const D3DXVECTOR3& vec);
	void ReflectionEffect_1(const D3DXVECTOR3& pos, const D3DXVECTOR3& vec);
	void ReflectionEffect_2(const D3DXVECTOR3& pos, const D3DXVECTOR3& vec);
	

private:
	// 移動量
	D3DXVECTOR3 m_move;
	// バウンド回数
	int m_nReflectionCount;
	// 今どの壁に当たったか
	PLAYER_STATE m_playerState;
	//無敵状態の有無
	bool m_bInvincible;
	//無敵時間
	int m_nInvincibleTime;
};


#endif	// _PLAYER_H_