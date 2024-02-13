//==========================================================
//
// 落とし穴 [gimmick_pitfall.cpp]
// Author : Soma Ishihara
//
//==========================================================
#include "gimmick_pitfall.h"
#include "Xfile.h"
#include "model.h"
#include "input.h"
#include "manager.h"
#include "gimmick_button.h"

// マクロ定義
#define ROTATE_ANGLE	(0.05f * D3DX_PI)	//床の開く角度

//==========================================================
// コンストラクタ
//==========================================================
CGimmickPitFall::CGimmickPitFall()
{
	// 値のクリア
	m_bOpen = false;
	m_fAngle = 0.0f;
	m_pButton = nullptr;
}

//==========================================================
// デストラクタ
//==========================================================
CGimmickPitFall::~CGimmickPitFall()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGimmickPitFall::Init(void)
{
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGimmickPitFall::Uninit(void)
{
	ListOut();
	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CGimmickPitFall::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (m_pButton != nullptr) {	// ボタンが使用されている
		if (m_pButton->GetState() == CGimmickButton::STATE_PRESS) {	// ボタンが押されている
			m_bOpen = true;
		}
		else {	// 押されていない
			m_bOpen = false;
		}
	}

	//開閉による角度設定
	if (m_bOpen == true)
	{//開いてる
		m_fAngle += ROTATE_ANGLE;
		if (m_fAngle > 0.5f * D3DX_PI)
		{
			m_fAngle = 0.5f * D3DX_PI;
		}
	}
	else
	{//閉じてる
		m_fAngle -= ROTATE_ANGLE;
		if (m_fAngle < 0.0f * D3DX_PI)
		{
			m_fAngle = 0.0f * D3DX_PI;
		}
	}

	//モデルに設定
	D3DXVECTOR3 rot = m_apModel[0]->GetRotation();
	m_apModel[0]->SetRotation(D3DXVECTOR3(0.0f, rot.y, -m_fAngle));

	rot = m_apModel[1]->GetRotation();
	m_apModel[1]->SetRotation(D3DXVECTOR3(0.0f, rot.y, -m_fAngle));
}

//==========================================================
// 生成
//==========================================================
CGimmickPitFall *CGimmickPitFall::Create(const D3DXVECTOR3 pos)
{
	CGimmickPitFall *pPitFall = nullptr;

	pPitFall = new CGimmickPitFall;

	if (pPitFall != nullptr)
	{
		// 初期化処理
		pPitFall->Init();
		pPitFall->SetPosition(pos);
		pPitFall->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pPitFall->BindType(TYPE_PITFALL);

		//オブジェクト生成
		//左側
		pPitFall->m_apModel[FLOOR_LEFT] = CModel::Create("data\\MODEL\\pitfall_floor.x");
		pPitFall->m_apModel[FLOOR_LEFT]->SetPosition(pos - D3DXVECTOR3(100.0f, 0.0f, 0.0f));
		pPitFall->m_apModel[FLOOR_LEFT]->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pPitFall->m_apModel[FLOOR_LEFT]->SetShadow();

		//右側
		pPitFall->m_apModel[FLOOR_RIGHT] = CModel::Create("data\\MODEL\\pitfall_floor.x");
		pPitFall->m_apModel[FLOOR_RIGHT]->SetPosition(pos + D3DXVECTOR3(100.0f, 0.0f, 0.0f));
		pPitFall->m_apModel[FLOOR_RIGHT]->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		pPitFall->m_apModel[FLOOR_RIGHT]->SetShadow();
	}

	return pPitFall;
}

//==========================================================
// 当たり判定
//==========================================================
bool CGimmickPitFall::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand)
{
	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (bLand != nullptr)
	{
		*bLand = false;
	}

	//Y
	if (m_bOpen == false)
	{//開いてないときのみ当たり判定を行う
		for (int cnt = 0; cnt < FLOOR_MAX; cnt++)
		{
			// 向きを反映
			int nID = m_apModel[cnt]->GetId();
			m_apModel[cnt]->SetRotSize(vtxObjMax,
				vtxObjMin,
				pFile->GetMax(nID),
				pFile->GetMin(nID),
				m_apModel[cnt]->GetRotation().y);

			D3DXVECTOR3 posObj = m_apModel[cnt]->GetPosition();

			if (pos.x + vtxMax.x >= posObj.x + vtxObjMin.x
				&& pos.x + vtxMin.x <= posObj.x + vtxObjMax.x
				&& pos.z + vtxMax.z >= posObj.z + vtxObjMin.z
				&& pos.z + vtxMin.z <= posObj.z + vtxObjMax.z)
			{//範囲内にある
			 //上からの判定
				if (posOld.y >= posObj.y + vtxObjMax.y
					&& pos.y < posObj.y + vtxObjMax.y)
				{//上からめり込んだ
				 //上にのせる
					pos.y = posObj.y + vtxObjMax.y;
					move.y = 0.0f;
					if (bLand != nullptr)
					{
						*bLand = true;
					}
				}
			}
		}
	}

	return false;
}

//==========================================================
// 使用ボタン割り当て
//==========================================================
void CGimmickPitFall::IdSetButton(void)
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