//==================================================
// shadow.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>

#include "manager.h"

#include "shadow.h"

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CShadow::CShadow()
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CShadow::~CShadow()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CShadow::Init()
{
	// 初期化
	CObject3D::Init();

	// テクスチャの設定
	CObject3D::SetTexture(CTexture::TEXTURE_SHADOW);

	return S_OK;
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CShadow::Draw(DRAW_MODE drawMode)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	//減算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画
	CObject3D::Draw(drawMode);

	//設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CShadow *CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CShadow *pShadow = nullptr;
	pShadow = new CShadow;

	if (pShadow != nullptr)
	{
		pShadow->Init();
		pShadow->SetPos(pos);
		pShadow->SetSize(size);
	}
	else
	{
		assert(false);
	}

	return pShadow;
}
