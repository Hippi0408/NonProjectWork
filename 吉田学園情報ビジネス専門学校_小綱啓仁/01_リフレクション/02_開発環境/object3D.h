//==================================================
// object3D.h
// Author: Buriya Kota
//==================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//**************************************************
// インクルード
//**************************************************
#include "object.h"
#include "texture.h"

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************

//**************************************************
// 定数定義
//**************************************************
// 頂点フォーマット
const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);		// 座標・法線・カラー・テクスチャ

//**************************************************
// 構造体定義
//**************************************************
// 頂点の情報[3D]の構造体を定義
struct VERTEX_3D
{
	D3DXVECTOR3 pos;	// 頂点座標	
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー	
	D3DXVECTOR2 tex;	// テクスチャの座標
};

//**************************************************
// クラス
//**************************************************
class CObject3D : public CObject
{
public:
	static const float BILLBORAD_WIDTH;
	static const float BILLBOARD_HEIGHT;

public:
	explicit CObject3D(int nPriority = PRIORITY_OBJECT);
	virtual ~CObject3D() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw(DRAW_MODE drawMode) override;

	// セッター
	void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	void SetSize(const D3DXVECTOR3& size) { m_size = size; }
	void SetWorldMtx(const D3DXMATRIX& mtx) { m_mtxWorld = mtx; }
	void SetBillboard(const bool& billboard) { m_bBill = billboard; }
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }	// テクスチャの設定
	void SetCol(const D3DXCOLOR& col);
	void SetMove(const D3DXVECTOR3& move) { m_move = move; }

	// ゲッター
	D3DXVECTOR3 GetMove() { return m_move; }
	const D3DXVECTOR3& GetPos() const { return m_pos; }
	const D3DXVECTOR3& GetRot() const { return m_rot; }
	const D3DXVECTOR3& GetSize() const { return m_size; }
	const D3DXMATRIX& GetWorldMtx() const { return m_mtxWorld; }

	static CObject3D *Create(const D3DXVECTOR3 pos,const D3DXVECTOR3 size);

	void AnimTexture(int nPattern, int nPatternMax);
	void MovePos(const D3DXVECTOR3& move) { m_pos += move; }

private:
	// 位置
	D3DXVECTOR3 m_pos;
	// 移動量
	D3DXVECTOR3 m_move;
	// 回転
	D3DXVECTOR3 m_rot;
	// 頂点カラー
	D3DXCOLOR m_col;
	// ワールドマトリックス
	D3DXMATRIX m_mtxWorld;
	// テクスチャの列挙型
	CTexture::TEXTURE m_texture;
	// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	// 大きさ
	D3DXVECTOR3 m_size;
	// ビルボードかどうか
	bool m_bBill;
	// 親子かどうか
	bool m_bFamily;

};

#endif	// _OBJECT3D_H_