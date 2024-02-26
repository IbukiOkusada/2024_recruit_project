//==========================================================
//
// チェックポイントマネージャー [checkpoint_manager.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "checkpoint_manager.h"
#include "checkpoint.h"

// 静的メンバ変数宣言
CCheckPointManager* CCheckPointManager::m_pInstance = nullptr;	// インスタンス

//==========================================================
// コンストラクタ
//==========================================================
CCheckPointManager::CCheckPointManager()
{
	// 値のクリア
	m_pCur = nullptr;
	m_pTop = nullptr;
	m_nNum = 0;
}

//==========================================================
// デストラクタ
//==========================================================
CCheckPointManager::~CCheckPointManager()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CCheckPointManager::Init(void)
{
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CCheckPointManager::Uninit(void)
{

}

//==========================================================
// 更新処理
//==========================================================
void CCheckPointManager::Update(void)
{

}

//==========================================================
// インスタンスを確保
//==========================================================
CCheckPointManager* CCheckPointManager::GetInstance(void)
{
	if (m_pInstance == nullptr) {	// 使われていない
		m_pInstance = new CCheckPointManager;
	}

	return m_pInstance;
}

//==========================================================
// インスタンスをリリース
//==========================================================
void CCheckPointManager::Release(void)
{
	if (m_pInstance != nullptr) {	// インスタンスを確保されている
		m_pInstance->Uninit();
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

//==========================================================
// リストに挿入
//==========================================================
void CCheckPointManager::ListIn(CCheckPoint* pCheckPoint)
{
	if (m_pTop != nullptr)
	{// 先頭が存在している場合
		m_pCur->SetNext(pCheckPoint);	// 現在最後尾のオブジェクトのポインタにつなげる
		pCheckPoint->SetPrev(m_pCur);
		m_pCur = pCheckPoint;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合
		m_pTop = pCheckPoint;	// 自分自身が先頭になる
		m_pCur = pCheckPoint;	// 自分自身が最後尾になる
	}

	m_nNum++;
}

//==========================================================
// リストから外す
//==========================================================
void CCheckPointManager::ListOut(CCheckPoint* pCheckPoint)
{
	// リストから自分自身を削除する
	if (m_pTop == pCheckPoint)
	{// 自身が先頭
		if (pCheckPoint->GetNext() != nullptr)
		{// 次が存在している
			m_pTop = pCheckPoint->GetNext();	// 次を先頭にする
			m_pTop->SetPrev(nullptr);	// 次の前のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else if (m_pCur == pCheckPoint)
	{// 自身が最後尾
		if (pCheckPoint->GetPrev() != nullptr)
		{// 次が存在している
			m_pCur = pCheckPoint->GetPrev();		// 前を最後尾にする
			m_pCur->SetNext(nullptr);			// 前の次のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else
	{
		if (pCheckPoint->GetNext() != nullptr)
		{
			pCheckPoint->GetNext()->SetPrev(pCheckPoint->GetPrev());	// 自身の次に前のポインタを覚えさせる
		}
		if (pCheckPoint->GetPrev() != nullptr)
		{
			pCheckPoint->GetPrev()->SetNext(pCheckPoint->GetNext());	// 自身の前に次のポインタを覚えさせる
		}
	}

	m_nNum--;
}

//==========================================================
// 攻撃ヒット確認
//==========================================================
void CCheckPointManager::Hit(const D3DXVECTOR3& pos, D3DXVECTOR3& CheckPointPos)
{
	CCheckPoint* pCheckPoint = m_pTop;

	//個別判定
	while (pCheckPoint != nullptr) {
		CCheckPoint* pCheckPointNext = pCheckPoint->GetNext();
		pCheckPoint->Hit(pos, CheckPointPos);
		pCheckPoint = pCheckPointNext;
	}
}