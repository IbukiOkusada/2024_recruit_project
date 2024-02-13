//==========================================================
//
// ギミック生える槍 [gimmick_multidoor.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_multidoor.h"
#include "model.h"
#include "manager.h"
#include "debugproc.h"
#include "player.h"
#include "Xfile.h"
#include "gimmick_button.h"
#include "input.h"
#include "manager.h"
#include "sound.h"

// 無名名前空間
namespace {
	const char* FILENAME = "data\\MODEL\\multidoor.x";	// 使用するモデルファイル名
	const D3DXVECTOR3 SETPOS = { 150.0f, 0.0f, 0.0f };		// 初期設定座標
	const float INERMULTI[CGimmickMultiDoor::STATE_MAX] = {0.75f, 0.15f, 0.0045f};	// 補正倍率
	const D3DXVECTOR3 OPENPOS = { 300.0f, 0.0f, 0.0f };
	const int CLOSECOUNTER = (600);
}

//==========================================================
// コンストラクタ
//==========================================================
CGimmickMultiDoor::CGimmickMultiDoor()
{
	// 値のクリア
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		m_aObject[nCnt].pModel = nullptr;
		m_aObject[nCnt].posDest = { 0.0f, 0.0f, 0.0f };
		m_aObject[nCnt].posOld = { 0.0f, 0.0f, 0.0f };
	}

	m_state = STATE_NEUTRAL;
	m_nStateCnt = 0;
	m_nNumSwitch = 0;
	m_nNumLinkSwitch = 0;
	m_fInerMulti = 0.0f;
	m_ppButton = nullptr;
	m_nActiveSwitch = 0;
	m_nCount = 0;
}

//==========================================================
// デストラクタ
//==========================================================
CGimmickMultiDoor::~CGimmickMultiDoor()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGimmickMultiDoor::Init(void)
{
	// モデルの生成
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		m_aObject[nCnt].pModel = CModel::Create(FILENAME);
		m_aObject[nCnt].pModel->SetParent(GetMtxWorld());
		m_aObject[nCnt].pModel->SetPosition({ SETPOS.x - (SETPOS.x * nCnt * 2), SETPOS.y, SETPOS.z});
		m_aObject[nCnt].pModel->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * nCnt, 0.0f));
	}

	StateSet();

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGimmickMultiDoor::Uninit(void)
{
	// リストから削除
	ListOut();

	// 使用オブジェクトの削除
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		if (m_aObject[nCnt].pModel != nullptr) {	// モデルを使用されている場合
			m_aObject[nCnt].pModel->Uninit();
			m_aObject[nCnt].pModel = nullptr;
		}
	}

	if (m_ppButton != nullptr) {	// 生成前に使用されていた
		// 前回の数繰り返し
		for (int nCnt = 0; nCnt < m_nNumSwitch; nCnt++) {

			if (m_ppButton[nCnt] != nullptr) {	// 使用されている
				m_ppButton[nCnt] = nullptr;
			}
		}
		delete[] m_ppButton;	// ポインタの開放
		m_ppButton = nullptr;	// 使用していない状態にする
	}

	// リリース
	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CGimmickMultiDoor::Update(void)
{
	// マトリックス設定
	SetMtxWorld();

	// 状態更新
	UpdateState();

	// 補正
	ObjIner();

	int nActive = 0;
	// 開ける状態か確認
	for (int nCnt = 0; nCnt < m_nNumSwitch; nCnt++) {
		if (m_ppButton[nCnt] != nullptr) {	// 使用されている
			if (m_ppButton[nCnt]->GetState() == CGimmickButton::STATE_PRESS) {
				nActive++;
				continue;
			}
		}
	}

	if (nActive >= m_nNumLinkSwitch ||
		nActive >= m_nActiveSwitch) {	// スイッチが規定値押されている
		m_state = STATE_OPEN;
		StateSet();
	}
	else
	{
		if (m_state == STATE_OPEN) {
			m_state = STATE_CLOSE;
			StateSet();
		}
	}

	// マテリアル設定
	SetButtonMaterial(nActive);
}

//==========================================================
// 生成
//==========================================================
CGimmickMultiDoor *CGimmickMultiDoor::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	CGimmickMultiDoor *pGimmick = nullptr;

	pGimmick = new CGimmickMultiDoor;

	if (pGimmick != nullptr)
	{
		// 初期化処理
		pGimmick->Init();

		// 値の設定
		pGimmick->SetPosition(pos);
		pGimmick->SetRotation(rot);
		pGimmick->CGimmick::BindType(TYPE_SPEAR);
	}

	return pGimmick;
}

//==========================================================
// 状態管理
//==========================================================
void CGimmickMultiDoor::StateSet(void)
{
	// 状態ごとに設定
	switch (m_state) 
	{
	case STATE_NEUTRAL:

		for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
			if (m_aObject[nCnt].pModel != nullptr) {	// モデルを使用されている場合
				m_aObject[nCnt].posDest = m_aObject[nCnt].pModel->GetPosition();	// 閉じた状態を目標に
			}
		}

		break;

	case STATE_OPEN:

		for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
			if (m_aObject[nCnt].pModel != nullptr) {	// モデルを使用されている場合
				m_aObject[nCnt].posDest = { OPENPOS.x - (OPENPOS.x * nCnt * 2), OPENPOS.y, OPENPOS.z };	// 開いた状態を目標に
			}
		}

		break;

	case STATE_CLOSE:
		
		for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
			if (m_aObject[nCnt].pModel != nullptr) {	// モデルを使用されている場合
				m_aObject[nCnt].posDest = m_aObject[nCnt].pModel->GetPosition();	// 閉じた状態を目標に
			}
		}

		m_nStateCnt = CLOSECOUNTER;

		break;
	}

	// 補正倍率を設定
	m_fInerMulti = INERMULTI[m_state];
}

//==========================================================
// 判定確認
//==========================================================
bool CGimmickMultiDoor::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand)
{
	bool bValue = false;
	CXFile *pFile = CManager::GetInstance()->GetModelFile();

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		if (m_aObject[nCnt].pModel != nullptr) {	// モデルを使用されている場合
			D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 ObjPos = D3DXVECTOR3(m_aObject[nCnt].pModel->GetMtx()->_41, m_aObject[nCnt].pModel->GetMtx()->_42, m_aObject[nCnt].pModel->GetMtx()->_43);
			D3DXVECTOR3 ObjRot = GetRotation();

			// 向きを反映
			int nID = m_aObject[nCnt].pModel->GetId();
			m_aObject[nCnt].pModel->SetRotSize(vtxObjMax,
				vtxObjMin,
				pFile->GetMax(nID),
				pFile->GetMin(nID),
				m_aObject[nCnt].pModel->GetRotation().y);

			if (pos.y + vtxMax.y > ObjPos.y + vtxObjMin.y
				&& pos.y + vtxMin.y <= ObjPos.y + vtxObjMax.y)
			{//プレイヤーとモデルが同じ高さにある
				if (posOld.x + vtxMin.x >= m_aObject[nCnt].posOld.x + vtxObjMax.x
					&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x
					&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
					&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
				{//右から左にめり込んだ
					//位置を戻す
					pos.x = ObjPos.x + vtxObjMax.x - vtxMin.x + 0.1f;
					move.x = 0.0f;
				}
				else if (posOld.x + vtxMax.x <= m_aObject[nCnt].posOld.x + vtxObjMin.x
					&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
					&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
					&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
				{//左から右にめり込んだ
					//位置を戻す
					pos.x = ObjPos.x + vtxObjMin.x - vtxMax.x - 0.1f;
					move.x = 0.0f;
				}
				else if (posOld.z + vtxMin.z >= m_aObject[nCnt].posOld.z + vtxObjMax.z
					&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z
					&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
					&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x)
				{//奥から手前にめり込んだ
					//位置を戻す
					pos.z = ObjPos.z + vtxObjMax.z - vtxMin.z + 0.1f;
					move.z = 0.0f;
				}
				else if (posOld.z + vtxMax.z <= m_aObject[nCnt].posOld.z + vtxObjMin.z
					&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
					&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
					&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x)
				{//手前から奥にめり込んだt
				 //位置を戻す
					pos.z = ObjPos.z + vtxObjMin.z - vtxMax.z - 0.1f;
					move.z = 0.0f;
				}
			}

			//Y
			if (pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
				&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x
				&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
				&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
			{//範囲内にある
			 //上からの判定
				if (posOld.y + vtxMin.y >= ObjPos.y + vtxObjMax.y
					&& pos.y + vtxMin.y < ObjPos.y + vtxObjMax.y)
				{//上からめり込んだ
				 //上にのせる
					pos.y = ObjPos.y + vtxObjMax.y - vtxMin.y;
					move.y = 0.0f;
				}
			}
		}
	}

	return bValue;
}

//==========================================================
// 状態ごとの更新
//==========================================================
void CGimmickMultiDoor::UpdateState(void)
{
	// 状態ごとに設定
	switch (m_state) 
	{
	case STATE_NEUTRAL:

		break;

	case STATE_OPEN:

		m_nCount++;

		if (m_nCount == 1)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_OPEN00);
		}

		break;

	case STATE_CLOSE:

		m_nCount = 0;
		m_nStateCnt--;

		if (m_nStateCnt == 299)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_OPEN01);
		}

		if (m_nStateCnt < 0) {
			m_state = STATE_NEUTRAL;	// 待機状態に変更
			StateSet();
			
		}

		break;
	}
}

//==========================================================
// オブジェクトの補正倍率
//==========================================================
void CGimmickMultiDoor::ObjIner(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		if (m_aObject[nCnt].pModel != nullptr) {	// モデルを使用されている場合
			m_aObject[nCnt].posOld = { m_aObject[nCnt].pModel->GetMtx()->_41 , m_aObject[nCnt].pModel->GetMtx()->_42, m_aObject[nCnt].pModel->GetMtx()->_43};
			D3DXVECTOR3 pos = m_aObject[nCnt].pModel->GetCurrentPosition();
			D3DXVECTOR3 posDiff = m_aObject[nCnt].posDest - pos;
			pos += posDiff * m_fInerMulti;
			m_aObject[nCnt].pModel->SetCurrentPosition(pos);
		}
	}
}

//==========================================================
// ボタンをリンクさせる
//==========================================================
void CGimmickMultiDoor::BindButton(CGimmickButton *pButton)
{
	if(m_ppButton == nullptr){	// 使用先ボタンが生成されていない
		return;
	}

	m_nNumLinkSwitch = 0;

	for (int nCnt = 0; nCnt < m_nNumSwitch; nCnt++) {
		if (m_ppButton[nCnt] != nullptr) {	// 使用されている
			m_nNumLinkSwitch++;
			continue;
		}

		m_ppButton[nCnt] = pButton;
		m_nNumLinkSwitch++;
		break;
	}
}

//==========================================================
// ボタン使用数を設定
//==========================================================
void CGimmickMultiDoor::SetNumButton(const int nNum)
{
	CGimmickButton **ppButtonOld = m_ppButton;
	int nNumOld = m_nNumSwitch;

	if (m_ppButton != nullptr) {	// 既に使用されている
		m_ppButton = nullptr;
	}

	// ポインタを生成
	m_ppButton = new CGimmickButton*[nNum];
	m_nNumSwitch = nNum;
	m_nActiveSwitch = nNum;

	for (int nCnt = 0; nCnt < m_nNumSwitch; nCnt++) {
		m_ppButton[nCnt] = nullptr;
	}

	if (ppButtonOld != nullptr) {	// 生成前に使用されていた
		// 前回の数繰り返し
		for (int nCnt = 0; nCnt < nNumOld; nCnt++){
			
			if (ppButtonOld[nCnt] != nullptr) {	// 使用されている
				BindButton(ppButtonOld[nCnt]);	// 使用されていたボタンを再設定
			}
		}
		delete[] ppButtonOld;	// ポインタの開放
		ppButtonOld = nullptr;	// 使用していない状態にする
	}
}

//==========================================================
// ボタン使用数を設定
//==========================================================
void CGimmickMultiDoor::SetActiveButton(const int nNum)
{
	m_nActiveSwitch = nNum;

	if (m_nActiveSwitch > m_nNumSwitch) {	// 規定数よりも多い場合
		m_nActiveSwitch = m_nNumSwitch;
	}
	else if (m_nActiveSwitch < 0) {	// 0未満
		m_nActiveSwitch = 0;
	}
}

//==========================================================
// 使用ボタン割り当て
//==========================================================
void CGimmickMultiDoor::IdSetButton(void)
{
	CGimmick *pGimk = CGimmick::GetTop();

	// 最後まで繰り返し
	while (pGimk != nullptr){
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

		pGimk = pGimkNext;
	}
}

//==========================================================
// 外積当たり判定
//==========================================================
bool CGimmickMultiDoor::CollisionCheckCloss(D3DXVECTOR3 & pos, D3DXVECTOR3 & posOld, D3DXVECTOR3 * posCollisioned)
{
	CXFile* pFile = CManager::GetInstance()->GetModelFile();
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool bCollision = false;
	D3DXVECTOR3 posCulcNear = D3DXVECTOR3(FLT_MAX, 0.0f, 0.0f);

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) 
	{
		if (m_aObject[nCnt].pModel != nullptr) 
		{// モデルを使用されている場合
			D3DXVECTOR3 ObjPos = D3DXVECTOR3(m_aObject[nCnt].pModel->GetMtx()->_41, m_aObject[nCnt].pModel->GetMtx()->_42, m_aObject[nCnt].pModel->GetMtx()->_43);

			// 向きを反映
			int nID = m_aObject[nCnt].pModel->GetId();
			m_aObject[nCnt].pModel->SetRotSize(vtxObjMax,
				vtxObjMin,
				pFile->GetMax(nID),
				pFile->GetMin(nID),
				m_aObject[nCnt].pModel->GetRotation().y);

			D3DXVECTOR3 posPoint[4] =
			{
				D3DXVECTOR3(ObjPos.x + vtxObjMin.x,0.0f,ObjPos.z + vtxObjMin.z),
				D3DXVECTOR3(ObjPos.x + vtxObjMax.x,0.0f,ObjPos.z + vtxObjMin.z),
				D3DXVECTOR3(ObjPos.x + vtxObjMax.x,0.0f,ObjPos.z + vtxObjMax.z),
				D3DXVECTOR3(ObjPos.x + vtxObjMin.x,0.0f,ObjPos.z + vtxObjMax.z)
			};

			D3DXVECTOR3 vecMove, vecLine;
			D3DXVECTOR3 vecToPos, vecToPosOld;
			float fAreaA = 1.0f, fAreaB = 1.1f;

			for (int cnt = 0; cnt < 4; cnt++)
			{
				vecMove = pos - posOld;
				vecLine = posPoint[(cnt + 1) % 4] - posPoint[cnt];	//境界線ベクトル
				vecToPos = pos - posPoint[cnt];
				vecToPos.y = 0.0f;
				vecToPosOld = posOld - posPoint[cnt];
				vecToPosOld.y = 0.0f;

				//面積求める
				fAreaA = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
				fAreaB = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);

				if ((vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z) >= 0.0f && (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < 0.0f)
				{
					if (fAreaA / fAreaB >= 0.0f && fAreaA / fAreaB <= 1.0f)
					{//ごっつん
					 //衝突位置（XZのみ。Yはposの値を使用）が欲しければあげる
						if (posCollisioned != nullptr)
						{//ほしいみたいなのであげる
							float fRate = fAreaA / fAreaB;
							D3DXVECTOR3 posCulc = posPoint[cnt];
							posCulc.x += vecLine.x * fRate;
							posCulc.y = posOld.y;
							posCulc.z += vecLine.z * fRate;

							if (D3DXVec3Length(&(posCulc - posOld)) < D3DXVec3Length(&(posCulcNear - posOld)))
							{
								posCulcNear = posCulc;
							}
						}

						bCollision = true;
					}
				}
			}
		}
	}

	if (bCollision == true)
	{
		*posCollisioned = posCulcNear;
	}

	return bCollision;
}

//==========================================================
// 入力数に応じて色の変更
//==========================================================
void CGimmickMultiDoor::SetButtonMaterial(int nPressCnt)
{
	if (m_ppButton == nullptr) {
		return;
	}

	D3DMATERIAL9 mat = {};	// マテリアル格納
	D3DXCOLOR col = D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f);
	
	// 押されていないボタンの残りの数に合わせて色を変える
	switch (m_nActiveSwitch - nPressCnt) {
	case 1:
		col = D3DXCOLOR(0.35f, 0.3f, 0.05f, 1.0f);
		break;

	case 2:
		col = D3DXCOLOR(0.3f, 0.05f, 0.4f, 1.0f);
		break;

	case 3:
		col = D3DXCOLOR(0.1f, 0.1f, 0.5f, 1.0f);
		break;

	default:
		col = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);
		break;
	}

	// 色を入れる
	mat.Ambient = col;
	mat.Diffuse = col;
	mat.Emissive = col;

	for (int nCnt = 0; nCnt < m_nNumSwitch; nCnt++) {
		if (m_ppButton[nCnt] == nullptr) {	// 使用されていない
			continue;
		}
		// マテリアル変更
		m_ppButton[nCnt]->SetPressMaterial(mat);
	}
}
