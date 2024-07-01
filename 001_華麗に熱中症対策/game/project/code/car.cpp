//==========================================================
//
// 車の処理 [car.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "car.h"
#include "Xfile.h"
#include "manager.h"
#include "debugproc.h"
#include "shadow.h"
#include "texture.h"
#include "slow.h"

//==========================================================
// コンストラクタ
//==========================================================
CCar::CCar() : CObjectX(3)
{
	// 値のクリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pPrev = NULL;
	m_pNext = NULL;
	m_pShadow = NULL;
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//==========================================================
// デストラクタ
//==========================================================
CCar::~CCar()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CCar::Init(void)
{
	CObjectX::Init();
	m_pShadow = CShadow::Create(GetPosition(), 100.0f, 200.0f);
	m_pShadow->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\shadow001.jpg"));

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CCar::Uninit(void)
{
	CObjectX::Uninit();

	if (m_pShadow != NULL)
	{
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}
}

//==========================================================
// 更新処理
//==========================================================
void CCar::Update(void)
{
	m_posOld = GetPosition();
	D3DXVECTOR3 pos = GetPosition();
	pos += m_move * CManager::GetSlow()->Get();
	SetPosition(pos);

	if (m_pShadow != NULL)
	{
		m_pShadow->SetPosition(pos);
	}
}

//==========================================================
// 描画処理
//==========================================================
void CCar::Draw(void)
{
	CObjectX::Draw();
}

//==========================================================
// 生成
//==========================================================
CCar *CCar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *pFileName, int nType, const int nPriority)
{
	CCar *pObjectX = NULL;
	CXFile *pModelFile = CManager::GetModelFile();

	// オブジェクトXの生成
	pObjectX = new CCar;

	if (pObjectX != NULL)
	{// 生成できた場合
		// 初期化処理
		pObjectX->Init();

		// 座標
		pObjectX->SetPosition(pos);
		pObjectX->SetOldPos(pos);

		// 向き
		pObjectX->SetRotation(rot);

		// 種類設定
		pObjectX->SetType(TYPE_CAR);

		// 種類
		pObjectX->m_nType = nType;

		// 読み込み確認
		pObjectX->BindFile(pModelFile->Regist(pFileName));
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pObjectX;
}
