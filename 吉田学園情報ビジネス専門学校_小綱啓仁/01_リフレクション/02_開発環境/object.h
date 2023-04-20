//==================================================
// object.h
// Author: Buriya Kota
//==================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

//**************************************************
// インクルード
//**************************************************
#include <functional>

#include "renderer.h"

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************

//**************************************************
// クラス
//**************************************************
class CObject
{
public:
	// オブジェクトの最大数
	static const int MAX_OBjECT = 384;
	// プライオリティの最大数
	static const int MAX_PRIO = 7;

	enum TYPE_3D
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_BULLET,
		TYPE_BOSS_BULLET,
		TYPE_HOMING_LASER,
		TYPE_EXPLOSION,
		TYPE_EFFECT,
		TYPE_BILL,
		TYPE_ROAD,
		TYPE_SCORE,
		TYPE_NUMBER,
		TYPE_MODE,
		TYPE_ITEM,
		TYPE_COIN,
		TYPE_GOAL,
		TYPE_OBSTACLE,
		TYPE_MAX
	};

	enum PRIORITY
	{
		PRIORITY_BG = 0,
		PRIORITY_EFFECT0,
		PRIORITY_PLAYER,
		PRIORITY_OBJECT,
		PRIORITY_EFFECT,
		PRIORITY_UI,
		PRIORITY_FADE,
		PRIORITY_MAX
	};

	enum DRAW_MODE
	{
		DRAW_MODE_SHADOW = 0,
		DRAW_MODE_OBJECT,
		DRAW_MODE_MAX
	};

public:
	explicit CObject(int nPriority = PRIORITY_OBJECT);
	virtual ~CObject();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw(DRAW_MODE drawMode) = 0;

	// すべての破棄、更新、描画
	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	static void ReleaseWithoutMode();

	void SetType(TYPE_3D type) { m_type = type; }
	TYPE_3D GetType() { return m_type; }

	void DeletedObj();
	bool IsDeleted() { return m_bDeleted; }

	static CObject *GetTop(int nPrio) { return m_pTop[nPrio]; }
	CObject *GetNext() { return m_pNext; }

protected:
	void Release();

private:
	// 先頭のオブジェクトのポインタ
	static CObject *m_pTop[MAX_PRIO];
	// 現在（一番後ろ）のオブジェクトへのポインタ
	static CObject *m_pCurrent[MAX_PRIO];
	// 前のオブジェクトへのポインタ
	CObject *m_pPrev;
	// 次のオブジェクトへのポインタ
	CObject *m_pNext;

	// タイプ
	TYPE_3D m_type;
	// 総数
	static int m_nNumAll;
	// 番号
	int m_Index;
	// プライオリティ番号
	int m_nPriority;
	// 削除されたかどうか
	bool m_bDeleted;
};

// 球の当たり判定
bool IsCollisionSphere(D3DXVECTOR3 targetPos, D3DXVECTOR3 pos, D3DXVECTOR3 targetSize, D3DXVECTOR3 size);
bool IsCollision(D3DXVECTOR3 targetPos, D3DXVECTOR3 pos, D3DXVECTOR3 targetSize, D3DXVECTOR3 size);

//円の当たり判定
bool IsCollisionCircle(D3DXVECTOR3 pos1, float fRadius1, D3DXVECTOR3 pos2, float fRadius2);

#endif	// _OBJECT_H_