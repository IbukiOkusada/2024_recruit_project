//==========================================================
//
// ギミック開始地点ドア [gimmick_startdoor.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_startdoor.h"
#include "gimmick_lever.h"
#include "model.h"
#include "manager.h"
#include "debugproc.h"
#include "player.h"
#include "Xfile.h"
#include "sound.h"
#include "effect.h"

// マクロ定義
#define COLLISION_RANGE	(50.0f)
#define UPPOSITION		(150.0f)

// 静的メンバ変数宣言
char *CGimmickStartDoor::m_pFileName = {
	"data\\MODEL\\start_door.x",
};

//==========================================================
// コンストラクタ
//==========================================================
CGimmickStartDoor::CGimmickStartDoor()
{
	// 値のクリア
	m_pObj = nullptr;
	m_state = STATE_NONE;
	m_nSoundUp = 0;
	m_nSoundDown = 0;

	for (int nCnt = 0; nCnt < DOOR_EFFECT::NUM_EFFECT; nCnt++) {
		m_apEffect[nCnt] = nullptr;
	}
}

//==========================================================
// デストラクタ
//==========================================================
CGimmickStartDoor::~CGimmickStartDoor()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGimmickStartDoor::Init(void)
{
	// モデルの生成
	m_pObj = CModel::Create(m_pFileName);

	if (m_pObj != nullptr)
	{
		m_pObj->SetShadow(true);
		m_pObj->SetParent(GetMtxWorld());
	}

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGimmickStartDoor::Uninit(void)
{
	// 値のクリア
	if (m_pObj != nullptr)
	{
		m_pObj->Uninit();
		m_pObj = nullptr;
	}

	for (int nCnt = 0; nCnt < DOOR_EFFECT::NUM_EFFECT; nCnt++) {
		if (m_apEffect[nCnt] == nullptr) {
			continue;
		}
		m_apEffect[nCnt]->Uninit();
		m_apEffect[nCnt] = nullptr;
	}

	// リストから削除
	ListOut();

	// リリース
	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CGimmickStartDoor::Update(void)
{
	// マトリックス設定
	SetMtxWorld();

	// 状態に合わせてボタンの色を変更
	switch (m_state)
	{
	case STATE_NONE:	// 何もない
		m_PosDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		m_nSoundDown++;

		if (m_nSoundDown == 1)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_OPEN02);
		}
		break;

	case STATE_OPEN:	// 押されている
		m_PosDest = D3DXVECTOR3(0.0f, UPPOSITION, 0.0f);
		m_nSoundUp++;

		if (m_nSoundUp == 1)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_OPEN02);
		}
	
		break;
	}

	if (m_pObj != nullptr) {	// オブジェクトが使われている
		D3DXVECTOR3 pos = m_pObj->GetCurrentPosition();
		D3DXVECTOR3 posDiff = m_PosDest - pos;

		pos += posDiff * 0.1f;
		m_pObj->SetCurrentPosition(pos);

		if (m_PosDest.y == UPPOSITION && posDiff.y >= 0.1f && posDiff.y <= 5.0f)
		{
			// エフェクトを生成する
			for (int nCnt = 0; nCnt < DOOR_EFFECT::NUM_EFFECT; nCnt++) {
				if (m_apEffect[nCnt] != nullptr) {
					continue;
				}
				// 座標の設定
				D3DXVECTOR3 pos = GetPosition();

				if (m_pObj != nullptr) {
					pos.x = m_pObj->GetMtx()->_41;
					pos.y = m_pObj->GetMtx()->_42;
					pos.z = m_pObj->GetMtx()->_43;
				}

				pos.x += sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 1.0f;
				pos.z += cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 1.0f;

				//移動量の設定
				D3DXVECTOR3 move = { 0.0f, 0.0f, 0.0f };
				move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;
				move.y = ((float)(rand() % 10 + 1)) * 0.5f;
				move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;

				float frand = rand() % 8 * 0.1f;

				//色の設定
				D3DXCOLOR col = D3DXCOLOR(frand, frand, frand, 1.0f);

				//半径の設定
				float fRadius = 10.0f;

				//寿命の設定
				float fLife = 200.0f;

				m_apEffect[nCnt] = CEffect::Create(pos + move, move, col, fRadius, fLife, CEffect::TYPE_DUST);
			}
		}
	}

	STATE stateOld = m_state;

	if (m_pLever != nullptr) {	// レバーが使われている

		switch (m_pLever->GetState())
		{
		case CGimmickLever::STATE_NONE:
			m_state = STATE_NONE;
			break;

		case CGimmickLever::STATE_PRESS:
			m_state = STATE_OPEN;
			break;

		default:
			m_state = STATE_NONE;
			break;
		}
	}

	// エフェクトの更新処理
	for (int nCnt = 0; nCnt < DOOR_EFFECT::NUM_EFFECT; nCnt++) {
		if (m_apEffect[nCnt] == nullptr) {
			continue;
		}

		// 更新処理
		m_apEffect[nCnt]->Update();

		if (m_apEffect[nCnt]->GetCol().a <= 0.0f || m_apEffect[nCnt]->GetRange() <= 0.0f || m_apEffect[nCnt]->GetLife() <= 0.0f) {	// 色が消えたもしくはサイズが小さくなった
			m_apEffect[nCnt]->Uninit();
			m_apEffect[nCnt] = nullptr;
		}
	}
}

//==========================================================
// 生成
//==========================================================
CGimmickStartDoor *CGimmickStartDoor::Create(const D3DXVECTOR3 pos)
{
	CGimmickStartDoor *pSample = nullptr;

	pSample = new CGimmickStartDoor;

	if (pSample != nullptr)
	{
		// 初期化処理
		pSample->Init();

		pSample->SetPosition(pos);
		pSample->BindType(TYPE_STARTDOOR);
	}

	return pSample;
}

//==========================================================
// 判定確認
//==========================================================
bool CGimmickStartDoor::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand)
{
	if (m_state == STATE_OPEN) {	// 開く扉
		return false;
	}

	bool bValue = false;
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 向きを反映
	int nID = m_pObj->GetId();
	m_pObj->SetRotSize(vtxObjMax,
		vtxObjMin,
		pFile->GetMax(nID),
		pFile->GetMin(nID),
		GetRotation().y);

	// X
	if (pos.y + vtxMax.y > ObjPos.y + vtxObjMin.y
		&& pos.y + vtxMin.y < ObjPos.y + vtxObjMax.y
		&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
		&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
	{//範囲内にある
		if (posOld.x + vtxMin.x >= ObjPos.x + vtxObjMax.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x)
		{//右から左にめり込んだ
			move.x = 0.0f;
			pos.x = ObjPos.x + vtxObjMax.x - vtxMin.x + 0.1f + move.x;
		}
		else if (posOld.x + vtxMax.x <= ObjPos.x + vtxObjMin.x
			&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x)
		{//左から右にめり込んだ
		 //位置を戻す
			move.x = 0.0f;
			pos.x = ObjPos.x + vtxObjMin.x - vtxMax.x - 0.1f + move.x;
		}
	}

	//Z
	if (pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
		&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x
		&& pos.y + vtxMax.y > ObjPos.y + vtxObjMin.y
		&& pos.y + vtxMin.y < ObjPos.y + vtxObjMax.y)
	{//範囲内にある
		if (posOld.z + vtxMin.z >= ObjPos.z + vtxObjMax.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
		{//奥から手前にめり込んだ
		 //位置を戻す
			move.z = 0.0f;
			pos.z = ObjPos.z + vtxObjMax.z - vtxMin.z + 0.1f + move.z;
		}
		else if (posOld.z + vtxMax.z <= ObjPos.z + vtxObjMin.z
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z)
		{//手前から奥にめり込んだ
		 //位置を戻す
			move.z = 0.0f;
			pos.z = ObjPos.z + vtxObjMin.z - vtxMax.z - 0.1f + move.z;
		}
	}

	//Y
	if (pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
		&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x
		&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
		&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
	{// 範囲内にある
	 // 上からの判定
		if (posOld.y + vtxMin.y >= ObjPos.y + vtxObjMax.y
			&& pos.y + vtxMin.y < ObjPos.y + vtxObjMax.y)
		{//上からめり込んだ
			// 上にのせる
			pos.y = ObjPos.y + vtxObjMax.y - vtxMin.y;
			move.y = 0.0f;
		}
	}

	return bValue;
}