//==========================================================
//
// アイテムの処理 [item.cpp]
// Author : Yuuka Sakuma
//
//==========================================================
#include "item.h"
#include "Xfile.h"
#include "manager.h"
#include "debugproc.h"
#include "texture.h"
#include "model.h"
#include "objectX.h"
#include "meshfield.h"

// 無名名前空間
namespace {
	const char* MODELNAME[CItem::TYPE_MAX] = {	// モデルファイル名
		"data\\MODEL\\coin.x",
		"data\\MODEL\\bracelet00.x",
		"data\\MODEL\\cup00.x",
		"data\\MODEL\\gem00.x",
		"data\\MODEL\\gem01.x",
		"data\\MODEL\\goldbar00.x",
		"data\\MODEL\\jar001.x",
		"data\\MODEL\\kunai.x",
		"data\\MODEL\\ring00.x",
		"data\\MODEL\\scroll00.x",
		"data\\MODEL\\shuriken.x",
	};
}

// マクロ定義
#define COLLISION_SIZE	(75.0f)
#define BOUND_COUNT	(4)
#define GRAVITY	(-1.0f)		//プレイヤー重力

// 静的メンバ変数
CItem *CItem::m_pTop = nullptr;	// 先頭のオブジェクトへのポインタ
CItem *CItem::m_pCur = nullptr;	// 最後尾のオブジェクトへのポインタ

//==========================================================
// コンストラクタ
//==========================================================
CItem::CItem()
{
	// 値のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = TYPE_COIN;
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_nBound = 0;

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
CItem::~CItem()
{

}

//==========================================================
// 生成
//==========================================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type, int nType)
{
	CItem *pObjectX = nullptr;

	// オブジェクトXの生成
	pObjectX = new CItem;

	if (pObjectX != nullptr)
	{// 生成できた場合
	
		// 座標
		pObjectX->SetPosition(pos);
		//pObjectX->SetOldPos(pos);

		// 向き
		pObjectX->SetRotation(rot);

		//// 種類設定
		//pObjectX->SetType(nType);

		// 種類
		pObjectX->m_nState = nType;
		// 初期化処理
		pObjectX->Init(type);
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pObjectX;
}


//==========================================================
// 初期化処理
//==========================================================
HRESULT CItem::Init(void)
{
	return S_OK;
}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CItem::Init(int type)
{
	m_pObject = CModel::Create(MODELNAME[type]);
	m_pObject->SetRotation(m_rot);
	m_type = type;

	//当たり判定生成
	CXFile* pFile = CManager::GetInstance()->GetModelFile();
	D3DXVECTOR3 vtxObjMin = pFile->GetMin(m_pObject->GetId());
	m_pos.y = m_pos.y - vtxObjMin.y;

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CItem::Uninit(void)
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

	if (nullptr != m_pObject) {
		m_pObject->Uninit();
		m_pObject = NULL;
	}

	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CItem::Update(void)
{
	if (m_pObject == nullptr)
	{
		return;
	}

	switch (m_nState)
	{
	case STATE_NORMAL:
	{
		D3DXVECTOR3 pos = m_pos;
		m_posOld = m_pos;
		m_pos = m_pObject->GetPosition();
		m_rot = m_pObject->GetRotation();

		//角度を求める
		m_fCurve += (D3DX_PI * 0.02f);

		//位置を代入する
		m_pos.y = m_posOld.y + sinf(m_fCurve) * 1.5f;

		if (m_pObject->GetMtxParent() == nullptr) {
			m_rot.y += sinf(D3DX_PI * 0.02f);
		}
		else {

			if ((m_rot.x >= D3DX_PI * 0.1f && m_rot.x <= D3DX_PI * 0.6f) || (m_rot.x <= -D3DX_PI * 0.4f && m_rot.x >= -D3DX_PI * 0.9f)) {
				m_rot.x += sinf(D3DX_PI * 0.005f);
			}
			else
			{
				m_rot.x += sinf(D3DX_PI * 0.04f);
			}
		}

		m_pos = pos;
	}
		break;

	case STATE_DROP:
	{
		m_posOld = m_pos;

		//位置を代入する
		m_move.y += GRAVITY;
		m_pos += m_move;
		m_rot.y += sinf(D3DX_PI * (0.02f + (BOUND_COUNT - m_nBound) * 0.01f));

		CXFile* pFile = CManager::GetInstance()->GetModelFile();
		D3DXVECTOR3 vtxObjMin = pFile->GetMin(m_pObject->GetId());

		if (m_pos.y + vtxObjMin.y <= 0.0f)
		{
			m_pos.y = 0.0f - vtxObjMin.y;
			m_move *= 0.4f;
			m_move.y *= -1.0f;
			m_nBound++;

			if (m_nBound >= BOUND_COUNT)
			{
				m_move.y = 0.0f;
				m_nState = STATE_NORMAL;
				m_nBound = 0;
			}
		}
	}
		break;

	case STATE_CRASH:
	{
		m_posOld = m_pos;

		//位置を代入する
		m_move.x += GRAVITY;
		m_pos += m_move;

		if (m_pos.x <= -900.0f)
		{
			m_pos.x = -900.0f;
			m_move *= 0.8f;
			m_move.x *= -1.0f;
			m_nState = STATE_NORMAL;

			Uninit();
		}
	}

	break;

	case STATE_DOWN:
		m_posOld = m_pos;

		//位置を代入する
		m_move.y += GRAVITY;
		m_pos += m_move;

		if (m_pos.y <= -700.0f)
		{
			m_pos.y = -700.0f;
			m_move *= 0.8f;
			m_move.y *= -1.0f;
			m_nState = STATE_NORMAL;

			Uninit();
		}
	}
	


	while (1)
	{
		if (m_rot.y > D3DX_PI || m_rot.y < -D3DX_PI)
		{//-3.14～3.14の範囲外の場合
			if (m_rot.y > D3DX_PI)
			{
				m_rot.y += (-D3DX_PI * 2);
			}
			else if (m_rot.y < -D3DX_PI)
			{
				m_rot.y += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}

	// 使用オブジェクト更新
	if (nullptr != m_pObject) {
		D3DXVECTOR3 ObjPos = m_pos;
		ObjPos.y += 10.0f;
		m_pObject->SetPosition(ObjPos);
		m_pObject->SetRotation(m_rot);
	}

	// 当たり判定
	if (CManager::GetInstance()->GetMode() != CScene::MODE_RANKING && CManager::GetInstance()->GetMode() != CScene::MODE_TITLE)
	{
		//当たり判定生成
		CXFile* pFile = CManager::GetInstance()->GetModelFile();
		int nID = m_pObject->GetId();
		D3DXVECTOR3 vtxObjMax = pFile->GetMax(nID);
		D3DXVECTOR3 vtxObjMin = pFile->GetMin(nID);

		//オブジェクト
		CObjectX::Collision(m_pos, m_posOld, m_move, vtxObjMin, vtxObjMax);
	}
}

////==========================================================
//// 描画処理
////==========================================================
//void CItem::Draw(void)
//{
//	m_pObject->
//}
//

//==========================================================
// 当たり判定
//==========================================================
CItem *CItem::Collision(D3DXVECTOR3 &pos)
{
	CItem *pObj = m_pTop;	// 先頭取得

	while (pObj != NULL)
	{
		CItem* pObjNext = pObj->m_pNext;
		if (pObj->CollisionCheck(pos))
		{
			if (pObj->m_pObject != nullptr)
			{
				pObj->m_pObject->SetDraw(false);
				return pObj;
			}
		}

		pObj = pObjNext;
	}

	return nullptr;
}

//==========================================================
// それぞれのスコア設定
//==========================================================
int CItem::GetEachScore(void)
{
	switch (m_type)
	{
	case TYPE_COIN:
	{
		return 500;
	}

	break;

	case TYPE_BRECELET:
	{
		return 1500;
	}

	break;

	case TYPE_CUP:
	{
		return 1000;
	}

	break;

	case TYPE_GEM00:
	{
		return 2000;
	}

	break;

	case TYPE_GEM01:
	{
		return 2000;
	}

	break;

	case TYPE_GOLDBAR:
	{
		return 3000;
	}

	break;

	case TYPE_JAR:
	{
		return 700;
	}

	break;

	case TYPE_KUNAI:
	{
		return 1300;
	}

	break;

	case TYPE_RING00:
	{
		return 5000;
	}

	break;

	case TYPE_SCROLL:
	{
		return 2500;
	}

	break;

	case TYPE_SHURIKEN:
	{
		return 1000;
	}

	break;

	case TYPE_MAX:
	{

	}

	break;

	}
	return 0;
}

//==========================================================
// 個別判定チェック
//==========================================================
bool CItem::CollisionCheck(D3DXVECTOR3 &pos)
{
	D3DXVECTOR3 ObjPos = GetPosition();

	if (m_pObject == nullptr){	// オブジェクトがない
		return false;
	}

	if (!m_pObject->GetDraw()){	// 描画しない
		return false;
	}

	if (m_nState == STATE_DROP)
	{
		return false;
	}

	// 距離を取得
	float fLength = sqrtf((pos.x - ObjPos.x) * (pos.x - ObjPos.x)
		+ (pos.z - ObjPos.z) * (pos.z - ObjPos.z));

	float fSize = COLLISION_SIZE;

	if (fLength > fSize)
	{// 触れていない
		return false;
	}

	if (pos.y >= ObjPos.y - fSize && pos.y <= ObjPos.y + fSize)
	{// 高さも一緒
		return true;
	}
	return false;
}