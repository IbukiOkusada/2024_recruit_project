//===============================================
//
// スコアの処理 [target.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "target.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"
#include "texture.h"
#include "object2D.h"
#include "enemy.h"
#include "fade.h"

//==========================================================
// 定数定義
//==========================================================
namespace
{
	const float WIDTH = (30.0f);	// 幅
	const float X_SPACE = (55.0f);	// 横の移動量
	const float HEIGHT = (50.0f);	// 高さ
}

//===============================================
// コンストラクタ
//===============================================
CTarget::CTarget()
{
	// 値をクリア
	for (int nCnt = 0; nCnt < NUM_TARGET; nCnt++)
	{
		m_apNumber[nCnt] = NULL;
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNum = 0;
	m_nIdxTexture = -1;

	while (1)
	{
		if (CObject::GetNumEnemAll() < 50)
		{
			int nRand = rand() % 201 - 100;
			float fRot = D3DX_PI * ((float)nRand * 0.01f);

			CEnemy::Create(D3DXVECTOR3(0.0f + rand() % 300 - 150, 0.0f, 0.0f + rand() % 300 - 150), D3DXVECTOR3(0.0f, fRot, 0.0f),
				D3DXVECTOR3(-sinf(fRot) * 4.0f, 0.0f, -cosf(fRot) * 4.0f), "data\\TXT\\motion_murabito.txt");
		}
		else
		{
			break;
		}
	}
}

//===============================================
// デストラクタ
//===============================================
CTarget::~CTarget()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CTarget::Init(void)
{
	CTexture *pTexture = CManager::GetTexture();

	// テクスチャの読み込み
	m_nIdxTexture = pTexture->Regist(CTexture::GetFileName(CTexture::TYPE_SCORE));

	for (int nCnt = 0; nCnt < NUM_TARGET; nCnt++)
	{
		CNumber *pNum = m_apNumber[nCnt];	// 数字のポインタ

		if (m_apNumber[nCnt] == NULL)
		{// 使用していない場合
			D3DXVECTOR3 pos = m_pos;	// 設定座標
			pos.x += nCnt * X_SPACE;	// 横移動
			m_apNumber[nCnt] = CNumber::Create(pos, WIDTH, HEIGHT);

			if (m_apNumber[nCnt] != NULL)
			{// 使用している場合
				m_apNumber[nCnt]->GetObject2D()->BindTexture(m_nIdxTexture);
				
			}
		}
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CTarget::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_TARGET; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{// 使用していない場合
			m_apNumber[nCnt]->Uninit();

			delete m_apNumber[nCnt];

			m_apNumber[nCnt] = NULL;
		}
	}
}

//===============================================
// 更新処理
//===============================================
void CTarget::Update(void)
{
	int nNum = CObject::GetNumEnemAll();

	if (nNum <= 0)
	{
		CManager::GetFade()->Set(CScene::MODE_RESULT);
	}
	else
	{
		Set(nNum);
	}
}

//===============================================
// 生成
//===============================================
CTarget *CTarget::Create(D3DXVECTOR3 pos)
{
	CTarget *pBg = NULL;

	// オブジェクト2Dの生成
	pBg = new CTarget;

	if (pBg != NULL)
	{// 生成できた場合

		pBg->m_pos = pos;

		// 初期化処理
		pBg->Init();
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pBg;
}

//===============================================
// スコア加算
//===============================================
void CTarget::Add(int nValue)
{
	// 代入
	m_nNum += nValue;

	// 数値設定
	SetValue();
}

//===============================================
// スコア設定
//===============================================
void CTarget::Set(int nValue)
{
	// 代入
	m_nNum = nValue;

	// 数値設定
	SetValue();
}

//===============================================
// 桁ごとの値設定
//===============================================
void CTarget::SetValue(void)
{
	if (m_nNum < 0)
	{// 0以下の場合
		m_nNum = 0;
	}

	//スコアを各配列に格納
	for (int nCnt = 0; nCnt < NUM_TARGET; nCnt++)
	{
		// 現在の桁の値を求める
		int nNum = m_nNum % (int)pow(10, (NUM_TARGET - nCnt)) / (int)pow(10, (NUM_TARGET - nCnt) - 1);

		if (m_apNumber[nCnt] != NULL)
		{// 使用されている場合
			// 値を設定
			m_apNumber[nCnt]->SetIdx(nNum);
		}
	}
}

//===============================================
// 色設定
//===============================================
void CTarget::SetColor(D3DXCOLOR col)
{
	for (int nCnt = 0; nCnt < NUM_TARGET; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{// 使用していない場合
			m_apNumber[nCnt]->GetObject2D()->SetCol(col);
		}
	}
}
