//===============================================
//
// オブジェクト(マップの巻物)の処理 [object2D.cpp]
// Author : Soma Ishihara
//
//===============================================
#include "object2DScroll.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//====================================================================================
// マップの巻物クラス
//====================================================================================
//===============================================
// コンストラクタ
//===============================================
CObject2DScroll::CObject2DScroll() : CObject2D(3)
{
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CObject2DScroll::CObject2DScroll(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nPriority) : CObject2D(pos,rot,nPriority)
{
}

//===============================================
// デストラクタ
//===============================================
CObject2DScroll::~CObject2DScroll()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CObject2DScroll::Init(void)
{
	CObject2D::Init();
	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CObject2DScroll::Uninit(void)
{
	CObject2D::Uninit();

	// 廃棄
	Release();
}

//===============================================
// 更新処理
//===============================================
void CObject2DScroll::Update(void)
{
	CObject2D::Update();
}

//===============================================
// 描画処理
//===============================================
void CObject2DScroll::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスへのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//アルファテスト有効・無効設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 180);

	// 描画
	CObject2D::Draw();

	//アルファテスト有効・無効設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//===============================================
// 頂点情報設定
//===============================================
CObject2DScroll *CObject2DScroll::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nPriority)
{
	CObject2DScroll *pObject2D = NULL;

	// オブジェクト2Dの生成
	pObject2D = new CObject2DScroll(pos, rot, nPriority);

	if (pObject2D != NULL)
	{// 生成できた場合
		// 初期化処理

		pObject2D->Init();

		// 種類設定
		pObject2D->SetType(TYPE_NONE);
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pObject2D;
}