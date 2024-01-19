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
#include "slow.h"
#include "texture.h"
#include "meshfield.h"
#include "Xfile.h"
#include "meshorbit.h"
#include "game.h"
#include "filter.h"
#include "object2D.h"
#include "meshwall.h"
#include "objectX.h"
#include "sound.h"
#include <assert.h>
#include "billboard.h"
#include "character.h"
#include "motion.h"
#include "sound.h"
#include "waist.h"
#include "model.h"
#include "score.h"
#include "particle.h"
#include "effect.h"
#include <math.h>
#include "meshwall.h"
#include "enemy_manager.h"
#include "player_manager.h"

//===============================================
// マクロ定義
//===============================================
#define MOVE	(4.5f)		// 移動量
#define GRAVITY	(-0.9f)		//プレイヤー重力
#define ROT_MULTI	(0.1f)	// 向き補正倍率
#define WIDTH	(20.0f)		// 幅
#define HEIGHT	(80.0f)	// 高さ
#define INER	(0.3f)		// 慣性
#define JUMP	(18.0f)

namespace {
	const D3DXVECTOR3 PLAYERSTARTPOS = { 0.0f, 0.0f, -2300.0f };  // プレイヤーのスタート位置
	const int HEADPARTS_IDX = (1);	// 頭のパーツインデックス
	const float DAMAGE_INTERVAL = (10.0f);	// ダメージインターバル
	const float DAMAGE_APPEAR = (110.0f);	// 無敵時間インターバル
	const float DEATH_INTERVAL = (120.0f);	// 死亡インターバル
	const float SPAWN_INTERVAL = (60.0f);	// 生成インターバル
	const float SLIDING_INER = (0.015f);	// スライディング慣性
	const float SLIDING_MINMOVE = (1.0f);	// スライディング可能最低移動量
	const float SLIDING_STARTMOVE = (6.0f);	// スライディング開始可能移動量
	const float SLIDING_SPEED = (0.5f);
	const float WALLKICK_MOVE = (45.0f);	// 壁キック移動量
	const float WALLKICK_INER = (0.1f);		// 壁キック中慣性
	const float WALLKICK_SPEED = (1.0f);	// 壁キック中移動速度
	const float WALLSLIDE_GRAVITY = (-1.5f);	// 壁ずり中落下速度
	const float SLIDEJUMP_INER = (0.02f);
	const float SLIDEJUMP_GRAVITY = (-0.25f);
	const float SLIDEJUMP = (7.0f);
	const float SLIDEJUMP_SPEED = (1.75f);
	const float WALLSLIDE_MOVE = (0.05f);
	const float WALLDUSH_MOVE = (5.0f);
	const float CAMROT_INER = (0.2f);
	const float SLIDINNG_ROTZ = (D3DX_PI * 0.51f);
	const float SLIDING_LENGTH = (200.0f);
	const D3DXVECTOR3 COLLIMAX = { 20.0f, 70.0f, 20.0f };
	const D3DXVECTOR3 COLLIMIN = { -20.0f, 0.0f, -20.0f };
}

// 前方宣言

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
CPlayer::CPlayer()
{
	// 値をクリアする
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRotMove = 0.0f;
	m_fRotDiff = 0.0f;
	m_fRotDest = 0.0f;
	m_pBody = nullptr;
	m_pLeg = nullptr;
	m_pWaist = nullptr;
	m_type = TYPE_NONE;
	m_pMyCamera = nullptr;
	m_bJump = false;
	m_nId = -1;
	m_bMove = false;
	m_nLife = 5;
	m_Info.fSlideMove = 0.0f;

	CPlayerManager::GetInstance()->ListIn(this);
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
	if (m_pWaist == nullptr)
	{
		m_pWaist = new CWaist;
		m_pWaist->SetParent(&m_Info.mtxWorld);
	}

	// 胴体の設定
	m_pBody = CCharacter::Create("data\\TXT\\motion_ninjabody.txt");
	m_pBody->SetParent(m_pWaist->GetMtxWorld());

	if (m_pBody->GetMotion() != nullptr)
	{
		// 初期モーションの設定
		m_pBody->GetMotion()->InitSet(m_action);
	}

	// 下半身の設定
	m_pLeg = CCharacter::Create("data\\TXT\\motion_ninjaleg.txt");
	m_pLeg->SetParent(m_pWaist->GetMtxWorld());

	if (m_pLeg->GetMotion() != nullptr)
	{
		// 初期モーションの設定
		m_pLeg->GetMotion()->InitSet(m_action);
	}

	// 腰の高さを合わせる
	if (m_pLeg != nullptr)
	{// 脚が使用されている場合
		CModel* pModel = m_pLeg->GetParts(0);	// 腰パーツを取得

		if (pModel != nullptr)
		{// パーツが存在する場合
			D3DXVECTOR3 pos = pModel->GetPosition();	// モデルの相対位置を取得

			// 高さを設定
			m_pWaist->SetHeight(pos);

			// 腰のモデルの位置を変更
			pModel->SetPosition(pos);
		}
	}

	m_Info.state = STATE_APPEAR;
	m_action = ACTION_NEUTRAL;
	m_type = TYPE_NONE;

	return S_OK;
}

//===============================================
// 初期化処理(オーバーロード)
//===============================================
HRESULT CPlayer::Init(const char *pBodyName, const char *pLegName)
{
	SetMatrix();

	// 腰の生成
	if (m_pWaist == nullptr)
	{
		m_pWaist = new CWaist;
		m_pWaist->SetParent(&m_Info.mtxWorld);
		m_pWaist->SetMatrix();
	}

	// 胴体の設定
	if (pBodyName != nullptr)
	{// ファイル名が存在している
		m_pBody = CCharacter::Create(pBodyName);
		m_pBody->SetParent(m_pWaist->GetMtxWorld());
		m_pBody->SetShadow(true);
		m_pBody->SetDraw();

		if (m_pBody->GetMotion() != nullptr)
		{
			// 初期モーションの設定
			m_pBody->GetMotion()->InitSet(m_action);
		}
	}

	// 下半身の設定
	if (pLegName != nullptr)
	{// ファイル名が存在している
		m_pLeg = CCharacter::Create(pLegName);
		m_pLeg->SetParent(m_pWaist->GetMtxWorld());
		m_pLeg->SetShadow(true);
		m_pLeg->SetDraw();

		if (m_pLeg->GetMotion() != nullptr)
		{
			// 初期モーションの設定
			m_pLeg->GetMotion()->InitSet(m_action);
		}
	}

	// 腰の高さを合わせる
	if (m_pLeg != nullptr)
	{// 脚が使用されている場合
		CModel *pModel = m_pLeg->GetParts(0);	// 腰パーツを取得

		if (pModel != nullptr)
		{// パーツが存在する場合
			D3DXVECTOR3 pos = pModel->GetPosition();	// モデルの相対位置を取得

			// 高さを設定
			m_pWaist->SetHeight(pos);

			// 腰のモデルの位置を変更
			pModel->SetPosition(pos);
		}
	}

	/*if (m_pScore == nullptr)
	{
		m_pScore = CScore::Create(D3DXVECTOR3(50.0f + (m_nNumCount - 1) * 500.0f, 50.0f, 0.0f), 30.0f, 30.0f);
	}*/

	m_type = TYPE_NONE;
	m_action = ACTION_NEUTRAL;

	//m_pScore->AddScore(500 * m_nItemCnt);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CPlayer::Uninit(void)
{
	// 胴体の終了
	if (m_pBody != nullptr) {
		m_pBody->Uninit();
		delete m_pBody;
		m_pBody = nullptr;
	}

	// 下半身の終了
	if (m_pLeg != nullptr) {
		m_pLeg->Uninit();
		delete m_pLeg;
		m_pLeg = nullptr;
	}

	// 腰の廃棄
	if (m_pWaist != nullptr){
		delete m_pWaist;
		m_pWaist = nullptr;
	}

	CPlayerManager::GetInstance()->ListOut(this);

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

	StateSet();	

	if (m_type == TYPE_ACTIVE)
	{
		if (m_Info.state != STATE_SPAWN)
		{
			// プレイヤー操作
			Controller();
		}

		if (m_pMyCamera != nullptr) {
			// 追従処理
			m_pMyCamera->Update();
		}

		// オンライン送信
		CManager::GetInstance()->GetScene()->SendPosition(m_Info.pos);
		CManager::GetInstance()->GetScene()->SendRotation(m_Info.rot);
	}
	else {

	}

	// カメラ追従
	if (m_pMyCamera != nullptr) {
		// 追従処理
		m_pMyCamera->CollisionObj();

		if (m_pMyCamera->GetMode() == CCamera::MODE_NORMAL)
		{
			// 角度調整
			float fRot = m_fCamRotZ;
			D3DXVECTOR3 CamRot = m_pMyCamera->GetRotation();
			if (m_action == ACTION_SLIDING || m_action == ACTION_CEILINGDUSH) {	// スライディングの時
				fRot = SLIDINNG_ROTZ;
			}
			CamRot.z = fRot;
			m_pMyCamera->InerRot(CamRot, CAMROT_INER);

			// 追従
			float fLength = m_fCamLength;
			if (m_action == ACTION_SLIDING || m_action == ACTION_CEILINGDUSH) {	// スライディングの時
				fLength = SLIDING_LENGTH;
			}
			m_pMyCamera->Pursue(GetPosition(), GetRotation(), fLength);
		}
	}
	
	SetMatrix();

	BodySet();

	CManager::GetInstance()->GetDebugProc()->Print("体力 [ %d ]\n", m_nLife);
}

//===============================================
// 生成
//===============================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, const char *pBodyName, const char *pLegName)
{
	CPlayer *pPlayer = nullptr;

	// オブジェクト2Dの生成
	pPlayer = new CPlayer();

	if (nullptr != pPlayer)
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
	}
	else
	{// 生成に失敗した場合
		return nullptr;
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
	float fIner = INER;
	m_fRotMove = rot.y;	//現在の向きを取得

	// 操作処理
		if (m_Info.state != STATE_DEATH) {	// 死亡していない
			
			Move();			// 移動
			Rotation();		// 回転
			WallSlide();	// 壁ずり確認
			Jump();			// ジャンプ
			WallDush();		// 壁走り
			CeilingDush();	// 天井走り
			Slide();		// スライディング
			Attack();		// 攻撃
			Hit();			// 攻撃チェック
		}

	MotionSet();	// モーション設定
	CManager::GetInstance()->GetDebugProc()->Print("アクション[ %d ]\n", m_action);

	// 重力設定
	Gravity();
	pos = GetPosition();	// 座標を取得

	// 慣性を変更
	switch (m_action) {
	case ACTION_SLIDING:
		fIner = SLIDING_INER;
		break;

	case ACTION_WALLKICK:
		fIner = WALLKICK_INER;
		break;

	case ACTION_SLIDEJUMP:
		fIner = SLIDEJUMP_INER;
		break;

	default:

		break;
	}

	m_Info.move.x += (0.0f - m_Info.move.x) * fIner;	//x座標
	m_Info.move.z += (0.0f - m_Info.move.z) * fIner;	//x座標

	pos.x += m_Info.move.x * CManager::GetInstance()->GetSlow()->Get();
	pos.z += m_Info.move.z * CManager::GetInstance()->GetSlow()->Get();

	// 調整
	Adjust();

	m_Info.pos = pos;
	bool bOld = m_bJump;
	m_bJump = true;	// ジャンプ状態リセット

	// 起伏との当たり判定
	// メッシュフィールドとの判定
	{
		float fHeight = CMeshField::GetHeight(m_Info.pos);
		if (m_Info.pos.y < fHeight && m_Info.posOld.y >= fHeight)
		{
			m_Info.pos.y = fHeight;
			m_Info.move.y = 0.0f;
			m_bJump = false;

			if (m_action == ACTION_WALLKICK) {	// 壁蹴りの場合
				m_action = ACTION_NEUTRAL;
				if (BodyCheck(m_pBody) && BodyCheck(m_pLeg)) {	// 上下どちらも使用中
					m_pBody->GetMotion()->Set(m_action);
					m_pLeg->GetMotion()->Set(m_action);
				}
			}
			else if (m_action == ACTION_SLIDEJUMP) {
				m_action = ACTION_NEUTRAL;
			}
		}
	}

	// オブジェクトとの当たり判定
	D3DXVECTOR3 vtxMax = COLLIMAX;
	D3DXVECTOR3 vtxMin = COLLIMIN;
	D3DXVECTOR3 vtxMaxOld = vtxMax;
	D3DXVECTOR3 vtxMinOld = vtxMin;

	if (m_action == ACTION_SLIDING) {	// スライディング
		vtxMax.y *= 0.3f;
	}
	else if (m_action == ACTION_CEILINGDUSH) { // 天井走り
		vtxMin.y = vtxMax.y * 0.5f;
		vtxMinOld.y = vtxMin.y;
	}

	CObjectX::COLLISION_AXIS ColiAxis = CObjectX::TYPE_MAX;	// 当たっている方向をリセット

	m_ColiNor = CObjectX::Collision(m_Info.pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax, vtxMinOld, vtxMaxOld, ColiAxis);

	if (ColiAxis == CObjectX::TYPE_Y && m_ColiNor.y > 0.0f) {	// 上から乗っている
		m_bJump = false;

		if (m_action == ACTION_WALLKICK) {	// 壁蹴りの場合
			m_action = ACTION_NEUTRAL;
			if (BodyCheck(m_pBody) && BodyCheck(m_pLeg)) {	// 上下どちらも使用中
				m_pBody->GetMotion()->Set(m_action);
				m_pLeg->GetMotion()->Set(m_action);
			}
		}
		else if (m_action == ACTION_SLIDEJUMP) {
			m_action = ACTION_NEUTRAL;
		}
	}

	D3DXVECTOR3 nor = CMeshWall::Collision(m_Info.pos, m_Info.posOld, m_Info.move, vtxMax, vtxMin, ColiAxis);

	if (nor.x != 0.0f || nor.z != 0.0f) {
		m_ColiNor = nor;
	}

	if (bOld && !m_bJump) {
		CParticle::Create(m_Info.pos, CEffect::TYPE_LAND);
		
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_LAND);
	}

	// ギミックとの判定
	bool bLand = false;

	if (bLand == true)
	{
		m_bJump = false;
	}

	if (m_Info.pos.y <= -100.0f) {
		m_Info.pos = PLAYERSTARTPOS;
	}
}

//===============================================
// 移動
//===============================================
void CPlayer::Move(void)
{
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();	// キーボードのポインタ
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	// 入力装置確認
	if (nullptr == pInputKey){
		return;
	}

	if (m_action == ACTION_SLIDEJUMP) {
		return;
	}

	//プレイヤーの更新
	MoveController();
}

//===============================================
// 回転
//===============================================
void CPlayer::Rotation(void)
{
	CCamera *pCamera = m_pMyCamera;		// カメラのポインタ

	if (m_pMyCamera == nullptr) {
		pCamera = CManager::GetInstance()->GetCamera();
	}

	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	// 入力装置確認
	if (nullptr == pInputPad){
		return;
	}

	if (m_nId < 0 || m_nId >= PLAYER_MAX)
	{// コントローラー数オーバー
		return;
	}

	if (!pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_X, 0.1f, CInputPad::STICK_PLUS) && 
		!pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_X, 0.1f, CInputPad::STICK_MINUS) &&
		!pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.1f, CInputPad::STICK_PLUS) &&
		!pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.1f, CInputPad::STICK_MINUS))
	{// コントローラー入力無し
		KeyBoardRotation();
		return;
	}
}

//===============================================
// 回転
//===============================================
void CPlayer::KeyBoardRotation(void)
{
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();	// キーボードのポインタ

	if (nullptr == pInputKey){
		return;
	}
}

//===============================================
// 調整
//===============================================
void CPlayer::MoveController(void)
{
	m_bMove = false;

	if (m_nId < 0 || m_nId >= PLAYER_MAX)
	{// コントローラー数おーばー
		return;
	}

	CCamera *pCamera = m_pMyCamera;		// カメラのポインタ

	if (m_pMyCamera == nullptr) {
		pCamera = CManager::GetInstance()->GetCamera();
	}

	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();	// キーボードのポインタ
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();
	float fSpeed = MOVE;	// 移動量
	D3DXVECTOR3 move = {0.0f, 0.0f, 0.0f};
	float fRotDestOld = m_fRotDest;

	// 移動量を変更
	switch (m_action) {
	case ACTION_SLIDING:
		fSpeed = SLIDING_SPEED;
		break;

	case ACTION_WALLKICK:
		fSpeed = WALLKICK_SPEED;
		break;

	case ACTION_WALLSTAND:
		fSpeed = WALLSLIDE_MOVE;
		break;

	case ACTION_WALLDUSH:
		fSpeed = WALLDUSH_MOVE;
		break;
	}

	// 入力確認
	if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_X, 0.8f, CInputPad::STICK_MINUS) || pInputKey->GetPress(DIK_A))
	{
		if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_PLUS) || pInputKey->GetPress(DIK_W))
		{
			move.x += cosf(CamRot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			move.z += sinf(CamRot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_fRotDest = (-CamRot.y + D3DX_PI * 0.25f);
		}
		else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_MINUS) || pInputKey->GetPress(DIK_S))
		{
			move.x += cosf(CamRot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			move.z += sinf(CamRot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			m_fRotDest = (-CamRot.y + -D3DX_PI * 0.25f);
		}
		else
		{
			move.x += cosf(CamRot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			move.z += sinf(CamRot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			m_fRotDest = -CamRot.y;
		}

		// 移動した状態にする
		m_bMove = true;
	}
	else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_X, 0.8f, CInputPad::STICK_PLUS) || pInputKey->GetPress(DIK_D))
	{
		if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_PLUS) || pInputKey->GetPress(DIK_W))
		{
			move.x += cosf(CamRot.y + (D3DX_PI * 0.75f)) * fSpeed;
			move.z += sinf(CamRot.y + (D3DX_PI * 0.75f)) * fSpeed;

			m_fRotDest = (-CamRot.y + D3DX_PI * 0.75f);
		}
		else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_MINUS) || pInputKey->GetPress(DIK_S))
		{
			move.x += cosf(CamRot.y + (D3DX_PI * 0.25f)) * fSpeed;
			move.z += sinf(CamRot.y + (D3DX_PI * 0.25f)) * fSpeed;

			m_fRotDest = (-CamRot.y + -D3DX_PI * 0.75f);
		}
		else
		{
			move.x += cosf(CamRot.y + (D3DX_PI * 0.5f)) * fSpeed;
			move.z += sinf(CamRot.y + (D3DX_PI * 0.5f)) * fSpeed;
			m_fRotDest = (-CamRot.y + D3DX_PI * 1.0f);
		}

		// 移動した状態にする
		m_bMove = true;
	}
	else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_PLUS) || pInputKey->GetPress(DIK_W))
	{
		move.x += -cosf(CamRot.y) * fSpeed;
		move.z += -sinf(CamRot.y) * fSpeed;
		m_fRotDest = (-CamRot.y + D3DX_PI * 0.5f);

		// 移動した状態にする
		m_bMove = true;

	}
	else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_MINUS) || pInputKey->GetPress(DIK_S))
	{
		move.x += cosf(CamRot.y) * fSpeed;
		move.z += sinf(CamRot.y) * fSpeed;
		m_fRotDest = (-CamRot.y + -D3DX_PI * 0.5f);

		// 移動した状態にする
		m_bMove = true;
	}

	// 移動量を加算するか確認
	if (m_action == ACTION_SLIDING || m_action == ACTION_WALLKICK) {
		
		float fRot = atan2f(-move.x, -move.z);
		float fRotDest = fRotDestOld - fRot;

		// 補正
		while (1) {
			if (fRotDest > D3DX_PI || fRotDest < -D3DX_PI) {//-3.14～3.14の範囲外の場合
				if (fRotDest > D3DX_PI) {
					fRotDest += (-D3DX_PI * 2);
				}
				else if (fRotDest < -D3DX_PI) {
					fRotDest += (D3DX_PI * 2);
				}
			}
			else {
				break;
			}
		}

		switch (m_action) {
		case ACTION_SLIDING:
		{
			float fData = static_cast<float>(fabs(fRotDest));
			if (fData <= D3DX_PI * 0.25f || fData >= D3DX_PI * 0.75f) {
				m_fRotDest = fRotDestOld;
				return;
			}
		}
			break;

		case ACTION_WALLKICK:
			if (!BodyCheck(m_pBody)) {
				m_fRotDest = fRotDestOld;
				return;
			}

			if (m_pBody->GetMotion()->GetNowKey() <= 2) {
				if (static_cast<float>(fabs(fRotDest)) >= D3DX_PI * 0.25f) {
					m_fRotDest = fRotDestOld;
					return;
				}
			}
			break;
		}
	}
	else if (m_action == ACTION_WALLSTAND) {
		m_fRotDest = atan2f(m_ColiNor.x, m_ColiNor.z);
	}

	m_Info.move += move;
}

//===============================================
// ジャンプ
//===============================================
void CPlayer::Jump(void)
{
	if (m_nId < 0 || m_nId >= PLAYER_MAX)
	{// コントローラー数オーバー
		return;
	}

	CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();	// キーボードのポインタ
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	// 入力
	if (pInputPad->GetTrigger(CInputPad::BUTTON_B, m_nId) || pInputKey->GetTrigger(DIK_SPACE))
	{
		if (m_bJump == false)
		{// ジャンプしていない場合
			m_bJump = true;
		
			
			CParticle::Create(m_Info.pos, CEffect::TYPE_JUMP);
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_JUMP);

			if (m_action == ACTION_SLIDING) {
				m_Info.move.y = SLIDEJUMP;
				m_Info.move.x *= SLIDEJUMP_SPEED;
				m_Info.move.z *= SLIDEJUMP_SPEED;
				m_action = ACTION_SLIDEJUMP;
			}
			else {
				m_Info.move.y = JUMP;
			}
		}
		else {	// ジャンプしている
			if (m_action == ACTION_WALLSTAND) {	// 壁ずり中
				m_Info.move += m_ColiNor * WALLKICK_MOVE;
				m_Info.move.y = JUMP;
				m_fRotDest = atan2f(-m_Info.move.x, -m_Info.move.z);
				m_action = ACTION_WALLKICK;
			}
		}

		return;
	}
}

//===============================================
// スライディング
//===============================================
void CPlayer::Slide(void)
{
	bool bSlide = false;

	if (m_nId < 0 || m_nId >= PLAYER_MAX)
	{// コントローラー数オーバー
		return;
	}

	CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();	// キーボードのポインタ
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	// 現在の移動量を取得
	float fMove = static_cast<float>((fabs(m_Info.move.x) + fabs(m_Info.move.z)));

	// 入力開始時の移動量を取得
	if (pInputPad->GetTrigger(CInputPad::BUTTON_A, m_nId) || pInputKey->GetTrigger(DIK_RETURN)) {
		m_Info.fSlideMove = fMove;
	}

	// 入力
	if (fMove > SLIDING_MINMOVE && m_Info.fSlideMove >= SLIDING_STARTMOVE) {	// スライディングできる移動量かつ開始時にも移動量がある
		if (pInputPad->GetPress(CInputPad::BUTTON_A, m_nId) || pInputKey->GetPress(DIK_RETURN))
		{
			if (m_bJump == false)
			{// ジャンプしていない場合
				m_action = ACTION_SLIDING;
				bSlide = true;
			}
		}
	}
	else {
		if (m_action == ACTION_SLIDEJUMP) {	// 現在スライディングジャンプ中
			m_action = ACTION_NEUTRAL;
			m_Info.fSlideMove = 0.0f;
		}
	}

	// 終了確認
	if(!bSlide){	// スライディングしていない
		if (m_action == ACTION_SLIDING) {	// していた
			m_action = ACTION_NEUTRAL;
			m_Info.fSlideMove = 0.0f;
		}
	}
}

//===============================================
// 壁ずり
//===============================================
void CPlayer::WallSlide(void)
{
	if (!m_bJump) {	// ジャンプしていない

		if (m_action == ACTION_WALLSTAND) {	// 壁ずり状態の場合
			m_action = ACTION_NEUTRAL;
		}
		return;
	}	

	// 壁ずり判定
	if (m_ColiNor.x != 0.0f || m_ColiNor.z != 0.0f) {	// オブジェクトに触れている
		if (m_action < ACTION_WALLDUSH) {
			m_action = ACTION_WALLSTAND;
		}
	}
	else { // 触れていない
		if (m_action == ACTION_WALLSTAND) { // 壁ずり状態の場合
			m_action = ACTION_NEUTRAL;

			if (BodyCheck(m_pBody) && BodyCheck(m_pLeg)) {	// 上下どちらも使用中
				m_pBody->GetMotion()->Set(m_action);
				m_pLeg->GetMotion()->Set(m_action);
			}
		}
	}
}

//===============================================
// 壁走り
//===============================================
void CPlayer::WallDush(void)
{
	if (m_action != ACTION_WALLSTAND) {	// 壁ずり中じゃない
		if (m_action != ACTION_WALLDUSH) {	// 壁走りもしてない
			return;
		}
	}

	// 壁ずり判定
	if (m_ColiNor.x == 0.0f && m_ColiNor.z == 0.0f) {	// オブジェクトに触れていない
		if (m_action == ACTION_WALLDUSH) {
			m_action = ACTION_NEUTRAL;
		}
		return;
	}

	// 壁dash確認
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();
	CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();	// キーボードのポインタ

	// 入力中
	if (pInputPad->GetPress(CInputPad::BUTTON_B, m_nId) || pInputKey->GetPress(DIK_SPACE)) {
		m_action = ACTION_WALLDUSH;
	}
	else {	// 離した

		if (m_action == ACTION_WALLDUSH) {	// 壁ずり中
			m_Info.move += m_ColiNor * WALLKICK_MOVE * 1.0f;
			m_fRotDest = atan2f(-m_Info.move.x, -m_Info.move.z);
			m_Info.move.y = JUMP;
			m_action = ACTION_WALLKICK;
		}
	}
}

//===============================================
// 天井走り
//===============================================
void CPlayer::CeilingDush(void)
{
	// 入力確認
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();
	CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();	// キーボードのポインタ

	// ぶつかり判定
	if (m_ColiNor.y >= 0.0f) {	// 天井に触れていない
		if (m_action == ACTION_CEILINGDUSH) {
			m_action = ACTION_NEUTRAL;
			m_Info.move.y =0.0f;
		}
		return;
	}

	// 入力中
	if (pInputPad->GetPress(CInputPad::BUTTON_B, m_nId) || pInputKey->GetPress(DIK_SPACE)) {
		m_action = ACTION_CEILINGDUSH;
		m_Info.move.y = JUMP;
	}
	else {
		if (m_action == ACTION_CEILINGDUSH) {
			m_action = ACTION_NEUTRAL;
			m_Info.move.y = 0.0f;
		}
	}
}

//===============================================
// 重力設定
//===============================================
void CPlayer::Gravity(void)
{
	float fGravity = GRAVITY;
	// 慣性を変更
	switch (m_action) {
	case ACTION_WALLSTAND:
		if (m_Info.move.y <= 0.0f) {
			fGravity = 0.0f;
			m_Info.move.y = WALLSLIDE_GRAVITY;
		}
		break;

	case ACTION_SLIDEJUMP:
		fGravity = SLIDEJUMP_GRAVITY;
		break;

	case ACTION_WALLDUSH:
		if (m_Info.move.y <= 0.0f) {
			fGravity = 0.0f;
			m_Info.move.y = WALLSLIDE_GRAVITY;
		}
		break;

	default:

		break;
	}

	// 重力にスロー倍率を掛け合わせる
	fGravity *= CManager::GetInstance()->GetSlow()->Get();

	m_Info.move.y += fGravity;
	m_Info.pos.y += m_Info.move.y * CManager::GetInstance()->GetSlow()->Get();
}

//===============================================
// 調整
//===============================================
void CPlayer::Adjust(void)
{
	while (1)
	{
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
		else
		{
			break;
		}
	}

	m_fRotDiff = m_fRotDest - m_fRotMove;	//目標までの移動方向の差分

	while (1)
	{
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
		else
		{
			break;
		}
	}

	m_Info.rot.y += m_fRotDiff * ROT_MULTI;

	while (1)
	{
		if (m_Info.rot.y > D3DX_PI || m_Info.rot.y < -D3DX_PI)
		{//-3.14～3.14の範囲外の場合
			if (m_Info.rot.y > D3DX_PI)
			{
				m_Info.rot.y += (-D3DX_PI * 2);
			}
			else if (m_Info.rot.y < -D3DX_PI)
			{
				m_Info.rot.y += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}
}

//===============================================
// 状態管理
//===============================================
void CPlayer::StateSet(void)
{
	float fSlawMul = CManager::GetInstance()->GetSlow()->Get();
	switch (m_Info.state)
	{
	case STATE_APPEAR:
	{
		m_Info.fStateCounter -= fSlawMul;

		if (m_Info.fStateCounter <= 0.0f)
		{
			m_Info.fStateCounter = 0.0f;
			m_Info.state = STATE_NORMAL;
		}
	}
		break;

	case STATE_NORMAL:
	{

	}
		break;

	case STATE_DAMAGE:
	{
		m_Info.fStateCounter -= fSlawMul;

		if (m_Info.fStateCounter <= 0.0f)
		{
			m_Info.fStateCounter = DAMAGE_APPEAR;
			m_Info.state = STATE_APPEAR;
		}
	}
		break;

	case STATE_DEATH:
	{
		m_Info.fStateCounter -= fSlawMul;

		if (m_Info.fStateCounter <= 0.0f)
		{
			m_Info.fStateCounter = DAMAGE_APPEAR;
			m_Info.state = STATE_SPAWN;

			if (m_pBody != nullptr) {
				m_pBody->SetDraw();
			}

			if (m_pLeg != nullptr) {
				m_pLeg->SetDraw();
			}

			CModel *pModel = m_pLeg->GetParts(0);  // 腰のパーツ

			// 煙のパーティクル生成
			CParticle::Create(D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43), CEffect::TYPE_SMAKE);
		}
	}
		break;

	case STATE_SPAWN:
	{
		m_Info.fStateCounter -= fSlawMul;

		if (m_Info.fStateCounter <= 0.0f)
		{
			m_Info.fStateCounter = SPAWN_INTERVAL;
			m_Info.state = STATE_APPEAR;
		}
	}
		break;

	}
}

//===============================================
// ダメージ処理
//===============================================
void CPlayer::Damage(int nDamage) 
{ 
	if (m_Info.state != STATE_NORMAL)
	{// ダメージを食らわない
		return;
	}

	// エフェクトの生成
	if (m_pBody != nullptr) {
		if (m_pBody->GetParts(HEADPARTS_IDX) != nullptr) {
			D3DXVECTOR3 pos = D3DXVECTOR3(m_pBody->GetParts(HEADPARTS_IDX)->GetMtx()->_41,
				m_pBody->GetParts(HEADPARTS_IDX)->GetMtx()->_42,
				m_pBody->GetParts(HEADPARTS_IDX)->GetMtx()->_43);
			CParticle::Create(pos, CEffect::TYPE_HIT);
		}
	}

	int nOldLife = m_nLife;
	m_nLife -= nDamage;

	if (m_nLife < 0)
	{
		m_nLife = 0;
	}
	
	if (m_nLife != nOldLife)
	{
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DAMAGE);
		m_Info.fStateCounter = DAMAGE_INTERVAL;
		m_Info.state = STATE_DAMAGE;

		if (m_nLife <= 0)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DEATH);
			m_nLife = 0;
			m_Info.state = STATE_DEATH;
			m_Info.fStateCounter = DAMAGE_APPEAR;

			CModel *pModel = m_pLeg->GetParts(0);  // 腰のパーツ

			// 煙のパーティクル生成
			CParticle::Create(D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43), CEffect::TYPE_BLACKSMAKE);

			if (m_pBody != nullptr){
				m_pBody->SetDraw(false);
			}

			if (m_pLeg != nullptr){
				m_pLeg->SetDraw(false);
			}
		}
	}
}

//===============================================
// 体力設定
//===============================================
void CPlayer::SetLife(int nLife)
{
	m_nLife = nLife;

	if (m_nLife < 0)
	{
		m_nLife = 0;
	}
}

//===============================================
// 体力設定
//===============================================
void CPlayer::SetType(TYPE type)
{
	m_type = type;
}

//===============================================
// モーション設定
//===============================================
void CPlayer::MotionSet(void)
{
	if (!BodyCheck(m_pBody)) {// 胴体確認失敗
		return;
	}

	if (!BodyCheck(m_pLeg)) {// 下半身確認失敗
		return;
	}

	if (m_Info.state == STATE_DAMAGE)
	{// ダメージ状態
		m_pBody->GetMotion()->Set(m_action);
		m_pLeg->GetMotion()->Set(m_action);

		if (m_pBody->GetMotion()->GetEnd())
		{// モーション終了
			m_action = ACTION_NEUTRAL;	// 保持状態に変更
		}
		else
		{
			return;
		}
	}

	if (m_action > ACTION_JUMP) {	// 派生アクションの場合
		// 派生モーション設定
		
		switch (m_action) {
		case ACTION_SLIDING:
		{
			m_pBody->GetMotion()->BlendSet(m_action);
			m_pLeg->GetMotion()->BlendSet(m_action);
		}
			break;

		case ACTION_WALLSTAND:
		{
			m_pBody->GetMotion()->BlendSet(m_action);
			m_pLeg->GetMotion()->BlendSet(m_action);
		}
			break;

		case ACTION_WALLKICK:
		{
			if (m_pBody->GetMotion()->GetEnd())
			{// モーション終了
				m_action = ACTION_JUMP;
			}

			m_pBody->GetMotion()->BlendSet(m_action);
			m_pLeg->GetMotion()->BlendSet(m_action);
		}
			break;

		case ACTION_NORMALATK:

			if (m_pBody->GetMotion()->GetEnd())
			{// モーション終了
				m_action = ACTION_NEUTRAL;
			}
		{
			m_pBody->GetMotion()->BlendSet(m_action);
			m_pLeg->GetMotion()->BlendSet(m_action);
		}
			break;

		case ACTION_CEILINGDUSH:

			{
				m_pBody->GetMotion()->BlendSet(m_action);
				m_pLeg->GetMotion()->BlendSet(m_action);
			}
			break;

		default:

			break;
		}

		return;
	}
	else if (!m_bJump && !m_bMove && 
		m_action >= ACTION_NEUTRAL && m_action <= ACTION_JUMP)
	{// 何もしていない
		m_action = ACTION_NEUTRAL;
		m_pBody->GetMotion()->BlendSet(m_action);
	}
	else if(m_bJump && 
		m_action >= ACTION_NEUTRAL && m_action <= ACTION_JUMP)
	{// ジャンプした
		m_action = ACTION_JUMP;
		m_pBody->GetMotion()->BlendSet(m_action);
	}
	else if (m_bMove &&
		m_action >= ACTION_NEUTRAL && m_action <= ACTION_JUMP)
	{// 移動した
		m_action = ACTION_WALK;
		m_pBody->GetMotion()->BlendSet(m_action);
	}

	if (m_action == ACTION_SLIDING) {
		m_pLeg->GetMotion()->Set(m_action);
	}
	else if (m_bJump)
	{
		m_pLeg->GetMotion()->BlendSet(ACTION_JUMP);
	}
	else if (m_bMove)
	{
		m_pLeg->GetMotion()->BlendSet(ACTION_WALK);
		if (m_pLeg->GetMotion()->GetNowFrame() == 0 && (m_pLeg->GetMotion()->GetNowKey() == 0 || m_pLeg->GetMotion()->GetNowKey() == 2))
		{
			CParticle::Create(m_Info.pos, CEffect::TYPE_WALK);
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_STEP);
		}
	}
	else
	{
		m_pLeg->GetMotion()->BlendSet(ACTION_NEUTRAL);
	}
}

//===============================================
// 攻撃
//===============================================
void CPlayer::Attack(void)
{
	CInputKeyboard* pInputKey = CManager::GetInstance()->GetInputKeyboard();	// キーボードのポインタ
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();				// パッドのポインタ

	if (pInputKey->GetTrigger(DIK_I) || pInputPad->GetTrigger(CInputPad::BUTTON_Y, m_nId)) {
		m_action = ACTION_NORMALATK;
	}
}

//===============================================
// マトリックス設定
//===============================================
void CPlayer::SetMatrix(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイスへのポインタを取得
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
}

//===============================================
// 攻撃のヒット確認
//===============================================
bool CPlayer::HitCheck(D3DXVECTOR3 pos, float fRange, int nDamage)
{
	bool m_bValue = false;
	if (m_Info.state != STATE_NORMAL) {
		return m_bValue;
	}

	if (m_pBody == nullptr) {
		return m_bValue;
	}

	if (pos.x + fRange < m_Info.pos.x + COLLIMIN.x ||
		pos.x - fRange > m_Info.pos.x + COLLIMAX.x)
	{// X範囲外
		return m_bValue;
	}

	if (pos.z + fRange < m_Info.pos.z + COLLIMIN.z ||
		pos.z - fRange > m_Info.pos.z + COLLIMAX.z)
	{// Z範囲外
		return m_bValue;
	}

	if (pos.y + fRange < m_Info.pos.y + COLLIMIN.y ||
		pos.y - fRange > m_Info.pos.y + COLLIMAX.y)
	{// Y範囲外
		return m_bValue;
	}

	m_bValue = true;
	Damage(nDamage);

	return m_bValue;
}

//===============================================
// 指定モーションに設定
//===============================================
void CPlayer::SetMotion(int nMotion) {
	if (m_pBody == nullptr) {
		return;
	}

	if (m_pBody->GetMotion() == nullptr) {
		return;
	}

	m_pBody->GetMotion()->InitSet(nMotion);

	if (m_pLeg == nullptr) {
		return;
	}

	if (m_pLeg->GetMotion() == nullptr) {
		return;
	}

	m_pLeg->GetMotion()->InitSet(nMotion);
}

//===============================================
// モーションを取得
//===============================================
int CPlayer::GetMotion(void) {

	if (m_pBody == nullptr) {
		return -1;
	}

	if (m_pBody->GetMotion() == nullptr) {
		return -1;
	}

	return m_pBody->GetMotion()->GetNowMotion();
}

//===============================================
// リザルト失敗時のパーティクル
//===============================================
void CPlayer::SetFailedParticle(void)
{
	if (!BodyCheck(m_pBody)) {	// 胴体確認失敗
		return;
	}

	if (m_pBody->GetMotion()->GetNowFrame() != 0)	// 現在0フレームではない
	{
		return;
		CParticle::Create(m_Info.pos, CEffect::TYPE_WALK);
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_STEP);
	}

	CModel *pModel = nullptr;

	if (m_pBody->GetMotion()->GetNowKey() == 0)
	{
		pModel = m_pBody->GetParts(4);
	}
	else if (m_pBody->GetMotion()->GetNowKey() == 1)
	{
		pModel = m_pBody->GetParts(m_pBody->GetNumParts() - 1);
	}

	if (pModel == nullptr) {	// 使われていない
		return;
	}

	D3DXVECTOR3 pos = D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43);
	CParticle::Create(pos, CEffect::TYPE_RESULTZITABATA);
}

//===============================================
// プレイヤーの描画設定
//===============================================
void CPlayer::SetDraw(bool bDraw)
{
	if (m_pBody != nullptr) {
		m_pBody->SetDraw(bDraw);
	}

	if (m_pLeg != nullptr) {
		m_pLeg->SetDraw(bDraw);
	}
}

//===============================================
// 使用階層構造の設定
//===============================================
void CPlayer::BodySet(void)
{
	// 下半身更新
	if (BodyCheck(m_pLeg))
	{// 使用されている場合

		// 腰の設定
		if (m_pWaist != nullptr)
		{
			CModel* pModel = m_pLeg->GetParts(0);

			// 腰の高さを補填
			m_pWaist->SetPosition(m_pWaist->GetSetPosition() + pModel->GetOldPosition());
			m_pWaist->SetMatrix();
		}

		if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME) {
			m_pLeg->Update();
		}

		
		CModel* pModel = m_pLeg->GetParts(0);
		pModel->SetCurrentPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// 胴体更新
	if (BodyCheck(m_pBody))
	{// 使用されている場合
		m_pBody->Update();
	}
}

//===============================================
// 体使用確認
//===============================================
bool CPlayer::BodyCheck(CCharacter* pBody)
{
	if (pBody == nullptr) {	// 使用されている
		return false;
	}

	if (pBody->GetMotion() == nullptr) {	// モーションも使用されている
		return false;
	}

	return true;
}

//===============================================
// 攻撃チェック
//===============================================
void CPlayer::Hit(void)
{
	if (!BodyCheck(m_pBody) || !BodyCheck(m_pLeg)) {
		return;
	}

	CModel* pModel = m_pLeg->GetParts(3);
	float fRange = 50.0f;
	int nDamage = 1;
	D3DXVECTOR3 pos = { pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43 };
	CEnemyManager::GetInstance()->Hit(pos, fRange, nDamage);
}

//===============================================
// カメラ設定
//===============================================
void CPlayer::SetCamera(CCamera* pCamera) 
{ 
	m_pMyCamera = pCamera; 
	if (m_pMyCamera == nullptr) {
		return;
	}

	m_fCamRotZ = m_pMyCamera->GetRotation().z; 
	m_fCamLength = m_pMyCamera->GetLength();
}
