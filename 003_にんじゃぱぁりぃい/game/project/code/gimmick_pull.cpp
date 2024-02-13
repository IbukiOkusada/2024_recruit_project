//==========================================================
//
// ギミック引けるオブジェクト [gimmick_pull.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_pull.h"
#include "model.h"
#include "manager.h"
#include "debugproc.h"
#include "player.h"
#include "Xfile.h"
#include "particle.h"

// 無名名前空間
namespace {
	const float COLLISION_RANGE = (50.0f);
	const float UPPOSITION = (150.0f);
	const float ROTATE_SPEED = (D3DX_PI * 0.045f);
	const float SET_POSZ = (75.0f);
	const float SET_POSX = (50.0f);
	const int PARTICLE_CT = (5);
}

// 静的メンバ変数宣言
char *CGimmickPull::m_pFileName = {
	"data\\MODEL\\jar.x",
};

//==========================================================
// コンストラクタ
//==========================================================
CGimmickPull::CGimmickPull()
{
	// 値のクリア
	m_pObj = nullptr;
	m_pMtxParent = nullptr;
	m_state = STATE_NONE;
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//==========================================================
// デストラクタ
//==========================================================
CGimmickPull::~CGimmickPull()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGimmickPull::Init(void)
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
void CGimmickPull::Uninit(void)
{
	// 値のクリア
	if (m_pObj != nullptr)
	{
		m_pObj->Uninit();
		m_pObj = nullptr;
	}

	// リストから削除
	ListOut();

	// リリース
	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CGimmickPull::Update(void)
{
	m_posOld = D3DXVECTOR3(GetMtxWorld()->_41, GetMtxWorld()->_42, GetMtxWorld()->_43);

	// 状態に合わせてボタンの色を変更
	switch (m_state)
	{
	case STATE_NONE:	// 何もない
		// マトリックス設定
		SetMtxWorld();

		m_nParticleTimer--;
		if (m_nParticleTimer <= 0) {
			D3DXVECTOR3 pos = GetPosition();
			pos.y += 40.0f;
			CParticle::Create(pos, CEffect::TYPE_PULLSTAR);
			m_nParticleTimer = PARTICLE_CT;
		}

		break;

	case STATE_PULL:	// 押されている
	{
		// マトリックス設定
		SetParentMatrix();

		m_nParticleTimer--;
		if (m_nParticleTimer <= 0) {
			D3DXVECTOR3 pos = GetPosition();
			pos.x = GetMtxWorld()->_41;
			pos.y = GetMtxWorld()->_42 + 50.0f;
			pos.z = GetMtxWorld()->_43;
			CParticle::Create(pos, CEffect::TYPE_PULLSMAKE);
			m_nParticleTimer = PARTICLE_CT - 2;
		}
	}
	break;
	}

	bool bLand = false;
	CGimmick *pGimmick = nullptr;
	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 SetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(GetMtxWorld()->_41, GetMtxWorld()->_42, GetMtxWorld()->_43);

	// 向きを反映
	int nID = m_pObj->GetId();
	m_pObj->SetRotSize(vtxMax,
		vtxMin,
		pFile->GetMax(nID),
		pFile->GetMin(nID),
		0.0f);

	CGimmick::Collision(pos, m_posOld, m_move, SetPos, vtxMin, vtxMax, -1, &pGimmick, &bLand);
}

//==========================================================
// 生成
//==========================================================
CGimmickPull *CGimmickPull::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CGimmickPull *pPull = nullptr;

	pPull = new CGimmickPull;

	if (pPull != nullptr)
	{
		// 初期化処理
		pPull->Init();

		// 値の設定
		pPull->SetPosition(pos);
		pPull->SetRotation(rot);
		pPull->BindType(TYPE_ROTATEDOOR);
	}

	return pPull;
}

//==========================================================
// 判定確認
//==========================================================
bool CGimmickPull::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand)
{
	bool bValue = false;

	if (m_pMtxParent != nullptr) {	// 誰にも掴まれていない
		return bValue;
	}

	if (ppGimmick == nullptr) {
		return bValue;
	}

	if (*ppGimmick != nullptr) {	// 既に何かのギミックを触れている
		return bValue;
	}

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
		ObjRot.y);

	if (pos.x + vtxMin.x <= ObjPos.x + vtxObjMax.x && pos.x + vtxMax.x >= ObjPos.x + vtxObjMin.x &&
		pos.z + vtxMin.z <= ObjPos.z + vtxObjMax.z && pos.z + vtxMax.z >= ObjPos.z + vtxObjMin.z) {
		
		if (nAction == CPlayer::ACTION_CATCH) {
			m_state = STATE_PULL;
			SetPosition(D3DXVECTOR3(0.0f, 0.0f, -100.0f));
			SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			*ppGimmick = this;
		}
	}

	return bValue;
}

//==========================================================
// 親のマトリックスと組み合わせる
//==========================================================
void CGimmickPull::SetParentMatrix(void)
{
	if (m_pMtxParent == NULL)
	{// 覚えている場合
		SetPosition(D3DXVECTOR3(GetMtxWorld()->_41, GetMtxWorld()->_42, GetMtxWorld()->_43));
		m_state = STATE_NONE;
		return;
	}

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DXMATRIX *pMtxWorld = GetMtxWorld();	// 親のマトリックス情報
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(pMtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxTrans);

	// パーツのマトリックスと親のマトリックスをかけ合わせる
	D3DXMatrixMultiply(pMtxWorld,
		pMtxWorld, m_pMtxParent);
}