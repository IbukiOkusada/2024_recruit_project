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
#include "item.h"
#include "gimmick.h"
#include "gimmick_rotatedoor.h"
#include "enemy.h"
#include "goal.h"
#include "score.h"
#include "life.h"
#include "ui.h"
#include "bullet.h"
#include "air.h"
#include "particle.h"
#include "effect.h"
#include "gage.h"
#include "throwitem.h"
#include "headui.h"

//===============================================
// マクロ定義
//===============================================
#define MOVE	(4.0f)		// 移動量
#define GRAVITY	(-1.0f)		//プレイヤー重力
#define ROT_MULTI	(0.1f)	// 向き補正倍率
#define WIDTH	(20.0f)		// 幅
#define HEIGHT	(80.0f)	// 高さ
#define INER	(0.3f)		// 慣性
#define BLOW_INER (0.1f)	// 吹き飛ばし中慣性
#define STEP_SPEED	(50.0f)
#define STEP_COOLTIME	(90.0f)
#define START_LIFE	(6)	// 初期体力
#define DAMAGE_INTERVAL	(10.0f)
#define DAMAGE_APPEAR	(110.0f)
#define DEATH_INTERVAL	(120.0f)
#define DASH_INTERVAL	(60.0f)
#define SPAWN_INTERVAL	(60.0f)
#define PARTICLE_TIMER	 (5.0f)
#define SHADOW_ALPHA	(0.4f)
#define JUMP	(20.0f)
#define ATK_RANGE	(50.0f)
#define CATCH_RANGE	(100.0f)
#define DROP_CNT	(4)
#define START_COIN	(10)
#define CATCH_LIMIT	(90)
#define CATCH_MOVE	(2.0f)
#define SPEED_DECAY (0.04f)  // 持っているアイテムの数に応じてスピードが下がる
#define HAND_PARTS	(4)	 // 手のモデル番号(後ろから
#define MAX_GAGE		(100.0f)		// ゲージ最大

namespace {
	const float BULLET_MOVE = (22.0f);	// 弾の移動量
	const float HIT_RANGE = (100.0f);	// 当たり判定のサイズ
	const float KUNAI_GAGE = (50.0f);	// クナイのゲージ必要量
	const float AIR_GAGE = (100.0f);	// 風神の術のゲージ必要量
	const float KAKUREMI_GAGE = (1.0f);	// 隠れ蓑術のゲージ必要量
	const float GAGE_UPHEIGHT = (150.0f);	// ゲージの設置高さ
	const float FADE_COLA = (0.05f);
	const D3DXVECTOR2 GAGE_SIZE = { 75.0f, 5.0f };	// ゲージのポリゴンサイズ
	const float ITEMUI_UPHEIGHT = (180.0f); // アイテムUIの設置高さ
	const D3DXVECTOR2 ITEMUI_SIZE = { 75.0f, 25.0f };	// アイテムUIのポリゴンサイズ
	const D3DXVECTOR2 NUMBER_SIZE = { 8.0f, 16.0f };	// 頭の上の数字UIのポリゴンサイズ
	const int HEADPARTS_IDX = (1);
	const D3DXVECTOR3 POS_WARP = D3DXVECTOR3(-760.0f, 1000.0f, 1400.0f);	//屋根裏から帰ってくる時のワープ先
	const D3DXVECTOR3 POS_WARP_ATTIC = D3DXVECTOR3(4500.0f, 1000.0f, 1400.0f);	//屋根裏へのワープ先
	const int GOAL_WAITTIME = (180);
	const int GOAL_QUITTIME = (GOAL_WAITTIME - 60);
	const D3DXVECTOR3 GOAL_CAMERAROT = { 0.0f, D3DX_PI * 1.0f, D3DX_PI * 0.46f };
}

// 前方宣言
CPlayer *CPlayer::m_pTop = nullptr;	// 先頭のオブジェクトへのポインタ
CPlayer *CPlayer::m_pCur = nullptr;	// 最後尾のオブジェクトへのポインタ
int CPlayer::m_nNumCount = 0;

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
	m_Catch.pPlayer = nullptr;
	m_Catch.pGimmick = nullptr;
	m_pScore = nullptr;
	m_Catch.SetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Catch.nMoveCnt = 0;
	m_nLife = 0;
	m_type = TYPE_NONE;
	m_nId = m_nNumCount;
	m_nNumItemCoin = 0;
	m_nNumItemBrecetet = 0;
	m_nNumItemCup = 0;
	m_nNumItemEmerald = 0;
	m_nNumItemDiamond = 0;
	m_nNumItemGold = 0;
	m_nNumItemJar = 0;
	m_nNumItemKunai = 0;
	m_nNumItemRing = 0;
	m_nNumItemScroll = 0;
	m_nNumItemShuriken = 0;
	m_nItemId = CItem::TYPE_COIN;
	m_action = ACTION_NEUTRAL;
	m_bJump = false;
	m_bGoal = false;
	m_bEnd = false;
	m_nItemCnt = 0;
	m_fGage = 0.0f;
	m_pMyCamera = nullptr;
	m_pGage = nullptr;
	m_pThrowItem = nullptr;
	m_pHeadUI = nullptr;
	m_pFade = nullptr;
	m_pEscape = nullptr;
	m_bWarp = false;
	m_nQuitCounter = 0;

	for (int i = 0; i < MAX_ITEM; i++)
	{
		m_aSaveType[i] = 0;
	}
	
	// 自分自身をリストに追加
	if (m_pTop != nullptr)
	{// 先頭が存在している場合
		m_pCur->m_pNext = this;	// 現在最後尾のオブジェクトのポインタにつなげる
		m_pPrev = m_pCur;
		m_pCur = this;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合
		m_pTop = this;	// 自分自身が先頭になる
		m_pCur = this;	// 自分自身が最後尾になる
	}

	m_nNumCount++;
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

	if (m_pScore == nullptr)
	{
		m_pScore = CScore::Create(D3DXVECTOR3(100.0f, 100.0f, 0.0f), 6, 0.75f, 30.0f, 30.0f);
	}
	
	if (m_pScore != nullptr)
	{
		m_pScore->Init();
	}

	m_pThrowItem = CThrowItem::Create(&m_Info.pos, ITEMUI_UPHEIGHT, ITEMUI_SIZE.x, ITEMUI_SIZE.y);
	m_pGage = CGage::Create(&m_Info.pos, GAGE_UPHEIGHT, GAGE_SIZE.x, GAGE_SIZE.y);
	m_pHeadUI = CHeadUI::Create(&m_Info.pos, m_pThrowItem->GetObjectBillBoard()->GetMtx(), GAGE_UPHEIGHT, NUMBER_SIZE.x, NUMBER_SIZE.y);
	m_fGage = MAX_GAGE;
	m_Info.state = STATE_APPEAR;
	m_action = ACTION_NEUTRAL;
	m_type = TYPE_NONE;
	m_nLife = START_LIFE;
	m_bJump = false;
	m_nItemCnt = 0;

	//m_pScore->AddScore(500 * m_nItemCnt);

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

	if (m_pScore != nullptr)
	{
		m_pScore->Init();
	}

	m_pThrowItem = CThrowItem::Create(&m_Info.pos, ITEMUI_UPHEIGHT, ITEMUI_SIZE.x, ITEMUI_SIZE.y);
	m_pGage = CGage::Create(&m_Info.pos, GAGE_UPHEIGHT, GAGE_SIZE.x, GAGE_SIZE.y);
	m_pHeadUI = CHeadUI::Create(&m_Info.pos, m_pThrowItem->GetObjectBillBoard()->GetMtx(), GAGE_UPHEIGHT, NUMBER_SIZE.x, NUMBER_SIZE.y);
	m_fGage = MAX_GAGE;
	m_nLife = START_LIFE;
	m_type = TYPE_NONE;
	m_action = ACTION_NEUTRAL;
	m_bJump = false;
	m_nItemCnt = 0;
	m_fFadeCola = FADE_COLA;

	//m_pScore->AddScore(500 * m_nItemCnt);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CPlayer::Uninit(void)
{
	// リストから自分自身を削除する
	if (m_pTop == this) {	// 自身が先頭
		if (m_pNext != nullptr)
		{// 次が存在している
			m_pTop = m_pNext;	// 次を先頭にする
			m_pNext->m_pPrev = nullptr;	// 次の前のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else if (m_pCur == this) {	// 自身が最後尾
		if (m_pPrev != nullptr)
		{	// 次が存在している
			m_pCur = m_pPrev;			// 前を最後尾にする
			m_pPrev->m_pNext = nullptr;	// 前の次のポインタを覚えていないようにする
		}
		else
		{	// 存在していない
			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else { // それ以外
		if (m_pNext != nullptr)
		{
			m_pNext->m_pPrev = m_pPrev;	// 自身の次に前のポインタを覚えさせる
		}
		if (m_pPrev != nullptr)
		{
			m_pPrev->m_pNext = m_pNext;	// 自身の前に次のポインタを覚えさせる
		}
	}

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

	// スコアの終了
	if (m_pUI != nullptr) {// 使用されている場合
		 // 終了処理
		m_pUI->Uninit();
		// 使用されていない状態にする
		m_pUI = nullptr;
	}

	// スコアの終了
	if (m_pScore != nullptr) {// 使用されている場合

		// 終了処理
		m_pScore->Uninit();

		// 開放
		delete m_pScore;

		// 使用されていない状態にする
		m_pScore = nullptr;
	}

	// ゲージの終了
	if (m_pGage != nullptr) {
		m_pGage->Uninit();
		m_pGage = nullptr;
	}

	if (m_pHeadUI != nullptr) {
		m_pHeadUI->Uninit();
		m_pHeadUI = nullptr;
	}

	if (m_pThrowItem != nullptr) {
		m_pThrowItem->Uninit();
		m_pThrowItem = nullptr;
	}

	if (m_pFade != nullptr) {
		m_pFade->Uninit();
		m_pFade = nullptr;
	}

	if (m_pEscape != nullptr) {
		m_pEscape->Uninit();
		m_pEscape = nullptr;
	}
	// 人数を減らす
	m_nNumCount--;

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
		CManager::GetInstance()->GetScene()->SendLife(m_nLife);
	}
	else if (m_type == TYPE_AUTOMOVE)
	{// 操作キャラではない
		if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME) {
			float fIner = INER;
			D3DXVECTOR3 pos = GetPosition();	// 座標を取得
			m_bMove = true;
			m_Info.move.x = 10.0f;

			MotionSet();	// モーション設定
			pos.x += m_Info.move.x * CManager::GetInstance()->GetSlow()->Get();
			m_Info.pos = pos;
		}
	}
	else {
		if (m_bGoal) {	// ゴール状態
			GoalWait();
		}
	}

	// カメラ追従
	if (m_pMyCamera != nullptr) {
		// 追従処理
		m_pMyCamera->CollisionObj();

		if (m_pMyCamera->GetMode() == CCamera::MODE_NORMAL)
		{
			m_pMyCamera->Pursue(GetPosition(), GetRotation());
		}

		SetWarpFadeCol();
	}

	if (m_Catch.pPlayer != nullptr) {
		if (m_Catch.pPlayer->m_Info.state == STATE_CATCH && m_Info.state == STATE_CATCH) {	// どっちも掴んでいる場合
			m_Catch.pPlayer->m_Info.state = STATE_NORMAL;
			m_Catch.pPlayer->m_Catch.pPlayer = nullptr;
			m_Catch.pPlayer = nullptr;
		}
	}

	CManager::GetInstance()->GetDebugProc()->Print("向き [%f, %f, %f] : ID [ %d]\n", GetRotation().x, GetRotation().y, GetRotation().z, m_nId);
	CManager::GetInstance()->GetDebugProc()->Print("位置 [%f, %f, %f]", GetPosition().x, GetPosition().y, GetPosition().z);
	CManager::GetInstance()->GetDebugProc()->Print("体力 [ %d ] : 状態 [ %d ] : アイテム所持数 [ %d ] : 選択中のアイテム [ %d ] : 忍術ゲージ [ %f ]\n", m_nLife, m_Info.state, m_nItemCnt, m_nItemId, m_fGage);

	// マトリックス設定
	if (m_Info.state == STATE_CATCH) {	// キャッチされている場合!!!!
		SetCatchMatrix();
	}
	else {	// それ以外!!!!!!
		SetMatrix();
	}

	BodySet();

	if (m_nLife <= 0) {	// 体力が0
		m_Info.state = STATE_DEATH;
	}

	if (m_pLeg != nullptr)
	{// 使用されている場合
		CModel *pModel = m_pLeg->GetParts(0);

		pModel->SetCurrentPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// ゲージの設定
	{
		if (m_pGage == nullptr) {
			return;
		}

		float fRate = m_fGage / MAX_GAGE;
		m_pGage->SetRate(fRate);
	}

	// 捨てるアイテム
	{
		if (m_pThrowItem != nullptr)
		{
			m_pThrowItem->SetItem(m_nItemId);
		}
	}

	// 所持しているアイテムの数
	{
		if (m_pHeadUI != nullptr)
		{
			m_pHeadUI->SetIdx(GetSelectItem(m_nItemId));
		}
	}
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
	CGimmick *pGimmick = &*m_Catch.pGimmick;

	// 操作処理
	if(m_action != ACTION_DAMAGE){	// ダメージリアクションをしていない
		if (m_Info.state != STATE_DEATH) {	// 死亡していない
			if (m_Info.state != STATE_BLOW) {	// 吹き飛ばし以外
				Move();		// 移動
			}
			else {
				fIner = BLOW_INER;
			}

			Rotation();	// 回転
			Jump();		// ジャンプ
			Attack();	// 攻撃
			Catch();		// 掴む
			Throw();		// 投げる
			Ninjutsu();	// 術の使用
			SelectItem();   // 捨てるアイテム選択
		}
	}

	MotionSet();	// モーション設定
	pos = GetPosition();	// 座標を取得

	float fGravity = GRAVITY * CManager::GetInstance()->GetSlow()->Get();
	m_Info.move.y += fGravity;
	pos.y += m_Info.move.y * CManager::GetInstance()->GetSlow()->Get();


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
		if (m_Info.pos.y < fHeight)
		{
			m_Info.pos.y = fHeight;
			m_Info.move.y = 0.0f;
			m_bJump = false;
		}
	}

	// オブジェクトとの当たり判定
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(50.0f, 120.0f, 50.0f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-50.0f, 0.0f, -50.0f);
	D3DXVECTOR3 vtxMaxOld = vtxMax;
	D3DXVECTOR3 vtxMinOld = vtxMin;
	if (m_Catch.pPlayer != nullptr) {
		vtxMax.x *= 2;
		vtxMax.z *= 2;
		vtxMin.x *= 2;
		vtxMin.z *= 2;
	}

	if (m_Catch.pGimmick != nullptr) {
		vtxMax.x *= 3;
		vtxMax.z *= 3;
		vtxMin.x *= 3;
		vtxMin.z *= 3;
	}

	if (CObjectX::Collision(m_Info.pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax, vtxMinOld, vtxMaxOld))
	{
		m_bJump = false;
	}

	if (bOld && !m_bJump) {
		CParticle::Create(m_Info.pos, CEffect::TYPE_LAND);
		
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_LAND);
	}

	// アイテムとの当たり判定
	CItem *pItem = CItem::Collision(m_Info.pos);

	if (pItem != nullptr) {
		m_nItemCnt++;
		if (m_pScore != nullptr)
		{
			m_pScore->AddScore(pItem->GetEachScore());

			// パーティクルの設定
			CParticle::Create(m_Info.pos, CEffect::TYPE_ITEMGET);
			//CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_GET);
		}

		for (int i = 0; i < MAX_ITEM; i++)
		{
			if (m_aSaveType[i] == 0)
			{
				m_aSaveType[i] = pItem->GetType();

				break;
			}
		}

		AddItemCount(pItem->GetType());

		pItem->Uninit();
	}

	if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME)
	{
		if (m_pUI != nullptr) {
			m_pUI->SetLife(m_nLife);
		}
	}

	// ギミックとの判定
	bool bLand = false;

	if (CGimmick::Collision(m_Info.pos, m_Info.posOld, m_Info.move, m_Catch.SetPos, vtxMin, vtxMax, m_action, &m_Catch.pGimmick,&bLand)) {
		Damage(1);
	}

	if (pGimmick != m_Catch.pGimmick && m_Catch.pGimmick != nullptr) {

		if (m_Catch.pPlayer != nullptr) {
			m_Catch.pGimmick->SetMtxParent(nullptr);
			m_Catch.pGimmick = nullptr;
		}
		else {
			if (m_Catch.pGimmick->GetPull() != nullptr)
			{
				if (m_pBody->GetMotion()->GetNowKey() == m_pBody->GetMotion()->GetNowNumKey() - 1 && m_pBody->GetMotion()->GetNowFrame() == 0)
				{// 掴むことができるモーションタイミング
					m_Catch.pGimmick->SetMtxParent(&m_Info.mtxWorld);
					m_action = ACTION_CATCH;
					CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_CATCH);
				}
				else
				{
					m_Catch.pGimmick->SetMtxParent(nullptr);
					m_Catch.pGimmick = nullptr;
				}
			}
		}
	}

	if (bLand == true)
	{
		m_bJump = false;
	}

	//屋根裏にワープ
	if (CManager::GetInstance()->GetInstance()->GetMode() == CScene::MODE_GAME) {
		if (pos.x <= 1500.0f
			&& pos.y <= -100.0f)
		{

			m_bWarp = true;

			if (pos.y <= -1000.0f) {
				m_Info.move.y = 0.0f;
				m_Info.pos.x = POS_WARP_ATTIC.x;
				m_Info.pos.y = POS_WARP_ATTIC.y;
				m_Info.pos.z = POS_WARP_ATTIC.z;
			}
		}

		//屋根裏からワープ
		if (pos.x > 1500.0f
			&& pos.y <= -100.0f)
		{

			m_bWarp = true;

			if (pos.y <= -1000.0f) {
				m_Info.move.y = 0.0f;
				m_Info.pos.x = POS_WARP.x;
				m_Info.pos.y = POS_WARP.y;
				m_Info.pos.z = POS_WARP.z;
			}
		}
	}
	else {	// チュートリアル
		if (m_Info.pos.y < 1.0f)
		{
			m_Info.pos.y = 1.0f;
			m_Info.move.y = 0.0f;
			m_bJump = false;
		}
	}

	// ゴールとの判定
	if (!m_bGoal) {	// まだゴールしていない
		if (CGoal::Collision(m_Info.pos, m_Info.posOld)) {	// ゴールを跨いだ
			m_bGoal = true;
			m_type = TYPE_NONE;
		}
	}

	if (m_Catch.pGimmick != nullptr && m_Info.state != STATE_DEATH) {
		m_Info.state = STATE_CATCH;
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

	// 入力装置確認
	if (nullptr == pInputPad){
		return;
	}

	//落とし穴に落ちなければ操作可能に
	if (m_Info.pos.y >= 0.0f)
	{
		//プレイヤーの更新
		MoveController();
	}
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

	// コントローラーの入力方向取得
	/*D3DXVECTOR2 vec;
	vec.y = pInputPad->GetStickAdd(m_nId, CInputPad::BUTTON_LEFT_X, 0.1f, CInputPad::STICK_PLUS);
	vec.x = pInputPad->GetStickAdd(m_nId, CInputPad::BUTTON_LEFT_Y, 0.1f, CInputPad::STICK_PLUS);
	D3DXVec2Normalize(&vec, &vec);
	m_fRotDest = atan2f(vec.y, vec.x);*/
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

	if (pInputKey->GetPress(DIK_W))
	{
		if (pInputKey->GetPress(DIK_A))
		{
			m_fRotDest = -D3DX_PI * 0.25f;
		}
		else if (pInputKey->GetPress(DIK_D))
		{
			m_fRotDest = D3DX_PI * 0.25f;
		}
		else
		{
			m_fRotDest = D3DX_PI * 0.0f;
		}
	}
	else if (pInputKey->GetPress(DIK_S))
	{
		if (pInputKey->GetPress(DIK_A))
		{
			m_fRotDest = -D3DX_PI * 0.75f;
		}
		else if (pInputKey->GetPress(DIK_D))
		{
			m_fRotDest = D3DX_PI * 0.75f;
		}
		else
		{
			m_fRotDest = D3DX_PI * 1.0f;
		}
	}
	else if (pInputKey->GetPress(DIK_A))
	{
		m_fRotDest = -D3DX_PI * 0.5f;
	}
	else if (pInputKey->GetPress(DIK_D))
	{
		m_fRotDest = D3DX_PI * 0.5f;
	}
}

//===============================================
// 調整
//===============================================
void CPlayer::MoveController(void)
{
	m_bMove = false;

	if (m_action == ACTION_AIR) {
		return;
	}

	if (m_nId < 0 || m_nId >= PLAYER_MAX)
	{// コントローラー数おーばー
		return;
	}

	CCamera *pCamera = m_pMyCamera;		// カメラのポインタ

	if (m_pMyCamera == nullptr) {
		pCamera = CManager::GetInstance()->GetCamera();
	}

	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();
	float fSpeed = MOVE;	// 移動量
	fSpeed -= (m_nItemCnt * SPEED_DECAY);

	if (m_Catch.pPlayer != nullptr || m_Catch.pGimmick != nullptr) {
		fSpeed = CATCH_MOVE;
		fSpeed -= (m_nItemCnt * (SPEED_DECAY * 0.5f));
	}

	fSpeed -= (m_nItemCnt * SPEED_DECAY);

	if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_X, 0.8f, CInputPad::STICK_MINUS) == true)
	{
		if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_PLUS))
		{
			m_Info.move.x += cosf(CamRot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_Info.move.z += sinf(CamRot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_fRotDest = (-CamRot.y + D3DX_PI * 0.25f);
		}
		else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_MINUS))
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
	else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_X, 0.8f, CInputPad::STICK_PLUS) == true)
	{
		if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_PLUS))
		{
			m_Info.move.x += cosf(CamRot.y + (D3DX_PI * 0.75f)) * fSpeed;
			m_Info.move.z += sinf(CamRot.y + (D3DX_PI * 0.75f)) * fSpeed;

			m_fRotDest = (-CamRot.y + D3DX_PI * 0.75f);
		}
		else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_MINUS))
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
	else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_PLUS))
	{
		m_Info.move.x += -cosf(CamRot.y) * fSpeed;
		m_Info.move.z += -sinf(CamRot.y) * fSpeed;
		m_fRotDest = (-CamRot.y + D3DX_PI * 0.5f);

		// 移動した状態にする
		m_bMove = true;

	}
	else if (pInputPad->GetStickPress(m_nId, CInputPad::BUTTON_LEFT_Y, 0.8f, CInputPad::STICK_MINUS))
	{
		m_Info.move.x += cosf(CamRot.y) * fSpeed;
		m_Info.move.z += sinf(CamRot.y) * fSpeed;
		m_fRotDest = (-CamRot.y + -D3DX_PI * 0.5f);

		// 移動した状態にする
		m_bMove = true;
	}
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

	if (m_Catch.pPlayer != nullptr && m_Info.state != STATE_CATCH && m_action == ACTION_AIR)
	{
		return;
	}

	if (m_Catch.pGimmick != nullptr)
	{
		return;
	}

	if (m_action == ACTION_AIR) {
		return;
	}

	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	// 入力
	if (pInputPad->GetTrigger(CInputPad::BUTTON_B, m_nId))
	{
		if (m_bJump == false)
		{// ジャンプしていない場合
			m_bJump = true;

			if(m_Catch.pPlayer != nullptr)
			{
				m_Info.move.y = JUMP * 0.5f;	
				// 少なめのパーティクルに
				CParticle::Create(m_Info.pos, CEffect::TYPE_WALK);
			}
			else
			{
				m_Info.move.y = JUMP;
				CParticle::Create(m_Info.pos, CEffect::TYPE_JUMP);
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_JUMP);
			}
		}
	}
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
			m_nLife = START_LIFE;

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

	case STATE_CATCH:
	{

	}
		break;

	case STATE_BLOW:
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
		if (m_Info.state != STATE_CATCH)
		{
			return;
		}
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

	if (m_nLife > 0)
	{
		Drop(DROP_CNT * (nOldLife - m_nLife));
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

			DropAll();
		}
	}

	if (m_action == ACTION_HENGE) {	// 変化だった
		CModel *pModel = m_pLeg->GetParts(0);  // 腰のパーツ
		m_action = ACTION_DAMAGE;

		// 煙のパーティクル生成
		CParticle::Create(D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43), CEffect::TYPE_SMAKE);
		ChangeBody();
	}

	if (m_Catch.pPlayer != nullptr) {	// 他のプレイヤーを持っている
		m_Catch.pPlayer->m_Info.state = STATE_NORMAL;
		m_Catch.pPlayer->m_Catch.pPlayer = nullptr;
		m_Catch.pPlayer = nullptr;
	}

	if (m_Catch.pGimmick != nullptr) {	// 他のプレイヤーを持っている
		m_Catch.pGimmick->SetMtxParent(nullptr);
		m_Catch.pGimmick = nullptr;
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
	if (nullptr == m_pBody)
	{// オブジェクト無し
		return;
	}

	if (nullptr == m_pBody->GetMotion())
	{// モーション無し
		return;
	}

	if (m_Info.state == STATE_DAMAGE)
	{// ダメージ状態
		m_action = ACTION_DAMAGE;
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
	else if (m_Info.state == STATE_BLOW)
	{// ダメージ状態
		m_pBody->GetMotion()->Set(ACTION_DAMAGE);
		m_pLeg->GetMotion()->Set(ACTION_DAMAGE);

		if (m_pBody->GetMotion()->GetEnd())
		{// モーション終了
			m_action = ACTION_NEUTRAL;	// 保持状態に変更
			m_Info.state = STATE_NORMAL;
		}
		else
		{
			return;
		}
	}

	if (m_Info.state == STATE_CATCH && m_Catch.pPlayer != nullptr) {	// 掴まれている
		m_pBody->GetMotion()->Set(ACTION_FLUTTERING);
		m_pLeg->GetMotion()->Set(ACTION_FLUTTERING);

		// 汗の生成
		if (m_pBody->GetMotion()->GetNowFrame() == 0) {	// タイミングがあった
			if (m_pBody->GetParts(1) != nullptr) {	// パーツが存在している
				D3DXVECTOR3 pos = D3DXVECTOR3(m_pBody->GetParts(1)->GetMtx()->_41,
					m_pBody->GetParts(1)->GetMtx()->_42 + 20.0f,
					m_pBody->GetParts(1)->GetMtx()->_43);
				CParticle::Create(pos, CEffect::TYPE_CATCH);
				
			}
		}

		return;
	}

	if (!m_bJump && !m_bMove && 
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
	else if (m_action == ACTION_ATK)
	{// 攻撃した
		m_pBody->GetMotion()->Set(m_action);
		if (m_pBody->GetMotion()->GetEnd())
		{// モーション終了
			m_action = ACTION_NEUTRAL;
		}
	}
	else if (m_action == ACTION_CATCH)
	{// 持った
		m_pBody->GetMotion()->BlendSet(m_action);
		if (m_pBody->GetMotion()->GetEnd())
		{// モーション終了
			if (m_Catch.pPlayer == nullptr && m_Catch.pGimmick == nullptr)
			{
				m_action = ACTION_NEUTRAL;
			}
		}
		else
		{
			if (m_Catch.pPlayer == nullptr){	// プレイヤーを掴んでいない
				CModel *pModel = m_pBody->GetParts(m_pBody->GetNumParts() - HAND_PARTS);
				D3DXVECTOR3 pos = D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43);
				PlayerCatch(pos);
			}
		}
	}
	else if (m_action == ACTION_HOLD)
	{// 保持している
		m_pBody->GetMotion()->BlendSet(m_action);
		if (m_pBody->GetMotion()->GetEnd())
		{// モーション終了
		
		}
	}
	else if (m_action == ACTION_THROW)
	{// 投げる
		m_pBody->GetMotion()->BlendSet(m_action);
		if (m_pBody->GetMotion()->GetEnd())
		{// モーション終了
			m_action = ACTION_NEUTRAL;
		}
	}
	else if (m_action == ACTION_DAMAGE)
	{// ダメージ
		m_pBody->GetMotion()->BlendSet(m_action);
		if (m_pBody->GetMotion()->GetEnd())
		{// モーション終了
			m_action = ACTION_NEUTRAL;
		}

		return;
	}
	else if (m_action == ACTION_HENGE)
	{// 変化中
		if (!m_bJump && !m_bMove)
		{// 何もしていない
			m_pBody->GetMotion()->BlendSet(ACTION_NEUTRAL);
		}
		else if (m_bJump)
		{// ジャンプした
			m_pBody->GetMotion()->BlendSet(ACTION_JUMP);
		}
		else if (m_bMove)
		{// 移動した
			m_pBody->GetMotion()->BlendSet(ACTION_WALK);
		}
	}
	else if (m_action == ACTION_KUNAI) 
	{// クナイ投げ
		m_pBody->GetMotion()->Set(ACTION_ATK);

		if (m_pBody->GetMotion()->GetNowFrame() == 0 && m_pBody->GetMotion()->GetNowKey() == m_pBody->GetMotion()->GetNowNumKey() - 2)
		{
			BulletSet();
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_KUNAI);
		}

		if (m_pBody->GetMotion()->GetEnd())
		{// モーション終了
			m_action = ACTION_NEUTRAL;
		}
	}
	else if (m_action == ACTION_AIR)
	{// 風の術
		m_pBody->GetMotion()->Set(ACTION_HENGE);
		m_pLeg->GetMotion()->Set(ACTION_HENGE);

		if (m_pBody->GetMotion()->GetNowFrame() == 0 && m_pBody->GetMotion()->GetNowKey() == m_pBody->GetMotion()->GetNowNumKey() - 2)
		{
			CAir::Create(m_Info.pos, m_nId);
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_AIR);
		}

		if (m_pBody->GetMotion()->GetEnd())
		{// モーション終了
			m_action = ACTION_NEUTRAL;
		}
		else
		{
			return;
		}
	}

	if (nullptr == m_pLeg){	// 脚がない
		return;
	}

	if (nullptr == m_pLeg->GetMotion()) {	// モーションがない
		return;
	}

	if (m_bJump)
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

		if (m_Catch.pGimmick != nullptr) {	// ギミックを持っている
			CParticle::Create(D3DXVECTOR3(m_Catch.pGimmick->GetMtxWorld()->_41, m_Catch.pGimmick->GetMtxWorld()->_42, m_Catch.pGimmick->GetMtxWorld()->_43),
				CEffect::TYPE_PULLNOW);

			if (m_pLeg->GetMotion()->GetNowFrame() == 0 && (m_pLeg->GetMotion()->GetNowKey() == 0 || m_pLeg->GetMotion()->GetNowKey() == 2))
			{
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_MOVE);
			}
		}

		if (m_Catch.pPlayer != nullptr) {	// プレイヤーを持っている
			if (m_pLeg->GetMotion()->GetNowFrame() == 0 && (m_pLeg->GetMotion()->GetNowKey() == 0 || m_pLeg->GetMotion()->GetNowKey() == 2))
			{
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_RAGE);
			}
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
	if (m_action < ACTION_NEUTRAL || m_action > ACTION_JUMP || m_Info.state == STATE_CATCH)
	{// 攻撃不可能
		if (m_action != ACTION_ATK)
		{// 攻撃中ではない
			return;
		}
	}

	// パッド入力
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	// 入力
	if (pInputPad->GetTrigger(CInputPad::BUTTON_A, m_nId))
	{
		if (m_action != ACTION_ATK)
		{// 攻撃中ではない
			m_action = ACTION_ATK;
		}
	}

	// 攻撃判定
	if (m_pBody == nullptr){	// 胴体がない
		return;
	}

	if (m_pBody->GetMotion() == nullptr){	// モーションがない
		return;
	}

	if (m_pBody->GetMotion()->GetNowMotion() == ACTION_ATK &&
		m_pBody->GetMotion()->GetNowKey() == m_pBody->GetMotion()->GetNowNumKey() - 3)
	{// 攻撃判定中
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_ATTACK);
	}

	if (m_pBody->GetMotion()->GetNowMotion() == ACTION_ATK && 
		m_pBody->GetMotion()->GetNowKey() == m_pBody->GetMotion()->GetNowNumKey() - 2)
	{// 攻撃判定中
		CModel *pModel = m_pBody->GetParts(m_pBody->GetNumParts() - HAND_PARTS);	// パーツ
		D3DXVECTOR3 pos = D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43);
		DamageCollision(pos);

		// 当たり判定確認
		AttackCheck();
	}
}

//===============================================
// 物を持つ
//===============================================
void CPlayer::Catch(void)
{
	if (m_action < ACTION_NEUTRAL || m_action > ACTION_WALK || m_Info.state == STATE_CATCH)
	{// 待機か移動中
		if (m_action != ACTION_CATCH && (m_Catch.pPlayer == nullptr && m_Catch.pGimmick == nullptr))
		{
			return;
		}
	}

	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	// 入力
	if (pInputPad->GetTrigger(CInputPad::BUTTON_X, m_nId))
	{
		if (m_action != ACTION_CATCH)
		{
			m_action = ACTION_CATCH;
		}
	}

	// 持った対象の判定
	if (m_Catch.pPlayer != nullptr && m_Info.state != STATE_CATCH) {	// 他のプレイヤーを持っている
		if (m_Catch.pPlayer->m_Info.state != STATE_CATCH) {	// 相手の状態が変わった場合
			m_Catch.pPlayer->m_Catch.pPlayer = nullptr;
			m_Catch.pPlayer = nullptr;

		}
		else
		{
			m_fRotDest = m_fRotMove;
			if (m_Catch.pPlayer->m_bMove) {	// 相手が移動
				m_Catch.nMoveCnt++;	// カウントアップ
				if (m_Catch.pPlayer->m_bJump)
				{
					m_Info.pos.x += m_Catch.pPlayer->m_Info.move.x * 1.5f;	// 相手の移動量に引っ張られる
					m_Info.pos.z += m_Catch.pPlayer->m_Info.move.z * 1.5f;	// 相手の移動量に引っ張られる
				}
				else
				{
					m_Info.pos.x += m_Catch.pPlayer->m_Info.move.x * 0.75f;	// 相手の移動量に引っ張られる
					m_Info.pos.z += m_Catch.pPlayer->m_Info.move.z * 0.75f;	// 相手の移動量に引っ張られる
				}

				m_Catch.pPlayer->m_Info.pos.y += m_Catch.pPlayer->m_Info.move.y;

				if (m_Catch.pPlayer->m_Info.pos.y < m_Info.pos.y)
				{
					m_Catch.pPlayer->m_Info.pos.y = m_Info.pos.y;
					m_Catch.pPlayer->m_Info.move.y = 0.0f;
				}
			}

			if (m_Catch.nMoveCnt >= CATCH_LIMIT							// カウント限界値
				|| pInputPad->GetTrigger(CInputPad::BUTTON_X, m_nId)) {	// キー入力
				m_Catch.pPlayer->m_Info.state = STATE_NORMAL;
				m_Catch.pPlayer->m_Catch.pPlayer = nullptr;
				m_Catch.pPlayer = nullptr;
				
			}
		}
	}
	else if(m_Catch.pGimmick != nullptr) {	// ギミックを持っている
		CGimmickRotateDoor *pDoor = m_Catch.pGimmick->GetRotateDoor();

		if (pDoor != nullptr) {	// ドアを持っている

			if (pDoor->GetModel() == nullptr) {	// ドアのモデルが使用されていない
				GimmickRelease();	// ギミックを離す
			}

			if (pDoor->GetModel()->GetCurrentRotation().y == 0.0f) {	// 回転が止まっている
				GimmickRelease();	// ギミックを離す
			}
		}
		else
		{
			if(pInputPad->GetTrigger(CInputPad::BUTTON_X, m_nId)) {	// キー入力
				m_Info.state = STATE_NORMAL;
				m_Catch.pGimmick->SetMtxParent(nullptr);
				m_Catch.pGimmick = nullptr;
			}
		}
	}
}

//===============================================
// 所持しているギミックを外す
//===============================================
void CPlayer::GimmickRelease(void)
{
	m_Info.state = STATE_NORMAL;
	m_Catch.pGimmick = nullptr;
}

//===============================================
// アイテムのファイル設定
//===============================================
const char *CPlayer::ItemFileName(int type)
{
	char m_aString[64] = "\n";

	switch (type)
	{
	
	case CItem::TYPE_COIN:
	{
		return  "data\\MODEL\\coin.x";
	}

	break;

	case CItem::TYPE_BRECELET:
	{
		return   "data\\MODEL\\bracelet00.x";
	}

	break;

	case CItem::TYPE_CUP:
	{
		return  "data\\MODEL\\cup00.x";
	}

	break;

	case CItem::TYPE_GEM00:
	{
		return  "data\\MODEL\\gem00.x";
	}

	break;

	case CItem::TYPE_GEM01:
	{
		return  "data\\MODEL\\gem01.x";
	}

	break;

	case CItem::TYPE_GOLDBAR:
	{
		return  "data\\MODEL\\goldbar00.x";
	}

	break;

	case CItem::TYPE_JAR:
	{
		return  "data\\MODEL\\jar001.x";
	}

	break;

	case CItem::TYPE_KUNAI:
	{
		return  "data\\MODEL\\kunai.x";
	}

	break;

	case CItem::TYPE_RING00:
	{
		return  "data\\MODEL\\ring00.x";
	}

	break;

	case CItem::TYPE_SCROLL:
	{
		return  "data\\MODEL\\scroll00.x";
	}

	break;

	case CItem::TYPE_SHURIKEN:
	{
		return  "data\\MODEL\\shuriken.x";
	}

	break;

	case CItem::TYPE_MAX:
	{
		return  "";
	}

	break;

	}

	return  "";
}

//===============================================
// アイテムのソート
//===============================================
void CPlayer::ItemSort(void)
{
	for (int nCount = 0; nCount < MAX_ITEM - 1; nCount++)
	{
		for (int nCntRank = 1 + nCount; nCntRank < MAX_ITEM; nCntRank++)
		{
			//大きかったら入れ替え
			if (m_aSaveType[nCount] == 0)
			{//入れ替え
				m_aSaveType[nCount] = 0;
				int nTmp = m_aSaveType[nCntRank];
				m_aSaveType[nCntRank] = m_aSaveType[nCount];
				m_aSaveType[nCount] = nTmp;
			}
		}
	}
}

//===============================================
// それぞれのアイテム加算
//===============================================
void CPlayer::AddItemCount(int type)
{
	switch (type)
	{
	
	case CItem::TYPE_COIN:  // コイン
	{
		m_nNumItemCoin++;
	}

	break;

	case CItem::TYPE_BRECELET:  // ブレスレット
	{
		m_nNumItemBrecetet++;
	}

	break;

	case CItem::TYPE_CUP:       // 盃
	{
		m_nNumItemCup++;
	}

	break;

	case CItem::TYPE_GEM00:     // エメラルド
	{
		m_nNumItemEmerald++;
	}

	break;

	case CItem::TYPE_GEM01:     // ダイヤモンド
	{
		m_nNumItemDiamond++;
	}

	break;

	case CItem::TYPE_GOLDBAR:   // 金塊
	{
		m_nNumItemGold++;
	}

	break;

	case CItem::TYPE_JAR:       // 瓶
	{
		m_nNumItemJar++;
	}

	break;

	case CItem::TYPE_KUNAI:     // クナイ
	{
		m_nNumItemKunai++;
	}

	break;

	case CItem::TYPE_RING00:    // リング
	{
		m_nNumItemRing++;
	}

	break;

	case CItem::TYPE_SCROLL:    // 巻物
	{
		m_nNumItemScroll++;
	}

	break;

	case CItem::TYPE_SHURIKEN:  // 手裏剣
	{
		m_nNumItemShuriken++;
	}

	break;

	case CItem::TYPE_MAX:
	{
		
	}

	break;

	}
}

//===============================================
// それぞれのアイテム減算
//===============================================
void CPlayer::SubItemCount(int type)
{
	switch (type)
	{
	
	case CItem::TYPE_COIN:  // コイン
	{
		m_nNumItemCoin--;
	}

	break;

	case CItem::TYPE_BRECELET:  // ブレスレット
	{
		m_nNumItemBrecetet--;
	}

	break;

	case CItem::TYPE_CUP:       // 盃
	{
		m_nNumItemCup--;
	}

	break;

	case CItem::TYPE_GEM00:     // エメラルド
	{
		m_nNumItemEmerald--;
	}

	break;

	case CItem::TYPE_GEM01:     // ダイヤモンド
	{
		m_nNumItemDiamond--;
	}

	break;

	case CItem::TYPE_GOLDBAR:   // 金塊
	{
		m_nNumItemGold--;
	}

	break;

	case CItem::TYPE_JAR:       // 瓶
	{
		m_nNumItemJar--;
	}

	break;

	case CItem::TYPE_KUNAI:     // クナイ
	{
		m_nNumItemKunai--;
	}

	break;

	case CItem::TYPE_RING00:    // リング
	{
		m_nNumItemRing--;
	}

	break;

	case CItem::TYPE_SCROLL:    // 巻物
	{
		m_nNumItemScroll--;
	}

	break;

	case CItem::TYPE_SHURIKEN:  // 手裏剣
	{
		m_nNumItemShuriken--;
	}

	break;

	case CItem::TYPE_MAX:
	{
		
	}

	break;

	}
}

//===============================================
// 捨てるアイテム選択
//===============================================
void CPlayer::SelectItem(void)
{
	// ゲームパッドの情報を取得
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	if (pInputPad->GetTrigger(CInputPad::BUTTON_RIGHT, m_nId))
	{
		m_nItemId++;
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT);
	}

	if (pInputPad->GetTrigger(CInputPad::BUTTON_LEFT, m_nId))
	{
		m_nItemId--;
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT);
	}

	if (m_nItemId > 10)
	{
		m_nItemId = 0;
	}
	else if(m_nItemId < 0)
	{
		m_nItemId = 10;
	}

	if (pInputPad->GetTrigger(CInputPad::BUTTON_UP, m_nId))
	{
		if (m_nItemCnt > 0 && GetSelectItem(m_nItemId) > 0)
		{
			CItem *pItem = CItem::Create(m_Info.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_nItemId, CItem::STATE_DROP);

			if (m_pScore != nullptr)
			{
				// スコアへらすう
				m_pScore->LowerScore(pItem->GetEachScore());
			}

			m_aSaveType[m_nItemId] = 0;

			if (nullptr != pItem)
			{
				D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				//移動量の設定
				move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.08f;
				move.y = 18.0f;
				move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.08f;
				pItem->SetMove(move);
			}

			m_nItemCnt--;

			SubItemCount(m_nItemId);

			ItemSort();
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DROP);
		}
	}
}

//===============================================
// それぞれのアイテムの総数取得
//===============================================
int CPlayer::GetSelectItem(int type)
{
	switch (type)
	{
	
	case CItem::TYPE_COIN:  // コイン
	{
		return m_nNumItemCoin;
	}

	break;

	case CItem::TYPE_BRECELET:  // ブレスレット
	{
		return m_nNumItemBrecetet;
	}

	break;

	case CItem::TYPE_CUP:       // 盃
	{
		return m_nNumItemCup;
	}

	break;

	case CItem::TYPE_GEM00:     // エメラルド
	{
		return m_nNumItemEmerald;
	}

	break;

	case CItem::TYPE_GEM01:     // ダイヤモンド
	{
		return m_nNumItemDiamond;
	}

	break;

	case CItem::TYPE_GOLDBAR:   // 金塊
	{
		return m_nNumItemGold;
	}

	break;

	case CItem::TYPE_JAR:       // 瓶
	{
		return m_nNumItemJar;
	}

	break;

	case CItem::TYPE_KUNAI:     // クナイ
	{
		return m_nNumItemKunai;
	}

	break;

	case CItem::TYPE_RING00:    // リング
	{
		return m_nNumItemRing;
	}

	break;

	case CItem::TYPE_SCROLL:    // 巻物
	{
		return m_nNumItemScroll;
	}

	break;

	case CItem::TYPE_SHURIKEN:  // 手裏剣
	{
		return m_nNumItemShuriken;
	}

	break;

	case CItem::TYPE_MAX:
	{
		return 0;
	}

	break;

	}

	return 0;
}

//===============================================
// 物を投げる
//===============================================
void CPlayer::Throw(void)
{
	if (m_action != ACTION_HOLD)
	{// 持っていない
		return;
	}

	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	// 入力
	if (pInputPad->GetTrigger(CInputPad::BUTTON_X, m_nId))
	{
		m_action = ACTION_THROW;
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
// プレイヤー同士の攻撃判定
//===============================================
void CPlayer::DamageCollision(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer = GetTop();	// 先頭を受け取る
	CXFile *pFile = CManager::GetInstance()->GetModelFile();

	while (pPlayer != nullptr)
	{
		CPlayer *pPlayerNext = pPlayer->GetNext();	// 次を保持

		if (pPlayer == this)
		{// 自分自身か相手が通常状態以外
			pPlayer = pPlayerNext;
			continue;
		}

		D3DXVECTOR3 ObjPos = pPlayer->GetPosition();	// 座標
		D3DXVECTOR3 HeadPos = pPlayer->m_pBody->GetParts(1)->GetPosition();
		D3DXVECTOR3 HeadMax = pFile->GetMax(pPlayer->m_pBody->GetParts(1)->GetId());
		D3DXVECTOR3 vtxMax = pFile->GetMax(pPlayer->m_pBody->GetParts(0)->GetId());
		D3DXVECTOR3 vtxMin = pFile->GetMin(pPlayer->m_pBody->GetParts(0)->GetId());

		if (pos.x + -ATK_RANGE <= ObjPos.x + vtxMax.x && pos.x + ATK_RANGE >= ObjPos.x + vtxMin.x
			&& pos.z + -ATK_RANGE <= ObjPos.z + vtxMax.z && pos.z + ATK_RANGE >= ObjPos.z + vtxMin.z)
		{// 左右判定内
			if (pos.y >= ObjPos.y && pos.y <= ObjPos.y + HeadPos.y + HeadMax.y)
			{// 高さ判定内
				pPlayer->Damage(1);

			}
		}

		pPlayer = pPlayerNext;
	}
}

//===============================================
// アイテムを落とす
//===============================================
void CPlayer::Drop(int nDropCnt)
{
	int nOldCnt = m_nItemCnt;

	m_nItemCnt -= nDropCnt;

	if (m_nItemCnt < 0) {	// 所持数が0を下回った
		m_nItemCnt = 0;
	}

	int nDiff = nOldCnt - m_nItemCnt;	// 減少量

	// 落とした分生成
	for (int nCnt = 0; nCnt < nDiff; nCnt++)
	{
		CItem *pItem = CItem::Create(m_Info.pos, D3DXVECTOR3(0.0f, 0.0f ,0.0f), m_aSaveType[nCnt], CItem::STATE_DROP);

		if (m_pScore != nullptr)
		{
			// スコアへらすう
			m_pScore->LowerScore(pItem->GetEachScore());
		}

		SubItemCount(m_aSaveType[nCnt]);

		m_aSaveType[nCnt] = 0;

		if (nullptr != pItem)
		{
			D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.08f;
			move.y = 18.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.08f;
			pItem->SetMove(move);
		}
	}

	ItemSort();
}

//===============================================
// アイテム全部落とす
//===============================================
void CPlayer::DropAll(void)
{
	// 落とした分生成
	for (int nCnt = 0; nCnt < m_nItemCnt; nCnt++)
	{
		char aString[258] = "\n";

		CItem *pItem = CItem::Create(m_Info.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_aSaveType[nCnt], CItem::STATE_DROP);

		if (m_pScore != nullptr)
		{
			// スコアへらすう
			m_pScore->LowerScore(pItem->GetEachScore());
		}

		m_aSaveType[nCnt] = 0;

		if (nullptr != pItem)
		{
			D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.08f;
			move.y = 18.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.08f;
			pItem->SetMove(move);
		}
	}

	//各アイテム数を0にする
	m_nNumItemCoin = 0;
	m_nNumItemBrecetet = 0;
	m_nNumItemCup = 0;
	m_nNumItemEmerald = 0;
	m_nNumItemDiamond = 0;
	m_nNumItemGold = 0;
	m_nNumItemJar = 0;
	m_nNumItemKunai = 0;
	m_nNumItemRing = 0;
	m_nNumItemScroll = 0;
	m_nNumItemShuriken = 0;

	// 所持しているアイテムの総数をゼロにする
	m_nItemCnt = 0;
}

//===============================================
// プレイヤーを掴む
//===============================================
void CPlayer::PlayerCatch(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer = GetTop();	// 先頭を受け取る
	CXFile *pFile = CManager::GetInstance()->GetModelFile();

	while (pPlayer != nullptr)
	{
		CPlayer *pPlayerNext = pPlayer->GetNext();	// 次を保持

		if (pPlayer == this)
		{// 自分自身か相手が通常状態以外
			pPlayer = pPlayerNext;
			continue;
		}

		D3DXVECTOR3 ObjPos = pPlayer->GetPosition();	// 座標
		D3DXVECTOR3 HeadPos = pPlayer->m_pBody->GetParts(1)->GetPosition();
		D3DXVECTOR3 HeadMax = pFile->GetMax(pPlayer->m_pBody->GetParts(1)->GetId());
		D3DXVECTOR3 vtxMax = pFile->GetMax(pPlayer->m_pBody->GetParts(0)->GetId());
		D3DXVECTOR3 vtxMin = pFile->GetMin(pPlayer->m_pBody->GetParts(0)->GetId());

		if (pos.x + -CATCH_RANGE <= ObjPos.x + vtxMax.x && pos.x + CATCH_RANGE >= ObjPos.x + vtxMin.x
			&& pos.z + -CATCH_RANGE <= ObjPos.z + vtxMax.z && pos.z + CATCH_RANGE >= ObjPos.z + vtxMin.z)
		{// 左右判定内
			if (pos.y >= ObjPos.y && pos.y <= ObjPos.y + HeadPos.y + HeadMax.y)
			{// 高さ判定内
				if (pPlayer->m_Info.state == STATE_NORMAL)
				{// 通常状態の場合
					pPlayer->m_Info.state = STATE_CATCH;	// 相手を掴まれている状態に
					pPlayer->m_Catch.pPlayer = this;		// 相手に自分を指定
					m_Catch.pPlayer = pPlayer;				// 自分のポインタに相手を指定

					if (pPlayer->m_Catch.pGimmick != nullptr) {
						pPlayer->m_Catch.pGimmick->SetMtxParent(nullptr);
						pPlayer->m_Catch.pGimmick = nullptr;
					}

					m_Catch.nMoveCnt = 0;
					CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_CATCH);
				}
			}
		}

		pPlayer = pPlayerNext;
	}
}

//===============================================
// プレイヤーのマトリックス
//===============================================
void CPlayer::SetCatchMatrix(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_Info.mtxWorld);

	if (m_Catch.pPlayer != nullptr) {	// プレイヤー
		if (m_Catch.pPlayer->m_Catch.pPlayer == this) {
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, m_Info.pos.y - m_Catch.pPlayer->m_Info.mtxWorld._42, -50.0f);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxTrans);

			// パーツのマトリックスと親のマトリックスをかけ合わせる
			D3DXMatrixMultiply(&m_Info.mtxWorld,
				&m_Info.mtxWorld, &m_Catch.pPlayer->m_Info.mtxWorld);

			m_Info.pos = D3DXVECTOR3(m_Info.mtxWorld._41, m_Info.mtxWorld._42, m_Info.mtxWorld._43);

			return;
		}
		else {
			m_Info.state = STATE_NORMAL;
			m_Catch.pPlayer = nullptr;
		}
	}
	else if (m_Catch.pGimmick != nullptr) { // ギミック

		D3DXVECTOR3 pos = m_Catch.SetPos;
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		CGimmickRotateDoor *pDoor = m_Catch.pGimmick->GetRotateDoor();
		
		if (pDoor != nullptr) {

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxTrans);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxRot);

			if (pDoor->GetModel() != nullptr) {

				// パーツのマトリックスと親のマトリックスをかけ合わせる
				D3DXMatrixMultiply(&m_Info.mtxWorld,
					&m_Info.mtxWorld, pDoor->GetModel()->GetMtx());

				m_Info.pos = D3DXVECTOR3(m_Info.mtxWorld._41, m_Info.mtxWorld._42, m_Info.mtxWorld._43);
			}

			return;
		}
	}

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxTrans);
}

//===============================================
// 攻撃中の当たり判定確認
//===============================================
void CPlayer::AttackCheck(void)
{
	if (m_pBody == nullptr) {	// 体がない
		return;
	}

	CModel *pModel = m_pBody->GetParts(m_pBody->GetNumParts() - HAND_PARTS);	// 手を取得する

	if (pModel == nullptr) {	// モデルがない
		return;
	}

	D3DXVECTOR3 AtkPos = D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43);	// 攻撃座標
	CEnemy *pEnem = CEnemy::GetTop()->GetTop();	// 敵の先頭を取得
	
	// 敵数分確認
	while (pEnem != nullptr) {
		CEnemy *pEnemNext = pEnem->GetNext();
		pEnem->HitCheck(AtkPos, ATK_RANGE);	// 触れているかチェック
		pEnem = pEnemNext;
	}
}

//===============================================
// 使用階層構造の設定
//===============================================
void CPlayer::BodySet(void)
{
	// 下半身更新
	if (m_pLeg != nullptr)
	{// 使用されている場合

		if (CManager::GetInstance()->GetMode() != CScene::MODE_GAME) {
			m_pLeg->Update();
		}

		// 腰の設定
		if (m_pWaist != nullptr)
		{
			CModel *pModel = m_pLeg->GetParts(0);

			// 腰の高さを補填
			m_pWaist->SetPosition(m_pWaist->GetSetPosition() + pModel->GetCurrentPosition());
			m_pWaist->SetMatrix();
		}

		if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME) {
			m_pLeg->Update();
		}
	}

	// 胴体更新
	if (m_pBody != nullptr)
	{// 使用されている場合
		m_pBody->Update();
	}
}

//===============================================
// 忍術設定
//===============================================
void CPlayer::Ninjutsu(void)
{
	if (m_action < ACTION_NEUTRAL || m_action > ACTION_JUMP || m_Info.state == STATE_CATCH)
	{// 待機か移動中
		if (m_action < ACTION_HENGE) {
			return;
		}
	}

	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	// 入力装置確認
	if (nullptr == pInputPad) {
		return;
	}

	if (m_nId < 0 || m_nId >= PLAYER_MAX)
	{// コントローラー数オーバー
		return;
	}

	if (pInputPad->GetPress(CInputPad::BUTTON_LEFTBUTTON, m_nId)) {	// 変化

		if (m_fGage >= KAKUREMI_GAGE)
		{
			if (m_action != ACTION_HENGE)
			{
				m_action = ACTION_HENGE;

				CModel *pModel = m_pLeg->GetParts(0);  // 腰のパーツ

				// 煙のパーティクル生成
				CParticle::Create(D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43), CEffect::TYPE_SMAKE);

				ChangeBody();
			}

			m_fGage -= KAKUREMI_GAGE;
			m_action = ACTION_HENGE;

			if (m_fGage <= 0.0f)
			{
				if (m_action == ACTION_HENGE) {
					m_action = ACTION_NEUTRAL;

					CModel *pModel = m_pLeg->GetParts(0);  // 腰のパーツ

					// 煙のパーティクル生成
					CParticle::Create(D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43), CEffect::TYPE_SMAKE);
					ChangeBody();
				}
			}
		}
	}
	else if (pInputPad->GetTrigger(CInputPad::BUTTON_RIGHTBUTTON, m_nId)) {	// クナイ
		if (m_fGage >= KUNAI_GAGE && m_action != ACTION_KUNAI)
		{
			m_fGage -= KUNAI_GAGE;
			m_action = ACTION_KUNAI;
		}
	}
	else if (pInputPad->GetTrigger(CInputPad::BUTTON_Y, m_nId)) {	// 風神
		if (m_fGage >= AIR_GAGE && m_action != ACTION_AIR && !m_bJump) {	// ゲージが足りる
			m_fGage -= AIR_GAGE;
			m_action = ACTION_AIR;
		}
	}
	else
	{
		if (m_action == ACTION_HENGE) {	// 変化だった
			m_action = ACTION_NEUTRAL;

			CModel *pModel = m_pLeg->GetParts(0);  // 腰のパーツ

			// 煙のパーティクル生成
			CParticle::Create(D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43), CEffect::TYPE_SMAKE);
			ChangeBody();
		}

		if (m_action < ACTION_HENGE) {	// 術を使用していない

			if (m_fGage < MAX_GAGE) {
				m_fGage += CManager::GetInstance()->GetSlow()->Get();

				if (m_fGage > MAX_GAGE) {
					m_fGage = MAX_GAGE;
				}
			}
		}
	}

	if (m_fGage < 0.0f)
	{
		m_fGage = 0.0f;
	}
}

//===============================================
// 見た目変更
//===============================================
void CPlayer::ChangeBody(void)
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

	char aBodyPass[256] = "";
	char aLegPass[256] = "";

	if (m_action == ACTION_HENGE) {
		sprintf(&aBodyPass[0], "data\\TXT\\playerkakuremi\\motion_ninjabody.txt");
		sprintf(&aLegPass[0], "data\\TXT\\playerkakuremi\\motion_ninjaleg.txt");	
	}
	else
	{
		sprintf(&aBodyPass[0], "data\\TXT\\player%d\\motion_ninjabody.txt", m_nId);
		sprintf(&aLegPass[0], "data\\TXT\\player%d\\motion_ninjaleg.txt", m_nId);
	}

	// 胴体の設定
	m_pBody = CCharacter::Create(&aBodyPass[0]);
	m_pBody->SetParent(m_pWaist->GetMtxWorld());
	m_pBody->SetShadow(true);
	m_pBody->SetDraw();

	if (m_pBody->GetMotion() != nullptr)
	{
		// 初期モーションの設定
		m_pBody->GetMotion()->InitSet(ACTION_WALK);
	}

	// 下半身の設定
	m_pLeg = CCharacter::Create(&aLegPass[0]);
	m_pLeg->SetParent(m_pWaist->GetMtxWorld());
	m_pLeg->SetShadow(true);
	m_pLeg->SetDraw();

	// 腰の高さを合わせる
	if (m_pLeg != nullptr)
	{// 脚が使用されている場合
		CModel *pModel = m_pLeg->GetParts(0);	// 腰パーツを取得

		if (pModel != nullptr)
		{// パーツが存在する場合
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// モデルの相対位置を取得

			// 腰のモデルの位置を変更
			pModel->SetPosition(pos);

			if (m_pLeg->GetMotion() != nullptr)
			{
				// 初期モーションの設定
				m_pLeg->GetMotion()->InitSet(ACTION_WALK);
			}
		}
	}

	SetMatrix();
	BodySet();
}

//===============================================
// 弾の設定
//===============================================
void CPlayer::BulletSet(void)
{
	if (m_pBody == nullptr) {	// 体がない
		return;
	}

	if (m_pBody->GetParts(m_pBody->GetNumParts() - HAND_PARTS) == nullptr) {	// 手がない
		return;
	}

	CModel *pModel = m_pBody->GetParts(m_pBody->GetNumParts() - HAND_PARTS);	//　手のパーツ

	D3DXVECTOR3 pos = D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43);
	D3DXVECTOR3 move = D3DXVECTOR3(-sinf(m_Info.rot.y) * BULLET_MOVE, 0.0f, -cosf(m_Info.rot.y) * BULLET_MOVE);
	CBullet *pBullet = CBullet::Create(pos, m_Info.rot, move);
	pBullet->BindId(m_nId);
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

	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f,
		m_pBody->GetParts(1)->GetMtx()->_42 - ObjPos.y + pFile->GetMax(m_pBody->GetParts(1)->GetId()).y,
		0.0f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, -10.0f, 0.0f);

	if (pos.y >= ObjPos.y + vtxMax.y || pos.y <= ObjPos.y - vtxMin.y) {	// 高さ範囲外
		return m_bValue;
	}

	// 範囲内チェック
	float fLength =
		sqrtf((pos.x - ObjPos.x) * (pos.x - ObjPos.x)
			+ (pos.z - ObjPos.z) * (pos.z - ObjPos.z));

	if (fLength > HIT_RANGE + fRange) {		// 範囲外
		return m_bValue;
	}

	m_bValue = true;
	Damage(nDamage);

	return m_bValue;
}

//===============================================
// 吹き飛ばされる
//===============================================
void CPlayer::Blow(void) {
	m_Info.fStateCounter = DAMAGE_APPEAR;
	m_Info.state = STATE_BLOW;

	if (m_Catch.pGimmick != nullptr) {
		m_Catch.pGimmick->SetMtxParent(nullptr);
		m_Catch.pGimmick = nullptr;
	}
}

//===============================================
// 吹き飛ばされる
//===============================================
void CPlayer::BindUI(CUI *pUI) 
{ 
	m_pUI = pUI; m_pUI->SetLife(m_nLife); 
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
	if (m_pBody == nullptr) {	// 体がない
		return;
	}

	if (m_pBody->GetMotion() == nullptr) {	// モーションが無い
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
// ゴール後待機状態
//===============================================
void CPlayer::GoalWait(void)
{
	if (m_nQuitCounter >= GOAL_WAITTIME) {	// カウンター規定値の場合

		// フェードの色を濃くする
		if (m_pFade != nullptr) {
			D3DXCOLOR col = m_pFade->GetCol();
			if (col.a < 1.0f) {
				col.a += 0.01f;
			}
			m_pFade->SetCol(col);
			m_pFade->SetDraw(true);
		}

		// フェードの色を濃くする
		if (m_pEscape != nullptr) {
			D3DXCOLOR col = m_pEscape->GetCol();
			if (col.a < 1.0f) {
				col.a += 0.01f;
			}
			m_pEscape->SetCol(col);
			m_pEscape->SetDraw(true);
		}
		return;
	}

	// カウンター増加
	m_nQuitCounter++;

	if (m_nQuitCounter < GOAL_QUITTIME) {	// 退散タイミング

		// 自動移動
		float fSpeed = MOVE;	// 移動量
		m_Info.move.x = -sinf(m_Info.rot.y) * fSpeed;
		m_Info.move.z = -cosf(m_Info.rot.y) * fSpeed;

		float fGravity = GRAVITY * CManager::GetInstance()->GetSlow()->Get();
		m_Info.move.y += fGravity;
		m_Info.pos.y += m_Info.move.y * CManager::GetInstance()->GetSlow()->Get();

		m_Info.pos.x += m_Info.move.x;
		m_Info.pos.z += m_Info.move.z;

		// メッシュフィールドとの判定
		{
			float fHeight = CMeshField::GetHeight(m_Info.pos);
			if (m_Info.pos.y < fHeight)
			{
				m_Info.pos.y = fHeight;
				m_Info.move.y = 0.0f;
				m_bJump = false;
			}
		}

		// モーションを歩きに変更
		if (m_pBody != nullptr) {
			m_pBody->GetMotion()->Set(ACTION_WALK);
		}
		if (m_pLeg != nullptr) {
			m_pLeg->GetMotion()->Set(ACTION_WALK);
		}
	}
	else if (m_nQuitCounter == GOAL_QUITTIME) {	// 退散タイミング
		// モーションを術の動きに変更
		if (m_pBody != nullptr) {
			m_pBody->GetMotion()->Set(ACTION_HENGE);
		}
		if (m_pLeg != nullptr) {
			m_pLeg->GetMotion()->Set(ACTION_HENGE);
		}
	}

	// カメラの向きを補正
	if (m_pMyCamera != nullptr) {
		// カメラを目標の向きまで回転させる
		D3DXVECTOR3 rotDest = GOAL_CAMERAROT - m_pMyCamera->GetRotation();
		m_pMyCamera->SetRotation(m_pMyCamera->GetRotation() + rotDest * 0.05f);
	}

	if (m_nQuitCounter >= GOAL_WAITTIME) {	// カウンター規定値の場合
		if (m_pBody != nullptr) {
			m_pBody->SetDraw(false);
		}
		if (m_pLeg != nullptr) {
			m_pLeg->SetDraw(false);
		}

		CModel *pModel = m_pLeg->GetParts(0);  // 腰のパーツ

		// 煙のパーティクル生成
		CParticle::Create(D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43), CEffect::TYPE_SMAKE);
		m_bEnd = true;
	}
}

//===============================================
// カメラ設定
//===============================================
void CPlayer::SetCamera(CCamera *pCamera) {

	m_pMyCamera = pCamera;

	// フェードの生成
	if (m_pMyCamera != nullptr) {	// 分割されている
		m_pFade = CObject2D::Create(6);
		m_pFade->SetDraw(false);
		m_pEscape = CObject2D::Create(6);
		m_pEscape->SetDraw(false);
		m_pEscape->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\escape000.png"));
	}

	if (m_pFade != nullptr) {
		m_pFade->SetDraw(false);
		float fPosX = SCREEN_WIDTH * 0.5f;
		float fPosY = SCREEN_HEIGHT * 0.5f;
		float fWidth = SCREEN_WIDTH * 0.5f;
		float fHeight = SCREEN_HEIGHT * 0.5f;

		// 座標設定
		if (m_nNumCount != 1) {	// 1人以外
			fPosX = (m_nId % 2) * SCREEN_WIDTH * 0.5f + SCREEN_WIDTH * 0.25f;
			fWidth = SCREEN_WIDTH * 0.25f;

			if (m_nNumCount >= 3) {
				fHeight = SCREEN_HEIGHT * 0.25f;
				fPosY = (m_nId / 2) * SCREEN_HEIGHT * 0.5f + SCREEN_HEIGHT * 0.25f;
			}
		}

		// ポリゴンのサイズ調整
		m_pFade->SetPosition(D3DXVECTOR3(fPosX, fPosY, 0.0f));
		m_pFade->SetSize(fWidth, fHeight);
		m_pFade->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		if (m_pEscape != nullptr) {
			m_pEscape->SetDraw(false);
			m_pEscape->SetPosition(D3DXVECTOR3(fPosX, fPosY, 0.0f));
			m_pEscape->SetSize(fWidth * 0.5f, fHeight * ((m_nNumCount == 2) ? 0.25f : 0.5f));
			m_pEscape->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}
}

//===============================================
// フェードの色変更
//===============================================
void CPlayer::SetWarpFadeCol(void)
{
	if (!m_bWarp) {	// warpしていない
		return;
	}

	if (m_pFade == nullptr) {	// フェードがない
		return;
	}


	m_pFade->SetDraw(true);
	D3DXCOLOR col = m_pFade->GetCol();

	col.a += m_fFadeCola;

	if (col.a <= 0.0f) {
		col.a = 0.0f;
		m_fFadeCola = FADE_COLA;
		m_bWarp = false;
		m_pFade->SetDraw(false);
	}
	else if (col.a >= 1.5f) {
		m_fFadeCola = -FADE_COLA * 0.75f;
		col.a = 1.5f;
	}

	m_pFade->SetCol(col);
}