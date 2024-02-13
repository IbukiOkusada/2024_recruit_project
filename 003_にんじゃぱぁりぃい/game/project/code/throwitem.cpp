//==========================================================
//
// 捨てるアイテム [throwitem.cpp]
// Author : Ryosuke Ohara
//
//==========================================================
#include "throwitem.h"
#include "billboard.h"
#include "manager.h"
#include "texture.h"
#include "number.h"

// 無名名前空間
namespace {
	const int DEF_PRI = (7);	// 優先順位
	const char* FILENAME[CThrowItem::TYPE_MAX] = {	// テクスチャファイル名
		"data\\TEXTURE\\arrow01.png",
		"data\\TEXTURE\\item_icon.png",
		"data\\TEXTURE\\frame.png",
		
	};
}

//==========================================================
// コンストラクタ
//==========================================================
CThrowItem::CThrowItem()
{
	// 値のクリア
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		m_apObject[nCnt] = nullptr;
	}

	m_nBeforeID = 0;
	m_nNowID = 0;
	m_nNextID = 0;
}

//==========================================================
// デストラクタ
//==========================================================
CThrowItem::~CThrowItem()
{

}

//==========================================================
// 生成
//==========================================================
CThrowItem * CThrowItem::Create(D3DXVECTOR3 * pPos, const float fUpHeight, const float fPolyWidth, const float fPolyHeight)
{
	CThrowItem *pThrowItem = nullptr;

	pThrowItem = new CThrowItem;

	if (pThrowItem != nullptr)
	{
		// 初期化処理
		pThrowItem->Init();

		// 親の設定
		pThrowItem->SetParent(pPos);

		// 相対座標の高さを設定
		pThrowItem->SetUpHeight(fUpHeight);

		// ポリゴンサイズを設定
		pThrowItem->SetPolySize(fPolyWidth, fPolyHeight);

		// 座標を設定
		pThrowItem->SetMixPosition();
	}

	return pThrowItem;
}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CThrowItem::Init(void)
{
	// ビルボード生成
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		m_apObject[nCount] = CObjectBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), DEF_PRI);
		m_apObject[nCount]->SetDraw(false);
		m_apObject[nCount]->SetAlphaText(false);
		m_apObject[nCount]->SetLighting(true);
		m_apObject[nCount]->SetZTest(false);
		m_apObject[nCount]->SetFusion(CObjectBillboard::FUSION_NORMAL);
		m_apObject[nCount]->BindTexture(CManager::GetInstance()->GetTexture()->Regist(FILENAME[nCount]));
	}
	
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CThrowItem::Uninit(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_apObject[nCount] != nullptr)
		{
			// 終了処理
			m_apObject[nCount]->Uninit();

			// 使用していない状態にする
			m_apObject[nCount] = nullptr;
		}
	}

	// 開放
	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CThrowItem::Update(void)
{
	// 座標の更新
	SetMixPosition();

	if (CManager::GetInstance()->GetMode() == CScene::MODE_TUTORIAL || CManager::GetInstance()->GetMode() == CScene::MODE_GAME)
	{// シーンがチュートリアルまたはゲームのとき

		for (int nCount = 0; nCount < TYPE_MAX; nCount++)
		{
			// 描画する
			m_apObject[nCount]->SetDraw();
		}
	}
}

//==========================================================
// 幅と高さの適応
//==========================================================
void CThrowItem::SetPolySize(const float fWidth, const float fHeight)
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

	if (m_apObject[TYPE_ITEM] != nullptr)
	{
		// 座標設定
		m_apObject[TYPE_ITEM]->SetSize(m_fPolyWidth, m_fPolyHeight);
	}

	if (m_apObject[TYPE_FRAME] != nullptr)
	{
		// 座標設定
		m_apObject[TYPE_FRAME]->SetSize(m_fPolyWidth - 50.0f, m_fPolyHeight );
	}

	if (m_apObject[TYPE_ARROW] != nullptr)
	{
		// 座標設定
		m_apObject[TYPE_ARROW]->SetSize(m_fPolyWidth - 15.0f, m_fPolyHeight - 10.0f);
	}
}

//==========================================================
// 設定高さと混ぜた座標を設定
//==========================================================
void CThrowItem::SetMixPosition(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 基準の座標
	if (m_pPos != nullptr) {	// 親が存在している
		pos = *m_pPos;
	}
	pos.y += m_fUpHeight;	// 設定された高さを上げる

	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_apObject[nCount] != nullptr)
		{
			if (nCount == TYPE_ARROW)
			{
				// 座標設定
				m_apObject[nCount]->SetPosition(D3DXVECTOR3(pos.x, pos.y - 20.0f, pos.z));
			}
			else
			{
				// 座標設定
				m_apObject[nCount]->SetPosition(D3DXVECTOR3(pos));
			}
		}
	}
}

//==========================================================
// どのアイテムを選択しているか
//==========================================================
void CThrowItem::SetItem(int nThrowItemID)
{
	// 現在選択している番号
	m_nNowID = nThrowItemID;

	if (m_apObject[TYPE_ITEM] != nullptr)
	{
		m_apObject[TYPE_ITEM]->SetVtx(((float)(m_nNowID - 1) * 0.09f), 1.0f);
	}
}