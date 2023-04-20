//==================================================
// pause.h
// Author: Buriya Kota
//==================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//**************************************************
// インクルード
//**************************************************
#include "object.h"

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************
class CObject2D;

//**************************************************
// クラス
//**************************************************
class CPause : public CObject
{
public:
	enum SELECT_MODE
	{
		SELECT_MODE_RESTART = 0,
		SELECT_MODE_TAITLE,
		SELECT_MODE_CLOSE,
		SELECT_MODE_MAX
	};

	explicit CPause(int nPriority = PRIORITY_UI);
	~CPause() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw(DRAW_MODE /*drawMode*/) override {}

	bool GetIsPause() { return m_IsPause; }

	static CPause *Create();

private:
	// オブジェクト2Dの箱
	CObject2D *m_pObject2D[2];
	CObject2D *m_pMenu[3];
	// 位置
	D3DXVECTOR3 m_pos;
	// 大きさ
	D3DXVECTOR3 m_size;
	// 選択しているモードのカウント
	int m_nSelect;
	// 今がポーズ中かどうか
	bool m_IsPause;
};

#endif	// _PAUSE_H_