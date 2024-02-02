//==========================================================
//
// ボス敵 [enemy_boss.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "enemy_boss.h"
#include "waist.h"
#include "character.h"
#include "mesh.h"
#include "model.h"
#include "motion.h"
#include "player_manager.h"
#include "meshwall.h"
#include "objectX.h"
#include "player.h"
#include "debugproc.h"
#include "manager.h"
#include "slow.h"
#include "meshfield.h"
#include "bullet.h"
#include "input.h"
#include "Xfile.h"
#include "bridge.h"

// 無名名前空間
namespace
{
	const char* BODYFILEPASS = "data\\TXT\\enemy_boss\\motion_body.txt";	// ファイルのパス
	const char* LEGFILEPASS = "data\\TXT\\enemy_boss\\motion_leg.txt";	// ファイルのパス
	const char* ARMFILEPASS[CEnemyBoss::PARTS_MAX] = {
		"data\\TXT\\enemy_boss\\motion_leftarm.txt",
		"data\\TXT\\enemy_boss\\motion_rightarm.txt",
	};
	const D3DXVECTOR3 COLLIMAX = { 20.0f, 120.0f, 20.0f };	// 当たり判定最大
	const D3DXVECTOR3 COLLIMIN = { -20.0f, 0.0f, -20.0f };	// 当たり判定最小
	const int DAMAGEINTERVAL = (60);	// ダメージインターバル
	const float CHASE_MAXLENGTH = (1000.0f);	// 追跡最長距離
	const float CHASE_NEARLENGTH = (700.0f);	// 追跡近距離
	const float CHASE_MINLENGTH = (400.0f);		// 追跡0距離
	const float SEARCH_HEIGHT = (180.0f);		// 探索高さ制限
	const float MOVE_INER = (0.3f);			// 移動慣性
	const int ATK_RANDRANGE = (6);			// 攻撃中ランダム範囲
	const int BACK_RANDRANGE = (2);
	const int FRONT_RANDRANGE = (3);
	const float ROTDIFF_INER = (0.05f);
}

// 移動速度名前空間
namespace SPEED
{
	const float MOVE_FAR = (2.0f);	// 遠距離移動
	const float MOVE_NEAR = (-1.0f);	// 近距離移動
	const float MOVE_MIN = (0.15f);	// 移動量移動
	const float GRAVITY = (-0.9f);	// 重力
	const float DAMAGE_MOVE = (2.0f);	// 移動量
	const float BULLET = (-6.0f);	// 弾速
}

// インターバル
namespace INTERVAL
{
	const float DAMAGE = (20.0f);	// ダメージ
	const float ATTACK = (500.0f);	// 攻撃
}

//==========================================================
// コンストラクタ
//==========================================================
CEnemyBoss::CEnemyBoss()
{
	// 値のクリア
	m_nAction = ACTION_NEUTRAL;
	m_pBody = nullptr;
	m_pLeg = nullptr;
	m_pWaist = nullptr;
	m_Chase.pTarget = nullptr;
	m_pBridge = nullptr;
	m_Chase.fLength = 0.0f;
	m_fInterVal = 0;
	m_StateInfo.state = STATE_APPEAR;
	m_StateInfo.fCounter = 0.0f;
	m_fAtkCnter = 0.0f;
	m_nArmAction = 0;

	for (int i = 0; i < PARTS_MAX; i++) {
		m_apArm[i] = nullptr;
	}
}

//==========================================================
// デストラクタ
//==========================================================
CEnemyBoss::~CEnemyBoss()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CEnemyBoss::Init(void)
{
	// 種類をボスに設定
	SetType(TYPE_BOSS);

	SInfo* pInfo = GetInfo();

	// 腰の生成
	if (m_pWaist == nullptr)
	{
		m_pWaist = new CWaist;
		m_pWaist->SetParent(&pInfo->mtxWorld);
		m_pWaist->SetMatrix();
	}

	// 胴体の設定
	m_pBody = CCharacter::Create(BODYFILEPASS);
	m_pBody->SetParent(m_pWaist->GetMtxWorld());
	m_pBody->SetShadow(true);
	m_pBody->SetDraw();

	if (m_pBody->GetMotion() != nullptr)
	{
		// 初期モーションの設定
		m_pBody->GetMotion()->InitSet(m_nAction);
	}

	// 下半身の設定
	m_pLeg = CCharacter::Create(LEGFILEPASS);
	m_pLeg->SetParent(m_pWaist->GetMtxWorld());
	m_pLeg->SetShadow(true);
	m_pLeg->SetDraw();

	if (m_pLeg->GetMotion() != nullptr)
	{
		// 初期モーションの設定
		m_pLeg->GetMotion()->InitSet(m_nAction);
	}

	// 追加腕の設定
	for (int i = 0; i < PARTS_MAX; i++) {
		m_apArm[i] = CCharacter::Create(ARMFILEPASS[i]);
		m_apArm[i]->SetParent(m_pWaist->GetMtxWorld());
		m_apArm[i]->SetShadow(true);
		m_apArm[i]->SetDraw();

		if (m_apArm[i]->GetMotion() != nullptr)
		{
			// 初期モーションの設定
			m_apArm[i]->GetMotion()->InitSet(ARM_NEUTRAL);
		}
	}

	m_nArmAction = ARM_NEUTRAL;
	m_NowArm = PARTS_LEFTARM;

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

	// 初回ヒット判定対策
	m_fInterVal = 1;
	SetRotMulti(ROTDIFF_INER);

	// 動作する橋を生成
	m_pBridge = CBridge::Create(D3DXVECTOR3(-2700.0f, 1010.0f, 3750.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CEnemyBoss::Uninit(void)
{
	// 追加腕の設定
	for (int i = 0; i < PARTS_MAX; i++) {
		if (m_apArm[i] != nullptr){
			m_apArm[i]->Uninit();
			delete m_apArm[i];
			m_apArm[i] = nullptr;
		}
	}

	if (m_pBody != nullptr) {
		m_pBody->Uninit();
		delete m_pBody;
		m_pBody = nullptr;
	}

	if (m_pLeg != nullptr) {
		m_pLeg->Uninit();
		delete m_pLeg;
		m_pLeg = nullptr;
	}

	if (m_pWaist != nullptr) {
		delete m_pWaist;
		m_pWaist = nullptr;
	}

	if (m_pBridge != nullptr) {
		m_pBridge->SetClose(true);
		m_pBridge = nullptr;
	}

	// 親クラスの終了呼び出し
	CEnemy::Uninit();
}

//==========================================================
// 更新処理
//==========================================================
void CEnemyBoss::Update(void)
{
	// 前回の座標を取得
	{
		SInfo* pInfo = GetInfo();
		pInfo->posOld = pInfo->pos;
	}
	m_fInterVal--;

	// 処理
	MethodLine();

	//マトリックス設定
	CEnemy::Update();
	BodySet();

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_H)) {
		ArmDamage();
	}
}

//===============================================
// 処理の順番
//===============================================
void CEnemyBoss::MethodLine(void)
{
	SInfo* pInfo = GetInfo();

	if (m_StateInfo.state != STATE_DEATH || m_StateInfo.state != STATE_DAMAGE) {

		// 捜索
		m_Chase.pTarget = Search(m_Chase.fLength);

		// 追跡
		LockOn();

		// 移動
		AddMove();

		// 攻撃
		Attack();
	}

	// 重力
	if (m_NowArm >= PARTS_MAX) {
		Gravity();
	}
	else {
		FootCheck();
	}

	// 当たり判定確認
	CObjectX::COLLISION_AXIS axis = CObjectX::TYPE_MAX;
	CMeshWall::Collision(pInfo->pos, pInfo->posOld, pInfo->move, COLLIMAX, COLLIMIN, axis);
	float fHeight = CMeshField::GetHeight(pInfo->pos);
	if (pInfo->pos.y < fHeight && pInfo->posOld.y >= fHeight) {
		pInfo->pos.y = fHeight;
		pInfo->move.y = 0.0f;
	}

	// アクション設定
	SetMotion();

	// 状態管理
	SetState();
}

//==========================================================
// 生成
//==========================================================
CEnemyBoss *CEnemyBoss::Create(D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	CEnemyBoss *pEnemyBoss = nullptr;

	pEnemyBoss = new CEnemyBoss;

	if (pEnemyBoss != nullptr)
	{
		// 初期化処理
		pEnemyBoss->Init();

		// データリセット
		pEnemyBoss->InfoReset();

		// データ設定
		pEnemyBoss->SetPosition(pos);
		pEnemyBoss->SetRotation(rot);
		pEnemyBoss->SetRotDiff(rot.y);
		pEnemyBoss->SetIner(MOVE_INER);
	}

	return pEnemyBoss;
}

//===============================================
// 使用階層構造の設定
//===============================================
void CEnemyBoss::BodySet(void)
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

		m_pLeg->Update();

		CModel* pModel = m_pLeg->GetParts(0);
		pModel->SetCurrentPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// 胴体更新
	if (BodyCheck(m_pBody))
	{// 使用されている場合
		m_pBody->Update();
	}

	// 腕更新
	for (int i = 0; i < PARTS_MAX; i++) {
		if (BodyCheck(m_apArm[i]))
		{
			m_apArm[i]->Update();
		}
	}
}

//===============================================
// 体使用確認
//===============================================
bool CEnemyBoss::BodyCheck(CCharacter* pBody)
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
// 当たり判定確認
//===============================================
bool CEnemyBoss::Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage)
{
	SInfo* pInfo = GetInfo();

	if (m_fInterVal > 0) {
		return false;
	}

	if (m_NowArm < PARTS_MAX) {
		return false;
	}

	if (pos.x + fRange < pInfo->pos.x + COLLIMIN.x ||
		pos.x - fRange > pInfo->pos.x + COLLIMAX.x)
	{// X範囲外
		return false;
	}

	if (pos.z + fRange < pInfo->pos.z + COLLIMIN.z ||
		pos.z - fRange > pInfo->pos.z + COLLIMAX.z)
	{// Z範囲外
		return false;
	}

	if (pos.y + fRange < pInfo->pos.y + COLLIMIN.y ||
		pos.y - fRange > pInfo->pos.y + COLLIMAX.y)
	{// Y範囲外
		return false;
	}

	// ダメージ処理
	Damage(nDamage);
	return true;
}

//===============================================
// ダメージ処理
//===============================================
void CEnemyBoss::Damage(const int nDamage)
{
	if (m_fInterVal > 0) {	// インターバルが戻ってきていない
		return;
	}

	// 状態確認
	if (m_StateInfo.state == STATE_APPEAR || m_StateInfo.state == STATE_DEATH) {	// ダメージを受けない状態
		return;
	}

	// インターバルを変更
	m_fInterVal = DAMAGEINTERVAL;

	// 体力を減らす
	int nLife = GetLife();
	nLife -= nDamage;

	if (nLife <= 0) {	// 体力がなくなった
		m_StateInfo.state = STATE_DEATH;
		m_nAction = ACTION_DEATH;
	}
	else {	// まだある
		SetLife(nLife);
		m_StateInfo.state = STATE_DAMAGE;
		m_StateInfo.fCounter = INTERVAL::DAMAGE;
	}
}

//===============================================
// プレイヤーを探索
//===============================================
CPlayer* CEnemyBoss::Search(float& fChaseLength)
{
	CPlayer* pPlayer = CPlayerManager::GetInstance()->GetTop();
	CPlayer* pChasePlayer = nullptr;
	D3DXVECTOR3 MyPos = GetInfo()->pos;	// 自分の座標
	float fMinLength = CHASE_MAXLENGTH;	// 追跡するプレイヤーとの距離
	fChaseLength = fMinLength;

	// プレイヤー全員分繰り返す
	while (pPlayer != nullptr) {
		CPlayer* pPlayerNext = pPlayer->GetNext();

		D3DXVECTOR3 pos = pPlayer->GetPosition();

		if (pos.y < MyPos.y - SEARCH_HEIGHT || pos.y > MyPos.y + SEARCH_HEIGHT) {	// 高さ感知範囲外
			pPlayer = pPlayerNext;
			continue;
		}

		// 距離を取得
		float fLength = sqrtf((pos.x - MyPos.x) * (pos.x - MyPos.x)
			+ (pos.z - MyPos.z) * (pos.z - MyPos.z));

		// 追跡確認
		if (fLength < fMinLength) {	// 現在見ているプレイヤーの方が近い場合
			fMinLength = fLength;	// 一番近い長さを上書き
			fChaseLength = fLength;
			pChasePlayer = pPlayer;	// 追跡するプレイヤーを設定
		}

		pPlayer = pPlayerNext;
	}

	return pChasePlayer;
}

//===============================================
// チェイス中処理
//===============================================
void CEnemyBoss::LockOn(void)
{
	if (m_Chase.pTarget == nullptr) {
		if (m_StateInfo.state == STATE_LOCKON) {
			m_StateInfo.state = STATE_NORMAL;
		}
		return;
	}

	if (m_StateInfo.state >= STATE_DAMAGE) {
		if (m_StateInfo.state == STATE_LOCKON) {
			m_StateInfo.state = STATE_NORMAL;
		}
		return;
	}

	m_StateInfo.state = STATE_LOCKON;

	CManager::GetInstance()->GetDebugProc()->Print("プレイヤーとの距離 [%f]\n", m_Chase.fLength);

	// 目標向き設定
	D3DXVECTOR3 MyPos = GetInfo()->pos;	// 自分の座標
	D3DXVECTOR3 pos = m_Chase.pTarget->GetPosition();
	{
		float fDiff = GetRotDiff();
		if (m_nArmAction != ARM_ATTACK && m_NowArm < ARM_MAX || m_NowArm >= ARM_MAX) {
			fDiff = atan2f(pos.x - MyPos.x, pos.z - MyPos.z) + D3DX_PI;
			if (fDiff < -D3DX_PI) {
				fDiff += D3DX_PI * 2;
			}
			else if (fDiff > D3DX_PI) {
				fDiff += -D3DX_PI * 2;
			}
		}
		else if (m_nArmAction == ARM_ATTACK && m_NowArm < ARM_MAX) {
			fDiff = 0.0f;
		}

		SetRotDiff(fDiff);
	}
}

//===============================================
// 状態設定
//===============================================
void CEnemyBoss::SetState(void)
{
	m_StateInfo.fCounter -= CManager::GetInstance()->GetSlow()->Get();

	switch (m_StateInfo.state) {
	case STATE_APPEAR:
	{
		if (m_StateInfo.fCounter <= 0.0f) {	// カウンター終了
			m_StateInfo.state = STATE_NORMAL;
		}
	}
		break;

	case STATE_NORMAL:
	{

	}
		break;

	case STATE_DAMAGE:
	{
		if (m_StateInfo.fCounter <= 0.0f) {	// カウンター終了
			m_StateInfo.fCounter = 0.0f;
		}
		else {
			// 座標を後ろに下げる
			D3DXVECTOR3 move = GetMove();
			move.x = sinf(GetInfo()->rot.y) * SPEED::DAMAGE_MOVE;
			move.z = cosf(GetInfo()->rot.y) * SPEED::DAMAGE_MOVE;

			SetMove(move);
		}
	}
		break;

	case STATE_DEATH:
	{
		if (m_StateInfo.fCounter <= 0.0f) {	// カウンター終了
			if (!BodyCheck(m_pBody)) {// 胴体確認失敗
				// 爆発パーティクル設置

				// 敵を削除
				Uninit();
			}
			else {
				if (m_pBody->GetMotion()->GetEnd()) {	// 死亡モーション終了
					// 爆発パーティクル設置

					// 敵を削除
					Uninit();
				}
			}
		}
	}
		break;

	default:
	{

	}
		break;
	}
}

//===============================================
// アクション設定
//===============================================
void CEnemyBoss::SetMotion(void)
{
	if (!BodyCheck(m_pBody)) {// 胴体確認失敗
		return;
	}

	if (!BodyCheck(m_pLeg)) {// 下半身確認失敗
		return;
	}

	// ダメージ状態
	if (m_StateInfo.state == STATE_DAMAGE) {

		m_nAction = ACTION_DAMAGE;
		m_pBody->GetMotion()->BlendSet(m_nAction);
		m_pLeg->GetMotion()->BlendSet(m_nAction);

		if (m_pBody->GetMotion()->GetEnd())
		{// モーション終了
			m_nAction = ACTION_NEUTRAL;	// 保持状態に変更
			m_StateInfo.state = STATE_APPEAR;
		}
		else
		{
			return;
		}
	}

	switch (m_nAction) {
	case ACTION_NEUTRAL:
	{
		if (m_pBody->GetMotion()->GetEnd())
		{// モーション終了
			m_pBody->GetMotion()->BlendSet(m_nAction);
			m_pLeg->GetMotion()->BlendSet(m_nAction);
		}
	}
		break;

	case ACTION_WALK:
	{
		m_pBody->GetMotion()->BlendSet(m_nAction);
		m_pLeg->GetMotion()->BlendSet(m_nAction);
	}
		break;

	case ACTION_ATK:
	{
		m_pBody->GetMotion()->Set(m_nAction);
		m_pLeg->GetMotion()->Set(m_nAction);

		if (m_pBody->GetMotion()->GetEnd())
		{// モーション終了
			m_nAction = ACTION_NEUTRAL;
		}
	}
		break;

	case ACTION_DAMAGE:
	{
		m_pBody->GetMotion()->BlendSet(m_nAction);
		m_pLeg->GetMotion()->BlendSet(m_nAction);

		if (m_pBody->GetMotion()->GetEnd())
		{// モーション終了	
			m_nAction = ACTION_NEUTRAL;
		}
	}
		break;

	case ACTION_DEATH:
	{
		m_pBody->GetMotion()->Set(m_nAction);
		m_pLeg->GetMotion()->Set(m_nAction);
	}
		break;
	}
}

//===============================================
// 重力設定
//===============================================
void CEnemyBoss::Gravity(void)
{
	SInfo* pInfo = GetInfo();
	float fGravity = SPEED::GRAVITY;;

	// 重力にスロー倍率を掛け合わせる
	fGravity *= CManager::GetInstance()->GetSlow()->Get();

	pInfo->move.y += fGravity;
	pInfo->pos.y += pInfo->move.y * CManager::GetInstance()->GetSlow()->Get();
}

//===============================================
// 攻撃
//===============================================
void CEnemyBoss::Attack(const int nRandRange)
{
	float CMinusCnter = static_cast<float>(rand() % nRandRange);
	m_fAtkCnter -= (CMinusCnter) * CManager::GetInstance()->GetSlow()->Get();

	if (m_fAtkCnter > 0.0f) {	// 攻撃できない
		AttackCheck();
		return;
	}

	if (m_NowArm >= PARTS_MAX) {
		return;
	}

	if (!BodyCheck(m_apArm[m_NowArm])) {
		return;
	}
	m_fAtkCnter = INTERVAL::ATTACK;
	m_apArm[m_NowArm]->GetMotion()->BlendSet(ARM_ATTACK);
	m_nArmAction = ARM_ATTACK;
}

//===============================================
// 攻撃確認
//===============================================
void CEnemyBoss::AttackCheck(void)
{
	if (m_NowArm >= PARTS_MAX) {
		return;
	}

	if (!BodyCheck(m_apArm[m_NowArm])) {
		return;
	}

	if (m_apArm[m_NowArm]->GetMotion()->GetNowMotion() != ARM_ATTACK) {	// 現在攻撃中ではない
		return;
	}

	if (m_apArm[m_NowArm]->GetMotion()->GetEnd()) {	// モーション終了
		m_apArm[m_NowArm]->GetMotion()->BlendSet(ARM_NEUTRAL);
		m_nArmAction = ARM_NEUTRAL;
	}
}

//===============================================
// 足場確認
//===============================================
void CEnemyBoss::FootCheck(void)
{
	if (m_NowArm >= PARTS_MAX) {
		return;
	}

	if (!BodyCheck(m_apArm[m_NowArm])) {
		return;
	}

	if (m_nArmAction != ARM_ATTACK) {
		return;
	}

	if (m_apArm[m_NowArm]->GetMotion()->GetNowKey() != 3) {
		return;
	}

	int nNumParts = m_apArm[m_NowArm]->GetNumParts();

	for (int i = 0; i < nNumParts; i++) {
		CPlayer* pPlayer = CPlayerManager::GetInstance()->GetTop();
		D3DXVECTOR3 ArmPos = { m_apArm[m_NowArm]->GetParts(i)->GetMtx()->_41, GetPosition().y, m_apArm[m_NowArm]->GetParts(i)->GetMtx()->_43 };
		D3DXVECTOR3 VtxMax = CManager::GetInstance()->GetModelFile()->GetMax(m_apArm[m_NowArm]->GetParts(i)->GetId());
		D3DXVECTOR3 VtxMin = CManager::GetInstance()->GetModelFile()->GetMin(m_apArm[m_NowArm]->GetParts(i)->GetId());
		VtxMax.y *= 2;

		if (i == nNumParts - 1) {
			ArmPos.y += 60.0f;
		}

		// プレイヤー分繰り返し
		while (pPlayer != nullptr) {
			CPlayer* pNext = pPlayer->GetNext();
			D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();
			D3DXVECTOR3 PlayerPosOld = pPlayer->GetOldPosition();
			D3DXVECTOR3 PlayerMove = pPlayer->GetMove();
			D3DXVECTOR3 PlayerMax = pPlayer->GetColliMax();

			// Y座標確認
			if (PlayerPos.x >= ArmPos.x + VtxMin.x && PlayerPos.x <= ArmPos.x + VtxMax.x
				&& PlayerPos.z >= ArmPos.z + VtxMin.z && PlayerPos.z <= ArmPos.z + VtxMax.z) {

				if (PlayerPos.y < ArmPos.y + VtxMax.y && PlayerPosOld.y >= ArmPos.y + VtxMax.y) {
					PlayerPos.y = ArmPos.y + VtxMax.y;
					PlayerMove.y = 0.0f;
					pPlayer->SetJump(false);

					if (i == nNumParts - 1) {
						ArmDamage();
						return;
					}
				}

				if (PlayerPos.y + PlayerMax.y > ArmPos.y + VtxMin.y && PlayerPosOld.y + PlayerMax.y <= ArmPos.y + VtxMin.y) {
					PlayerPos.y = ArmPos.y + VtxMin.y - PlayerMax.y;
					PlayerMove.y = 0.0f;
				}
			}

			// XZ座標確認
			if (PlayerPos.y <= ArmPos.y + VtxMax.y && PlayerPos.y + PlayerMax.y >= ArmPos.y + VtxMin.y) {

				// X座標
				if (PlayerPos.z - PlayerMax.z <= ArmPos.z + VtxMax.z
					&& PlayerPos.z + PlayerMax.z >= ArmPos.z + VtxMin.z) {
					if (PlayerPos.x - PlayerMax.x < ArmPos.x + VtxMax.x
						&& PlayerPosOld.x - PlayerMax.x >= ArmPos.x + VtxMax.x) {
						PlayerPos.x = ArmPos.x + VtxMax.x + PlayerMax.x;
						PlayerMove.x = 0.0f;
					}
					if (PlayerPos.x + PlayerMax.x > ArmPos.x + VtxMin.x
						&& PlayerPosOld.x + PlayerMax.x <= ArmPos.x + VtxMin.x) {
						PlayerPos.x = ArmPos.x + VtxMin.x - PlayerMax.x;
						PlayerMove.x = 0.0f;
					}
				}

				// Z座標
				if (PlayerPos.x - PlayerMax.x <= ArmPos.x + VtxMax.x
					&& PlayerPos.x + PlayerMax.x >= ArmPos.x + VtxMin.x) {
					if (PlayerPos.z - PlayerMax.z < ArmPos.z + VtxMax.z
						&& PlayerPosOld.z - PlayerMax.z >= ArmPos.z + VtxMax.z) {
						PlayerPos.z = ArmPos.z + VtxMax.z + PlayerMax.z;
						PlayerMove.z = 0.0f;
					}
					if (PlayerPos.z + PlayerMax.z > ArmPos.z + VtxMin.z
						&& PlayerPosOld.z + PlayerMax.z <= ArmPos.z + VtxMin.z) {
						PlayerPos.z = ArmPos.z + VtxMin.z - PlayerMax.z;
						PlayerMove.z = 0.0f;
					}
				}
			}

			pPlayer->SetPosition(PlayerPos);
			pPlayer->SetMove(PlayerMove);

			pPlayer = pNext;
		}
	}
}

//===============================================
// 腕ダメージ
//===============================================
void CEnemyBoss::ArmDamage(void)
{
	if (m_NowArm >= PARTS_MAX) {
		return;
	}

	if (m_apArm[m_NowArm] != nullptr) {
		m_apArm[m_NowArm]->Uninit();
		delete m_apArm[m_NowArm];
		m_apArm[m_NowArm] = nullptr;
	}

	m_NowArm++;

	if (m_NowArm >= PARTS_MAX) {
		m_nArmAction = ARM_NEUTRAL;
	}
}