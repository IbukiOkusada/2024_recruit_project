//==========================================================
//
// フィルターの処理 [filter.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "filter.h"
#include "manager.h"
#include "game.h"
#include "texture.h"
#include "slow.h"
#include "renderer.h"

//===============================================
// マクロ定義
//===============================================
#define GUN_DEFHEIGHT	(700)				// デフォルトから追加で画面外に生成する高さ
#define GUN_DEFWIDTH	(GUN_DEFHEIGHT * 2)	// デフォルトから追加で画面外に生成する幅
#define GUN_MULTIWIDTH	(2)					// 倍率
#define GUN_MOVE		(-100.0f)
#define GUN_COLA		(0.1f)

//====================================================================================
// オブジェクト2Dクラス
//====================================================================================
//===============================================
// コンストラクタ
//===============================================
//CFilter::CFilter()
//{
//	// 値をクリアする
//}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CFilter::CFilter(int nPriority) : CObject2D(nPriority)
{
	// 値をクリアする
	m_fCola = 0.0f;
	m_fSpeed = 0.0f;
}

//===============================================
// デストラクタ
//===============================================
CFilter::~CFilter()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CFilter::Init(void)
{
	CObject2D::Init();

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	m_fSpeed = GUN_MOVE;
	m_fCola = GUN_COLA;
	m_state = STATE_START;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CFilter::Uninit(void)
{
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CFilter::Update(void)
{
	float fWidth = GetWidth();
	float fHeight = GetHeight();

	switch (m_state)
	{
	case STATE_NONE:
		break;

	case STATE_NORMAL:

		if (CManager::GetSlow()->Get() > CManager::GetSlow()->GetOld())
		{
			m_state = STATE_END;
		}

		break;

	case STATE_START:

		// サイズ変更
		fWidth += m_fSpeed * GUN_MULTIWIDTH;
		fHeight += m_fSpeed;
		m_col.a += m_fCola;

		if (fWidth < SCREEN_WIDTH * 0.5f || fHeight <= SCREEN_HEIGHT * 0.5f || (CManager::GetSlow()->Get() == 1.0f && CManager::GetSlow()->GetOld() != 1.0f))
		{// 規定値にたどり着いた
			fWidth = SCREEN_WIDTH * 0.5f;
			fHeight = SCREEN_HEIGHT * 0.5f;
			m_state = STATE_NORMAL;
			m_fSpeed *= -1.0f;
			m_fCola *= -1.0f;
			m_col.a = 1.0f;
		}

		SetSize(fWidth, fHeight);
		SetCol(m_col);

		break;

	case STATE_END:

		// サイズ変更
		fWidth += m_fSpeed * GUN_MULTIWIDTH;
		fHeight += m_fSpeed;
		m_col.a += m_fCola;
		SetSize(fWidth, fHeight);
		SetCol(m_col);

		if (fWidth > SCREEN_WIDTH * 0.5f + GUN_DEFWIDTH || fHeight > SCREEN_HEIGHT * 0.5f + GUN_DEFHEIGHT)
		{// 規定値にたどり着いた
			Uninit();
		}

		break;
	}
}

//===============================================
// 描画処理
//===============================================
void CFilter::Draw(void)
{
	CObject2D::Draw();
}

//===============================================
// 頂点情報設定
//===============================================
CFilter *CFilter::Create(TYPE type)
{
	CFilter *pObject2D = NULL;

	// オブジェクト2Dの生成
	pObject2D = new CFilter(6);

	if (pObject2D != NULL)
	{// 生成できた場合
	 // 初期化処理
		pObject2D->Init();

		pObject2D->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

		pObject2D->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		pObject2D->SetSize(SCREEN_WIDTH * 0.5f + GUN_DEFWIDTH, SCREEN_HEIGHT * 0.5f + GUN_DEFHEIGHT);

		// 種類設定
		pObject2D->SetType(TYPE_NONE);

		pObject2D->m_state = STATE_START;

		pObject2D->BindTexture(CManager::GetTexture()->Regist(CManager::GetTexture()->GetFileName(CTexture::TYPE_SLOWGUNFIL)));
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pObject2D;
}

//===============================================
// 色を取得 
//===============================================
D3DXCOLOR CFilter::GetCol(void)
{
	return m_col;
}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CHeatFilter::CHeatFilter(int nPriority) : CObject2D(nPriority)
{
	// 値をクリアする
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fTex = 0.0f;
}

//===============================================
// デストラクタ
//===============================================
CHeatFilter::~CHeatFilter()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CHeatFilter::Init(void)
{
	CObject2D::Init();

	m_col = D3DXCOLOR(1.0f, 0.7f, 0.5f, 0.0f);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CHeatFilter::Uninit(void)
{
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CHeatFilter::Update(void)
{
	D3DXVECTOR3 pos = GetPosition();
	m_move.x += (float)(rand() % 100 - 51) * 0.005f;

	if (pos.x > SCREEN_WIDTH * 0.52f)
	{
		pos.x = SCREEN_WIDTH * 0.52f;
		m_move.x = 0.0f;
	}
	else  if (pos.x < SCREEN_WIDTH * 0.48f)
	{
		pos.x = SCREEN_WIDTH * 0.48f;
		m_move.x = 0.0f;
	}

	if (pos.y > SCREEN_HEIGHT * 1.0f)
	{
		pos.y = SCREEN_HEIGHT * 1.0f;
		m_move.y = 0.0f;
	}
	else  if (pos.y < SCREEN_HEIGHT * 0.5f)
	{
		pos.y = SCREEN_HEIGHT * 0.5f;
		m_move.y = 0.0f;
	}

	m_fTex += (float)(rand() % 100 - 51) * 0.1f;

	if (m_fTex > SCREEN_WIDTH * 0.25f)
	{
		m_fTex = SCREEN_WIDTH * 0.25f;
	}
	else if (m_fTex < -SCREEN_WIDTH * 0.125f)
	{
		m_fTex = -SCREEN_WIDTH * 0.125f;
	}

	if (m_col.a > 0.4f)
	{
		m_col.a = 0.4f;
	}

	SetPosition(pos);
	SetSize(SCREEN_WIDTH * 0.75f + m_fTex, SCREEN_HEIGHT * 0.75f + m_fTex * 0.25f);
	SetCol(m_col);
}

//===============================================
// 描画処理
//===============================================
void CHeatFilter::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject2D::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===============================================
// 頂点情報設定
//===============================================
CHeatFilter *CHeatFilter::Create()
{
	CHeatFilter *pObject2D = NULL;

	// オブジェクト2Dの生成
	pObject2D = new CHeatFilter(7);

	if (pObject2D != NULL)
	{// 生成できた場合
		// 初期化処理
		pObject2D->Init();

		pObject2D->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 1.0f, 0.0f));

		pObject2D->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		pObject2D->SetSize(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.75f);

		pObject2D->SetCol(pObject2D->m_col);

		// 種類設定
		pObject2D->SetType(TYPE_NONE);

		pObject2D->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\smake001.jpg"));
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pObject2D;
}

//===============================================
// フィルターの濃度
//===============================================
void CHeatFilter::SetFil(float fDiff)
{
	if (fDiff > 1.0f)
	{
		fDiff = 1.0f;
	}

	m_col.a = 0.4f * fDiff;
	SetPosition(D3DXVECTOR3(GetPosition().x, SCREEN_HEIGHT * 1.0f + (SCREEN_HEIGHT * 0.5f - SCREEN_HEIGHT * 1.0f) * fDiff, GetPosition().z));
}