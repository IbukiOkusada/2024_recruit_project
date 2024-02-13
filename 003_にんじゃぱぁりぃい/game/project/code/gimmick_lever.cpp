//==========================================================
//
// ギミックレバー [gimmick_lever.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_lever.h"
#include "model.h"
#include "manager.h"
#include "debugproc.h"
#include "player.h"
#include "sound.h"

// マクロ定義
#define COLLISION_RANGE	(50.0f)
#define INTERVAL			(90)

// 静的メンバ変数宣言
char *CGimmickLever::m_apFileName[MODEL_MAX] = {
	"data\\MODEL\\lever_frame.x",
	"data\\MODEL\\lever.x",
};

//==========================================================
// コンストラクタ
//==========================================================
CGimmickLever::CGimmickLever()
{
	// 値のクリア
	for (int nCnt = 0; nCnt < MODEL_MAX; nCnt++)
	{
		m_apObj[nCnt] = nullptr;
	}

	m_nInterval = 0;
	m_state = STATE_NONE;
}

//==========================================================
// デストラクタ
//==========================================================
CGimmickLever::~CGimmickLever()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGimmickLever::Init(void)
{
	m_bUpDown = false;
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

	m_apObj[MODEL_BUTTON]->SetCurrentRotation(D3DXVECTOR3(D3DX_PI * 0.15f, 0.0f, 0.0f));

	D3DMATERIAL9 material = {};
	material.Ambient = D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f);
	material.Emissive = D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f);
	m_apObj[MODEL_BUTTON]->SetMaterial(material);

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGimmickLever::Uninit(void)
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
void CGimmickLever::Update(void)
{
	if (m_nInterval > 0){
		m_nInterval--;
	}

	// マトリックス設定
	SetMtxWorld();
	bool bChange = false;

	// 状態に合わせてボタンの色を変更
	switch (m_bUpDown)
	{
	case false:	// 何もない
		m_RotDest = D3DXVECTOR3(D3DX_PI * 0.15f, 0.0f, 0.0f);
		break;

	case true:	// 押されている
		m_RotDest = D3DXVECTOR3(-D3DX_PI * 0.15f, 0.0f, 0.0f);
		break;
	}

	if (m_state == STATE_PRESS) {
		bChange = true;
	}

	if (m_apObj[MODEL_BUTTON] != nullptr) {
		D3DXVECTOR3 rot = m_apObj[MODEL_BUTTON]->GetCurrentRotation();
		D3DXVECTOR3 rotDiff = m_RotDest - rot;
		rot += rotDiff * 0.1f;
		m_apObj[MODEL_BUTTON]->SetCurrentRotation(rot);
		m_apObj[MODEL_BUTTON]->ChangeCol(bChange);
	}
}

//==========================================================
// 生成
//==========================================================
CGimmickLever *CGimmickLever::Create(const D3DXVECTOR3 pos)
{
	CGimmickLever *pSample = nullptr;

	pSample = new CGimmickLever;

	if (pSample != nullptr)
	{
		// 初期化処理
		pSample->Init();
	
		pSample->SetPosition(pos);
		pSample->BindType(TYPE_LEVER);
	}

	return pSample;
}

//==========================================================
// 判定確認
//==========================================================
bool CGimmickLever::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand)
{
	bool bValue = false;
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	// 範囲内チェック
	float fLength =
		sqrtf((pos.x - ObjPos.x) * (pos.x - ObjPos.x)
			+ (pos.z - ObjPos.z) * (pos.z - ObjPos.z));

	if (fLength > COLLISION_RANGE) {		// 範囲外
		return bValue;
	}

	if (nAction != CPlayer::ACTION_ATK) {
		return bValue;
	}

	if(m_nInterval > 0)
	{
		return bValue;
	}
	
	m_bUpDown = m_bUpDown ? false : true;
	m_state = STATE_PRESS;	// 押された状態にする
	m_nInterval = INTERVAL;			// インターバル
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_LEVER);

	return bValue;
}

//==========================================================
// スイッチ切り替え
//==========================================================
void CGimmickLever::Switch(bool bUse)
{
	// 状態に合わせてボタンの色を変更
	switch (bUse)
	{
	case false:	// 何もない
		m_state = STATE_NONE;
		m_bUpDown = false;
		break;

	case true:	// 押されている
		m_state = STATE_PRESS;
		m_bUpDown = true;
		break;
	}
}