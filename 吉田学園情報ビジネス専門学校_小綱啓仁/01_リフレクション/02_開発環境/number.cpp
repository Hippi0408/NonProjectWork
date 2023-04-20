//==================================================
// number.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>

#include "number.h"

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CNumber::CNumber(int nPriority /* =4 */) : CObject2D(nPriority)
{
	SetType(TYPE_NUMBER);
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CNumber::~CNumber()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CNumber::Init()
{
	CObject2D::Init();

	SetTexture(CTexture::TEXTURE_NONE);

	return S_OK;
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CNumber* CNumber::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int priority)
{
	CNumber *pNumber;
	pNumber = new CNumber(priority);

	if (pNumber != nullptr)
	{
		pNumber->Init();
		pNumber->SetPos(pos);
		pNumber->SetSize(size);
	}
	else
	{
		assert(false);
	}

	return pNumber;
}
