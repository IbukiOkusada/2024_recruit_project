//===============================================
//
// 数字全般の処理 [score.cpp]
// Author : Ryosuke Ohhara
//
//===============================================
#include "score.h"
#include "object2D.h"
#include "number.h"

// 定数定義
namespace
{
	const int MAX_WIDTHPATTERN = 10;	// パターン数
	const float POLYSIZE = (25.0f);	// ポリゴンのサイズ
}

//===============================================
// コンストラクタ
//===============================================
CScore::CScore()
{
	// 値をクリアする
	m_nIdx = 0;
	m_nNumScore = 0;

	for (int nCount = 0; nCount < NUM_SCORE; nCount++)
	{
		m_apNumber[nCount] = nullptr;  // 使用していない状態にする
	}
}

//===============================================
// デストラクタ
//===============================================
CScore::~CScore()
{

}

//===============================================
// 初期化処理
//==============================================

HRESULT CScore::Init(D3DXVECTOR3 pos, int nDesit, float fGap, float fWidth, float fHeight)
{
	m_pos = pos;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_nDesit = nDesit;
	m_fGap = fGap;

	
	for (int nCount = 0; nCount < m_nDesit; nCount++)
	{
		if (m_apNumber[nCount] == nullptr)
		{// 使用されていない場合

			// 生成
			
			m_apNumber[nCount] = CNumber::Create(D3DXVECTOR3(pos.x + nCount * (fWidth + fWidth * fGap), pos.y, pos.z), fWidth, fHeight);

			if (m_apNumber[nCount] != nullptr)
			{// 使用されている場合

				// 初期化処理
				m_apNumber[nCount]->Init(pos, fWidth, fHeight);
			}
		}
	}

	return S_OK;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CScore::Init()
{
	
	for (int nCount = 0; nCount < m_nDesit; nCount++)
	{
		if (m_apNumber[nCount] == nullptr)
		{// 使用していない場合

			// 生成
			m_apNumber[nCount] = CNumber::Create();

			if (m_apNumber[nCount] != nullptr)
			{// 使用されていた場合

				// 初期化処理
				m_apNumber[nCount]->Init();
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
	
	for (int nCount = 0; nCount < NUM_SCORE; nCount++)
	{
		if (m_apNumber[nCount] != nullptr)
		{// 使用されていた場合

			// 終了処理
			m_apNumber[nCount]->Uninit();
			delete m_apNumber[nCount];
			

			// 使用していない状態にする
			m_apNumber[nCount] = nullptr;
		}
	}
}

//===============================================
// 更新処理
//===============================================
void CScore::Update(void)
{
	
	for (int nCount = 0; nCount < m_nDesit; nCount++)
	{
		if (m_apNumber[nCount] != nullptr)
		{// 使用されている場合

			// 更新処理
			m_apNumber[nCount]->Update();
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CScore::Draw(void)
{
	//if (m_pObject2D != NULL)
	//{// 使用されている場合
	//	// 描画処理
	//	m_pObject2D->Draw();
	//}
}

//===============================================
// 生成
//===============================================
CScore *CScore::Create(void)
{
	CScore *pNum = NULL;

	if (pNum != NULL)
	{// 使用されていない場合

		pNum = new CScore;

		if (pNum != NULL)
		{// 使用されている場合

			// 初期化処理
			pNum->Init();
		}
	}

	return pNum;
}

//===============================================
// 生成
//===============================================

CScore *CScore::Create(D3DXVECTOR3 pos, int nDesit, float fGap, float fWidth, float fHeight)
{
	CScore *pNum = NULL;

	if (pNum == NULL)
	{// 使用されていない場合

		pNum = new CScore;

		if (pNum != NULL)
		{// 使用されている場合

			// 初期化処理

			pNum->Init(pos, nDesit, fGap, fWidth, fHeight);
		}
	}

	return pNum;
}

//===============================================
//スコア加算処理
//===============================================
void CScore::AddScore(int nScore)
{
	m_nNumScore += nScore;


	//スコアを各配列に格納
	for (int nCnt = 0; nCnt < m_nDesit; nCnt++)
	{
		// 現在の桁の値を求める
		int nNum = m_nNumScore % (int)pow(10, (m_nDesit - nCnt)) / (int)pow(10, (m_nDesit - nCnt) - 1);

		if (m_apNumber[nCnt] != NULL)
		{// 使用されている場合
		 // 値を設定
			m_apNumber[nCnt]->SetIdx(nNum);
		}
	}

	/*for (int nCount = 0; nCount < NUM_SCORE; nCount++)
	{
		m_apNumber[nCount]->SetIdx(m_nNumScore);
	}*/
}

//===============================================
//スコア減算処理
//===============================================
void CScore::LowerScore(int nScore)
{
	m_nNumScore -= nScore;


	//スコアを各配列に格納
	for (int nCnt = 0; nCnt < m_nDesit; nCnt++)
	{
		// 現在の桁の値を求める
		int nNum = m_nNumScore % (int)pow(10, (m_nDesit - nCnt)) / (int)pow(10, (m_nDesit - nCnt) - 1);

		if (m_apNumber[nCnt] != NULL)
		{// 使用されている場合
		 // 値を設定
			m_apNumber[nCnt]->SetIdx(nNum);
		}
	}
}

//===============================================
//スコア設定処理
//===============================================
void CScore::SetScore(int nScore)
{
	m_nNumScore = nScore;


	//スコアを各配列に格納
	for (int nCnt = 0; nCnt < m_nDesit; nCnt++)
	{
		// 現在の桁の値を求める
		int nNum = m_nNumScore % (int)pow(10, (m_nDesit - nCnt)) / (int)pow(10, (m_nDesit - nCnt) - 1);

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
void CScore::SetClo(D3DXCOLOR col)
{

	for (int nCnt = 0; nCnt < m_nDesit; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{// 使用していない場合
			m_apNumber[nCnt]->GetObject2D()->SetCol(col);
		}
	}
}

//===============================================
// 座標設定
//===============================================
void CScore::SetPosition(const D3DXVECTOR3& pos) {
	m_pos = pos;


	for (int nCount = 0; nCount < m_nDesit; nCount++)
	{
		if (m_apNumber[nCount] != nullptr)
		{// 使用されていない場合

			// 生成
			m_apNumber[nCount]->SetPosition(D3DXVECTOR3(m_pos.x + nCount * (m_fWidth + m_fWidth * m_fGap), m_pos.y, m_pos.z));
		}
	}
}

////===============================================
//// 頂点設定
////===============================================
//void CScore::SetIdx(const int nIdx)
//{
//	m_nIdx = nIdx;	// 値を設定
//
//	if (m_nIdx > 10)
//	{// 値が限界を超えた場合
//		m_nIdx = 9;
//	}
//	else if (m_nIdx < 0)
//	{// 値が最低値を超えた場合
//		m_nIdx = 0;
//	}
//
//	if (m_pObject2D != NULL)
//	{// 使用されている場合
//	 // 頂点情報の設定
//		m_pObject2D->SetVtx(m_nIdx, MAX_WIDTHPATTERN, 1);
//	}
//}
//
////===============================================
//// ポリゴン廃棄
////===============================================
//void CScore::PolygonDelete(void)
//{
//	if (m_pObject2D != NULL)
//	{// 使用されている場合
//		m_pObject2D->Uninit();
//		m_pObject2D = NULL;
//	}
//}