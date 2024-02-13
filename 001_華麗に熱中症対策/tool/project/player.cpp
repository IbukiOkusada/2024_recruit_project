//===============================================
//
// プレイヤーの処理 [player.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "Xfile.h"
#include "shadow.h"
#include "waist.h"
#include "character.h"
#include "motion.h"
#include "model.h"
#include "texture.h"
#include "meshfield.h"
#include "edit.h"

//===============================================
// マクロ定義
//===============================================
#define PLAYER_GRAVITY	(-0.7f)	//プレイヤー重力
#define PLAYER_JUMP		(15.0f)	//プレイヤージャンプ力
#define MOVE	(2.0f)

//===============================================
// コンストラクタ
//===============================================
//CPlayer::CPlayer()
//{
//	// 値をクリアする
//	m_nCounterAnim = 0;
//	m_nPatternAnim = 0;
//}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CPlayer::CPlayer(const D3DXVECTOR3 pos)
{
	// 値をクリアする
	m_bJump = false;
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRotMove = 0.0f;
	m_fRotDiff = 0.0f;
	m_fRotDest = 0.0f;
	pShadow = NULL;
	m_pBody = NULL;
	m_pLeg = NULL;
	m_pWaist = NULL;
}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CPlayer::CPlayer(int nPriOrity)
{
	// 値をクリアする
	m_bJump = false;
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRotMove = 0.0f;
	m_fRotDiff = 0.0f;
	m_fRotDest = 0.0f;
	pShadow = NULL;
	m_pBody = NULL;
	m_pLeg = NULL;
	m_pWaist = NULL;
}

//===============================================
// デストラクタ
//===============================================
CPlayer::~CPlayer()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CPlayer::Init(void)
{
	// 腰の生成
	if (m_pWaist == NULL)
	{
		m_pWaist = new CWaist;
		m_pWaist->SetParent(&m_Info.mtxWorld);
	}

	return S_OK;
}

//===============================================
// 初期化処理(オーバーロード)
//===============================================
HRESULT CPlayer::Init(const char *pBodyName, const char *pLegName)
{
	CTexture *pTexture = CManager::GetTexture();

	// 腰の生成
	if (m_pWaist == NULL)
	{
		m_pWaist = new CWaist;
		m_pWaist->SetParent(&m_Info.mtxWorld);
	}

	// 胴体の設定
	if (pBodyName != NULL)
	{// ファイル名が存在している
		m_pBody = CCharacter::Create(pBodyName);
		m_pBody->SetParent(m_pWaist->GetMtxWorld());

		if (m_pBody->GetMotion() != NULL)
		{
			// 初期モーションの設定
			m_pBody->GetMotion()->Set(0);
		}
	}

	// 下半身の設定
	if (pLegName != NULL)
	{// ファイル名が存在している
		m_pLeg = CCharacter::Create(pLegName);
		m_pLeg->SetParent(m_pWaist->GetMtxWorld());

		if (m_pLeg->GetMotion() != NULL)
		{
			// 初期モーションの設定
			m_pLeg->GetMotion()->Set(0);
		}
	}

	// 腰の高さを合わせる
	if (m_pLeg != NULL)
	{// 脚が使用されている場合
		CModel *pModel = m_pLeg->GetParts(0);	// 腰パーツを取得

		if (pModel != NULL)
		{// パーツが存在する場合
			D3DXVECTOR3 pos = pModel->GetPosition();	// モデルの相対位置を取得

			// 高さを設定
			m_pWaist->SetHeight(pos);

			// 腰のモデルの位置を変更
			pModel->SetPosition(pos);

			CMotion::KEY key = m_pLeg->GetMotion()->GetKey(CManager::GetEdit()->GetKey(), CManager::GetEdit()->GetMotion(), 0);
			pos = { key.fPosX, key.fPosY, key.fPosZ };
			m_pWaist->SetPosition(m_pWaist->GetSetPosition() + pos);
		}
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CPlayer::Uninit(void)
{

	// 胴体の終了処理
	if (m_pBody != NULL)
	{
		m_pBody->Uninit();
		delete m_pBody;
		m_pBody = NULL;
	}

	// 下半身の終了処理
	if (m_pLeg != NULL)
	{
		m_pLeg->Uninit();
		delete m_pLeg;
		m_pLeg = NULL;
	}

	// 腰の終了処理
	if (m_pWaist != NULL)
	{
		delete m_pWaist;
		m_pWaist = NULL;
	}

	// 廃棄
	Release();
}

//===============================================
// 更新処理
//===============================================
void CPlayer::Update(void)
{
	// 前回の座標を取得
	m_Info.posOld = GetPosition();

	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();	// キーボードのポインタ

	// 胴体更新
	if (m_pBody != NULL)
	{// 使用されている場合
		m_pBody->Update();
	}
	
	// 下半身更新
	if (m_pLeg != NULL)
	{// 使用されている場合
		m_pLeg->Update();

		CModel *pModel = m_pLeg->GetParts(0);

		// 腰の高さを補填
		if (pModel != NULL)
		{
			m_pWaist->SetPosition(m_pWaist->GetSetPosition() + pModel->GetCurrentPosition());
		}
	}

	if (CManager::GetEdit()->GetState() == CEdit::STATE_PLAY)
	{
		// プレイヤー操作
		Controller();
	}

	// 向き設定
	//SetBodyRot();

	// カメラ追従
	CCamera *pCamera = CManager::GetCamera();
	//pCamera->Pursue(GetPosition(), GetRotation());
	
	// 影の座標更新
	if (pShadow != NULL)
	{
		if (m_pBody != NULL)
		{
			pShadow->SetPosition(D3DXVECTOR3(m_pBody->GetParts(0)->GetMtxWorld()->_41, 0.0f, m_pBody->GetParts(0)->GetMtxWorld()->_43));
		}
	}
}


//===============================================
// 描画処理
//===============================================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスへのポインタを取得
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_Info.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_Info.mtxWorld);

	// 腰の設定
	if (m_pWaist != NULL)
	{
		m_pWaist->SetMatrix();
	}

	// 下半身の描画
	if (m_pLeg != NULL)
	{
		CModel *pModel = m_pLeg->GetParts(0);

		D3DXVECTOR3 pos = pModel->GetCurrentPosition();

		pModel->SetCurrentPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		if (CManager::GetEdit()->GetType() == CEdit::TYPE_LEG && CManager::GetEdit()->GetState() == CEdit::STATE_EDIT)
		{
			m_pLeg->DrawCheck();
		}
		else
		{
			m_pLeg->Draw();
		}

		pModel->SetCurrentPosition(pos);
	}

	// 胴体の描画
	if (m_pBody != NULL)
	{
		if (CManager::GetEdit()->GetType() == CEdit::TYPE_BODY && CManager::GetEdit()->GetState() == CEdit::STATE_EDIT)
		{
			m_pBody->DrawCheck();
		}
		else
		{
			m_pBody->Draw();
		}
	}
}

//===============================================
// 生成
//===============================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, const char *pBodyName, const char *pLegName, const int nPriority)
{
	CPlayer *pPlayer = NULL;
	CXFile *pModelFile = CManager::GetModelFile();

	// オブジェクト2Dの生成
	pPlayer = new CPlayer(nPriority);

	if (pPlayer != NULL)
	{// 生成できた場合

		if (pPlayer->GetId() < 0)
		{// オブジェクト側で管理されていない場合
			delete pPlayer;

			return NULL;
		}

		// 初期化処理
		pPlayer->Init(pBodyName, pLegName);

		// 座標設定
		pPlayer->SetPosition(pos);

		// 向き設定
		pPlayer->SetRotation(rot);

		pPlayer->m_fRotDest = rot.y;

		// 移動量設定
		pPlayer->SetMove(move);

		// 種類の設定
		pPlayer->SetType(TYPE_PLAYER);

		// 影の設定
		pPlayer->pShadow = CShadow::Create(pos, 30.0f, 30.0f);
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pPlayer;
}

//===============================================
// 操作処理
//===============================================
void CPlayer::Controller(void)
{
	D3DXVECTOR3 pos = GetPosition();	// 座標を取得
	D3DXVECTOR3 rot = GetRotation();	// 向きを取得
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();	// キーボードのポインタ
	CCamera *pCamera = CManager::GetCamera();		// カメラのポインタ
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	bool bMove = false;	// 移動しているか否か
	float fSpeed = MOVE;

	m_fRotMove = rot.y;	//現在の向きを取得

	//プレイヤーの更新
	if (pInputKey->GetPress(DIK_A) == true)
	{
		if (pInputKey->GetPress(DIK_W) == true)
		{
			m_Info.move.x += cosf(CamRot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_Info.move.z += sinf(CamRot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_fRotDest = (-CamRot.y + D3DX_PI * 0.25f);
		}
		else if (pInputKey->GetPress(DIK_S) == true)
		{
			m_Info.move.x += cosf(CamRot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			m_Info.move.z += sinf(CamRot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			m_fRotDest = (-CamRot.y + -D3DX_PI * 0.25f);
		}
		else
		{
			m_Info.move.x += cosf(CamRot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			m_Info.move.z += sinf(CamRot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			m_fRotDest = -CamRot.y;
		}

		// 移動した状態にする
		bMove = true;
	}
	else if (pInputKey->GetPress(DIK_D) == true)
	{
		if (pInputKey->GetPress(DIK_W) == true)
		{
			m_Info.move.x += cosf(CamRot.y + (D3DX_PI * 0.75f)) * fSpeed;
			m_Info.move.z += sinf(CamRot.y + (D3DX_PI * 0.75f)) * fSpeed;

			m_fRotDest = (-CamRot.y + D3DX_PI * 0.75f);
		}
		else if (pInputKey->GetPress(DIK_S) == true)
		{
			m_Info.move.x += cosf(CamRot.y + (D3DX_PI * 0.25f)) * fSpeed;
			m_Info.move.z += sinf(CamRot.y + (D3DX_PI * 0.25f)) * fSpeed;

			m_fRotDest = (-CamRot.y + -D3DX_PI * 0.75f);
		}
		else
		{
			m_Info.move.x += cosf(CamRot.y + (D3DX_PI * 0.5f)) * fSpeed;
			m_Info.move.z += sinf(CamRot.y + (D3DX_PI * 0.5f)) * fSpeed;
			m_fRotDest = (-CamRot.y + D3DX_PI * 1.0f);
		}

		// 移動した状態にする
		bMove = true;
	}
	else if (pInputKey->GetPress(DIK_W) == true)
	{
		m_Info.move.x += -cosf(CamRot.y) * fSpeed;
		m_Info.move.z += -sinf(CamRot.y) * fSpeed;
		m_fRotDest = (-CamRot.y + D3DX_PI * 0.5f);

		// 移動した状態にする
		bMove = true;

	}
	else if (pInputKey->GetPress(DIK_S) == true)
	{
		m_Info.move.x += cosf(CamRot.y) * fSpeed;
		m_Info.move.z += sinf(CamRot.y) * fSpeed;
		m_fRotDest = (-CamRot.y + -D3DX_PI * 0.5f);

		// 移動した状態にする
		bMove = true;
	}

	if (m_bJump == false && bMove == true)
	{// ジャンプしていない状態で移動
		m_pBody->GetMotion()->Set(1);
		m_pLeg->GetMotion()->Set(1);
		CManager::GetEdit()->SetMotionNow(false);
	}
	else if (m_bJump == false && bMove == false)
	{
		if (CManager::GetEdit()->GetMotionNow() == false)
		{
			m_pBody->GetMotion()->Set(0);
			m_pLeg->GetMotion()->Set(0);
		}
	}

	// ジャンプ
	if (pInputKey->GetPress(DIK_SPACE) == true)
	{// SPACEキー
		if (m_bJump == false)
		{// ジャンプしていない場合
			m_bJump = true;
			m_Info.move.y = PLAYER_JUMP;
			m_pBody->GetMotion()->Set(3);
			m_pLeg->GetMotion()->Set(3);
		}
	}

	float fGravity = PLAYER_GRAVITY;
	m_Info.move.y += fGravity;
	m_Info.move.x += (0.0f - m_Info.move.x) * 0.25f;	//x座標
	m_Info.move.z += (0.0f - m_Info.move.z) * 0.25f;	//x座標

	pos.x += m_Info.move.x;
	pos.y += m_Info.move.y;
	pos.z += m_Info.move.z;

	if (m_fRotDest > D3DX_PI || m_fRotDest < -D3DX_PI)
	{//-3.14～3.14の範囲外の場合
		if (m_fRotDest > D3DX_PI)
		{
			m_fRotDest += (-D3DX_PI * 2);
		}
		else if (m_fRotDest < -D3DX_PI)
		{
			m_fRotDest += (D3DX_PI * 2);
		}
	}

	m_fRotDiff = m_fRotDest - m_fRotMove;	//目標までの移動方向の差分

	if (m_fRotDiff > D3DX_PI || m_fRotDiff < -D3DX_PI)
	{//-3.14～3.14の範囲外の場合
		if (m_fRotDiff > D3DX_PI)
		{
			m_fRotDiff += (-D3DX_PI * 2);
		}
		else if (m_fRotDiff < -D3DX_PI)
		{
			m_fRotDiff += (D3DX_PI * 2);
		}
	}

	rot.y += m_fRotDiff * 0.08f;

	if (rot.y > D3DX_PI || rot.y < -D3DX_PI)
	{//-3.14～3.14の範囲外の場合
		if (rot.y > D3DX_PI)
		{
			rot.y += (-D3DX_PI * 2);
		}
		else if (rot.y < -D3DX_PI)
		{
			rot.y += (D3DX_PI * 2);
		}
	}

	if (pos.y < 0.0f)
	{
		if (m_bJump == true)
		{

		}

		m_Info.move.y = 0.0f;
		pos.y = 0.0f;
		m_bJump = false;
	}
	
	// 頂点情報設定
	SetRotation(rot);
	SetPosition(pos);

	pos = GetPosition();
}

//===============================================
// 胴体の向き変更
//===============================================
void CPlayer::SetBodyRot(void)
{
	CCamera *pCamera = CManager::GetCamera();		// カメラのポインタ
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度

	if (m_pBody == NULL || m_pLeg == NULL)
	{// 二段階の階層構造ではない場合
		return;
	}

	D3DXVECTOR3 rot = m_pBody->GetRotation();	// 胴体の向きを取得

	rot.y = (-CamRot.y + D3DX_PI * 0.5f) - m_Info.rot.y;	// カメラの向いている方向に合わせる

	// 胴体の向きを設定
	m_pBody->SetRotation(rot);
}
