//===============================================
//
// 弾の処理 [bullet.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "particle.h"
#include "sound.h"
#include "score.h"
#include "texture.h"
#include "objectX.h"
#include "Xfile.h"
#include "slow.h"
#include "meshorbit.h"
#include "meshfield.h"
#include "wet.h"
#include "character.h"
#include "game.h"
#include "model.h"

//===============================================
// マクロ定義
//===============================================
#define LIFE	(120.0f)		// 寿命
#define GRAVITY	(-0.07f)		// 重力
#define EXPLOGRAVITY	(-0.35f)	// 爆発重力
#define GUN_DAMAGE	(100.0f)
#define SHW_DAMAGE	(20.0f)

// 静的メンバ変数
CBullet *CBullet::m_pTop = NULL;	// 先頭のオブジェクトへのポインタ
CBullet *CBullet::m_pCur = NULL;	// 最後尾のオブジェクトへのポインタ
int CBullet::m_nNumAll = 0;

//===============================================
// コンストラクタ
//===============================================
CBullet::CBullet()
{
	// 値のクリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLife = 0.0f;
}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CBullet::CBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	// 値のクリア
	SetPosition(pos);
	m_fLife = 0.0f;
	m_move = move;
	m_Inermove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pTarget = NULL;
	m_pNext = NULL;
	m_pPrev = NULL;
	m_pOrbit = NULL;

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

	m_pOrbit2 = NULL;
}

//===============================================
// デストラクタ
//===============================================
CBullet::~CBullet()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CBullet::Init(void)
{
	// オブジェクトの初期化処理
	CObject3D::Init();

	m_fLife = LIFE;
	m_nChangeTimer = 20;
	m_nType = 0;
	m_bDeath = false;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CBullet::Uninit(void)
{
	// オブジェクトの終了処理
	CObject3D::Uninit();

	if (m_pTarget != NULL)
	{
		delete m_pTarget;
		m_pTarget = NULL;
	}

	if (m_pOrbit != NULL)
	{
		m_pOrbit->Uninit();
	}

	if (m_pOrbit2 != NULL)
	{
		m_pOrbit2->Uninit();
	}

	m_bDeath = true;

	DeathCheck();
}

//===============================================
// 更新処理
//===============================================
void CBullet::Update(void)
{
	D3DXVECTOR3 pos = GetPosition();

	// 頂点情報更新
	//SetVtx();

	m_fLife -= CManager::GetSlow()->Get();

	if (m_fLife > 0.0f)
	{//  寿命がある場合
		// 座標更新
		Controller();
		m_nChangeTimer--;

		//弾との当たり判定
		if (Collision(pos, CObject::TYPE_ENEMY) == true)
		{// 当たった場合
			//スコアの加算
			CManager::GetSound()->Play(CSound::LABEL_SE_HIT);

			return;	// 終了しているため返す
		}
	}
	else
	{// 寿命切れ

		// 終了処理
		Uninit();

		// 死亡確認
		DeathCheck();
		return;
	}

	// マトリックス設定
	SetMtx();
}

//===============================================
// 描画処理
//===============================================
void CBullet::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//ライティングをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	//CObjectBillboard::Draw();

	//ライティングをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===============================================
// 頂点情報設定
//===============================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE type)
{
	CBullet *pBullet = NULL;
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// オブジェクト2Dの生成
	pBullet = new CBullet(pos, move);

	if (pBullet != NULL)
	{// 生成できた場合

		// 初期化処理
		pBullet->Init();

		// 弾の種類設定
		pBullet->m_nType = type;

		// オブジェクトタイプの設定
		pBullet->SetType(CObject::TYPE_BULLET);

		pBullet->BindTexture(-1);

		pBullet->SetSize(10.0f, 10.0f);

		pBullet->SetMtx();

		// 軌跡の
		if (type == TYPE_NONE)
		{
			pBullet->m_pOrbit = CMeshOrbit::Create(pBullet->GetMtx(), D3DXVECTOR3(0.0f, 3.0f, 0.0f), D3DXVECTOR3(0.0f, -3.0f, 0.0f), CMeshOrbit::TYPE_BULLET);
			pBullet->m_pOrbit2 = CMeshOrbit::Create(pBullet->GetMtx(), D3DXVECTOR3(0.0f, 3.0f, 0.0f), D3DXVECTOR3(0.0f, -3.0f, 0.0f), CMeshOrbit::TYPE_BULLET);
			pBullet->m_pOrbit2->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\water000.jpg"));
			pBullet->m_pOrbit->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\water001.jpg"));
		}
		else if (type == TYPE_SHOWER)
		{
			pBullet->m_pOrbit = CMeshOrbit::Create(pBullet->GetMtx(), D3DXVECTOR3(0.0f, 0.01f, 0.0f), D3DXVECTOR3(0.0f, -0.01f, 0.0f), CMeshOrbit::TYPE_BULLET);
			pBullet->m_pOrbit2 = CMeshOrbit::Create(pBullet->GetMtx(), D3DXVECTOR3(0.0f, 0.01f, 0.0f), D3DXVECTOR3(0.0f, -0.01f, 0.0f), CMeshOrbit::TYPE_BULLET);
			pBullet->m_pOrbit2->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\water002.jpg"));
			pBullet->m_pOrbit->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\water001.jpg"));
		}
		else if (type == TYPE_EXPLOSION)
		{
			pBullet->m_pOrbit = CMeshOrbit::Create(pBullet->GetMtx(), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), CMeshOrbit::TYPE_BULLET);
			pBullet->m_pOrbit2 = CMeshOrbit::Create(pBullet->GetMtx(), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), CMeshOrbit::TYPE_BULLET);
			pBullet->m_pOrbit2->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\water000.jpg"));
			pBullet->m_pOrbit->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\water001.jpg"));
			pBullet->m_pOrbit->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			pBullet->m_pOrbit2->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pBullet;
}

//===============================================
// 操作処理
//===============================================
void CBullet::Controller(void)
{
	D3DXVECTOR3 pos = GetPosition();	// 座標を取得
	CMeshField *pMesh = CGame::GetMeshField();
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fHeight = CMeshField::GetHeight(GetPosition());

	if (m_nType == TYPE_GRAVITY)
	{
		m_move.y += GRAVITY * CManager::GetSlow()->Get();
	}
	else if (m_nType == TYPE_SHOWER)
	{
		m_move.y += GRAVITY * CManager::GetSlow()->Get();
	}
	else if (m_nType == TYPE_EXPLOSION)
	{
		m_move.y += EXPLOGRAVITY * CManager::GetSlow()->Get();
	}
	else
	{
		m_fParTimer += CManager::GetSlow()->Get();

		if (m_fParTimer >= 1.0f)
		{
			CParticle::Create(pos, m_move, CEffect::TYPE_BULLET);
			//m_fParTimer = 0.0f;
		}
	}

	if (m_pTarget == NULL)
	{// ホーミングをしない場合
		pos += m_move * CManager::GetSlow()->Get();	// 移動量加算
	}
	else
	{// する場合
		CObject *pObj = m_pTarget->pObj;

		if (m_pTarget->pObj != NULL)
		{
			if (m_pTarget->pObj->GetDeath() == false && m_pTarget->pObj->GetType() == CObject::TYPE_ENEMY)
			{
				CEnemy *pEnemy = pObj->GetEnemy();
				if (pEnemy != NULL)
				{
					D3DXMATRIX mtxWorld = *pEnemy->GetBody()->GetParts(0)->GetMtxWorld();
					m_move = D3DXVECTOR3(mtxWorld._41 - pos.x, mtxWorld._42 - pos.y, mtxWorld._43 - pos.z);
					D3DXVec3Normalize(&m_move, &m_move);
					float fRotDest = atan2f(  mtxWorld._41 - pos.x, mtxWorld._43 - pos.z);	//目標までの角度
					float fRotDestXY = atan2f(mtxWorld._42 - pos.y, mtxWorld._41 - pos.x);
					float fRotDestZY = atan2f(mtxWorld._42 - pos.y, mtxWorld._43 - pos.z);
					if (m_nType == TYPE_NONE)
					{
						m_move.y *= m_pTarget->fSpeed;
					}

					m_move.x *= m_pTarget->fSpeed;
					m_move.z *= m_pTarget->fSpeed;
				}
				else
				{
					if (m_pTarget->pObj->GetMtx() != NULL)
					{
						m_move = D3DXVECTOR3(m_pTarget->pObj->GetMtx()->_41 - pos.x, m_pTarget->pObj->GetMtx()->_42 - pos.y, m_pTarget->pObj->GetMtx()->_43 - pos.z);
						D3DXVec3Normalize(&m_move, &m_move);
						float fRotDest = atan2f(m_pTarget->pObj->GetMtx()->_41 - pos.x, m_pTarget->pObj->GetMtx()->_43 - pos.z);	//目標までの角度
						float fRotDestXY = atan2f(m_pTarget->pObj->GetMtx()->_42 - pos.y, m_pTarget->pObj->GetMtx()->_41 - pos.x);
						float fRotDestZY = atan2f(m_pTarget->pObj->GetMtx()->_42 - pos.y, m_pTarget->pObj->GetMtx()->_43 - pos.z);
						if (m_nType == TYPE_NONE)
						{
							m_move.y *= m_pTarget->fSpeed;
						}

						m_move.x *= m_pTarget->fSpeed;
						m_move.z *= m_pTarget->fSpeed;
					}
				}
			}
			else
			{
				pObj = NULL;
			}
		}
		pos += m_move * CManager::GetSlow()->Get();	// 移動量加算
	}

	pos += m_Inermove * CManager::GetSlow()->Get();

	m_Inermove.x += (0.0f - m_Inermove.x) * 0.1f * CManager::GetSlow()->Get();	//x座標
	m_Inermove.y += (0.0f - m_Inermove.y) * 0.1f * CManager::GetSlow()->Get();	//x座標
	m_Inermove.z += (0.0f - m_Inermove.z) * 0.1f * CManager::GetSlow()->Get();	//x座標

	if (m_pOrbit != NULL)
	{
		D3DXVECTOR3 nor = pos - GetPosition();

		D3DXVec3Normalize(&nor, &nor);	// ベクトルを正規化する

		m_pOrbit->SetNor(nor);
	}

	if (m_pOrbit2 != NULL)
	{
		D3DXVECTOR3 nor = pos - GetPosition();

		D3DXVec3Normalize(&nor, &nor);	// ベクトルを正規化する

		m_pOrbit2->SetNor(nor);
	}

	if (m_nType == TYPE_NONE)
	{
		if (m_fParTimer >= 1.0f)
		{
			//CEffect::Create(pos, CEffect::TYPE_BULLET);
			CParticle::Create(pos, m_move, CEffect::TYPE_BULLET);
			m_fParTimer = 0.0f;
		}
	}

	// 頂点情報設定
	SetPosition(pos);

	if (pos.y < fHeight)
	{// 地面に触れた
		pos = GetPosition();
		pos.y = fHeight + 0.05f;
		CWet::Create(pos, 15.0f, 15.0f);
		Uninit();
	}
}

//===============================================
// 敵との当たり判定
//===============================================
bool CBullet::Collision(D3DXVECTOR3 pos, CObject::TYPE ObjType)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		CObject *pObj = CObject::GetTop(nCntPri);	// 先頭を取得

		while (pObj != NULL)
		{// 使用されていない状態まで

			CObject *pObjectNext = pObj->GetNext();	// 次のオブジェクトへのポインタを取得

			if (pObj->GetDeath() == false)
			{
				CObject::TYPE type;	// 種類

				// オブジェクトの種類を取得
				type = pObj->GetType();

				if (type == ObjType)
				{// 種類が敵の場合

					D3DXVECTOR3 ObjPos;	// オブジェクトの座標
					float fObjWidth = 0.0f;
					float fObjHeight = 0.0f;

					// 座標の取得
					ObjPos = pObj->GetPosition();
					CEnemy *pObjX = (CEnemy*)pObj;

					if (pObjX == NULL)
					{// 使用されていない
						pObj = pObjectNext;	// 次のオブジェクトに移動
						continue;
					}

					if (pos.x + GetWidth() >= ObjPos.x + -50.0f && pos.x - GetWidth() <= ObjPos.x + 50.0f
						&& pos.z + GetWidth() >= ObjPos.z + -50.0f && pos.z - GetWidth() <= ObjPos.z + 50.0f
						&& pos.y + GetHeight() >= ObjPos.y + -50.0f && pos.y - GetHeight() <= ObjPos.y + 50.0f)
					{// 敵と重なった場合

						// オブジェクトの終了処理
						if (pObjX->GetState() != CEnemy::STATE_COOL && pObjX->GetState() != CEnemy::STATE_COOLDOWN)
						{							
							if (m_nType == TYPE_NONE)
							{
								pObjX->Hit(GUN_DAMAGE);
							}
							else
							{
								pObjX->Hit(SHW_DAMAGE);
								//CGame::GetScore()->Add(5);
							}

							// 標的確認
							Check(pObjX);

							// 自分自身の終了処理
							if (m_nType == TYPE_NONE)
							{
								Uninit();

								return TRUE;	// TRUEを返す(当たった)
							}
						}
					}
				}
			}
			pObj = pObjectNext;	// 次のオブジェクトに移動
		}
	}

	return FALSE;	// FALSEを返す(何にも当たっていない)
}

//===============================================
// 敵との当たり判定
//===============================================
void CBullet::SetHom(CObject *pObj, float fSpeed)
{
	if (m_pTarget == NULL)
	{
		m_pTarget = new LockOn;
	}

	if (m_pTarget != NULL)
	{
		CEnemy *pEnemy = pObj->GetEnemy();
		if (pEnemy != NULL)
		{
			m_pTarget->pObj = pObj;
			m_pTarget->fSpeed = fSpeed;
		}
	}
}

//===============================================
// 標的チェック
//===============================================
void CBullet::Check(CObject *pObject, CBullet *pBullet)
{
	CBullet *pBul = m_pTop;	// 先頭を取得

	while (pBul != NULL)
	{// 使用されている間繰り返し
		CBullet *pBulNext = pBul->m_pNext;	// 次を保持

		if (pBul->m_pTarget != NULL)
		{// ロックオンしている
			if (pBul->m_pTarget->pObj == pObject)
			{// 同じ標的の場合
				pBul->m_pTarget->pObj = NULL;
				delete pBul->m_pTarget;
				pBul->m_pTarget = NULL;
			}
		}

		pBul = pBulNext;	// 次に移動
	}
}

//===============================================
// 死亡確認
//===============================================
void CBullet::DeathCheck(void)
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
				m_pCur = m_pPrev;			// 前を最後尾にする
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
// 慣性移動量を設定
//===============================================
void CBullet::SetInerMove(D3DXVECTOR3 move)
{
	m_Inermove = move;
}