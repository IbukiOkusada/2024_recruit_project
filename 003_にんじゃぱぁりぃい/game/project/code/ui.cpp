//==========================================================
//
// UI [ui.cpp]
// Author : Yuuka Sakuma
//
//==========================================================
#include "ui.h"
#include "object2D.h"
#include "manager.h"
#include "texture.h"
#include "life.h"

// マクロ定義
#define TEXTURE_MONEY	("data\\TEXTURE\\money0.png")	//お金の単位のテクスチャ
#define MONEY_POSY	(25.0f)		//単位の場所

// ファイル名
const char *CUI::m_apFrameFileName[FRAME_MAX] =
{
	"data\\TEXTURE\\frame0.png",
	"data\\TEXTURE\\frame1.png",
	"data\\TEXTURE\\frame2.png",
	"data\\TEXTURE\\frame3.png",
};

// ファイル名
const char *CUI::m_apIconFileName[ICON_MAX] =
{
	"data\\TEXTURE\\player_icon0.png",
	"data\\TEXTURE\\player_icon1.png",
	"data\\TEXTURE\\player_icon2.png",
	"data\\TEXTURE\\player_icon3.png",
};

//==========================================================
// コンストラクタ
//==========================================================
CUI::CUI()
{
	// 値のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;
	m_type = TYPE_NORMAL;
	m_nLife = 0;
	m_nNumPlayer = 0;

}

//==========================================================
// デストラクタ
//==========================================================
CUI::~CUI()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CUI::Init(void)
{
	for (int nCnt = 0; nCnt < NUM_OBJ; nCnt++)
	{
		m_pObject[nCnt] = CObject2D::Create();
	}

	m_pObject[0]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\frame0.png"));
	m_pObject[0]->SetPosition(m_pos);
	m_pObject[0]->SetRotation(m_rot);
	m_pObject[0]->SetLength(200.0f, 125.0f);

	m_pObject[1]->BindTexture(CManager::GetInstance()->GetTexture()->Regist(TEXTURE_MONEY));
	m_pObject[1]->SetPosition(D3DXVECTOR3(m_pos.x + 50.0f, m_pos.y + 38.0f, m_pos.z));
	m_pObject[1]->SetRotation(m_rot);
	m_pObject[1]->SetLength(50.0f, 25.0f);

	m_pObject[2]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\player_icon0.png"));
	m_pObject[2]->SetPosition(D3DXVECTOR3(m_pos.x - 110.0f, m_pos.y - 20.0f, m_pos.z));
	m_pObject[2]->SetRotation(m_rot);
	m_pObject[2]->SetLength(70.0f, 70.0f);

	return S_OK;
}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CUI::Init(int nFrame, int nIcon)
{
	for (int nCnt = 0; nCnt < NUM_OBJ; nCnt++)
	{
		m_pObject[nCnt] = CObject2D::Create(6);
	}

	m_nLife = GetLife();
	m_nNumPlayer = nFrame;

	//フレーム
	m_pObject[0]->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apFrameFileName[nFrame]));
	m_pObject[0]->SetPosition(m_pos);
	m_pObject[0]->SetLength(250.0f, 100.0f);

	//お金の単位
	m_pObject[1]->BindTexture(CManager::GetInstance()->GetTexture()->Regist(TEXTURE_MONEY));
	m_pObject[1]->SetLength(50.0f, 25.0f);

	//顔
	m_pObject[2]->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apIconFileName[nIcon]));
	m_pObject[2]->SetLength(45.0f, 45.0f);

	//タイプごとの位置設定
	if (nFrame == 0)
	{
		m_pObject[1]->SetPosition(D3DXVECTOR3(m_pos.x + 80.0f, m_pos.y - MONEY_POSY, m_pos.z));	//単位
		m_pObject[2]->SetPosition(D3DXVECTOR3(m_pos.x - 95.0f, m_pos.y + 15.0f, m_pos.z));	//顔
		CLife *pLife = CLife::Create(D3DXVECTOR3(m_pos.x - 50.0f, m_pos.y + 20.0f, m_pos.z), m_rot);
		pLife->SetLife(m_nLife);

	}
	else if (nFrame == 1)
	{
		m_pObject[1]->SetPosition(D3DXVECTOR3(m_pos.x + 90.0f, m_pos.y - MONEY_POSY, m_pos.z)); //単位
		m_pObject[2]->SetPosition(D3DXVECTOR3(m_pos.x + 90.0f, m_pos.y + 20.0f, m_pos.z));	//顔
		CLife *pLife = CLife::Create(D3DXVECTOR3(m_pos.x - 50.0f, m_pos.y + 20.0f, m_pos.z), m_rot);
		pLife->SetLife(m_nLife);
	}
	else if (nFrame == 2)
	{
		m_pObject[1]->SetPosition(D3DXVECTOR3(m_pos.x + 80.0f, m_pos.y + MONEY_POSY, m_pos.z)); //単位
		m_pObject[2]->SetPosition(D3DXVECTOR3(m_pos.x - 95.0f, m_pos.y - 20.0f, m_pos.z));	//顔
		CLife *pLife = CLife::Create(D3DXVECTOR3(m_pos.x - 50.0f, m_pos.y - 20.0f, m_pos.z), m_rot);
		pLife->SetLife(m_nLife);
	}
	else if (nFrame == 3)
	{	
		m_pObject[1]->SetPosition(D3DXVECTOR3(m_pos.x + 90.0f, m_pos.y + MONEY_POSY, m_pos.z)); //単位
		m_pObject[2]->SetPosition(D3DXVECTOR3(m_pos.x + 90.0f, m_pos.y - 20.0f, m_pos.z));	//顔
		CLife *pLife = CLife::Create(D3DXVECTOR3(m_pos.x - 50.0f, m_pos.y - 20.0f , m_pos.z), m_rot);
		pLife->SetLife(m_nLife);
	}

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CUI::Uninit(void)
{
	Release();

	for (int nCnt = 0; nCnt < NUM_OBJ; nCnt++)
	{
		if (nullptr != m_pObject[nCnt]) {
			m_pObject[nCnt]->Uninit();
			m_pObject[nCnt] = NULL;
		}
	}

	if (nullptr != m_pLife) {
		m_pLife->Uninit();
		m_pLife = NULL;
	}
}

//==========================================================
// 更新処理
//==========================================================
void CUI::Update(void)
{
	//サイズの設定
	for (int nCnt = 0; nCnt < NUM_OBJ; nCnt++)
	{
		m_pObject[nCnt]->SetVtx();
	}

	m_pLife->SetLife(m_nLife);
}

//==========================================================
// 生成
//==========================================================
CUI *CUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nFrame, int nIcon, TYPE type)
{
	CUI *pUI = nullptr;

	pUI = new CUI;

	if (pUI != nullptr)
	{
		pUI->SetPosition(pos);
		pUI->SetRotation(rot);
		pUI->SetType(type);
		// 初期化処理
		pUI->Init(nFrame, nIcon);
	}	

	return pUI;
}

//==========================================================
// 設定処理
//==========================================================
void CUI::SetLife(int nLife) 
{ 
	m_nLife = nLife;

	//タイプごとの位置設定
	if (m_pLife == NULL)
	{
		if (m_nNumPlayer == 0)
		{
			m_pLife = CLife::Create(D3DXVECTOR3(m_pos.x - 50.0f, m_pos.y + 20.0f, m_pos.z), m_rot);
			m_pLife->SetLife(m_nLife);
		}
		else if (m_nNumPlayer == 1)
		{
			m_pLife = CLife::Create(D3DXVECTOR3(m_pos.x - 50.0f, m_pos.y + 20.0f, m_pos.z), m_rot);
			m_pLife->SetLife(m_nLife);
		}
		else if (m_nNumPlayer == 2)
		{
			m_pLife = CLife::Create(D3DXVECTOR3(m_pos.x - 50.0f, m_pos.y - 20.0f, m_pos.z), m_rot);
			m_pLife->SetLife(m_nLife);
		}
		else if (m_nNumPlayer == 3)
		{
			m_pLife = CLife::Create(D3DXVECTOR3(m_pos.x - 50.0f, m_pos.y - 20.0f, m_pos.z), m_rot);
			m_pLife->SetLife(m_nLife);
		}
	}
}