//**************************************************
//
// Hackathon ( pause.h )
// Author  : katsuki mizuki
//
//**************************************************
#ifndef _PAUSE_H_	//このマクロ定義がされてなかったら
#define _PAUSE_H_	//２重インクルード防止のマクロ定義

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitPause(void);

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitPause(void);

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdatePause(void);

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawPause(void);

//--------------------------------------------------
// 設定
//--------------------------------------------------
void SetPause(void);

//--------------------------------------------------
// リセット
//--------------------------------------------------
void ResetPause(void);

#endif // !_PAUSE_H_
