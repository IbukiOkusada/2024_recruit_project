//==========================================================
//
// ロックオンの処理 [lockon.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "lockon.h"
#include "manager.h"
#include "texture.h"
#include "debugproc.h"
#include "renderer.h"
#include "objectX.h"
#include "model.h"
#include "camera.h"
#include "slow.h"
#include "enemy.h"
#include "character.h"
#include "sound.h"
#include "billboard.h"
#include "object_manager.h"
#include "enemy_manager.h"

// マクロ定義
#define MULTIDEF_SIZE	(50.0f)		// デフォルトサイズ
#define MULTISTART_SIZE	(1000.0f)	// 表示開始サイズ
#define MULTIMINUS_SIZE	(120.0f)	// サイズ変更
#define MULTIADD_COLA	(0.1f)		// 色
#define DEF_SIZE		(35.0f)		// デフォルトサイズ
#define START_SIZE		(300.0f)	// 表示開始サイズ
#define MINUS_SIZE		(30.0f)		// サイズ変更
#define ADD_COLA		(0.1f)		// 色
#define SHOT_LENGTH		(1500.0f)
#define MAX_LOCKON		(12)

// 静的メンバ変数
CLockOn *CLockOn::m_pTop = NULL;	// 先頭のオブジェクトへのポインタ
CLockOn *CLockOn::m_pCur = NULL;	// 最後尾のオブジェクトへのポインタ
int CLockOn::m_nNum = 0;

//==========================================================
// コンストラクタ
//==========================================================
CLockOn::CLockOn()
{
	// 値のクリア
	m_bLock = false;
	m_pMtx = NULL;
	m_bDeath = false;
	m_pNext = NULL;
	m_pPrev = NULL;
	m_pObj = nullptr;

	// 自分自身をリストに追加
	if (m_pTop != NULL)
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

	m_nNum++;
}

//==========================================================
// デストラクタ
//==========================================================
CLockOn::~CLockOn()
{
	m_nNum--;
}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CLockOn::Init(void)
{
	m_pObj = CObjectBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), NUM_PRIORITY);
	// 色を変更
	m_pObj->SetVtx(D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f));

	m_pObj->SetSize(MULTISTART_SIZE, MULTISTART_SIZE);

	m_pObj->BindTexture(CTexture::TYPE_LOCKON);

	m_pObj->SetDraw(false);

	m_bUse = false;

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CLockOn::Uninit(void)
{

	m_bDeath = true;

	DeathCheck();

	if (m_pObj != nullptr) {
		m_pObj->Uninit();
		m_pObj = nullptr;
	}

	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CLockOn::Update(void)
{
	if (m_pObj == nullptr) {
		return;
	}

	if (m_bUse == false && m_type == TYPE_TARGET)
	{
		return;
	}
	else if (m_type != TYPE_TARGET)
	{
		m_pObj->SetPosition(D3DXVECTOR3(m_pTarget->GetMtx()->_41, m_pTarget->GetMtx()->_42 + 50.0f, m_pTarget->GetMtx()->_43));

		float fSize = m_pObj->GetWidth();
		D3DXCOLOR col = m_pObj->GetCol();
		D3DXVECTOR3 rot = m_pObj->GetRotation();

		if (fSize > MULTIDEF_SIZE)
		{// 既定のサイズより大きい
			fSize -= MULTIMINUS_SIZE;
			col.a += MULTIADD_COLA;
			rot.z += D3DX_PI * 0.025f;

			if (fSize < MULTIDEF_SIZE)
			{
				fSize = MULTIDEF_SIZE;
				rot.z = 0.0f;
			}

			if (col.a > 1.0f)
			{
				col.a = 1.0f;
			}

			if (rot.z > D3DX_PI)
			{
				rot.z += -D3DX_PI * 2;
			}

			m_pObj->SetRotation(rot);
			m_pObj->SetSize(fSize, fSize);
			m_pObj->SetCol(col);
		}

		return;
	}

	// ロックオン処理
	LockOn();

	float fSize = m_pObj->GetWidth();
	D3DXCOLOR col = m_pObj->GetCol();
	D3DXVECTOR3 rot = m_pObj->GetRotation();

	if (fSize > DEF_SIZE)
	{// 既定のサイズより大きい
		fSize -= MINUS_SIZE;
		col.a += ADD_COLA;

		if (fSize < DEF_SIZE)
		{
			fSize = DEF_SIZE;
			col.a = 1.0f;
		}

		if (col.a > 1.0f)
		{
			col.a = 1.0f;
		}

		m_pObj->SetSize(fSize, fSize);
		m_pObj->SetCol(col);
	}
}

//==========================================================
// 生成
//==========================================================
CLockOn *CLockOn::Create(D3DXMATRIX *pMtx, TYPE type)
{
	CLockOn *pLock = NULL;

	// 生成
	pLock = new CLockOn();

	if (pLock == NULL)
	{// 確保できなかった場合
		return NULL;
	}

	// 初期化処理
	pLock->Init();

	// 親のマトリックス設定
	pLock->SetParent(pMtx);

	// 種類設定
	pLock->m_type = type;

	return pLock;
}

//==========================================================
// ロックオンターゲット設定
//==========================================================
void CLockOn::LockOn(void)
{
	CEnemy* pOldObj = NULL;
	D3DXMATRIX mtxProjection;
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 EnemyPos;
	D3DXVECTOR3 ScreenPos;
	D3DVIEWPORT9 Viewport;
	float fMultiScreen = 0.0f;	// 判定範囲

	if (CManager::GetInstance()->GetSlow()->Get() >= 1.0f) {
		return;
	}

	if (m_pObj == nullptr) {
		return;
	}

	if (m_pMtx == nullptr) {
		return;
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetViewport(&Viewport);
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	CEnemy* pObj = CEnemyManager::GetInstance()->GetTop();	// 先頭を取得

	while (pObj != NULL)
	{// 使用されていない状態まで

		CEnemy* pObjectNext = pObj->GetNext();	// 次のオブジェクトへのポインタを取得

		if (pObj == NULL)
		{// 使用されていない場合
			pObj = pObjectNext;	// 次のオブジェクトに移動
			continue;
		}

		if (pObj->GetDeath() == true)
		{
			pObj = pObjectNext;	// 次のオブジェクトに移動
			continue;
		}

		EnemyPos.x = pObj->GetMtx()->_41;
		EnemyPos.y = pObj->GetMtx()->_42;
		EnemyPos.z = pObj->GetMtx()->_43;

		D3DXVec3Project(&ScreenPos, &EnemyPos, &Viewport, &mtxProjection, &mtxView, &mtxWorld);

		if (ScreenPos.x < SCREEN_WIDTH * fMultiScreen || ScreenPos.x > SCREEN_WIDTH * (1.0f - fMultiScreen) ||
			ScreenPos.y < SCREEN_HEIGHT * fMultiScreen || ScreenPos.y > SCREEN_HEIGHT * (1.0f - fMultiScreen) || ScreenPos.z >= 1.0f)
		{// 画面に描画されていない
			pObj = pObjectNext;	// 次のオブジェクトに移動
			pObj = NULL;
			continue;
		}

		CLockOn* pLock = m_pTop;	// 先頭を取得

		while (pLock != NULL)
		{// 使用されている間繰り返し
			CLockOn* pLockNext = pLock->m_pNext;	// 次を保持

			if (pLock->m_bDeath == false && pLock->m_type == TYPE_MULTI)
			{
				if (pLock->m_pTarget == pObj)
				{// 同じ標的の場合
					pObj = NULL;
					break;
				}
			}

			pLock = pLockNext;	// 次に移動
		}

		if (pOldObj != NULL && pObj != nullptr)
		{// 一番近いオブジェクトがある場合

			D3DXVECTOR3 OldPos = pOldObj->GetPosition();

			// 一番近いモデルとプレイヤーの距離を求める
			float fOldObjLength =
				sqrtf((OldPos.x - m_pMtx->_41) * (OldPos.x - m_pMtx->_41)
					+ (OldPos.z - m_pMtx->_43) * (OldPos.z - m_pMtx->_43));

			// 今回のモデルとプレイヤーの距離を求める
			float fObjLength =
				sqrtf((EnemyPos.x - m_pMtx->_41) * (EnemyPos.x - m_pMtx->_41)
					+ (EnemyPos.z - m_pMtx->_43) * (EnemyPos.z - m_pMtx->_43));

			if (fObjLength < fOldObjLength)
			{// 今回のモデルの方が近い場合
				pOldObj = pObj;	// 一番近いモデルを変更
			}
		}
		else
		{// 前回モデルを覚えていない場合
			if (pObj != NULL)
			{
				pOldObj = pObj;
			}
		}

		pObj = pObjectNext;	// 次のオブジェクトに移動
	}

	if (pOldObj != NULL)
	{
		pObj = pOldObj;
	}

	if (pObj != NULL)
	{// 使用されている場合

		EnemyPos.x = pObj->GetMtx()->_41;
		EnemyPos.y = pObj->GetMtx()->_42;
		EnemyPos.z = pObj->GetMtx()->_43;
		// 今回のモデルとプレイヤーの距離を求める
		float fObjLength =
			sqrtf((EnemyPos.x - m_pMtx->_41) * (EnemyPos.x - m_pMtx->_41)
				+ (EnemyPos.z - m_pMtx->_43) * (EnemyPos.z - m_pMtx->_43));

		if (fObjLength >= SHOT_LENGTH)
		{
			m_bLock = false;
			m_pObj->SetSize(0.0f, 0.0f);
			m_pTarget = NULL;
			return;
		}

		
		{// スローしている
			if (CManager::GetInstance()->GetSlow()->Get() < 1.0f)
			{
				m_bLock = false;
				m_pObj->SetSize(0.0f, 0.0f);
				m_pTarget = NULL;

				if (m_nNum < MAX_LOCKON) {
					CLockOn* pLock = CLockOn::Create(pObj->GetMtx(), CLockOn::TYPE_MULTI);
					pLock->SetTag(pObj);
					pLock->GetObj()->SetCol(D3DXCOLOR(m_nNum * 0.1f, 1.0f - m_nNum * 0.1f, 0.5f, 0.3f));
					pLock->SetLock(true);
					pLock->GetObj()->SetDraw(true);
				}
			}
			else
			{
				m_bLock = false;
				m_pObj->SetSize(0.0f, 0.0f);
				m_pTarget = NULL;
			}
		}
	}
	else
	{
		m_bLock = false;
		m_pObj->SetSize(0.0f, 0.0f);
		m_pObj->SetDraw(false);
		m_pTarget = NULL;
	}

	//if (pObj != NULL)
	//{// 使用されている場合
	//	EnemyPos.x = pObj->GetMtx()->_41;
	//	EnemyPos.y = pObj->GetMtx()->_42;
	//	EnemyPos.z = pObj->GetMtx()->_43;
	//	// 今回のモデルとプレイヤーの距離を求める
	//	float fObjLength =
	//		sqrtf((EnemyPos.x - m_pMtx->_41) * (EnemyPos.x - m_pMtx->_41)
	//			+ (EnemyPos.z - m_pMtx->_43) * (EnemyPos.z - m_pMtx->_43));

	//	if (fObjLength >= SHOT_LENGTH)
	//	{
	//		m_bLock = false;
	//		m_pObj->SetSize(0.0f, 0.0f);
	//		m_pTarget = NULL;
	//		return;
	//	}	
	//	m_pObj->SetPosition(D3DXVECTOR3(pOldObj->GetPosition().x, pOldObj->GetPosition().y + 30.0f, pOldObj->GetPosition().z));
	//	if (m_pTarget != pObj)
	//	{
	//		m_pObj->SetSize(START_SIZE, START_SIZE);
	//		m_pObj->SetCol(D3DXCOLOR(m_pObj->GetCol().r, m_pObj->GetCol().g, m_pObj->GetCol().b, 0.3f));
	//		m_pObj->SetDraw(true);
	//	}
	//	m_bLock = true;
	//	m_pTarget = pObj;
	//}
	//else
	//{
	//	m_bLock = false;
	//	m_pObj->SetSize(0.0f, 0.0f);
	//	m_pObj->SetDraw(false);
	//	m_pTarget = NULL;
	//}
}

//==========================================================
// 使用する親マトリックスを設定
//==========================================================
void CLockOn::SetParent(D3DXMATRIX *pMtx)
{
	// 親のマトリックスを指定
	m_pMtx = pMtx;
}

//===============================================
// 死亡確認
//===============================================
void CLockOn::DeathCheck(void)
{
	if (m_bDeath == true)
	{
		// リストから自分自身を削除する
		if (m_pTop == this)
		{// 自身が先頭
			if (m_pNext != NULL)
			{// 次が存在している
				m_pTop = m_pNext;	// 次を先頭にする
				m_pNext->m_pPrev = NULL;	// 次の前のポインタを覚えていないようにする
			}
			else
			{// 存在していない
				m_pTop = NULL;	// 先頭がない状態にする
				m_pCur = NULL;	// 最後尾がない状態にする
			}
		}
		else if (m_pCur == this)
		{// 自身が最後尾
			if (m_pPrev != NULL)
			{// 次が存在している
				m_pCur = m_pPrev;	// 前を最後尾にする
				m_pPrev->m_pNext = NULL;	// 前の次のポインタを覚えていないようにする
			}
			else
			{// 存在していない
				m_pTop = NULL;	// 先頭がない状態にする
				m_pCur = NULL;	// 最後尾がない状態にする
			}
		}
		else
		{
			if (m_pNext != NULL)
			{
				m_pNext->m_pPrev = m_pPrev;	// 自身の次に前のポインタを覚えさせる
			}
			if (m_pPrev != NULL)
			{
				m_pPrev->m_pNext = m_pNext;	// 自身の前に次のポインタを覚えさせる
			}
		}
	}
}

//===============================================
// 使用設定
//===============================================
void  CLockOn::SetLock(bool bLock)
{
	m_bUse = bLock;

	if (!m_bUse) {
		m_pTarget = nullptr;
		m_bLock = false;

		if (m_pObj != nullptr) {
			m_pObj->SetDraw(false);
		}
	}
}

//===============================================
// 死亡確認
//===============================================
void CLockOn::Check(CEnemy* pObject)
{
	CLockOn* pLock = m_pTop;	// 先頭を取得

	while (pLock != NULL)
	{// 使用されている間繰り返し
		CLockOn* pLockNext = pLock->m_pNext;	// 次を保持

		if (pLock->m_bDeath == false && pLock->m_type == TYPE_MULTI)
		{
			if (pLock->m_pTarget == pObject)
			{// 同じ標的の場合
				pLock->Uninit();
			}
		}

		pLock = pLockNext;	// 次に移動
	}
}

//===============================================
// multiターゲット削除
//===============================================
void CLockOn::MultiDeath(void)
{
	CLockOn* pLock = m_pTop;	// 先頭を取得

	while (pLock != NULL)
	{// 使用されている間繰り返し
		CLockOn* pLockNext = pLock->m_pNext;	// 次を保持

		if (pLock->m_bDeath == false && pLock->m_type == TYPE_MULTI)
		{
			pLock->Uninit();
		}
		pLock = pLockNext;	// 次に移動
	}
}