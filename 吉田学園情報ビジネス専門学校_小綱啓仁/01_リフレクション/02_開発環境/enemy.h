//==================================================
// enemy.h
// Author: Arita Meigen
//==================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include"object2D.h"

class CEnemy :public CObject2D
{
public:
	enum MOVEPATTERN
	{//移動量
		MOVE_UP_STRAIGHT = 0,	//上に直進
		MOVE_DOWN_STRAIGHT,		//下に直進
		MOVE_LEFT_STRAIGHT,		//左に直進
		MOVE_RIGHT_STRAIGHT,	//右に直進
		MOVE_UPLEFT_STRAIGHT,	//左上に直進
		MOVE_DOWNLEFT_STRAIGHT,	//左下に直進
		MOVE_UPRIGHT_STRAIGHT,	//右上に直進
		MOVE_DOWNRIGHT_STRAIGHT,//右下に直進
		MOVE_MAX
	};
	enum SIZE_PATTERN
	{//サイズパターン
		SIZEPTN_NORMAL = 0,		//正方形
		SIZEPTN_ELASTICITY,		//伸縮
		SIZEPTN_MAX
	};
	enum ENEMY_TYPE
	{//敵のパターン
		ENEMY_NORMAL,		//左右上下の通常移動
		ENEMY_ELASTICITY,	//伸縮
		ENEMY_OBLIQUE,		//斜め移動
		ENEMY_ELA_OBL,		//斜め移動と伸縮同時
		ENEMY_INVINCIBLE	//無敵
	};

	explicit CEnemy(int nPriority = PRIORITY_OBJECT);
	~CEnemy();
	HRESULT Init()override;										// 初期化
	void Update()override;										// 更新
	static void Create(D3DXVECTOR3 pos, MOVEPATTERN moveptn, SIZE_PATTERN sizeptn);	// 生成
	bool Collision();											// 衝突
	void SetMovePattern(MOVEPATTERN moveptn);					// 行動パターンの設定
	void SetSizePattern(SIZE_PATTERN sizeptn);					// サイズパターンの設定
	void ScreenOut();											// 画面外の処理
	void SetEnemyType();
	void SelectTexture();										//テクスチャの選択
private:
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_movesize;		//サイズの移動量？
	MOVEPATTERN m_moveptn;		//移動パターン
	SIZE_PATTERN m_movesizeptn;		//移動パターン

	ENEMY_TYPE	m_EnemyType;		//敵のパターン
	bool m_bGulez;//グレイズ

};
#endif // !_ENEMY_H_
