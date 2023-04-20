//==================================================
// object2D.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>

#include "manager.h"
#include "object2D.h"
#include "renderer.h"

//**************************************************
// 定数定義
//**************************************************
namespace
{
	// ポリゴンの幅
	const float POLYGON_WIDTH = 50.0f;
	// ポリゴンの幅
	const float POLYGON_HEIGHT = 50.0f;
}

// 形成する四角形の基準値
const D3DXVECTOR3 sVtx[4] =
{
	D3DXVECTOR3(-1.0f, -1.0f, 0.0f),
	D3DXVECTOR3(+1.0f, -1.0f, 0.0f),
	D3DXVECTOR3(-1.0f, +1.0f, 0.0f),
	D3DXVECTOR3(+1.0f, +1.0f, 0.0f),
};

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	// 頂点バッファへのポインタ
	m_pVtxBuff = nullptr;
	// 元の座標
	m_posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 向き
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 四角形の大きさ変更
	m_fSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 色
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//Rotの加算量
	m_MoveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CObject2D::~CObject2D()
{
	assert(m_pVtxBuff == nullptr);
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CObject2D::Init()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	// 初期化
	m_texture = CTexture::TEXTURE_NONE;

	// 頂点バッファの生成（動的確保）
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_posOrigin.x - m_fSize.x * 0.5f, m_posOrigin.y - m_fSize.y * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posOrigin.x + m_fSize.x * 0.5f, m_posOrigin.y - m_fSize.y * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posOrigin.x - m_fSize.x * 0.5f, m_posOrigin.y + m_fSize.y * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posOrigin.x + m_fSize.x * 0.5f, m_posOrigin.y + m_fSize.y * 0.5f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CObject2D::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	CObject::DeletedObj();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CObject2D::Update()
{
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// タイマー
	m_fTimer += 1.0f;

	// サインカーブ
	//float fSin = sinf(m_fTimer * 0.02f);

	D3DXVECTOR3 addPos[4];	// 足すposの位置
	D3DXMATRIX mtx;			// 計算用マトリックス

	//Rot計算
	m_Rot += m_MoveRot;

	// マトリックスの生成
	D3DXMatrixIdentity(&mtx);

	// ヨー、ピッチ、ロールを指定してマトリックスを作成
	//D3DXMatrixRotationYawPitchRoll(&mtx, 0.0f, 0.0f, ((D3DX_PI * 2.0f) / 100.0f) * m_fTimer);		// 回転
	D3DXMatrixRotationYawPitchRoll(&mtx, m_Rot.y, m_Rot.x, m_Rot.z);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標						↓頂点数
	for (int nCntVtx = 0; nCntVtx < 4; ++nCntVtx)
	{
		D3DXVec3TransformCoord(&addPos[nCntVtx], &sVtx[nCntVtx], &mtx);
		pVtx[nCntVtx].pos.x = m_posOrigin.x + addPos[nCntVtx].x * (m_fSize.x * 0.5f/* * fSin*/);	//<-サイズ変更
		pVtx[nCntVtx].pos.y = m_posOrigin.y + addPos[nCntVtx].y * (m_fSize.y * 0.5f/* * fSin*/);
		pVtx[nCntVtx].pos.z = m_posOrigin.z + addPos[nCntVtx].z * (m_fSize.z * 0.5f/* * fSin*/);
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CObject2D::Draw(DRAW_MODE /*drawMode*/)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();
	CTexture* pTexture = CManager::GetManager()->GetTexture();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブタイプ（頂点を結びつける順番を決めたりする）を指定する。
		0,		// 描画を開始する最初の頂点の番号。
		2);		// プリミティブ(ポリゴン)数

	// テクスチャの解除
	pDevice->SetTexture(0, nullptr);
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CObject2D *CObject2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nPriority)
{
	CObject2D *pObject2D;
	pObject2D = new CObject2D(nPriority);

	if (pObject2D != nullptr)
	{
		pObject2D->Init();
		pObject2D->SetPos(pos);
		pObject2D->SetSize(size);
	}
	else
	{
		assert(false);
	}

	return pObject2D;
}

//--------------------------------------------------
// 色の変更
//--------------------------------------------------
void CObject2D::SetCol(const D3DXCOLOR &col)
{
	m_col = col;

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	//if (m_pVtxBuff != nullptr)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
// テクスチャのサイズの変更
//--------------------------------------------------
void CObject2D::SetTextureSize(int nXNum, int nYNum)
{
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f * nXNum, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f * nYNum);
	pVtx[3].tex = D3DXVECTOR2(1.0f * nXNum, 1.0f * nYNum);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//--------------------------------------------------
// アニメーションの設定
//--------------------------------------------------
void CObject2D::AnimTexture(int nPattern, int nPatternMax)
{
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	float nDivisionRate = 1.0f / nPatternMax;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(nPattern * nDivisionRate, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((nPattern + 1) * nDivisionRate, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nPattern * nDivisionRate, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((nPattern + 1) * nDivisionRate, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
