//==================================================
// silhouette.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>

#include "manager.h"
#include "silhouette.h"

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CSilhoette::CSilhoette(int nPriority) : CObject2D(nPriority)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CSilhoette::~CSilhoette()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CSilhoette::Init()
{
	CObject2D::Init();

	return S_OK;
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CSilhoette::Draw(DRAW_MODE drawMode)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	// ステンシルバッファ -> 有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// ステンシルバッファと比較する参照値設定 -> ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	// ステンシルバッファの値に対してのマスク設定 -> 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// ステンシルテストの比較方法 ->
	// （参照値 >= ステンシルバッファの参照値）なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	// ステンシルテストの結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);		// Zとステンシル成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);			// Zとステンシル失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);			// Zのみ失敗

	CObject2D::Draw(drawMode);

	// ステンシルバッファ -> 無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CSilhoette *CSilhoette::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	CSilhoette *pSilhoette = nullptr;
	pSilhoette = new CSilhoette;

	if (pSilhoette != nullptr)
	{
		pSilhoette->Init();
		pSilhoette->SetPos(pos);
		pSilhoette->SetSize(size);
		pSilhoette->SetCol(col);
	}
	else
	{
		assert(false);
	}

	return pSilhoette;
}
