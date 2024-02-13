//==========================================================
//
// ゲージ [gage.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gage.h"
#include "billboard.h"
#include "manager.h"
#include "texture.h"

// 無名名前空間
namespace {
	const int DEF_PRI = (3);	// 優先順位
	const char* FILENAME[CGage::TYPE_MAX] = {	// テクスチャファイル名
		"data\\TEXTURE\\map001.png",
		"data\\TEXTURE\\gage000.jpg"
	};
}

//==========================================================
// コンストラクタ
//==========================================================
CGage::CGage()
{
	// 値のクリア
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		m_apObject[nCnt] = nullptr;
	}
	m_fPolyHeight = 0.0f;
	m_fPolyWidth = 0.0f;
	m_fUpHeight = 0.0f;
	m_fRate = 0.0f;
	m_pPos = nullptr;
}

//==========================================================
// デストラクタ
//==========================================================
CGage::~CGage()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGage::Init(void)
{
	// オブジェクトの生成
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		m_apObject[nCnt] = CObjectBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), DEF_PRI + nCnt);
		m_apObject[nCnt]->SetAlphaText(false);
		m_apObject[nCnt]->SetLighting(true);
		m_apObject[nCnt]->SetZTest(false);
		m_apObject[nCnt]->SetFusion(CObjectBillboard::FUSION_NORMAL);
		m_apObject[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->Regist(FILENAME[nCnt]));
	}

	m_fRate = 1.0f;

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGage::Uninit(void)
{
	// オブジェクトの終了処理
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		if (m_apObject[nCnt] != nullptr) {
			m_apObject[nCnt]->Uninit();
			m_apObject[nCnt] = nullptr;
		}
	}

	// 開放
	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CGage::Update(void)
{
	// 座標の更新
	SetMixPosition();

	// サイズを設定
	{
		if (m_apObject[TYPE_CONTENT] == nullptr) {
			return;
		}

		m_apObject[TYPE_CONTENT]->SetSize(m_fPolyWidth * m_fRate, m_fPolyHeight);
	}
}

//==========================================================
// 生成
//==========================================================
CGage *CGage::Create(D3DXVECTOR3* pPos, const float fUpHeight, const float fPolyWidth, const float fPolyHeight)
{
	CGage *pGage = nullptr;

	pGage = new CGage;

	if (pGage != nullptr)
	{
		// 初期化処理
		pGage->Init();

		// 親の設定
		pGage->SetParent(pPos);

		// 相対座標の高さを設定
		pGage->SetUpHeight(fUpHeight);

		// ポリゴンサイズを設定
		pGage->SetPolySize(fPolyWidth, fPolyHeight);

		// 座標を設定
		pGage->SetMixPosition();
	}

	return pGage;
}

//==========================================================
// 割合の設定
//==========================================================
void CGage::SetRate(const float fRate)
{
	// 割合を代入
	m_fRate = fRate;

	if (m_fRate < 0.0f) {	// 値がマイナスになっている
		m_fRate = 0.0f;
	}
	else if (m_fRate > 1.0f) {	// 値が基準を超えた
		m_fRate = 1.0f;
	}

	if (m_fRate >= 1.0f) { // MAXの場合
		for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
			if (m_apObject[nCnt] == nullptr) {	// 使用されていない
				continue;
			}

			// 描画設定(しないように)
			m_apObject[nCnt]->SetDraw(false);
		}
	}
	else {	// 少しでも減っている
		for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
			if (m_apObject[nCnt] == nullptr) {	// 使用されていない
				continue;
			}

			// 描画設定(するように)
			m_apObject[nCnt]->SetDraw();
		}
	}
}

//==========================================================
// 幅と高さの適応
//==========================================================
void CGage::SetPolySize(const float fWidth, const float fHeight)
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

	// オブジェクトに適応させる
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		if (m_apObject[nCnt] == nullptr) {	// 使用されていない
			continue;
		}

		// 座標設定
		m_apObject[nCnt]->SetSize(m_fPolyWidth, m_fPolyHeight);
	}
}

//==========================================================
// 設定高さと混ぜた座標を設定
//==========================================================
void CGage::SetMixPosition(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 基準の座標
	if (m_pPos != nullptr) {	// 親が存在している
		pos = *m_pPos;
	}
	pos.y += m_fUpHeight;	// 設定された高さを上げる

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		if (m_apObject[nCnt] == nullptr) {	// 使用されていない
			continue;
		}

		// 座標設定
		m_apObject[nCnt]->SetPosition(D3DXVECTOR3(pos));
	}
}