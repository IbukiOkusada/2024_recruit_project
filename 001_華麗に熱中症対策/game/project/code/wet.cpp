//===============================================
//
// 濡れ跡の処理 [wet.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "wet.h"
#include "object3D.h"
#include "manager.h"
#include "slow.h"
#include "texture.h"

//===============================================
// マクロ定義
//===============================================
#define LIFE	(250)	// 体力

//===============================================
// コンストラクタ
//===============================================
CWet::CWet()
{
	// 値のクリア
	m_fLife = 0.0f;
	m_fDefWidth = 0.0f;
	m_fDefHeight = 0.0f;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//===============================================
// デストラクタ
//===============================================
CWet::~CWet()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CWet::Init(void)
{
	// 初期化
	CShadow::Init();

	// 値を初期化する
	m_fLife = LIFE;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CWet::Uninit(void)
{
	CShadow::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CWet::Update(void)
{
	m_fLife -= CManager::GetSlow()->Get();	// 体力の減少 

	float fRate = (float)(m_fLife / LIFE);	// 現在の最大体力の割合

	// サイズの調整
	float fWidth = m_fDefWidth * fRate;
	float fHeight = m_fDefHeight * fRate;
	m_col.a = 1.0f * (fRate - 0.5f);

	// サイズ設定
	SetpVtx(fWidth, fHeight);
	SetCol(m_col);

	if (m_fLife <= 0.0f || m_col.a <= 0.0f)
	{// 寿命が来た
		Uninit();
	}
}

//===============================================
// 描画処理
//===============================================
void CWet::Draw(void)
{
	CShadow::Draw();
}

//===============================================
// 生成
//===============================================
CWet *CWet::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CWet *pWet = NULL;
	CTexture *pTexture = CManager::GetTexture();	// テクスチャのポインタ

	pWet = new CWet;

	if (pWet == NULL)
	{
		return NULL;
	}

	// 初期化
	pWet->Init();

	// 座標設定
	pWet->SetPosition(pos);

	// サイズ設定
	pWet->SetSize(fWidth, fHeight);

	// 種類設定
	pWet->SetType(TYPE_NONE);

	// テクスチャ設定
	pWet->BindTexture(pTexture->Regist(CTexture::GetFileName(CTexture::TYPE_SHADOW)));

	return pWet;
}

//===============================================
// デフォルトサイズ設定
//===============================================
void CWet::SetSize(float fWidth, float fHeight)
{
	m_fDefWidth = fWidth;
	m_fDefHeight = fHeight;

	// サイズ設定
	SetpVtx(m_fDefWidth, m_fDefHeight);
}