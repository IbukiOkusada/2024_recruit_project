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
#include "bullet.h"
#include "explosion.h"
#include "multibg.h"
#include "particle.h"
#include "camera.h"
#include "Xfile.h"
#include "shadow.h"
#include "waist.h"
#include "character.h"
#include "motion.h"
#include "model.h"
#include "slow.h"
#include "texture.h"
#include "meshfield.h"
#include "Xfile.h"
#include "lockon.h"
#include "meshorbit.h"
#include "game.h"
#include "filter.h"
#include "object2D.h"
#include "meshballoon.h"
#include "meshwall.h"
#include "objectX.h"
#include "car_manager.h"
#include "car.h"
#include "sound.h"

//===============================================
// マクロ定義
//===============================================
#define MOVE	(2.2f)		// 移動量
#define SHW_MOVE	(1.0f)	// シャワー中移動量
#define PLAYER_GRAVITY	(-0.15f)		//プレイヤー重力
#define PLAYER_JUMP		(10.0f)		//プレイヤージャンプ力
#define DEF_SLOWTIME	(60 * 5)	// スロー可能時間
#define SLOW_OK			(60 * 2)	// スロー可能になる時間
#define SHOT_LENGTH	(4000.0f)		// 射程範囲
#define SHOT_RANGE	(D3DX_PI * 0.9f)	// 射程角度
#define SHOT_INTERVAL	(10)		// 攻撃インターバル
#define GUN_BULMOVE	(50.0f)
#define DEF_SLOWGAGELENGSH	(SCREEN_WIDTH * 0.4f)	// スローゲージマックスサイズ
#define SLOWGAGE_HEIGHT	(SCREEN_HEIGHT * 0.015f)
#define GAGE_TEXNAME	"data\\TEXTURE\\gage000.jpg"	// ゲージファイル名
#define MANUAL_TEXNAME	"data\\TEXTURE\\slow_button.png"
#define NOCHARGE_CNT	(20)		// チャージまでのカウント数
#define BALLOON_MOVE	(25.0f)		// 風船移動量
#define BALLOON_WEIGHT	(150.0f)	// 最大重量
#define WIDTH	(20.0f)		// 幅
#define HEIGHT	(80.0f)		// 高さ
#define GAGE_SPEED	(0.001f)

//===============================================
// 武器ごとの設定
//===============================================
const D3DXVECTOR3 CPlayer::SetWepPos[WEAPON_MAX] =	// 武器設定位置
{
	{ -13.0f, 4.5f, 0.0f },
	{ 13.0f, 4.5f, 0.0f },
	{ -7.0f, 13.0f, 0.0f },
};

//===============================================
// 武器ごとの攻撃タイマー
//===============================================
const int CPlayer::m_aWepTimer[ATK_MAX] =		// 攻撃タイマー
{
	21,
	5,
};

//===============================================
// 武器ファイル名
//===============================================
const char *CPlayer::m_apFileName[WEAPON_MAX] = 
{
	"data\\MODEL\\sample\\watergunL.x",
	"data\\MODEL\\sample\\watergunR.x",
	"data\\MODEL\\sample\\watershower.x",
};

//===============================================
// 風船初期設定
//===============================================
const CMeshBalloon::SET CPlayer::m_SetBalloon =
{
	D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20.0f, 10.0f, 5, 10, 10,
};

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
	m_nSlowTime = 0;
	pShadow = NULL;
	m_pBody = NULL;
	m_pLeg = NULL;
	m_pWaist = NULL;
	m_pWeaponL = NULL;
	m_pWeaponR = NULL;
	m_pSlowGage = NULL;
	m_pBalloon = NULL;
	m_pMapIcon = NULL;
	m_fMove = 0.0f;
	m_fTexU = 0.0f;
}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CPlayer::CPlayer(int nPriOrity)
{
	// 値をクリアする
	m_bJump = false;
	m_bActiveSlow = false;
	m_bSlow = false;
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRotMove = 0.0f;
	m_fRotDiff = 0.0f;
	m_fRotDest = 0.0f;
	m_nSlowTime = 0;
	pShadow = NULL;
	m_pBody = NULL;
	m_pLeg = NULL;
	m_pWaist = NULL;
	m_pWeaponL = NULL;
	m_pWeaponR = NULL;
	m_pSlowGage = NULL;
	m_pBalloon = NULL;
	m_pMapIcon = NULL;
	m_bAttack = false;
	m_fAttackTimer = 0;
	m_nAttackHand = 0;
	m_fChargeCnt = 0;
	m_pCar = NULL;
	m_pSlowManual = NULL;
	m_fMove = 0.0f;
	m_fTexU = 0.0f;

	for (int nCnt = 0; nCnt < WEAPON_MAX; nCnt++)
	{
		m_aWepNum[nCnt] = -1;
	}
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
	m_fMove = 1.0f;

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
			m_pBody->GetMotion()->InitSet(BMOTION_NEUTRAL);
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
			m_pLeg->GetMotion()->InitSet(LMOTION_NEUTRAL);
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
		}
	}

	// スロー可能時間を初期設定
	m_nSlowTime = 0;

	CModel *pModel = m_pBody->GetParts(9);	// 左手パーツを取得

	// 武器の設定
	m_pWeaponL = CModel::Create(m_apFileName[WEAPON_GUNL]);
	m_pWeaponL->SetPosition(SetWepPos[WEAPON_GUNL]);
	m_pWeaponL->SetParent(pModel->GetMtxWorld());

	pModel = m_pBody->GetParts(5);	// 右手パーツを取得

	// 武器の設定
	m_pWeaponR = CModel::Create(m_apFileName[WEAPON_GUNR]);
	m_pWeaponR->SetPosition(SetWepPos[WEAPON_GUNR]);
	m_pWeaponR->SetParent(pModel->GetMtxWorld());

	// ゲージの設定
	m_pSlowGage = CObject2D::Create(7);
	m_pSlowManual = CObject2D::Create(7);

	if (m_pSlowGage != NULL)
	{
		m_pSlowGage->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.95f, 0.0f));
		m_pSlowGage->SetSize(0, SLOWGAGE_HEIGHT);
		m_pSlowGage->BindTexture(CManager::GetTexture()->Regist(GAGE_TEXNAME));
	}

	if (m_pSlowManual != NULL)
	{
		m_pSlowManual->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.85f, 0.0f));
		m_pSlowManual->SetSize(SCREEN_WIDTH * 0.12f, SCREEN_HEIGHT * 0.09f);
		m_pSlowManual->BindTexture(CManager::GetTexture()->Regist(MANUAL_TEXNAME));
	}

	// 種類設定
	m_WepType = ATK_GUN;
	m_WepTypeOld = m_WepType;

	// 武器番号取得
	for (int nCnt = 0; nCnt < WEAPON_MAX; nCnt++)
	{
		m_aWepNum[nCnt] = CManager::GetModelFile()->Regist(m_apFileName[nCnt]);
	}

	// ロックオンの生成
	m_pLockon = CLockOn::Create(m_pBody->GetMtxWorld());
	m_pLockon->SetLock(true);

	// 軌跡の生成
	m_pOrbit = CMeshOrbit::Create(&m_Info.mtxWorld, D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// マップアイコンの生成
	if (m_pMapIcon == NULL)
	{
		m_pMapIcon = CObject3D::Create(GetPosition(), GetRotation());
		m_pMapIcon->SetType(CObject::TYPE_MAP);
		m_pMapIcon->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\mapicon000.png"));
		m_pMapIcon->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
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

	// 武器の終了
	if (m_pWeaponL != NULL)
	{
		m_pWeaponL->Uninit();
		delete m_pWeaponL;
		m_pWeaponL = NULL;
	}
	if (m_pWeaponR != NULL)
	{
		m_pWeaponR->Uninit();
		delete m_pWeaponR;
		m_pWeaponR = NULL;
	}

	if (m_pSlowGage != NULL)
	{
		m_pSlowGage = NULL;
	}

	// 廃棄
	Release();
}

//===============================================
// 更新処理
//===============================================
void CPlayer::Update(void)
{
	if (CManager::GetSlow()->Get() > 1.0f)
	{
		return;
	}

	// 前回の座標を取得
	m_Info.posOld = GetPosition();
	int nSlowTimerOld = m_nSlowTime;	// 変更前のスロー可能時間

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

	// プレイヤー操作
	Controller();

	// 向き設定
	SetBodyRot();

	// カメラ追従
	CCamera *pCamera = CManager::GetCamera();

	// 追従処理
	pCamera->Pursue(GetPosition(), GetRotation());

	CManager::GetDebugProc()->Print("向き [%f, %f, %f]", GetRotation().x, GetRotation().y, GetRotation().z);

	// パーティクル
	Particle();

	if (m_bLock == false)
	{// ロックオンしない
		pCamera->SetRot(GetRotation());
	}

	if (m_pSlowGage == NULL)
	{// 使用していない、またはスロー時間に変更がない
		return;
	}

	// スロー最大時間からの割合を求める
	float fRate = (float)m_nSlowTime / (float)DEF_SLOWTIME;

	m_fTexU += GAGE_SPEED;

	if (m_fTexU > 1.0f)
	{
		m_fTexU -= 1.0f;
	}

	m_pSlowGage->SetSize(DEF_SLOWGAGELENGSH * fRate, SLOWGAGE_HEIGHT);
	m_pSlowGage->SetTex(-5.0f * fRate, m_fTexU, 5.0f * fRate);
	SetGageColor(fRate);
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

		m_pLeg->Draw();

		pModel->SetCurrentPosition(pos);
	}

	// 胴体の描画
	if (m_pBody != NULL)
	{
		m_pBody->Draw();
	}
	
	// 武器の描画
	if (m_pWeaponL != NULL)
	{
		m_pWeaponL->Draw();
	}

	// 武器の描画
	if (m_pWeaponR != NULL && m_WepType == ATK_GUN)
	{// 使用しているかつ二丁持ち
		m_pWeaponR->Draw();
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

		CManager::GetCamera()->Setting(pos, rot);
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
	CInputMouse *pInputMouse = CManager::GetInputMouse();
	CInputPad *pInputPad = CManager::GetInputPad();
	CCamera *pCamera = CManager::GetCamera();		// カメラのポインタ
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	m_fRotMove = rot.y;	//現在の向きを取得
	m_bMove = false;	// 移動状態リセット

	// 移動
	if (m_bJump == false)
	{// ジャンプしていない場合
		Move();
	}

	if (m_bJump == false && m_bMove == true)
	{// ジャンプしていない状態で移動
		if (m_bAttack == false)
		{
			m_pBody->GetMotion()->BlendSet(BMOTION_WALK);
		}
		m_pLeg->GetMotion()->BlendSet(LMOTION_WALK);
	}
	else if (m_bJump == false && m_bMove == false)
	{
		if (m_bAttack == false)
		{
			m_pBody->GetMotion()->BlendSet(BMOTION_NEUTRAL);
		}
		m_pLeg->GetMotion()->BlendSet(LMOTION_NEUTRAL);
	}
	else if (m_bJump == true && m_bAttack == false)
	{
		m_pBody->GetMotion()->BlendSet(BMOTION_JUMP);
	}

	// 武器切り替え
	if ((pInputKey->GetTrigger(DIK_Q) == true || pInputPad->GetTrigger(CInputPad::BUTTON_Y, 0)) && CManager::GetSlow()->Get() == 1.0f)
	{// Qキー
		m_WepTypeOld = m_WepType;
		m_WepType = (ATK)((m_WepType + 1) % ATK_MAX);

		switch (m_WepType)
		{
		case ATK_GUN:
			m_pWeaponL->BindModelFile(m_aWepNum[WEAPON_GUNL]);
			m_pWeaponL->SetPosition(SetWepPos[WEAPON_GUNL]);
			break;

		case ATK_SHOWER:
			m_pWeaponL->BindModelFile(m_aWepNum[WEAPON_SHOWER]);
			m_pWeaponL->SetPosition(SetWepPos[WEAPON_SHOWER]);
			break;
		}

		CLockOn::MultiDeath();

		if (m_WepType == ATK_GUN)
		{
			m_pLockon->SetLock(true);
		}
		else
		{
			m_pLockon->SetLock(false);
		}
	}

	// ジャンプ
	Jump();

	//攻撃
	Attack();

	// スロー操作
	Slow();

	float fGravity = PLAYER_GRAVITY * CManager::GetSlow()->Get();
	m_Info.move.y += fGravity;
	pos.y += m_Info.move.y * CManager::GetSlow()->Get();

	if (m_bJump == false)
	{
		m_Info.move.x += (0.0f - m_Info.move.x) * 0.12f;	//x座標
		m_Info.move.z += (0.0f - m_Info.move.z) * 0.12f;	//x座標
	}

	pos.x += m_Info.move.x * CManager::GetSlow()->Get();
	pos.z += m_Info.move.z * CManager::GetSlow()->Get();

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

	//場外判定
	if (pos.x > 3800.0f)
	{
		pos.x = 3800.0f;
		m_Info.move.x = 0.0f;
	}
	else if (pos.x < -4300.0f)
	{
		pos.x = -4300.0f;
		m_Info.move.x = 0.0f;
	}

	if (pos.z > 3800.0f)
	{
		pos.z = 3800.0f;
		m_Info.move.z = 0.0f;
	}
	else if (pos.z < -3800.0f)
	{
		pos.z = -3800.0f;
		m_Info.move.z = 0.0f;
	}

	// 最低ライン判定
	bool m_bOld = m_bJump;
	m_bJump = true;
	bool bOldJump;

	// オブジェクトとの当たり判定
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(WIDTH, HEIGHT, WIDTH);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-WIDTH, 0.0f, -WIDTH);
	m_bJump = CObjectX::Collision(pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax);
	bOldJump = m_bJump;

	// 車との当たり判定
	m_pCar = CManager::GetScene()->GetCarManager()->Collision(pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax, &m_bJump);

	if (bOldJump == false && m_bJump == true)
	{
		m_bJump = bOldJump;
	}

	// 壁との当たり判定
	CMeshWall::Collision(pos, m_Info.posOld);

	// 起伏との当たり判定
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fHeight = CMeshField::GetHeight(pos);

	if (pos.y < -30.0f)
	{
		m_Info.move.y = 0.0f;
		pos.y = -30.0f;
		m_bJump = false;
	}

	if (fHeight > pos.y)
	{// 
		pos.y = fHeight;
		//m_Info.move.y = 0.0f;
		m_bJump = false;
	}

	if (m_bJump == true)
	{
		if (m_bAttack == false)
		{
			m_pBody->GetMotion()->BlendSet(BMOTION_JUMP);
		}
		m_pLeg->GetMotion()->BlendSet(LMOTION_JUMP);
	}


	if (m_bOld == true && m_bJump == false)
	{
		CManager::GetSound()->Play(CSound::LABEL_SE_LAND);
	}

	// 影の座標更新
	if (pShadow != NULL)
	{
		pShadow->SetPosition(D3DXVECTOR3(m_pBody->GetParts(0)->GetMtxWorld()->_41, fHeight + 0.01f, m_pBody->GetParts(0)->GetMtxWorld()->_43));
	}

	// 頂点情報設定
	SetRotation(rot);
	SetPosition(pos);

	// マップアイコンの移動
	if (m_pMapIcon != NULL)
	{
		m_pMapIcon->SetPosition(GetPosition());
		m_pMapIcon->SetRotation(GetRotation());
	}

	pos = GetPosition();

	//デバッグ表示
	CManager::GetDebugProc()->Print("\n移動[W,A,S,D] : ジャンプ[SPACE] : 発射[L, マウス左クリック(長押し可)]\n"
		"スロー[ゲージを貯めてENTER, マウス右クリック] : 武器切り替え[Q]\n");
	CManager::GetDebugProc()->Print("プレイヤーの座標[ %f, %f, %f ]\n", pos.x, pos.y, pos.z);
	CManager::GetDebugProc()->Print("プレイヤーの向き[ %f, %f, %f ]\n", rot.x, rot.y, rot.z);
	CManager::GetDebugProc()->Print("プレイヤーの移動量[ %f, %f, %f ]\n", m_Info.move.x, m_Info.move.y, m_Info.move.z);
	CManager::GetDebugProc()->Print("スロー可能タイマー[ %d ]\n", m_nSlowTime);
}

//===============================================
// 移動
//===============================================
void CPlayer::Move(void)
{
	CCamera *pCamera = CManager::GetCamera();		// カメラのポインタ
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();	// キーボードのポインタ
	CInputPad *pInputPad = CManager::GetInputPad();
	float fSpeed = MOVE;	// 移動量

	if (m_bAttack == true && m_WepType == ATK_SHOWER)
	{
		fSpeed = SHW_MOVE;
	}

	//プレイヤーの更新
	if (pInputKey->GetPress(DIK_A) == true || pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_X, 0.5f, CInputPad::STICK_MINUS) == true)
	{
		if (pInputKey->GetPress(DIK_W) == true || pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_Y, 0.5f, CInputPad::STICK_PLUS))
		{
			m_Info.move.x += cosf(CamRot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_Info.move.z += sinf(CamRot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_fRotDest = (-CamRot.y + D3DX_PI * 0.25f);
		}
		else if (pInputKey->GetPress(DIK_S) == true || pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_Y, 0.5f, CInputPad::STICK_MINUS))
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
		m_bMove = true;
	}
	else if (pInputKey->GetPress(DIK_D) == true || pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_X, 0.5f, CInputPad::STICK_PLUS) == true)
	{
		if (pInputKey->GetPress(DIK_W) == true || pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_Y, 0.5f, CInputPad::STICK_PLUS))
		{
			m_Info.move.x += cosf(CamRot.y + (D3DX_PI * 0.75f)) * fSpeed;
			m_Info.move.z += sinf(CamRot.y + (D3DX_PI * 0.75f)) * fSpeed;

			m_fRotDest = (-CamRot.y + D3DX_PI * 0.75f);
		}
		else if (pInputKey->GetPress(DIK_S) == true || pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_Y, 0.5f, CInputPad::STICK_MINUS))
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
		m_bMove = true;
	}
	else if (pInputKey->GetPress(DIK_W) == true || pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_Y, 0.5f, CInputPad::STICK_PLUS))
	{
		m_Info.move.x += -cosf(CamRot.y) * fSpeed;
		m_Info.move.z += -sinf(CamRot.y) * fSpeed;
		m_fRotDest = (-CamRot.y + D3DX_PI * 0.5f);

		// 移動した状態にする
		m_bMove = true;

	}
	else if (pInputKey->GetPress(DIK_S) == true || pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_Y, 0.5f, CInputPad::STICK_MINUS))
	{
		m_Info.move.x += cosf(CamRot.y) * fSpeed;
		m_Info.move.z += sinf(CamRot.y) * fSpeed;
		m_fRotDest = (-CamRot.y + -D3DX_PI * 0.5f);

		// 移動した状態にする
		m_bMove = true;
	}
}

//===============================================
// スロー関連
//===============================================
void CPlayer::Slow(void)
{
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();	// キーボードのポインタ
	CInputMouse *pInputMouse = CManager::GetInputMouse();
	CInputPad *pInputPad = CManager::GetInputPad();

	// スロータイマー
	if (m_bActiveSlow == false && m_bJump == true && m_nSlowTime == DEF_SLOWTIME)
	{// 入力可能
		if (pInputKey->GetTrigger(DIK_RETURN) == true || pInputMouse->GetTrigger(CInputMouse::BUTTON_RBUTTON) == true || pInputPad->GetTrigger(CInputPad::BUTTON_LEFTBUTTON, 0))
		{// スロー対応ボタントリガー入力
			m_bSlow = m_bSlow ? false : true;

			if (m_bSlow == true)
			{
				CManager::GetSound()->Play(CSound::LABEL_SE_ZONE);
				CManager::GetCamera()->SetOldRot(CManager::GetCamera()->GetRotation());
				m_bAttack = false;

				if (m_WepType == ATK_GUN)
				{
					CFilter::Create(CFilter::TYPE_SLOWGUN);
					CManager::GetCamera()->SetMode(CCamera::MODE_SLOWGUN);
				}
				else
				{
					CManager::GetCamera()->SetMode(CCamera::MODE_SLOWSHW);
				}
			}
			else
			{
				CManager::GetCamera()->SetMode(CCamera::MODE_NORMAL);
			}
		}
	}

	if (m_bSlow == true)
	{// ENTERキーが入力された場合
		if (m_nSlowTime > 0)
		{// 残っている場合
			m_nSlowTime--;
			CManager::GetSlow()->SetSlow(true);

			if (m_WepType == ATK_SHOWER)
			{
				SlowShw();
			}
			else if (m_WepType == ATK_GUN)
			{
				SlowGun();
			}
		}
		else
		{
			CManager::GetCamera()->SetMode(CCamera::MODE_NORMAL);
			m_bSlow = false;
			m_bActiveSlow = true;	// スローにできない状態にする
			CManager::GetSlow()->SetSlow(false);
		}
	}
	else
	{
		if (m_pBalloon != NULL)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(m_pBalloon->GetMtx()->_41, m_pBalloon->GetMtx()->_42, m_pBalloon->GetMtx()->_43);
			m_pBalloon->SetPosition(pos);
			m_pBalloon->SetParent(NULL);
			m_pBalloon = NULL;
		}

		CManager::GetCamera()->SetMode(CCamera::MODE_NORMAL);

		// 可能時間回復
		if (m_nSlowTime == DEF_SLOWTIME)
		{// 満タンまでたまった
			CParticle::Create(D3DXVECTOR3(m_pBody->GetParts(0)->GetMtxWorld()->_41, m_pBody->GetParts(0)->GetMtxWorld()->_42, m_pBody->GetParts(0)->GetMtxWorld()->_43),
				CEffect::TYPE_SLOWOK);
			m_pSlowManual->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pSlowManual->SetSize(m_pSlowManual->GetWidth() + m_fMove, m_pSlowManual->GetHeight() + m_fMove * 0.5f);

			if (m_pSlowManual->GetWidth() < SCREEN_WIDTH * 0.1f || m_pSlowManual->GetWidth() > SCREEN_WIDTH * 0.12f)
			{
				m_fMove *= -1.0f;
			}

			m_bActiveSlow = false;
		}
		else
		{
			m_pSlowManual->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}

		CManager::GetSlow()->SetSlow(false);
	}
}

//===============================================
// 胴体の向き変更
//===============================================
void CPlayer::SetBodyRot(void)
{
	CCamera *pCamera = CManager::GetCamera();		// カメラのポインタ
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	CInputPad *pInputPad = CManager::GetInputPad();
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();

	if (m_pBody == NULL || m_pLeg == NULL)
	{// 二段階の階層構造ではない場合
		return;
	}

	D3DXVECTOR3 rot = m_pBody->GetRotation();	// 胴体の向きを取得

	if (pInputPad->GetRightTriggerPress(0) >= 10 || pInputKey->GetPress(DIK_E))
	{// トリガー入力
		m_bLock = false;
	}
	else
	{
		m_bLock = true;
	}

	if (m_bLock == true)
	{
		float fRotDiff = (-CamRot.y + D3DX_PI * 0.5f) - m_Info.rot.y;	// カメラの向いている方向に合わせる

		if (fRotDiff < -D3DX_PI)
		{// z座標角度限界
			fRotDiff += D3DX_PI * 2;
		}
		else if (fRotDiff > D3DX_PI)
		{// z座標角度限界
			fRotDiff += -D3DX_PI * 2;
		}

		fRotDiff *= 0.6f;

		float fRotDest = fRotDiff - rot.y;

		rot.y += fRotDest * 0.1f * CManager::GetSlow()->Get();
	}
	else
	{
		rot.y = 0.0f;
	}

	// 胴体の向きを設定
	m_pBody->SetRotation(rot);
}

//===============================================
// 攻撃
//===============================================
void CPlayer::Attack(void)
{
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();	// キーボードのポインタ
	CInputMouse *pInputMouse = CManager::GetInputMouse();
	CCamera *pCamera = CManager::GetCamera();		// カメラのポインタ
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	D3DXVECTOR3 BulletMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CBullet *pBullet = NULL;
	CInputPad *pInputPad = CManager::GetInputPad();

	CManager::GetDebugProc()->Print("攻撃タイマー[%f]\n", m_fAttackTimer);
	CManager::GetDebugProc()->Print("キー[%d / %d]\n", m_pBody->GetMotion()->GetNowKey(), m_pBody->GetMotion()->GetNowNumKey());

	if (CManager::GetSlow()->Get() != 1.0f)
	{
		return;
	}

	if (pInputKey->GetPress(DIK_L) == true || pInputMouse->GetPress(CInputMouse::BUTTON_LBUTTON) == true || pInputPad->GetPress(CInputPad::BUTTON_RIGHTBUTTON, 0))
	{//ENTERキーが押されたとき

		if (m_bAttack == false)
		{// 攻撃していない
			switch (m_WepType)
			{
			case ATK_GUN:

				if (CManager::GetSlow()->Get() == 1.0f)
				{
					m_pBody->GetMotion()->Set(BMOTION_TWINATK);
				}

				break;

			case ATK_SHOWER:
				m_pBody->GetMotion()->Set(BMOTION_SHWATK);
				break;
			}
		}
		else
		{// 攻撃している
			if (m_WepTypeOld != m_WepType)
			{// 攻撃中に武器を持ち替えている
				m_fAttackTimer += CManager::GetSlow()->Get();	// 攻撃インターバル

				if (m_fAttackTimer < SHOT_INTERVAL)
				{// インターバル規定値ではない
					return;
				}

				m_fAttackTimer = 0;
				m_nAttackHand = 0;
			}

			switch (m_WepType)
			{
			case ATK_GUN:

				if (CManager::GetSlow()->Get() == 1.0f)
				{
					m_pBody->GetMotion()->BlendSet(BMOTION_TWINATK);
				}

				break;

			case ATK_SHOWER:
				m_pBody->GetMotion()->BlendSet(BMOTION_SHWATK);
				break;
			}

			m_WepTypeOld = m_WepType;
		}

		if (m_pBody->GetMotion()->GetNowFrame() != 0)
		{// そのキーの開始フレーム以外の場合
			return;
		}

		if (m_pBody->GetMotion()->GetNowKey() % (int)(m_pBody->GetMotion()->GetNowNumKey() * 0.5f) != 0 && m_pBody->GetMotion()->GetNowMotion() != BMOTION_SLOWATK)
		{// 攻撃タイミングではない
			return;
		}
		else if (m_pBody->GetMotion()->GetNowKey() != 1 && m_pBody->GetMotion()->GetNowMotion() == BMOTION_SLOWATK)
		{
			m_bAttack = true;
			return;
		}

		D3DXMATRIX mtx;
		switch (m_WepType)
		{
		case ATK_GUN:

			//m_pBody->GetMotion()->Set(BMOTION_TWINATK);

			if (CManager::GetSlow()->Get() == 1.0f)
			{
				if (m_nAttackHand == 0)
				{
					mtx = *m_pWeaponL->GetMtxWorld();
				}
				else
				{
					mtx = *m_pWeaponR->GetMtxWorld();
				}

				if (m_pLockon->GetLock() == false)
				{// ロックオンしていない
					BulletMove = { -cosf(CamRot.y) * GUN_BULMOVE, 0.0f, -sinf(CamRot.y) * GUN_BULMOVE };
				}
				else
				{// ロックオンしている
					D3DXVECTOR3 vec;
					vec.x = m_pLockon->GetPosition().x - mtx._41;
					vec.y = m_pLockon->GetPosition().y - mtx._42;
					vec.z = m_pLockon->GetPosition().z - mtx._43;

					D3DXVec3Normalize(&vec, &vec);	// ベクトルを正規化する

					BulletMove = vec * GUN_BULMOVE;
				}


				// 弾の発射
				pBullet = CBullet::Create(D3DXVECTOR3(mtx._41, mtx._42, mtx._43),
					D3DXVECTOR3(BulletMove.x, BulletMove.y, BulletMove.z), CBullet::TYPE_NONE);
				CManager::GetSound()->Play(CSound::LABEL_SE_WATERGUN);

				if (m_pLockon->GetLock() == true)
				{// ロックオンしている
					pBullet->SetHom(&*m_pLockon->GetTag(), GUN_BULMOVE);
				}
			}
			else
			{
				CLockOn *pLock = CLockOn::GetTop();

				while (pLock != NULL)
				{// 使用されている間繰り返し
					CLockOn *pBulNext = pLock->GetNext();	// 次を保持

					if (pLock->GetDeath() == false && pLock->GetType() == CLockOn::TYPE_MULTI)
					{
						if (pLock->GetTag() != NULL)
						{// 同じ標的の場合

							if (m_nAttackHand == 0)
							{
								mtx = *m_pWeaponL->GetMtxWorld();
							}
							else
							{
								mtx = *m_pWeaponR->GetMtxWorld();
							}

							D3DXVECTOR3 vec;
							vec.x = pLock->GetPosition().x - mtx._41;
							vec.y = pLock->GetPosition().y - mtx._42;
							vec.z = pLock->GetPosition().z - mtx._43;


							D3DXVec3Normalize(&vec, &vec);	// ベクトルを正規化する

							BulletMove = vec * 50.0f;

							// 弾の発射
							pBullet = CBullet::Create(D3DXVECTOR3(mtx._41, mtx._42, mtx._43),
								D3DXVECTOR3(BulletMove.x, BulletMove.y, BulletMove.z), CBullet::TYPE_NONE);

							pBullet->SetHom(&*pLock->GetTag(), 50.0f);
						}
					}

					m_nAttackHand ^= 1;

					pLock = pBulNext;	// 次に移動
				}
			}

			m_nAttackHand ^= 1;

			break;
		case ATK_SHOWER:
			m_pBody->GetMotion()->Set(BMOTION_SHWATK);
			mtx = *m_pWeaponL->GetMtxWorld();

			BulletMove = { -cosf(CamRot.y) * 2.0f, 1.0f, -sinf(CamRot.y) * 2.0f };

			// 弾の発射
			pBullet = CBullet::Create(D3DXVECTOR3(mtx._41, mtx._42, mtx._43),
				D3DXVECTOR3(BulletMove.x, BulletMove.y, BulletMove.z), CBullet::TYPE_GRAVITY);

			D3DXVec3Normalize(&BulletMove, &BulletMove);

			if (pBullet != NULL)
			{
				pBullet->SetSize(80.0f, 30.0f);
				pBullet->SetLife(300.0f);
			}

			for (int nCnt = 0; nCnt < 10; nCnt++)
			{
				// 座標の設定
				D3DXVECTOR3 pos = D3DXVECTOR3(mtx._41 + m_Info.move.x, mtx._42, mtx._43 + m_Info.move.z);
				D3DXVECTOR3 move;

				//移動量の設定
				move.x = -cosf(CamRot.y + -D3DX_PI * 0.09f + D3DX_PI * 0.18f * ((rand() % 10) * 0.1f)) * 5.0f;
				move.y = rand() % 30 * 0.1f;
				move.z = -sinf(CamRot.y + -D3DX_PI * 0.09f + D3DX_PI * 0.18f * ((rand() % 10) * 0.1f)) * 5.0f;

				pBullet = CBullet::Create(pos,
					move, CBullet::TYPE_SHOWER);
				pBullet->SetInerMove(D3DXVECTOR3(m_Info.move.x, 0.0f, m_Info.move.z));
				pBullet->SetLife(300.0f);
			}

			CParticle::Create(D3DXVECTOR3(mtx._41, mtx._42, mtx._43), CamRot, CEffect::TYPE_SHWBULLET);
			CManager::GetSound()->Play(CSound::LABEL_SE_SHW);

			break;
		}

		m_bAttack = true;
	}
	else
	{// 入力していない
		if (m_bAttack == true)
		{
			if (m_pBody->GetMotion()->GetNowFrame() != 0)
			{// そのキーの開始フレーム以外の場合
				return;
			}

			if (m_pBody->GetMotion()->GetNowKey() % (int)(m_pBody->GetMotion()->GetNowNumKey() * 0.5f) != 0)
			{// 攻撃タイミングではない
				return;
			}

			m_bAttack = false;
			m_nAttackHand = 0;
			m_fAttackTimer = 0;
		}
	}
}

//===============================================
// ジャンプ
//===============================================
void CPlayer::Jump(void)
{
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();	// キーボードのポインタ
	CInputPad *pInputPad = CManager::GetInputPad();

	// ジャンプ
	if (pInputKey->GetPress(DIK_SPACE) == true || pInputPad->GetTrigger(CInputPad::BUTTON_A, 0))
	{// SPACEキー
		if (m_bJump == false)
		{// ジャンプしていない場合
			m_bJump = true;
			m_Info.move.y = PLAYER_JUMP;
			if (m_bAttack == false)
			{
				m_pBody->GetMotion()->BlendSet(BMOTION_JUMP);
			}
			m_pLeg->GetMotion()->BlendSet(LMOTION_JUMP);

			m_Info.move.x += (0.0f - m_Info.move.x) * 0.25f;	//x座標
			m_Info.move.z += (0.0f - m_Info.move.z) * 0.25f;	//x座標

			if (m_pCar != NULL)
			{
				m_Info.move.x += m_pCar->GetMove().x * CManager::GetSlow()->Get();	//x座標
				m_Info.move.z += m_pCar->GetMove().z * CManager::GetSlow()->Get();	//x座標
			}
		}
	}
}

//===============================================
// スロー中シャワー挙動
//===============================================
void CPlayer::SlowShw(void)
{
	CCamera *pCamera = CManager::GetCamera();		// カメラのポインタ
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度

	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();	// キーボードのポインタ
	CInputMouse *pInputMouse = CManager::GetInputMouse();
	D3DXVECTOR3 BulletMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CInputPad *pInputPad = CManager::GetInputPad();
	CBullet *pBullet = NULL;

	if (pInputKey->GetPress(DIK_L) == true || pInputMouse->GetPress(CInputMouse::BUTTON_LBUTTON) == true || pInputPad->GetPress(CInputPad::BUTTON_RIGHTBUTTON, 0))
	{//ENTERキーが押されたとき

		m_pBody->GetMotion()->Set(BMOTION_SLOWSHW);
		m_bAttack = true;
		m_fChargeCnt += 1.0f;

		if (m_fChargeCnt > NOCHARGE_CNT)
		{// チャージ中
			if (m_pBalloon == NULL)
			{// チャージ開始していない場合
				m_pBalloon = CMeshBalloon::Create(m_SetBalloon.pos, m_SetBalloon.rot, m_SetBalloon.fLength, m_SetBalloon.fHeight,
				m_SetBalloon.nPriority, m_SetBalloon.nWidth, m_SetBalloon.nHeight);

				m_pBalloon->SetParent(m_pWeaponL->GetMtxWorld());

				// テクスチャのランダム変更
				int nRand = rand() % CMeshBalloon::TYPE_MAX;

				switch (nRand)
				{
				case CMeshBalloon::TYPE_PINK:

					m_pBalloon->BindTexture(CTexture::TYPE_BALLOONPINK);
					break;

				case CMeshBalloon::TYPE_BLUE:

					m_pBalloon->BindTexture(CTexture::TYPE_BALLOONBLUE);
					break;
				}
				
			}

			if (m_pBalloon != NULL)
			{
				D3DXVECTOR3 pos = m_pBalloon->GetPosition();
				m_pBalloon->AddLength(0.5f);
				pos.x += -0.5f;
				m_pBalloon->SetPosition(pos);
				CManager::GetSound()->Play(CSound::LABEL_SE_BALLOONUP);
			}
		}
	}
	else
	{// 攻撃ボタンを押してない、または離した
		if (m_fChargeCnt > 0.0f && m_fChargeCnt <= NOCHARGE_CNT)
		{// チャージせず撃つ場合
			D3DXMATRIX mtx;
			mtx = *m_pWeaponL->GetMtxWorld();

			BulletMove = { -cosf(CamRot.y) * 2.0f, 1.0f, -sinf(CamRot.y) * 2.0f };

			// 弾の発射
			pBullet = CBullet::Create(D3DXVECTOR3(mtx._41, mtx._42, mtx._43),
				D3DXVECTOR3(BulletMove.x, BulletMove.y, BulletMove.z), CBullet::TYPE_GRAVITY);

			pBullet->SetSize(300.0f, 50.0f);
			pBullet->SetLife(300.0f);

			BulletMove = { -cosf(CamRot.y) * 4.0f, 1.0f, -sinf(CamRot.y) * 4.0f };

			// 弾の発射
			pBullet = CBullet::Create(D3DXVECTOR3(mtx._41, mtx._42, mtx._43),
				D3DXVECTOR3(BulletMove.x, BulletMove.y, BulletMove.z), CBullet::TYPE_GRAVITY);

			pBullet->SetSize(300.0f, 50.0f);
			pBullet->SetLife(300.0f);

			for (int nCnt = 0; nCnt < 10; nCnt++)
			{
				// 座標の設定
				D3DXVECTOR3 pos = D3DXVECTOR3(mtx._41 + m_Info.move.x, mtx._42, mtx._43 + m_Info.move.z);
				D3DXVECTOR3 move;

				//移動量の設定
				move.x = -cosf(CamRot.y + -D3DX_PI * 0.18f + D3DX_PI * 0.36f * ((rand() % 10) * 0.1f)) * 7.0f;
				move.y = rand() % 30 * 0.1f;
				move.z = -sinf(CamRot.y + -D3DX_PI * 0.18f + D3DX_PI * 0.36f * ((rand() % 10) * 0.1f)) * 7.0f;

				pBullet = CBullet::Create(pos,
					move, CBullet::TYPE_SHOWER);
				//pBullet->SetInerMove(D3DXVECTOR3(m_Info.move.x, 0.0f, m_Info.move.z));
				pBullet->SetLife(300.0f);
			}

			CManager::GetSound()->Play(CSound::LABEL_SE_SHW);
		}
		else
		{
			// 風船発射
			if (m_pBalloon != NULL)
			{
				ShotBalloon();
				CManager::GetSound()->Play(CSound::LABEL_SE_BALLOONSHOT);
			}
		}

		m_fChargeCnt = 0.0f;
		m_bAttack = false;
	}
}

//===============================================
// スロー中二丁挙動
//===============================================
void CPlayer::SlowGun(void)
{
	if (m_nSlowTime <= 30 * 1)
	{
		m_pBody->GetMotion()->Set(BMOTION_SLOWATK);

		m_bAttack = true;

		if (m_pBody->GetMotion()->GetNowFrame() != 0)
		{// そのキーの開始フレーム以外の場合
			return;
		}

		if (m_pBody->GetMotion()->GetNowKey() != 1 && (m_pBody->GetMotion()->GetNowMotion() == BMOTION_SLOWATK || m_pBody->GetMotion()->GetNowMotion() == BMOTION_SLOWSHW))
		{
			m_bAttack = true;
			return;
		}

		CLockOn *pLock = CLockOn::GetTop();

		while (pLock != NULL)
		{// 使用されている間繰り返し
			CLockOn *pBulNext = pLock->GetNext();	// 次を保持
			D3DXVECTOR3 BulletMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXMATRIX mtx;
			CBullet *pBullet;

			if (pLock->GetDeath() == false && pLock->GetType() == CLockOn::TYPE_MULTI)
			{
				if (pLock->GetTag() != NULL)
				{// 同じ標的の場合

					if (m_WepType == ATK_GUN)
					{
						if (m_nAttackHand == 0)
						{
							mtx = *m_pWeaponL->GetMtxWorld();
						}
						else
						{
							mtx = *m_pWeaponR->GetMtxWorld();
						}

						D3DXVECTOR3 vec;
						vec.x = pLock->GetPosition().x - mtx._41;
						vec.y = pLock->GetPosition().y - mtx._42;
						vec.z = pLock->GetPosition().z - mtx._43;

						D3DXVec3Normalize(&vec, &vec);	// ベクトルを正規化する

						BulletMove = vec * GUN_BULMOVE;

						// 弾の発射
						pBullet = CBullet::Create(D3DXVECTOR3(mtx._41, mtx._42, mtx._43),
							D3DXVECTOR3(BulletMove.x, BulletMove.y, BulletMove.z), CBullet::TYPE_NONE);

						pBullet->SetHom(&*pLock->GetTag(), GUN_BULMOVE);
					}
				}
			}
			m_nAttackHand ^= 1;

			pLock = pBulNext;	// 次に移動
		}

		CManager::GetSound()->Play(CSound::LABEL_SE_ZONEGUN);
		m_bSlow = false;
		m_nSlowTime = 0;
	}
}

//===============================================
// スロー時間増加
//===============================================
void CPlayer::AddSlowTime(int nAddTime)
{
	int nSlowTimerOld = m_nSlowTime;	// 変更前のスロー可能時間
	m_nSlowTime += nAddTime;

	if (m_nSlowTime > DEF_SLOWTIME)
	{// 限界値を超えた
		m_nSlowTime = DEF_SLOWTIME;
	}

	if (m_pSlowGage == NULL || m_nSlowTime == nSlowTimerOld)
	{// 使用していない、またはスロー時間に変更がない
		return;
	}

	// スロー最大時間からの割合を求める
	float fRate = (float)m_nSlowTime / (float)DEF_SLOWTIME;

	m_pSlowGage->SetSize(DEF_SLOWGAGELENGSH * fRate, SLOWGAGE_HEIGHT);
	m_pSlowGage->SetTex(-5.0f * fRate, 0.0f, 5.0f * fRate);
	SetGageColor(fRate);
}

//===============================================
// ゲージカラー変更
//===============================================
void CPlayer::SetGageColor(float fRate)
{
	if (m_pSlowGage == NULL)
	{
		return;
	}

	if (fRate >= 1.0f)
	{
		m_pSlowGage->SetCol(D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f));
	}
	else if (fRate <= 0.1f)
	{
		m_pSlowGage->SetCol(D3DXCOLOR(0.1f, 1.0f, 0.1f, 1.0f));
		m_pSlowManual->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}
	else
	{
		m_pSlowManual->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pSlowGage->SetCol(D3DXCOLOR(1.0f * fRate, 1.0f, 1.0f * fRate, 1.0f));
	}
}

//===============================================
// パーティクル
//===============================================
void CPlayer::Particle(void)
{
	if (m_pLeg != NULL)
	{
		CMotion *pMotion = m_pLeg->GetMotion();

		if (pMotion->GetNowMotion() == BMOTION_WALK)
		{
			if (pMotion->GetNowFrame() == 0 && (pMotion->GetNowKey() == 0 || pMotion->GetNowKey() == 2))
			{// 地面を蹴った
				CParticle::Create(m_Info.pos, D3DXVECTOR3(m_Info.move.x, 0.0f, m_Info.move.z), CEffect::TYPE_DUST);
				CManager::GetSound()->Play(CSound::LABEL_SE_MOVE);
			}
		}
		else if (pMotion->GetNowMotion() == BMOTION_JUMP)
		{
			if (pMotion->GetNowFrame() == 0 && pMotion->GetOldMotion() != pMotion->GetNowMotion() && m_Info.move.y >= 0.0f)
			{// 地面を蹴った
				CParticle::Create(D3DXVECTOR3(m_Info.pos.x, m_Info.posOld.y, m_Info.pos.z), m_Info.move, CEffect::TYPE_DUST);
				CParticle::Create(m_Info.pos, m_Info.move, CEffect::TYPE_SWAP);
				CManager::GetSound()->Play(CSound::LABEL_SE_JUMP);
			}
		}
	}
}

//===============================================
// 風船発射処理
//===============================================
void CPlayer::ShotBalloon(void)
{
	CCamera *pCamera = CManager::GetCamera();		// カメラのポインタ
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fMultiMove = 1.0f - (float)(m_pBalloon->GetLength() / BALLOON_WEIGHT);
	Move = { -cosf(CamRot.y) * BALLOON_MOVE * fMultiMove, 0.0f, -sinf(CamRot.y) * BALLOON_MOVE * fMultiMove };

	D3DXVECTOR3 pos = D3DXVECTOR3(m_pBalloon->GetMtx()->_41, m_pBalloon->GetMtx()->_42, m_pBalloon->GetMtx()->_43);
	m_pBalloon->SetPosition(pos);
	m_pBalloon->SetMove(Move);
	m_pBalloon->SetParent(NULL);
	m_pBalloon = NULL;

}