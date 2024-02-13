//==========================================================
//
// ボタン [gimmick_button.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_button.h"
#include "model.h"
#include "manager.h"
#include "debugproc.h"
#include "particle.h"
#include "Xfile.h"
#include "sound.h"

// 静的メンバ変数宣言
char *CGimmickButton::m_apFileName[MODEL_MAX] = {
	"data\\MODEL\\trans_button.x",
	"data\\MODEL\\button_frame.x",
};

namespace
{
	const float COLLISION_RANGE = (50.0f);	// 当たり判定半径
	const float PRESS_NOWHEIGHT = (-6.0f);	// 押されている際の沈む量
}

//==========================================================
// コンストラクタ
//==========================================================
CGimmickButton::CGimmickButton()
{
	// 値のクリア
	for (int nCnt = 0; nCnt < MODEL_MAX; nCnt++)
	{
		m_apObj[nCnt] = nullptr;
	}

	m_state = STATE_NONE;

	m_nSound = 0;
}

//==========================================================
// デストラクタ
//==========================================================
CGimmickButton::~CGimmickButton()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGimmickButton::Init(void)
{
	// モデルの生成
	for (int nCnt = 0; nCnt < MODEL_MAX; nCnt++)
	{
		m_apObj[nCnt] = CModel::Create(m_apFileName[nCnt]);

		if (m_apObj[nCnt] != nullptr)
		{
			m_apObj[nCnt]->SetShadow(false);
			m_apObj[nCnt]->SetParent(GetMtxWorld());
		}
	}

	// ボタンの変更後の色を設定
	if (m_apObj[MODEL_BUTTON] != nullptr) {
		D3DMATERIAL9 material = {};
		material.Ambient = D3DXCOLOR (0.1f, 0.1f, 1.0f, 1.0f);
		material.Diffuse = D3DXCOLOR (0.1f, 0.1f, 1.0f, 1.0f);
		material.Emissive = D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f);
		m_apObj[MODEL_BUTTON]->SetMaterial(material);
	}

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGimmickButton::Uninit(void)
{
	// 値のクリア
	for (int nCnt = 0; nCnt < MODEL_MAX; nCnt++)
	{
		if (m_apObj[nCnt] != nullptr)
		{
			m_apObj[nCnt]->Uninit();
			m_apObj[nCnt] = nullptr;
		}
	}

	// リストから削除
	ListOut();
	
	// リリース
	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CGimmickButton::Update(void)
{
	// マトリックス設定
	SetMtxWorld();

	float fHeight = 0.0f;

	// 状態に合わせてボタンの色を変更
	switch (m_state)
	{
	case STATE_NONE:	// 何もない
		m_nSound = 0;
		if (m_apObj[MODEL_BUTTON] != nullptr) {
			m_apObj[MODEL_BUTTON]->ChangeCol();
		}
		break;

	case STATE_PRESS:	// 押されている
		m_nSound++;

		if (m_nSound == 1)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_BUTTON);
		}


		CParticle::Create(GetPosition(), CEffect::TYPE_BUTTON);
		fHeight = PRESS_NOWHEIGHT;
		if (m_apObj[MODEL_BUTTON] != nullptr) {
			m_apObj[MODEL_BUTTON]->ChangeCol(true);
		}
		break;
	}

	if (m_apObj[MODEL_BUTTON] == nullptr) {
		return;
	}

	D3DXVECTOR3 pos = m_apObj[MODEL_BUTTON]->GetCurrentPosition();
	float fDest = fHeight - pos.y;
	pos.y += fDest * 0.05f;
	m_apObj[MODEL_BUTTON]->SetCurrentPosition(pos);
}

//==========================================================
// 生成
//==========================================================
CGimmickButton *CGimmickButton::Create(const D3DXVECTOR3 pos)
{
	CGimmickButton *pSample = nullptr;

	pSample = new CGimmickButton;

	if (pSample != nullptr)
	{
		// 初期化処理
		pSample->Init();
		pSample->SetPosition(pos);
		pSample->BindType(TYPE_BUTTON);
	}

	return pSample;
}

//==========================================================
// 判定確認
//==========================================================
bool CGimmickButton::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand)
{
	bool bValue = false;
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	// 範囲内チェック
	float fLength =
		sqrtf((pos.x - ObjPos.x) * (pos.x - ObjPos.x)
			+ (pos.z - ObjPos.z) * (pos.z - ObjPos.z));

	if (fLength > COLLISION_RANGE){		// 範囲外
		return bValue;
	}

	m_state = STATE_PRESS;		// 押された状態にする
	CManager::GetInstance()->GetDebugProc()->Print("押されているよー\n");

	return bValue;
}

//==========================================================
// スイッチ切り替え
//==========================================================
void CGimmickButton::Switch(bool bUse)
{
	// 状態に合わせてボタンの色を変更
	switch (bUse)
	{
	case false:	// 何もない
		m_state = STATE_NONE;
		break;

	case true:	// 押されている
		m_state = STATE_PRESS;
		break;
	}
}

//==========================================================
// 状態を取得
//==========================================================
CGimmickButton::STATE CGimmickButton::GetState(void)
{
	return m_state;
}

//==========================================================
// トラップ色に変更
//==========================================================
void CGimmickButton::TrapMaterial(void)
{
	if (m_apObj[MODEL_BUTTON] != nullptr) {
		m_apObj[MODEL_BUTTON]->BindModelFile(
			CManager::GetInstance()->GetModelFile()->Regist("data\\MODEL\\trans_button_trap.x")
		);
	}
}

//==========================================================
// 色変更
//==========================================================
void CGimmickButton::SetPressMaterial(const D3DMATERIAL9& mat)
{
	if (m_apObj[MODEL_BUTTON] == nullptr) {
		return;
	}

	m_apObj[MODEL_BUTTON]->SetMaterial(mat);
}