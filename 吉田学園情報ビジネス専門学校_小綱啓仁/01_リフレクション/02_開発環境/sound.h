//**************************************************
//
// sound.h
// Author  : katsuki mizuki
//
//**************************************************
#ifndef _SOUND_H_	//このマクロ定義がされてなかったら
#define _SOUND_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>
#include <xaudio2.h>

//==================================================
// 定義
//==================================================
class CSound
{
	/* ↓定義↓ */
public:
	enum ELabel
	{
		LABEL_NONE = -1,
		LABEL_BGM_TITLE,		// タイトル
		LABEL_BGM_GAME,			// ゲーム
		LABEL_BGM_RANKING,		// ランキング
		LABEL_SE_ENTER,			// 決定音
		LABEL_SE_SELECT,		// 選択
		LABEL_SE_CURVE,			// 曲がるとき
		LABEL_SE_GET_ITEM,		// アイテム取得
		LABEL_SE_GOAL,			// ゴール
		LABEL_SE_DEATH,			// PL死亡
		LABEL_SE_WALL_HIT,		// 壁Het
		LABEL_MAX,
	};

	struct SParam
	{
		char *pFileName;	// ファイル名
		int loop;			// ループ
	};

	static const SParam PARAM[];	// パラメータの情報

	/* ↓メンバ関数↓ */
public:
	CSound();	// デフォルトコンストラクタ
	~CSound();	// デストラクタ

public:
	HRESULT Init(HWND hWnd);
	void Uninit();
	HRESULT Play(ELabel sound);
	void Stop(ELabel label);
	void Stop();

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);	// チャンクのチェック
	HRESULT LoadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	// チャンクデータの読み込み

	/* ↓メンバ変数↓ */
private: 
	IXAudio2* m_pXAudio2;							// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;		// マスターボイス
	IXAudio2SourceVoice* m_pSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE* m_pDataAudio[LABEL_MAX];					// オーディオデータ
	DWORD m_sizeAudio[LABEL_MAX];					// オーディオデータサイズ
};

#endif // !_SOUND_H_
