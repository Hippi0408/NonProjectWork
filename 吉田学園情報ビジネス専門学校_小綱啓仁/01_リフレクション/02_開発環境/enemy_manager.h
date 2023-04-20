//==================================================
// enemy.h
// Author: Arita Meigen
//==================================================
#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include"object2D.h"

#define ERASTIC_TIME	(600)	//�L�k�������o�Ă��鎞��

class CEnemy_Effect;

class CEnemyManager
{
public:
	CEnemyManager();
	~CEnemyManager();
	HRESULT Init();								//������
	void Update();								//�X�V
	static CEnemyManager* Create();				//����
private:					
	int m_Time;
	int m_SpawnTime;
	int m_SizePtn;	//���o��p�^�[����
	CEnemy_Effect* m_pObject;
};
#endif // !_ENEMY_H_
