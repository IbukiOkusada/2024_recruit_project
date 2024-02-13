//==========================================================
//
// ギミック生える槍 [gimmick_spear.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_spear.h"
#include "model.h"
#include "manager.h"
#include "debugproc.h"
#include "player.h"
#include "Xfile.h"
#include "gimmick_button.h"
#include "particle.h"
#include "sound.h"

// マクロ定義
#define COLLISION_RANGE	(300.0f)		// 当たり判定サイズ
#define SET_POSY			(-180.0f)	// 地面に埋まってる時の座標設定

// 静的メンバ変数宣言
char *CGimmickSpear::m_pFileName = {
	"data\\MODEL\\spear.x",
};

// 静的メンバ変数宣言
int CGimmickSpear::m_aStateSetCount[CGimmickSpear::STATE_MAX] = {
	30,
	30,
	60,
	70,
};

float CGimmickSpear::m_aStateDiffMulti[CGimmickSpear::STATE_MAX] = {
	0.03f,
	0.35f,
	0.03f,
	0.03f,
};

//==========================================================
// コンストラクタ
//==========================================================
CGimmickSpear::CGimmickSpear()
{
	// 値のクリア
	m_pObj = nullptr;
	m_bParticle = false;
	m_pInObj = nullptr;
	m_state = STATE_NONE;
}

//==========================================================
// デストラクタ
//==========================================================
CGimmickSpear::~CGimmickSpear()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGimmickSpear::Init(void)
{
	// モデルの生成
	m_pObj = CModel::Create(m_pFileName);

	if (m_pObj != nullptr)
	{
		m_pObj->SetShadow(true);
		m_pObj->SetParent(GetMtxWorld());
		m_pObj->SetCurrentPosition(D3DXVECTOR3(0.0f, SET_POSY, 0.0f));
	}

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGimmickSpear::Uninit(void)
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
void CGimmickSpear::Update(void)
{
	// マトリックス設定
	SetMtxWorld();

	if (m_bActive) {
		StateSet();	// 状態管理
	}

	// 種類更新
	UpdateType();

	if (m_pObj != nullptr) {	// オブジェクトが使われている
		D3DXVECTOR3 pos = m_pObj->GetCurrentPosition();
		D3DXVECTOR3 posDiff = m_PosDest - pos;

		pos += posDiff * m_fDiffMulti;
		m_pObj->SetCurrentPosition(pos);
	}
}

//==========================================================
// 生成
//==========================================================
CGimmickSpear *CGimmickSpear::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type)
{
	CGimmickSpear *pSample = nullptr;

	pSample = new CGimmickSpear;

	if (pSample != nullptr)
	{
		// 初期化処理
		pSample->Init();

		// 値の設定
		pSample->SetPosition(pos);
		pSample->SetRotation(rot);
		pSample->BindType(type);
		pSample->CGimmick::BindType(TYPE_SPEAR);
	}

	return pSample;
}

//==========================================================
// 状態管理
//==========================================================
void CGimmickSpear::StateSet(void)
{
	m_nStateCounter--;

	if (m_nStateCounter > 0) {
		return;
	}

	// 状態に合わせてボタンの色を変更
	switch (m_state)
	{
	case STATE_NONE:
	{
		m_state = STATE_APPEAR;
		m_PosDest.y = 0.0f;
		
	}
	break;

	case STATE_APPEAR:
	{
		m_state = STATE_ATKNOW;
		m_PosDest.y = 0.0f;
	}
	break;

	case STATE_ATKNOW:
	{
		m_state = STATE_BACK;
		m_PosDest.y = SET_POSY;
		m_bParticle = false;
	}
	break;

	case STATE_BACK:
	{
		m_state = STATE_NONE;
		m_PosDest.y = SET_POSY;
	}
	break;

	default:

		break;
	}

	// 次のカウント設定
	m_nStateCounter = m_aStateSetCount[m_state];
	m_fDiffMulti = m_aStateDiffMulti[m_state];
}

//==========================================================
// 判定確認
//==========================================================
bool CGimmickSpear::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand)
{
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	if (m_type == TYPE_SENSOR) {

		// 範囲内チェック
		{
			float fLength =
				sqrtf((pos.x - ObjPos.x) * (pos.x - ObjPos.x)
					+ (pos.z - ObjPos.z) * (pos.z - ObjPos.z));

			if (fLength < COLLISION_RANGE)
			{
				m_bActive = true;
			}
		}
	}

	if (m_state < STATE_APPEAR || m_state >= STATE_ATKNOW) {	// 開く扉
		return false;
	}

	bool bValue = false;
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

	if (pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
		&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x
		&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
		&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z) {	// 範囲内

		if (pos.y + vtxMin.y >= ObjPos.y + vtxObjMin.y + vtxMin.y && pos.y <= ObjPos.y + vtxObjMax.y) {	// 高さも範囲内

			ObjPos = pos;
			ObjPos.y += vtxObjMax.y;
			if (!m_bParticle)
			{
				CParticle::Create(ObjPos, CEffect::TYPE_SPEAR);
				m_bParticle = true;
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SPEAR);
			}
			bValue = true;
		}
	}

	return bValue;
}

//==========================================================
// 種類ごとの更新
//==========================================================
void CGimmickSpear::UpdateType(void)
{
	// 状態に合わせてボタンの色を変更
	switch (m_type)
	{
	case TYPE_AUTO:	// 自動
	{	 
		m_bActive = true;
	}
	break;

	case TYPE_PRESS:
	{
		if (m_pInObj == nullptr) {
			return;
		}

		if (m_pInObj->GetState() == CGimmickButton::STATE_PRESS) {	// 押されている
			m_bActive = true;
		}
		else
		{
			if (m_state == STATE_NONE) {
				m_bActive = false;
				m_nStateCounter = 0;
			}
		}
	}
	break;

	case TYPE_NOTPRESS:
	{
		if (m_pInObj == nullptr) {
			return;
		}

		if (m_pInObj->GetState() == CGimmickButton::STATE_PRESS) {	// 押されている
			if (m_state == STATE_NONE) {
				m_bActive = false;
			}
		}
		else
		{
			if (m_state == STATE_NONE) {
				m_bActive = true;
			}
		}
	}
	break;

	case TYPE_PRESSAUTO:
	{
		if (m_pInObj == nullptr) {
			return;
		}

		if (m_pInObj->GetState() == CGimmickButton::STATE_PRESS) {	// 押されている
			m_bActive = true;
			m_type = TYPE_AUTO;
		}
		else
		{
			m_bActive = false;
		}
	}
	break;

	case TYPE_SENSOR: 
	{
		if (m_state == STATE_NONE) {
			m_bActive = false;
			m_nStateCounter = 0;
		}
	}
	break;

	default:

		break;
	}
}

//==========================================================
// 使用ボタン割り当て
//==========================================================
void CGimmickSpear::IdSetButton(void)
{
	CGimmick *pGimk = CGimmick::GetTop();

	// 最後まで繰り返し
	while (pGimk != nullptr) {
		CGimmick *pGimkNext = pGimk->GetNext();

		if (pGimk->GetButton() == nullptr) {	// ボタンではない
			pGimk = pGimkNext;
			continue;
		}

		if (pGimk->GetId() != GetId()) {	// IDが不一致
			pGimk = pGimkNext;
			continue;
		}

		// 割り当て
		BindButton(pGimk->GetButton());
		break;
	}
}