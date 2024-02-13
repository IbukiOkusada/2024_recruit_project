//==========================================================
//
// ゴール [goal.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "goal.h"
#include "model.h"
#include "manager.h"
#include "debugproc.h"
#include "player.h"
#include "Xfile.h"
#include "meshfield.h"

// 無名名前空間
namespace {
	const D3DXVECTOR3 SETPOS = { 0.0f, 0.0f, 0.0f };	// 初期設定座標
	const char *FILENAME = "data\\TEXTURE\\line000.jpg";	// ファイル名
	const float HEIGHT = (25.0f);
}

// 静的メンバ変数宣言
CGoal *CGoal::m_pTop = nullptr;	// 先頭のオブジェクトへのポインタ
CGoal *CGoal::m_pCur = nullptr;	// 最後尾のオブジェクトへのポインタ

//==========================================================
// コンストラクタ
//==========================================================
CGoal::CGoal()
{
	// 値のクリア
	m_pos = SETPOS;
	m_rot = SETPOS;
	m_pNext = nullptr;
	m_pPrev = nullptr;

	// 自分自身をリストに追加
	if (m_pTop != nullptr)
	{// 先頭が存在している場合
		m_pCur->m_pNext = this;	// 現在最後尾のオブジェクトのポインタにつなげる
		m_pPrev = m_pCur;
		m_pCur = this;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合
		m_pTop = this;	// 自分自身が先頭になる
		m_pCur = this;	// 自分自身が最後尾になる
	}
}

//==========================================================
// デストラクタ
//==========================================================
CGoal::~CGoal()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGoal::Init(void)
{
	m_pObject = CMeshField::Create(SETPOS, SETPOS, 0.0f, 0.0f, FILENAME);
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGoal::Uninit(void)
{
	if (m_pObject != nullptr) {
		m_pObject->Uninit();
		m_pObject = nullptr;
	}

	// リストから削除
	ListOut();

	// リリース
	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CGoal::Update(void)
{
	if (m_pObject != nullptr) {
		m_pObject->SetPosition(m_pos);
		m_pObject->SetRotation(m_rot);
	}
}

//==========================================================
// 生成
//==========================================================
CGoal *CGoal::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, float fWidth)
{
	CGoal *pGoal = nullptr;

	pGoal = new CGoal;

	if (pGoal != nullptr)
	{
		// 初期化処理
		pGoal->Init();

		// 値の設定
		pGoal->SetPosition(pos);
		pGoal->SetRotation(rot);

		if (pGoal->m_pObject != nullptr) {	// フィールドの設定
			pGoal->m_pObject->SetSize(fWidth, HEIGHT);
		}
	}

	return pGoal;
}

//==========================================================
// リストから外す
//==========================================================
void CGoal::ListOut(void)
{
	// リストから自分自身を削除する
	if (m_pTop == this)
	{// 自身が先頭
		if (m_pNext != nullptr)
		{// 次が存在している
			m_pTop = m_pNext;	// 次を先頭にする
			m_pNext->m_pPrev = nullptr;	// 次の前のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else if (m_pCur == this)
	{// 自身が最後尾
		if (m_pPrev != nullptr)
		{// 次が存在している
			m_pCur = m_pPrev;			// 前を最後尾にする
			m_pPrev->m_pNext = nullptr;	// 前の次のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else
	{
		if (m_pNext != nullptr)
		{
			m_pNext->m_pPrev = m_pPrev;	// 自身の次に前のポインタを覚えさせる
		}
		if (m_pPrev != nullptr)
		{
			m_pPrev->m_pNext = m_pNext;	// 自身の前に次のポインタを覚えさせる
		}
	}
}

//==========================================================
// 当たり判定
//==========================================================
bool CGoal::Collision(const D3DXVECTOR3& pos, const D3DXVECTOR3& posOld)
{
	CGoal *pGl = m_pTop;	// 先頭

	while (pGl != nullptr) {
		CGoal *pGlNext = pGl->m_pNext;
		if (pGl->CollisionCheck(pos, posOld)) {	// 跨いだ
			return true;
		}

		pGl = pGlNext;
	}

	return false;
}

//==========================================================
// 判定確認
//==========================================================
bool CGoal::CollisionCheck(const D3DXVECTOR3& pos, const D3DXVECTOR3& posOld) 
{
	if (m_pObject == nullptr) {	// オブジェクトがない
		return false;
	}

	float fWidth = m_pObject->GetWidth() * 0.5f;

	if ((m_rot.y == SETPOS.y) || (m_rot.y == D3DX_PI) || (m_rot.y == -D3DX_PI)) {	// 初期値と同じ向き
		if ((pos.x >= m_pos.x + (-fWidth * m_pObject->GetNumWidth())) &&
			(pos.x <= m_pos.x + (fWidth * m_pObject->GetNumWidth()))) {	// 範囲内

			if ((posOld.z >= m_pos.z && pos.z <= m_pos.z) ||
				(pos.z >= m_pos.z && posOld.z <= m_pos.z)) {		// 跨いだ
				return true;
			}
		}
	}
	else   // 90度回転
	{
		if ((pos.z >= m_pos.z + (-fWidth * m_pObject->GetNumWidth())) &&
			(pos.z <= m_pos.z + (fWidth * m_pObject->GetNumWidth()))) {	// 範囲内

			if ((posOld.x >= m_pos.x && pos.x <= m_pos.x) ||
				(pos.x >= m_pos.x && posOld.x <= m_pos.x)) {		// 跨いだ
				return true;
			}
		}
	}

	return false;
}