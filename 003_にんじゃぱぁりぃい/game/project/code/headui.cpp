//===============================================
//
// 頭の上に出るUI処理 [overheadui.cpp]
// Author : Ryosuke Ohara
//
//===============================================
#include "headui.h"
#include "billboard.h"
#include "manager.h"
#include "texture.h"
#include "camera.h"
#include "debugproc.h"

// マクロ定義
#define MAX_WIDTHPATTERN	(10)	// パターン数
#define LEN (10.0f)
#define TEXTURENAME  ("data\\TEXTURE\\number004.png") // 数字

//===============================================
// コンストラクタ
//===============================================
CHeadUI::CHeadUI()
{
	// 値をクリアする
	m_nIdx = 0;
	m_fPolyHeight = 0.0f;
	m_fPolyWidth = 0.0f;
	m_fUpHeight = 0.0f;
	m_pPos = nullptr;

	for (int nCount = 0; nCount < NUM_DIGIT; nCount++)
	{
		m_apObject[nCount] = nullptr;
	}
}

//===============================================
// デストラクタ
//===============================================
CHeadUI::~CHeadUI()
{

}

//===============================================
// 初期化処理
//===============================================
//HRESULT CHeadUI::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
//{
//	for (int nCount = 0; nCount < NUM_DIGIT; nCount++)
//	{
//		if (m_pObject[nCount] == nullptr)
//		{// 使用されていない場合
//
//			m_pObject[nCount] = CObjectBillboard::Create()
//
//		}
//	}
//
//	return S_OK;
//}

//===============================================
// 初期化処理
//===============================================
HRESULT CHeadUI::Init()
{
	for (int nCount = 0; nCount < NUM_DIGIT; nCount++)
	{
		if (m_apObject[nCount] == nullptr)
		{// 使用していた場合

			m_apObject[nCount] = CObjectBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 7);
			m_apObject[nCount]->SetDraw(true);
			m_apObject[nCount]->SetAlphaText(false);
			m_apObject[nCount]->SetLighting(true);
			m_apObject[nCount]->SetZTest(false);
			m_apObject[nCount]->SetFusion(CObjectBillboard::FUSION_NORMAL);
			m_apObject[nCount]->BindTexture(CManager::GetInstance()->GetTexture()->Regist(TEXTURENAME));
		}
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CHeadUI::Uninit(void)
{
	for (int nCount = 0; nCount < NUM_DIGIT; nCount++)
	{
		if (m_apObject[nCount] != nullptr)
		{// 使用していた場合

		 // 終了処理
			m_apObject[nCount]->Uninit();

			// 使用していない状態にする
			m_apObject[nCount] = nullptr;
		}
	}

	// 開放
	Release();
}

//===============================================
// 更新処理
//===============================================
void CHeadUI::Update(void)
{
	SetMixPosition();
}

//===============================================
// 生成
//===============================================
CHeadUI *CHeadUI::Create(void)
{
	CHeadUI *pNum = NULL;

	if (pNum != NULL)
	{// 使用されていない場合
		pNum = new CHeadUI;

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
CHeadUI *CHeadUI::Create(D3DXVECTOR3 * pPos, D3DXMATRIX *Matrix, const float fUpHeight, const float fPolyWidth, const float fPolyHeight)
{
	CHeadUI *pOverUI = NULL;

	pOverUI = new CHeadUI;

	if (pOverUI != NULL)
	{// 使用されていない場合

		// 初期化処理
		pOverUI->Init();

		// 親の設定
		pOverUI->SetParent(pPos);

		// 相対座標の高さを設定
		pOverUI->SetUpHeight(fUpHeight);

		// ポリゴンサイズを設定
		pOverUI->SetPolySize(fPolyWidth, fPolyHeight);

		// 親のマトリックス設定
		pOverUI->SetMtxParent(Matrix);

		// 座標を設定
		pOverUI->SetMixPosition();
	}

	return pOverUI;
}

//===============================================
// 頂点設定
//===============================================
void CHeadUI::SetIdx(const int nIdx)
{
	m_nIdx = nIdx;	// 値を設定

	if (m_apObject[0] != nullptr && m_apObject[1] != nullptr)
	{// 使用していた場合

		// 頂点情報の設定
		m_apObject[0]->SetVtx(m_nIdx % 100 / 10, MAX_WIDTHPATTERN, 1);
		m_apObject[1]->SetVtx(m_nIdx % 10 / 1, MAX_WIDTHPATTERN, 1);
	}
}

//==========================================================
// 幅と高さの適応
//==========================================================
void CHeadUI::SetPolySize(const float fWidth, const float fHeight)
{
	// 幅の設定
	{
		m_fPolyWidth = fWidth;
		if (m_fPolyWidth < 0.0f) {	// 値がマイナス
			m_fPolyWidth = 0.0f;
		}
	}

	// 高さの設定
	{
		m_fPolyHeight = fHeight;
		if (m_fPolyHeight < 0.0f) {	// 値がマイナス
			m_fPolyHeight = 0.0f;
		}
	}

	for (int nCount = 0; nCount < NUM_DIGIT; nCount++)
	{
		if (m_apObject[nCount] != nullptr)
		{// 使用していた場合

		 // 座標設定
			m_apObject[nCount]->SetSize(m_fPolyWidth, m_fPolyHeight);
		}
	}
}

//==========================================================
// マトリックス設定
//==========================================================
void CHeadUI::SetMtxParent(D3DXMATRIX *mtxParent)
{
	m_mtxParent = mtxParent;
}

//==========================================================
// 設定高さと混ぜた座標を設定
//==========================================================
void CHeadUI::SetMixPosition(void)
{
	for (int nCount = 0; nCount < NUM_DIGIT; nCount++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(5.0f + (nCount * 12.0f), 0.0f, -20.0f);	// 基準の座標

		pos.y += 2.0f;	      // 設定された高さを上げる

		if(m_apObject[nCount] != nullptr)
		{
			D3DXMATRIX mtxTrans;	// 計算用マトリックス
			D3DXMATRIX mtxParent;	// 親のマトリックス情報

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&m_mtxWorld[nCount]);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&m_mtxWorld[nCount], &m_mtxWorld[nCount], &mtxTrans);

			if (m_mtxParent != nullptr)
			{// 使用されていた場合

				mtxParent = *m_mtxParent;

				// マトリックスと親のマトリックスをかけ合わせる
				D3DXMatrixMultiply(&m_mtxWorld[nCount],
					&m_mtxWorld[nCount], &mtxParent);

				m_apObject[nCount]->SetPosition(pos);
				m_apObject[nCount]->SetCurrent(m_mtxParent);
			}
		}
	}
}