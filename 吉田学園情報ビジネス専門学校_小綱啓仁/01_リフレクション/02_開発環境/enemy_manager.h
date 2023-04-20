//==================================================
// enemy.h
// Author: Arita Meigen
//==================================================
#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include"object2D.h"

#define ERASTIC_TIME	(600)	//伸縮するやつが出てくる時間

class CEnemy_Effect;

class CEnemyManager
{
public:
	CEnemyManager();
	~CEnemyManager();
	HRESULT Init();								//初期化
	void Update();								//更新
	static CEnemyManager* Create();				//生成
private:					
	int m_Time;
	int m_SpawnTime;
	int m_SizePtn;	//今出るパターン数
	CEnemy_Effect* m_pObject;
};
#endif // !_ENEMY_H_
