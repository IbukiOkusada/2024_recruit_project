//==========================================================
//
// 遠距離攻撃の敵 [enemy_gun.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "enemy_gun.h"
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
#include "particle.h"
#include "enemy_manager.h"

// 無名名前空間
namespace
{
	const char* BODYFILEPASS = "data\\TXT\\enemy_gun\\motion_body.txt";	// ファイルのパス
	const char* LEGFILEPASS = "data\\TXT\\enemy_gun\\motion_leg.txt";	// ファイルのパス
	const D3DXVECTOR3 COLLIMAX = { 20.0f, 120.0f, 20.0f };	// 当たり判定最大
	const D3DXVECTOR3 COLLIMIN = { -20.0f, 0.0f, -20.0f };	// 当たり判定最小
	const int DAMAGEINTERVAL = (60);	// ダメージインターバル
	const float CHASE_MAXLENGTH = (1000.0f);	// 追跡最長距離
	const float CHASE_NEARLENGTH = (700.0f);	// 追跡近距離
	const float CHASE_MINLENGTH = (400.0f);		// 追跡0距離
	const float SEARCH_HEIGHT = (220.0f);		// 探索高さ制限
	const float SEARCH_DOWN = (180.0f);
	const float MOVE_INER = (0.3f);			// 移動慣性
}

// 移動速度名前空間
namespace SPEED
{
	const float MOVE_FAR = (2.0f);	// 遠距離移動
	const float MOVE_NEAR = (-1.0f);	// 近距離移動
	const float MOVE_MIN = (0.15f);	// 移動量移動
	const float GRAVITY = (-0.9f);	// 重力
	const float DAMAGE_MOVE = (2.0f);	// 移動量
	const float BULLET = (12.0f);	// 弾速
}

// インターバル
namespace INTERVAL
{
	const float DAMAGE = (20.0f);	// ダメージ
	const float ATTACK = (140.0f);	// 攻撃
}

//==========================================================
// コンストラクタ
//==========================================================
CEnemyGun::CEnemyGun()
{
	// 値のクリア
	m_nAction = ACTION_NEUTRAL;
	m_pBody = nullptr;
	m_pLeg = nullptr;
	m_pWaist = nullptr;
	m_Chase.pTarget = nullptr;
	m_Chase.fLength = 0.0f;
	m_Chase.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fInterVal = 0;
	m_StateInfo.state = STATE_APPEAR;
	m_StateInfo.fCounter = 0.0f;
	m_fAtkCnter = 0.0f;
}

//==========================================================
// デストラクタ
//==========================================================
CEnemyGun::~CEnemyGun()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CEnemyGun::Init(void)
{
	// 種類を遠距離に設定
	SetType(TYPE_GUN);

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
	m_pBody->SetChangeMatCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

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
	m_pLeg->SetChangeMatCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

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
void CEnemyGun::Uninit(void)
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
void CEnemyGun::Update(void)
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
void CEnemyGun::MethodLine(void)
{
	SInfo* pInfo = GetInfo();

	if (m_StateInfo.state != STATE_DEATH) {

		// 捜索
		m_Chase.pTarget = Search(m_Chase.fLength);

		// 追跡
		LockOn();

		// 移動
		AddMove();
	}

	// 重力
	Gravity();

	// 敵同士の当たり判定確認
	CEnemyManager::GetInstance()->Bump(pInfo->pos, GetBumpSize(), this);

	// 当たり判定確認
	int nType = 0;
	CObjectX::COLLISION_AXIS axis = CObjectX::TYPE_MAX;
	CMeshWall::Collision(pInfo->pos, pInfo->posOld, pInfo->move, COLLIMAX, COLLIMIN, axis, nType);
	float fHeight = CMeshField::GetHeight(pInfo->pos, pInfo->posOld);
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
CEnemyGun *CEnemyGun::Create(D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	CEnemyGun *pEnemyGun = nullptr;

	pEnemyGun = new CEnemyGun;

	if (pEnemyGun != nullptr)
	{
		// 初期化処理
		pEnemyGun->Init();

		// データリセット
		pEnemyGun->InfoReset();

		// データ設定
		pEnemyGun->SetPosition(pos);
		pEnemyGun->SetRotation(rot);
		pEnemyGun->SetRotDiff(rot.y);
		pEnemyGun->SetIner(MOVE_INER);
	}

	return pEnemyGun;
}

//===============================================
// 使用階層構造の設定
//===============================================
void CEnemyGun::BodySet(void)
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
bool CEnemyGun::BodyCheck(CCharacter* pBody)
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
bool CEnemyGun::Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage)
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
void CEnemyGun::Damage(const int nDamage)
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

		if (BodyCheck(m_pBody) && BodyCheck(m_pLeg)) {
			m_pBody->SetChangeMat(true);
			m_pLeg->SetChangeMat(true);
		}
	}
}

//===============================================
// プレイヤーを探索
//===============================================
CPlayer* CEnemyGun::Search(float& fChaseLength)
{
	if (m_nAction == ACTION_ATK) {
		return m_Chase.pTarget;
	}

	CPlayer* pPlayer = CPlayerManager::GetInstance()->GetTop();
	CPlayer* pChasePlayer = nullptr;
	D3DXVECTOR3 MyPos = GetInfo()->pos;	// 自分の座標
	float fMinLength = CHASE_MAXLENGTH;	// 追跡するプレイヤーとの距離
	fChaseLength = fMinLength;

	// プレイヤー全員分繰り返す
	while (pPlayer != nullptr) {
		CPlayer* pPlayerNext = pPlayer->GetNext();

		D3DXVECTOR3 pos = pPlayer->GetPosition();

		if (pos.y < MyPos.y - SEARCH_DOWN || pos.y > MyPos.y + SEARCH_HEIGHT) {	// 高さ感知範囲外
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
void CEnemyGun::LockOn(void)
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
		float fSpeed = 0.0f;
		if (m_nAction == ACTION_ATK) {
			Attack();
		}
		else if (m_Chase.fLength >= CHASE_NEARLENGTH) {	// 適性距離より遠い
			fSpeed = SPEED::MOVE_FAR;
			m_nAction = ACTION_WALK;
		}
		else if (m_Chase.fLength <= CHASE_MINLENGTH) {	// 適性距離より近い
			fSpeed = SPEED::MOVE_NEAR;
			m_nAction = ACTION_WALK;
		}
		else {
			m_nAction = ACTION_ATK;
			Attack();
		}

		D3DXVECTOR3 move = GetMove();
		D3DXVECTOR3 nor = pos - MyPos;
		m_Chase.nor = nor;
		nor.y = 0.0f;
		D3DXVec3Normalize(&nor, &nor);
		D3DXVec3Normalize(&m_Chase.nor, &m_Chase.nor);
		move += nor * fSpeed;

		SetMove(move);
	}
}

//===============================================
// 状態設定
//===============================================
void CEnemyGun::SetState(void)
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
		if (BodyCheck(m_pBody) && BodyCheck(m_pLeg)) {
			m_pBody->SetChangeMat(false);
			m_pLeg->SetChangeMat(false);
		}
	}
		break;

	case STATE_DAMAGE:
	{
		if (m_StateInfo.fCounter <= 0.0f) {	// カウンター終了
			m_StateInfo.fCounter = 0.0f;

			if (BodyCheck(m_pBody) && BodyCheck(m_pLeg)) {
				m_pBody->SetChangeMat(false);
				m_pLeg->SetChangeMat(false);
			}
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
void CEnemyGun::SetMotion(void)
{
	if (!BodyCheck(m_pBody)) {// 胴体確認失敗
		return;
	}

	if (!BodyCheck(m_pLeg)) {// 下半身確認失敗
		return;
	}

	// ダメージ状態
	if (m_StateInfo.state == STATE_DAMAGE && m_nAction != ACTION_ATK) {

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
		m_pBody->GetMotion()->BlendSet(m_nAction);
		m_pLeg->GetMotion()->BlendSet(m_nAction);

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
void CEnemyGun::Gravity(void)
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
void CEnemyGun::Attack(void)
{
	if (!BodyCheck(m_pBody)) {
		return;
	}

	if (m_pBody->GetMotion()->GetNowMotion() != ACTION_ATK) {
		return;
	}

	CModel* pModel = m_pBody->GetParts(m_pBody->GetNumParts() - 1);
	if (pModel == nullptr) { return; }	// モデル使われていない

	// 銃口を取得
	D3DXVECTOR3 pos = { pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43 };

	if (m_pBody->GetMotion()->GetNowFrame() == 0 && m_pBody->GetMotion()->GetNowKey() == 2) {	// 射撃タイミング

		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_Chase.nor * SPEED::BULLET, CBullet::TYPE_ENEMY);
	}
	else if(m_pBody->GetMotion()->GetNowKey() < 2) {	// チャージ時間
		CParticle::Create(pos, CEffect::TYPE_GUNCHARGE);
	}
}