//==========================================================
//
// マップオブジェクト [object2DMap.cpp]
// Author : Soma Ishihara
//
//==========================================================
#include "object2DMap.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

// マクロ定義

//==========================================================
// コンストラクタ
//==========================================================
CObject2DMap::CObject2DMap(int nPriOrity) : CObject2D(nPriOrity)
{

}

//==========================================================
// デストラクタ
//==========================================================
CObject2DMap::~CObject2DMap()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CObject2DMap::Init(void)
{
	CObject2D::Init();
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CObject2DMap::Uninit(void)
{
	CObject2D::Uninit();
}

//==========================================================
// 描画処理
//==========================================================
void CObject2DMap::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスへのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtx();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		pVtxBuff,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// マップテクスチャの設定
	pDevice->SetTexture(0, m_pTextureMap);

	// 描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,
		2	//頂点情報構造体のサイズ
	);

	// まっくろくろすけテクスチャの設定
	pDevice->SetTexture(0, m_pTextureUnex);

	// 描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,
		2	//頂点情報構造体のサイズ
	);
}

//==========================================================
// 生成
//==========================================================
CObject2DMap *CObject2DMap::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight)
{
	CObject2DMap *pMap = nullptr;

	pMap = new CObject2DMap;

	if (pMap != nullptr)
	{
		// 初期化処理
		pMap->Init();
		pMap->SetPosition(pos);
		pMap->SetRotation(rot);
		pMap->SetSize(fWidth, fHeight);
	}

	return pMap;
}

//==========================================================
// テクスチャ設定
//==========================================================
void CObject2DMap::SetTexture(LPDIRECT3DTEXTURE9 pTexMap, LPDIRECT3DTEXTURE9 pTexUnex)
{
	m_pTextureMap = pTexMap;
	m_pTextureUnex = pTexUnex;
}
