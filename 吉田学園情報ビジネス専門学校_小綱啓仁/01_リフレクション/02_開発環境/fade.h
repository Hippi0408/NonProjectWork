//==================================================
// fade.h
// Author: Buriya Kota
//==================================================
#ifndef _FADE_H_
#define _FADE_H_

//**************************************************
// インクルード
//**************************************************
#include "object2D.h"

//**************************************************
// クラス
//**************************************************
class CFade : CObject2D
{
private:
	explicit CFade(int nPriority = PRIORITY_FADE);

public:
	// フェードの状態
	enum FADE
	{
		FADE_NONE = 0,			//何もしてない
		FADE_IN,				//フェードイン
		FADE_OUT,				//フェードアウト
		FADE_MAX
	};

	~CFade();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw(DRAW_MODE drawMode) override;

	void SetFade(CManager::MODE modeNext);
	FADE GetFade() { return m_fade; }

	static CFade *GetInstance();

private:
	static CFade *m_pFade;
	// オブジェクト2Dの箱
	CObject2D *m_pObject2D;
	// 次の画面
	CManager::MODE m_modeNext;
	// フェードの状態
	FADE m_fade;
	// フェードの速度
	float m_fFadeSpeed;
	// α値の上限
	float m_fMaxAlpha;
};

#endif	// _FADE_H_