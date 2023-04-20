//**************************************************
// model_data.h
// Author: Buriya Kota
//**************************************************
#ifndef _MODEL_DATA_H_
#define _MODEL_DATA_H_

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>

//**************************************************
// マクロ定義
//**************************************************
#define MAX_TEXTURE				(54)

//==================================================
// 定義
//==================================================
class CModelData
{
public: /* 定義 */
	enum MODEL_TYPE
	{
		MODEL_PLAYER_BODY = 0,		// 体
		MODEL_PLAYER_HEAD,			// 頭
		MODEL_PLAYER_RIGHT_ARM,		// 右腕
		MODEL_PLAYER_LEFT_ARM,		// 左腕
		MODEL_PLAYER_BOARD,			// 板
		MODEL_PLAYER_LFT_THIGH,		// 左太もも
		MODEL_PLAYER_RIGHT_THIGH,	// 右太もも
		MODEL_PLAYER_LEFT_LEG,		// 左足
		MODEL_PLAYER_RIGHT_LEG,		// 右足
		MODEL_BALL,					// 球
		MODEL_GIMMICK,				// ギミック
		MODEL_GOAL,					// ゴール
		MODEL_CUBE_PLAYER,			// キューブプレイヤー
		MODEL_TREE,					// 木
		MODEL_COIN,					// コイン
		MODEL_CLEENNESS_OBJECT,		// 透明オブジェクト
		MODEL_CLEENNESS_OBJECT_VERTICAL,
		MODEL_CLEENNESS_OBJECT_SHORT,				// 透明オブジェクト
		MODEL_CLEENNESS_OBJECT_VERTICAL_SHORT,
		MODEL_MAX,					// 最大数
		MODEL_NONE,					// 使用しない
	};

	struct MODEL
	{
		// メッシュ(頂点の集まり)情報へのポインタ
		LPD3DXMESH mesh;
		// マテリアル情報へのポインタ	←　1のXファイルに複数のマテリアルが入っている
		LPD3DXBUFFER buffMat;
		// マテリアル情報の数
		DWORD numMat;
		// テクスチャの列挙型
		LPDIRECT3DTEXTURE9 texture[MAX_TEXTURE];
		// 最大値
		D3DXVECTOR3 vtxMax;
		// 最小値
		D3DXVECTOR3 vtxMin;
		// 大きさ
		D3DXVECTOR3 size;
	};

	static const char* s_FileName[];	// ファイルパス

public:
	CModelData();		// デフォルトコンストラクタ
	~CModelData();		// デストラクタ

	void LoadAll();								// 全ての読み込み
	void Load(MODEL_TYPE inModel);				// 指定の読み込み
	void ReleaseAll();							// 全ての破棄
	void Release(MODEL_TYPE inModel);			// 指定の破棄
	MODEL GetModel(MODEL_TYPE inModel);	// 情報の取得

private:
	MODEL m_model[MODEL_MAX];
};

#endif // !_MODEL_DATA_H_
