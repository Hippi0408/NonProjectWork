//==================================================
// enemy.h
// Author: Arita Meigen
//==================================================
#ifndef _ENEMY_ADVENT_EFFECT_H_
#define _ENEMY_ADVENT_EFFECT_H_

#include"object2D.h"
#include"enemy.h"


class CEnemy;

class CEnemy_Effect :public CObject2D
{
public:
	explicit CEnemy_Effect(int nPriority = PRIORITY_OBJECT);
	~CEnemy_Effect();
	HRESULT Init()override;														//èâä˙âª
	void Update()override;														//çXêV
	static CEnemy_Effect* Create(D3DXVECTOR3 pos,int Life,CEnemy::SIZE_PATTERN sizptn);	//ê∂ê¨
private:
	int m_LifeTimer;
	D3DXCOLOR m_col;
	float m_fAlphagain;
	int m_moveptn;
	int m_sizeptn;
};
#endif // !_ENEMY_H_
