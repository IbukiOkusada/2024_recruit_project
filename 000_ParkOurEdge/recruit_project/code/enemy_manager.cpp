//==========================================================
//
// 敵マネージャー [enemy_manager.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "enemy_manager.h"
#include "enemy.h"
#include "lockon.h"

// 静的メンバ変数宣言
CEnemyManager* CEnemyManager::m_pInstance = nullptr;	// インスタンス

//==========================================================
// コンストラクタ
//==========================================================
CEnemyManager::CEnemyManager()
{
	// 値のクリア
	m_pCur = nullptr;
	m_pTop = nullptr;
	m_nNum = 0;
}

//==========================================================
// デストラクタ
//==========================================================
CEnemyManager::~CEnemyManager()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CEnemyManager::Init(void)
{
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CEnemyManager::Uninit(void)
{

}

//==========================================================
// 更新処理
//==========================================================
void CEnemyManager::Update(void)
{

}

//==========================================================
// インスタンスを確保
//==========================================================
CEnemyManager* CEnemyManager::GetInstance(void)
{
	if (m_pInstance == nullptr) {	// 使われていない
		m_pInstance = new CEnemyManager;
	}

	return m_pInstance;
}

//==========================================================
// インスタンスをリリース
//==========================================================
void CEnemyManager::Release(void)
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
void CEnemyManager::ListIn(CEnemy* pEnemy)
{
	if (m_pTop != nullptr)
	{// 先頭が存在している場合
		m_pCur->SetNext(pEnemy);	// 現在最後尾のオブジェクトのポインタにつなげる
		pEnemy->SetPrev(m_pCur);
		m_pCur = pEnemy;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合
		m_pTop = pEnemy;	// 自分自身が先頭になる
		m_pCur = pEnemy;	// 自分自身が最後尾になる
	}

	m_nNum++;
}

//==========================================================
// リストから外す
//==========================================================
void CEnemyManager::ListOut(CEnemy* pEnemy)
{
	// リストから自分自身を削除する
	if (m_pTop == pEnemy)
	{// 自身が先頭
		if (pEnemy->GetNext() != nullptr)
		{// 次が存在している
			m_pTop = pEnemy->GetNext();	// 次を先頭にする
			m_pTop->SetPrev(nullptr);	// 次の前のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else if (m_pCur == pEnemy)
	{// 自身が最後尾
		if (pEnemy->GetPrev() != nullptr)
		{// 次が存在している
			m_pCur = pEnemy->GetPrev();		// 前を最後尾にする
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
		if (pEnemy->GetNext() != nullptr)
		{
			pEnemy->GetNext()->SetPrev(pEnemy->GetPrev());	// 自身の次に前のポインタを覚えさせる
		}
		if (pEnemy->GetPrev() != nullptr)
		{
			pEnemy->GetPrev()->SetNext(pEnemy->GetNext());	// 自身の前に次のポインタを覚えさせる
		}
	}

	m_nNum--;
}

//==========================================================
// 攻撃ヒット確認
//==========================================================
bool CEnemyManager::Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage, CEnemy* pTarget)
{
	CEnemy* pEnemy = m_pTop;
	bool bhit = false;

	//個別判定
	while (pEnemy != nullptr) {
		CEnemy* pEnemyNext = pEnemy->GetNext();
		if (pTarget != nullptr && pTarget != pEnemy) { // 目標が存在していてかつ確認しているのと一致していない
			pEnemy = pEnemyNext;
			continue;
		}
		if (pEnemy->Hit(pos, fRange, nDamage)) { // 当たった
			bhit = true; 
			CLockOn::Check(pEnemy);
		}	
		pEnemy = pEnemyNext;
	}

	return bhit;
}

//==========================================================
// 衝突判定
//==========================================================
void CEnemyManager::Bump(D3DXVECTOR3& pos, const float fRange, CEnemy* pMy)
{
	CEnemy* pEnemy = m_pTop;

	//個別判定
	while (pEnemy != nullptr) {
		CEnemy* pEnemyNext = pEnemy->GetNext();
		if (pMy == pEnemy) { // 自分自身の場合
			pEnemy = pEnemyNext;
			continue;
		}

		// 距離を取得
		D3DXVECTOR3 EnemyPos = pEnemy->GetPosition();
		float fLength = sqrtf((pos.x - EnemyPos.x) * (pos.x - EnemyPos.x)
			+ (pos.z - EnemyPos.z) * (pos.z - EnemyPos.z));
		float fFusionRange = fRange + pEnemy->GetBumpSize();

		if (fLength > fFusionRange) {	// 距離が近い
			pEnemy = pEnemyNext;
			continue;
		}

		// 触れているのでめり込みを直す
		D3DXVECTOR3 vec = D3DXVECTOR3(pos.x - EnemyPos.x, pos.y - EnemyPos.y, pos.z - EnemyPos.z);
		D3DXVec3Normalize(&vec, &vec);
		pos = D3DXVECTOR3(EnemyPos.x + fFusionRange * vec.x, pos.y, EnemyPos.z + fFusionRange * vec.z);

		pEnemy = pEnemyNext;
	}
}