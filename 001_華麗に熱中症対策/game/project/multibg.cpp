//===============================================
//
// 多重スクロールの背景の処理 [bg.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "multibg.h"
#include "object2D.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "texture.h"

//===============================================
// マクロ定義
//===============================================
#define VTX	(4)	// 頂点数
#define WIDTH	(100.0f)	// 幅
#define HEIGHT	(100.0f)	// 高さ
#define POS_X	(300.0f)	// X座標
#define POS_Y	(300.0f)	// Y座標
#define TEXMOVE_U	(0.000005f)		// プレイヤー移動倍率U座標
#define TEXMOVE_V	(0.0000008f)	// プレイヤー移動倍率V座標
#define TEXCNTMOVE	(0.00005f)	// カウント毎背景移動倍率

//===============================================
// 静的メンバ変数宣言(ファイル名)
//===============================================
const char *CMultiBg::m_apTextureFileName[] =
{
	"data\\TEXTURE\\bg002.png",
	"data\\TEXTURE\\bg002.png",
	"data\\TEXTURE\\bg002.png",
};

// テクスチャ移動量
const CMultiBg::STex CMultiBg::apMoveTex[TYPE_MAX] =
{
	{ 0.1f, 0.0f, 0.0f, -0.00025f },
	{ 0.2f, 0.0f, 0.0f, -0.0005f },
	{ 0.3f, 0.0f, 0.0f, -0.001f },
};

//===============================================
// コンストラクタ
//===============================================
CMultiBg::CMultiBg() :CObject(0)
{
	// 値をクリア
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_aBg[nCnt].pObject2D = NULL;
		m_aBg[nCnt].sTex.fMoveU = 0.0f;
		m_aBg[nCnt].sTex.fMoveV = 0.0f;
		m_aBg[nCnt].sTex.fTexU = 0.0f;
		m_aBg[nCnt].sTex.fTexV = 0.0f;
		m_aIdxTexture[nCnt] = -1;
	}
}

//===============================================
// デストラクタ
//===============================================
CMultiBg::~CMultiBg()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CMultiBg::Init(void)
{
	CTexture *pTexture = CManager::GetTexture();

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// テクスチャの読み込み
		m_aIdxTexture[nCnt] = pTexture->Regist(m_apTextureFileName[nCnt]);
		SBg *pBg = &m_aBg[nCnt];	// 背景情報のポインタ

		if (pBg->pObject2D == NULL)
		{// 使用していない場合
			pBg->pObject2D = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

			CObject2D *pObjBg = pBg->pObject2D;	// 移動
			STex *pTex = &pBg->sTex;

			if (pObjBg != NULL)
			{// 使用している場合
				pObjBg->Init();

				// 種類の設定
				pObjBg->SetType(TYPE_BG);

				// テクスチャ割り当て
				pObjBg->BindTexture(m_aIdxTexture[nCnt]);

				m_aBg[nCnt].sTex = apMoveTex[nCnt];

				// オブジェクトの更新処理
				pObjBg->SetVtx(pTex->fTexU, pTex->fTexV, (float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f);
			}
		}
	}

	/*m_aBg[0].fMoveU = 0.001f;
	m_aBg[1].fMoveU = 0.003f;
	m_aBg[2].fMoveU = 0.005f;*/
	// オブジェクトの初期化処理
	//CObject2D::Init();

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CMultiBg::Uninit(void)
{
	// オブジェクトの終了処理
	CObject::Release();
}

//===============================================
// 更新処理
//===============================================
void CMultiBg::Update(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		SBg *pBg = &m_aBg[nCnt];

		if (pBg != NULL)
		{// 使用されている場合

			STex *pTex = &m_aBg[nCnt].sTex;	// テクスチャ情報のポインタ

			// テクスチャ座標の更新処理
			pTex->fTexU += pTex->fMoveU;
			pTex->fTexV += pTex->fMoveV;

			if (pTex->fTexU < 0.0f)
			{
				pTex->fTexU = 1.0f;
			}
			else if (pTex->fTexU > 1.0f)
			{
				pTex->fTexU = 0.0f;
			}

			if (pTex->fTexV < 0.0f)
			{
				pTex->fTexV = 1.0f;
			}
			else if (pTex->fTexV > 1.0f)
			{
				pTex->fTexV = 0.0f;
			}

			// オブジェクトの更新処理
			pBg->pObject2D->SetVtx(pTex->fTexU, pTex->fTexV, (float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f);
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CMultiBg::Draw(void)
{
	// 本体は描画無し
}

//===============================================
// 生成
//===============================================
CMultiBg *CMultiBg::Create()
{
	CMultiBg *pBg = NULL;

	// オブジェクト2Dの生成
	pBg = new CMultiBg;

	if (pBg != NULL)
	{// 生成できた場合
		// 初期化処理
		pBg->Init();

		// 種類の設定
		pBg->SetType(CObject::TYPE_NONE);
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pBg;
}

//===============================================
// 背景の慣性移動
//===============================================
void CMultiBg::SetPlayerMove(const D3DXVECTOR3 move)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		SBg *pBg = &m_aBg[nCnt];

		if (pBg != NULL)
		{// 使用されている場合
			pBg->sTex.fTexU += move.x * (TEXMOVE_U + nCnt * TEXCNTMOVE);
			pBg->sTex.fTexV += move.y * (TEXMOVE_V + nCnt * TEXCNTMOVE);
		}
	}
}