//==========================================================
//
// 近接攻撃の敵 [enemy_melee.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "enemy_melee.h"
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

// 無名名前空間
namespace
{
	const char* BODYFILEPASS = "data\\TXT\\enemy\\motion_body.txt";	// ファイルのパス
	const char* LEGFILEPASS = "data\\TXT\\enemy\\motion_leg.txt";	// ファイルのパス
	const D3DXVECTOR3 COLLIMAX = { 20.0f, 120.0f, 20.0f };	// 当たり判定最大
	const D3DXVECTOR3 COLLIMIN = { -20.0f, 0.0f, -20.0f };	// 当たり判定最小
	const int DAMAGEINTERVAL = (60);			// ダメージインターバル
	const float CHASE_MAXLENGTH = (1000.0f);	// 追跡最長距離
	const float CHASE_NEARLENGTH = (400.0f);	// 追跡近距離
	const float CHASE_MINLENGTH = (100.0f);		// 追跡0距離
	const float SEARCH_HEIGHT = (180.0f);		// 探索高さ制限
	const float MOVE_INER = (0.3f);				// 移動慣性
}

// 移動速度名前空間
namespace SPEED
{
	const float MOVE_FAR = (2.0f);	// 遠距離移動
	const float MOVE_NEAR = (1.0f);	// 近距離移動
	const float MOVE_MIN = (0.15f);	// 移動量移動
	const float GRAVITY = (-0.9f);	// 重力
	const float DAMAGE_MOVE = (2.0f);	// 移動量
}

// インターバル
namespace INTERVAL
{
	const float DAMAGE = (20.0f);	// ダメージ
	const float ATTACK = (30.0f);	// 攻撃
}

//==========================================================
// コンストラクタ
//==========================================================
CEnemyMelee::CEnemyMelee()
{
	// 値のクリア
	m_nAction = ACTION_NEUTRAL;
	m_pBody = nullptr;
	m_pLeg = nullptr;
	m_pWaist = nullptr;
	m_Chase.pTarget = nullptr;
	m_Chase.fLength = 0.0f;
	m_fInterVal = 0;
	m_StateInfo.state = STATE_APPEAR;
	m_StateInfo.fCounter = 0.0f;
}

//==========================================================
// デストラクタ
//==========================================================
CEnemyMelee::~CEnemyMelee()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CEnemyMelee::Init(void)
{
	// 種類を近距離に設定
	SetType(TYPE_MELEE);

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

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CEnemyMelee::Uninit(void)
{
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

	// 親クラスの終了呼び出し
	CEnemy::Uninit();
}

//==========================================================
// 更新処理
//==========================================================
void CEnemyMelee::Update(void)
{
	// 前回の座標を取得
	{
		SInfo* pInfo = GetInfo();
		pInfo->posOld = pInfo->pos;
	}
	m_fInterVal -= CManager::GetInstance()->GetSlow()->Get();

	// 処理
	MethodLine();

	//マトリックス設定
	CEnemy::Update();
	BodySet();
}

//===============================================
// 処理の順番
//===============================================
void CEnemyMelee::MethodLine(void)
{
	SInfo* pInfo = GetInfo();

	// 攻撃確認
	AttackCheck();

	if (m_StateInfo.state != STATE_DEATH || m_StateInfo.state != STATE_DAMAGE) {

		// 捜索
		m_Chase.pTarget = Search(m_Chase.fLength);

		// 追跡
		Chase();

		// 移動
		AddMove();
	}

	// 重力
	Gravity();

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
CEnemyMelee *CEnemyMelee::Create(D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	CEnemyMelee *pEnemyMelee = nullptr;

	pEnemyMelee = new CEnemyMelee;

	if (pEnemyMelee != nullptr)
	{
		// 初期化処理
		pEnemyMelee->Init();

		// データリセット
		pEnemyMelee->InfoReset();

		// データ設定
		pEnemyMelee->SetPosition(pos);
		pEnemyMelee->SetRotation(rot);
		pEnemyMelee->SetRotDiff(rot.y);
		pEnemyMelee->SetIner(MOVE_INER);
	}

	return pEnemyMelee;
}

//===============================================
// 使用階層構造の設定
//===============================================
void CEnemyMelee::BodySet(void)
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
}

//===============================================
// 体使用確認
//===============================================
bool CEnemyMelee::BodyCheck(CCharacter* pBody)
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
bool CEnemyMelee::Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage)
{
	SInfo* pInfo = GetInfo();

	if (m_fInterVal > 0) {
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
void CEnemyMelee::Damage(const int nDamage)
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
// 攻撃判定確認
//===============================================
void CEnemyMelee::AttackCheck(void)
{
	if (!BodyCheck(m_pBody) || !BodyCheck(m_pLeg)) {
		return;
	}

	if (m_StateInfo.state != STATE_ATTACK) {
		return;
	}

	if (m_StateInfo.fCounter > 0.0f) {
		return;
	}

	CModel* pModel = m_pLeg->GetParts(3);
	float fRange = 50.0f;
	int nDamage = 1;
	D3DXVECTOR3 pos = { pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43 };
	CPlayerManager::GetInstance()->Hit(pos, fRange, nDamage);
}

//===============================================
// プレイヤーを探索
//===============================================
CPlayer* CEnemyMelee::Search(float& fChaseLength)
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
void CEnemyMelee::Chase(void)
{
	if (m_Chase.pTarget == nullptr) {
		if (m_nAction <= ACTION_DUSH) {
			m_nAction = ACTION_NEUTRAL;
		}
		return;
	}

	if (m_StateInfo.state >= STATE_DAMAGE) {
		return;
	}

	CManager::GetInstance()->GetDebugProc()->Print("プレイヤーとの距離 [%f]\n", m_Chase.fLength);

	// 目標向き設定
	D3DXVECTOR3 MyPos = GetInfo()->pos;	// 自分の座標
	D3DXVECTOR3 pos = m_Chase.pTarget->GetPosition();
	{
		float fDiff = GetRotDiff();
		fDiff = atan2f(pos.x - MyPos.x, pos.z - MyPos.z) + D3DX_PI;
		if (fDiff < -D3DX_PI) {
			fDiff += D3DX_PI * 2;
		}
		else if (fDiff > D3DX_PI) {
			fDiff += -D3DX_PI * 2;
		}
		SetRotDiff(fDiff);
	}

	// 移動量を設定
	{
		float fSpeed = SPEED::MOVE_FAR;
		if (m_Chase.fLength <= CHASE_NEARLENGTH && 
			m_Chase.fLength > CHASE_MINLENGTH) {	// 近距離判定の距離
			fSpeed = SPEED::MOVE_NEAR + (rand() % 2) * SPEED::MOVE_MIN;
			m_nAction = ACTION_WALK;
			m_StateInfo.state = STATE_CHASE;
		}
		else if (m_Chase.fLength <= CHASE_MINLENGTH){	// 最も近い距離判定
			fSpeed = SPEED::MOVE_MIN + (rand() % 3) * SPEED::MOVE_MIN;	// 移動量に少しランダムを持たせる
			
			if (m_StateInfo.state <= STATE_CHASE) {
				m_StateInfo.state = STATE_ATTACK;
				m_nAction = ACTION_ATK;
			}
		}
		else {
			m_nAction = ACTION_DUSH;
			m_StateInfo.state = STATE_CHASE;
		}

		D3DXVECTOR3 move = GetMove();
		D3DXVECTOR3 nor = pos - MyPos;
		nor.y = 0.0f;
		D3DXVec3Normalize(&nor, &nor);
		move += nor * fSpeed;

		SetMove(move);
	}
}

//===============================================
// 状態設定
//===============================================
void CEnemyMelee::SetState(void)
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

	case STATE_CHASE:
	{

	}
		break;

	case STATE_ATTACK:
	{
		if (m_StateInfo.fCounter <= 0.0f) {	// カウンター終了
			m_StateInfo.fCounter = 0.0f;
		}
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
void CEnemyMelee::SetMotion(void)
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
		
	case ACTION_DUSH:
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
			if (m_Chase.fLength < CHASE_MINLENGTH) {
				m_nAction = ACTION_2NDATK;
			}
			else {
				m_nAction = ACTION_NEUTRAL;
			}
		}
	}
		break;

	case ACTION_2NDATK:
	{
		m_pBody->GetMotion()->BlendSet(m_nAction);
		m_pLeg->GetMotion()->BlendSet(m_nAction);

		if (m_pBody->GetMotion()->GetEnd())
		{// モーション終了
			m_StateInfo.fCounter = INTERVAL::ATTACK;
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
void CEnemyMelee::Gravity(void)
{
	SInfo* pInfo = GetInfo();
	float fGravity = SPEED::GRAVITY;;

	// 重力にスロー倍率を掛け合わせる
	fGravity *= CManager::GetInstance()->GetSlow()->Get();

	pInfo->move.y += fGravity;
	pInfo->pos.y += pInfo->move.y * CManager::GetInstance()->GetSlow()->Get();
}