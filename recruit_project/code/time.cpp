//===============================================
//
// タイムの処理 [time.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "time.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "number.h"
#include "object2D.h"
#include "slow.h"
#include "debugproc.h"
#include "sound.h"

// マクロ定義
#define X_SPACE		(50)	// 横の移動量
#define NUM_WIDTH		(20)
#define NUM_HEIGHT	(50)

namespace {
	const D3DXVECTOR3 WARNING_SETPOS = { SCREEN_WIDTH * 1.3f, SCREEN_HEIGHT * 0.5f, 0.0f };
	const D3DXVECTOR3 WARNING_SETROT = { 0.0f, 0.0f, D3DX_PI * 0.0f };
	const D3DXVECTOR2 WARNING_SIZE = { 470.0f, 150.0f };
	const float WARNING_MOVESPEED = (-1.5f);
	const float WARNING_MOVESIN = (0.05f);
	const float WARNING_MOVESIZE = (30.0f);
}

//===============================================
// コンストラクタ
//===============================================
CTime::CTime()
{
	// 値をクリア
	for (int nCnt = 0; nCnt < NUM_PLACE; nCnt++)
	{
		m_apNumber[nCnt] = NULL;
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNum = 0;
	m_fAnimTimer = 0.0f;
	m_nIdxTexture = -1;
	m_nMaxNum = 0;
	m_fDiff = 0.0f;
	m_nStartDeltaTime = 0;
	m_nPauseTimer = 0;
	m_mode = MODE_MAX;
	m_bActive = false;
	m_pWarning = nullptr;
	m_nSound = 0;
}

//===============================================
// デストラクタ
//===============================================
CTime::~CTime()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CTime::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	m_nStartDeltaTime = timeGetTime();
	m_mode = MODE_MINUS;

	// テクスチャの読み込み
	m_nIdxTexture = pTexture->Regist(CTexture::GetFileName(CTexture::TYPE_TIMER));
	CObject2D *pObj = CObject2D::Create(6);
	pObj->SetPosition(D3DXVECTOR3(m_pos.x + X_SPACE * 1.0f + NUM_WIDTH * 1.25f, m_pos.y, 0.0f));
	pObj->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));
	pObj->SetSize(NUM_WIDTH * 5.0f, NUM_HEIGHT * 0.7f);
	pObj->BindTexture(pTexture->Regist("data\\TEXTURE\\map001.png"));

	pObj = CObject2D::Create(6);
	pObj->SetPosition(D3DXVECTOR3(m_pos.x + X_SPACE * 1.0f + NUM_WIDTH * 1.4f, m_pos.y, 0.0f));
	pObj->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pObj->SetSize(NUM_WIDTH * 0.4f, NUM_HEIGHT * 0.45f);
	pObj->BindTexture(pTexture->Regist("data\\TEXTURE\\number002.png"));

	for (int nCnt = 0; nCnt < NUM_PLACE; nCnt++)
	{
		if (m_apNumber[nCnt] == NULL)
		{// 使用していない場合
			D3DXVECTOR3 pos = m_pos;	// 設定座標
			pos.x += nCnt * X_SPACE + 1.0f;	// 横移動
			m_apNumber[nCnt] = CNumber::Create(pos, NUM_WIDTH * 0.75f, NUM_HEIGHT * 0.5f);


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
void CTime::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_PLACE; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{// 使用していない場合
			m_apNumber[nCnt]->Uninit();

			delete m_apNumber[nCnt];

			m_apNumber[nCnt] = NULL;
		}
	}

	if (m_pWarning != nullptr)
	{
		m_pWarning->Uninit();
		m_pWarning = nullptr;
	}
}

//===============================================
// 更新処理
//===============================================
void CTime::Update(void)
{
	if(!m_bActive)
	{
		return;
	}

	m_fAnimTimer += CManager::GetInstance()->GetSlow()->Get();
	if (m_fAnimTimer >= 60)
	{// 12フレーム立った
		m_fAnimTimer = 0;	// カウンターリセット
		Add(-1);
	}

	// 警告更新
	if (m_pWarning == nullptr) {
		return;
	}

	{
		m_nSound++;
		if (m_nSound == 1)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_WARNING);
		}

		D3DXVECTOR3 pos = m_pWarning->GetPosition();

		// 移動
		if (pos.x > SCREEN_WIDTH * 0.75f)
		{
			pos.x -= 30.0f;
		}
		else if (pos.x < SCREEN_WIDTH * 0.35f)
		{
			pos.x -= 40.0f;
			m_pWarning->SetLength(m_pWarning->GetWidth() * 1.7f, m_pWarning->GetHeight() * 1.7f);
		}
		else
		{
			pos.x += WARNING_MOVESPEED;

			// 大きくしたり小さくする
			m_fWarningSin += WARNING_MOVESIN;
			float fSin = sinf(m_fWarningSin);
			if (fSin >= 0.0f) {
				m_pWarning->SetLength(WARNING_SIZE.x + fSin * WARNING_MOVESIZE, WARNING_SIZE.y + fSin * WARNING_MOVESIZE);
			}
		}

		m_pWarning->SetPosition(pos);
		m_pWarning->SetVtx();

		if (pos.x < -SCREEN_WIDTH * 0.5f)
		{
			m_pWarning->Uninit();
			m_pWarning = NULL;
			m_nSound = 0;
		}
	}
}

//===============================================
// 生成
//===============================================
CTime *CTime::Create(const D3DXVECTOR3& pos)
{
	CTime *pTime = NULL;

	// オブジェクト2Dの生成
	pTime = new CTime;

	if (pTime != NULL)
	{// 生成できた場合

		pTime->m_pos = pos;

		// 初期化処理
		pTime->Init();
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}


	return pTime;
}

//===============================================
// 加算
//===============================================
void CTime::Add(int nValue)
{
	// 代入
	m_nNum += nValue;

	// 数値設定
	SetValue();

	// 警告メッセージ生成確認
	if (m_nNum != 60) {	// 残り1分ではない
		return;
	}

	if (m_pWarning != nullptr) {	// すでに警告が表示中
		return;
	}

	// 警告を生成
	m_pWarning = CObject2D::Create(WARNING_SETPOS, WARNING_SETROT, 6);

	if (m_pWarning != nullptr) {	// 生成できた
		m_pWarning->SetLength(WARNING_SIZE.x, WARNING_SIZE.y);
		m_pWarning->BindTexture(CTexture::TYPE_WARNING);
	}
}

//===============================================
// 設定
//===============================================
void CTime::Set(int nValue)
{
	// 代入
	m_nNum = nValue;
	m_nSetNum = nValue;

	// 数値設定
	SetValue();
}

//===============================================
// 設定値
//===============================================
void CTime::SetNum(int nValue)
{
	// 代入
	m_nNum = nValue;

	// 数値設定
	SetValue();
}

//===============================================
// 桁ごとの値設定
//===============================================
void CTime::SetValue(void)
{
	int aTexU[NUM_PLACE] = {};	// 計算格納用

	if (m_nNum < 0)
	{// 0以下の場合
		m_nNum = 0;
	}
	else if ((m_nNum - m_nNum % 100) * 0.01f >= 60 * 100)
	{
		m_nNum = 60 * 100 - 1;
	}

	int nNum = m_nNum;
	aTexU[0] = nNum / 60 / 10;
	aTexU[1] = nNum / 60 - aTexU[0] * 10;
	aTexU[2] = nNum % 60 / 10;
	aTexU[3] = nNum % 60 - aTexU[2] * 10;

	//スコアを各配列に格納
	for (int nCnt = 0; nCnt < NUM_PLACE; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{// 使用されている場合
			// 値を設定
			m_apNumber[nCnt]->SetIdx(aTexU[nCnt]);
		}
	}
}

//===============================================
// 色設定
//===============================================
void CTime::SetColor(D3DXCOLOR col)
{
	for (int nCnt = 0; nCnt < NUM_PLACE; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{// 使用していない場合
			m_apNumber[nCnt]->GetObject2D()->SetCol(col);
		}
	}
}