//==================================================
// polygon.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>

#include "manager.h"
#include "objectX.h"
#include "object3D.h"

//**************************************************
// 静的メンバ変数
//**************************************************
const float CObject3D::BILLBORAD_WIDTH = 10.0f;
const float CObject3D::BILLBOARD_HEIGHT = 10.0f;

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CObject3D::CObject3D(int nPriority /* =3 */) : CObject(nPriority)
{
	// 頂点バッファへのポインタ
	m_pVtxBuff = nullptr;
	// 位置
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 回転
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 頂点カラー
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// ビルボードかどうか
	m_bBill = false;
	// 親子かどうか
	m_bFamily = false;
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CObject3D::~CObject3D()
{
	assert(m_pVtxBuff == nullptr);
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CObject3D::Init()
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	// 位置
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 回転
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 頂点カラー
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ワールドではなくローカル
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 各頂点の法専の設定(*ベクトルの大きさは1にする必要がある)
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャの座標設定
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
void CObject3D::Uninit()
{
	if (m_pVtxBuff != nullptr)
	{// 頂点バッファの破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	CObject::DeletedObj();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CObject3D::Update()
{
	//頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, m_size.z);			// ワールドではなくローカル
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CObject3D::Draw(DRAW_MODE /*drawMode*/)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();
	CTexture* pTexture = CManager::GetManager()->GetTexture();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;													// 計算用マトリックス
	
	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_bBill)
	{// ビルボードの使用
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// カメラの逆行列を設定
		m_mtxWorld._11 = mtxView._11;
		m_mtxWorld._12 = mtxView._21;
		m_mtxWorld._13 = mtxView._31;
		m_mtxWorld._21 = mtxView._12;
		m_mtxWorld._22 = mtxView._22;
		m_mtxWorld._23 = mtxView._32;
		m_mtxWorld._31 = mtxView._13;
		m_mtxWorld._32 = mtxView._23;
		m_mtxWorld._33 = mtxView._33;
	}
	else
	{
		// 向きを反映							↓rotの情報を使って回転行列を作る
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);		// 行列掛け算関数		第二引数 * 第三引数 を　第一引数に格納
	}

	// 位置を反映								↓posの情報を使って移動行列を作る
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);		// 行列掛け算関数		第二引数 * 第三引数 を　第一引数に格納

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//テクスチャの解除
	pDevice->SetTexture(0, nullptr);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//--------------------------------------------------
// 色の設定
//--------------------------------------------------
void CObject3D::SetCol(const D3DXCOLOR & col)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

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
// 生成
//--------------------------------------------------
CObject3D *CObject3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CObject3D *pObject3D;
	pObject3D = new CObject3D;

	if (pObject3D != nullptr)
	{
		pObject3D->Init();
		pObject3D->SetPos(pos);
		pObject3D->SetSize(size);
	}
	else
	{
		assert(false);
	}

	return pObject3D;
}

//--------------------------------------------------
// アニメーションの設定
//--------------------------------------------------
void CObject3D::AnimTexture(int nPattern, int nPatternMax)
{
	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

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
