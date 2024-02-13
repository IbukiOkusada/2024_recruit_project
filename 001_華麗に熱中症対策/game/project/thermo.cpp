//===============================================
//
// 温度表示の処理 [thermo.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "thermo.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

// マクロ定義
#define RATIO	(0.01f)

//===============================================
// コンストラクタ
//===============================================
//CThermo::CThermo()
//{
//	// 値のクリア
//	m_Info.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//	m_Info.fLife = 0;
//	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	m_Info.fRadius = 0.0f;
//	m_Info.Type = TYPE_NONE;
//}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CThermo::CThermo(int nPriority) : CObject3D(nPriority)
{
	// 値のクリア
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_colDiff = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_fRadius = 0.0f;
}

//===============================================
// デストラクタ
//===============================================
CThermo::~CThermo()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CThermo::Init(void)
{
	CObject3D::Init();

	SetType(TYPE_MAP);	// 種類設定
	BindTexture(CManager::GetTexture()->Regist(CTexture::GetFileName(CTexture::TYPE_EFFECT)));

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CThermo::Uninit(void)
{
	CObject3D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CThermo::Update(void)
{
	CObject3D::Update();

	// 差分を補正する
	D3DXCOLOR col = m_colDiff - m_col;

	m_col += col * RATIO;

	SetCol(m_col);
}

//===============================================
// 描画処理
//===============================================
void CThermo::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//ライティングをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject3D::Draw();

	//ライティングをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================================
// 生成
//==========================================================
CThermo *CThermo::Create(void)
{
	CThermo *pThermo = NULL;	// 影のポインタ
	CTexture *pTexture = CManager::GetTexture();	// テクスチャのポインタ

	// オブジェクト2Dの生成
	pThermo = new CThermo;

	if (pThermo != NULL)
	{// 生成できた場合

		// 初期化処理
		pThermo->Init();
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pThermo;
}

