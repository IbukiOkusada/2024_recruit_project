//===============================================
//
// スコアの処理 [score.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "score.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"
#include "texture.h"
#include "object2D.h"
#include "fade.h"
#include "enemy.h"

// マクロ定義
#define WIDTH	(30)	// 幅
#define X_SPACE	(55)	// 横の移動量
#define HEIGHT	(50)	// 高さ

//===============================================
// コンストラクタ
//===============================================
CScore::CScore()
{
	// 値をクリア
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		m_apNumber[nCnt] = NULL;
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNum = 0;
	m_nIdxTexture = -1;
}

//===============================================
// デストラクタ
//===============================================
CScore::~CScore()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CScore::Init(void)
{
	CTexture *pTexture = CManager::GetTexture();

	// テクスチャの読み込み
	m_nIdxTexture = pTexture->Regist(CTexture::GetFileName(CTexture::TYPE_SCORE));

	if (m_nDesit > NUM_SCORE)
	{
		m_nDesit = NUM_SCORE;
	}
	else if (m_nDesit <= 0)
	{
		m_nDesit = 1;
	}

	for (int nCnt = 0; nCnt < m_nDesit; nCnt++)
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
void CScore::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
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
void CScore::Update(void)
{
	int nNum = CObject::GetNumEnemAll();

	if (nNum <= 0)
	{
		Set(nNum);
		//CManager::GetFade()->Set(CScene::MODE_RESULT);
	}
	else
	{
		Set(nNum);
	}
}

//===============================================
// 生成
//===============================================
CScore *CScore::Create(D3DXVECTOR3 pos, const int nDegit)
{
	CScore *pBg = NULL;

	// オブジェクト2Dの生成
	pBg = new CScore;

	if (pBg != NULL)
	{// 生成できた場合

		pBg->m_nDesit = nDegit;

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
void CScore::Add(int nValue)
{
	// 代入
	m_nNum += nValue;

	// 数値設定
	SetValue();
}

//===============================================
// スコア設定
//===============================================
void CScore::Set(int nValue)
{
	// 代入
	m_nNum = nValue;

	// 数値設定
	SetValue();
}

//===============================================
// 桁ごとの値設定
//===============================================
void CScore::SetValue(void)
{
	if (m_nNum < 0)
	{// 0以下の場合
		m_nNum = 0;
	}

	//スコアを各配列に格納
	for (int nCnt = 0; nCnt < m_nDesit; nCnt++)
	{
		// 現在の桁の値を求める
		int nNum = m_nNum % (int)pow(10, (m_nDesit - nCnt)) / (int)pow(10, (m_nDesit - nCnt) - 1);

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
void CScore::SetColor(D3DXCOLOR col)
{
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{// 使用していない場合
			m_apNumber[nCnt]->GetObject2D()->SetCol(col);
		}
	}
}
