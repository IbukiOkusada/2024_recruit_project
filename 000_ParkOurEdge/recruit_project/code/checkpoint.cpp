//==========================================================
//
// チェックポイント処理 [checkpoinyt.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "checkpoint.h"
#include "checkpoint_manager.h"
#include "debugproc.h"
#include "manager.h"

// 名前空間
namespace {
	const float COLSIZE = (150.0f);
}

//==========================================================
// コンストラクタ
//==========================================================
CCheckPoint::CCheckPoint()
{
	// 値のクリア
	m_pos = { 0.0f, 0.0f, 0.0f };

	// リストに挿入
	CCheckPointManager::GetInstance()->ListIn(this);
}

//==========================================================
// デストラクタ
//==========================================================
CCheckPoint::~CCheckPoint()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CCheckPoint::Init(void)
{
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CCheckPoint::Uninit(void)
{
	// リストから外す
	CCheckPointManager::GetInstance()->ListOut(this);
	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CCheckPoint::Update(void)
{

}

//===============================================
// ヒット処理
//===============================================
void CCheckPoint::Hit(const D3DXVECTOR3& pos, D3DXVECTOR3& CheckPointPos)
{
	if (pos.x < m_pos.x - COLSIZE || pos.x > m_pos.x + COLSIZE ||
		pos.z < m_pos.z - COLSIZE || pos.z > m_pos.z + COLSIZE)
	{
		return;
	}

	// 自身の座標を渡して廃棄
	CheckPointPos = m_pos;
	Uninit();
}

//===============================================
// 生成
//===============================================
CCheckPoint* CCheckPoint::Create(const D3DXVECTOR3& pos)
{
	CCheckPoint* pCheckPoint = nullptr;

	pCheckPoint = new CCheckPoint;

	if (pCheckPoint != nullptr)
	{
		// 初期化処理
		pCheckPoint->Init();

		pCheckPoint->SetPosition(pos);
	}

	return pCheckPoint;
}