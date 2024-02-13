//===============================================
//
// 爆発の処理 [exploion.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "explosion.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "bullet.h"
#include "texture.h"

//===============================================
// マクロ定義
//===============================================
#define VTX	(4)	// 頂点数
#define WIDTH	(50.0f)	// 幅
#define HEIGHT	(50.0f)	// 高さ
#define ANIMTIMER		(5)		//一コマ進む時間
#define MAX_ANIMPATTERN	(8)	//アニメーションの最大パターン数
#define MAX_HEIFGHTPATTERN	(8)	//アニメーションの最大横パターン数

//===============================================
// コンストラクタ
//===============================================
CExplosion::CExplosion()
{
	// 値をクリアする
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CExplosion::CExplosion(const D3DXVECTOR3 pos)
{
	// 値をクリアする
	SetPosition(pos);
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//===============================================
// デストラクタ
//===============================================
CExplosion::~CExplosion()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CExplosion::Init(void)
{
	// オブジェクトの初期化処理
	CObjectBillboard::Init();

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CExplosion::Uninit(void)
{
	// オブジェクトの終了処理
	CObjectBillboard::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CExplosion::Update(void)
{
	// 頂点情報設定
	SetVtx(m_nPatternAnim, MAX_HEIFGHTPATTERN, MAX_ANIMPATTERN / MAX_HEIFGHTPATTERN);

	// プレイヤー操作
	Controller();
}

//===============================================
// 描画処理
//===============================================
void CExplosion::Draw(void)
{
	CObjectBillboard::Draw();
}

//===============================================
// 頂点情報設定
//===============================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos)
{
	CExplosion *pExplosion = NULL;
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// オブジェクト2Dの生成
	pExplosion = new CExplosion(pos);

	if (pExplosion != NULL)
	{// 生成できた場合

		// 初期化処理
		pExplosion->Init();

		// 種類の設定
		pExplosion->SetType(TYPE_EXPLOSION);

		// テクスチャ設定
		pExplosion->SetVtx(pExplosion->m_nPatternAnim, MAX_HEIFGHTPATTERN, MAX_ANIMPATTERN / MAX_HEIFGHTPATTERN);

		// テクスチャの割り当て
		pExplosion->BindTexture(pTexture->Regist(CTexture::GetFileName(CTexture::TYPE_EXPLOSION)));
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pExplosion;
}

//===============================================
// 操作処理
//===============================================
void CExplosion::Controller(void)
{
 	m_nCounterAnim++;	//カウンターを加算

	if (m_nCounterAnim % ANIMTIMER == 0)
	{// アニメーションタイマーの値を超えた場合

		m_nCounterAnim = 0;

		m_nPatternAnim++;	//パターンNo.を更新

		if (m_nPatternAnim >= MAX_ANIMPATTERN)
		{// 最後までアニメーションした場合
			Uninit();
		}
	}
}
