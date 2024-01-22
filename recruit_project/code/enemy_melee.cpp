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

// 無名名前空間
namespace
{
	const char* BODYFILEPASS = "data\\TXT\\player\\motion_body.txt";	// ファイルのパス
	const char* LEGFILEPASS = "data\\TXT\\player\\motion_leg.txt";	// ファイルのパス
	const D3DXVECTOR3 COLLIMAX = { 20.0f, 120.0f, 20.0f };
	const D3DXVECTOR3 COLLIMIN = { -20.0f, 0.0f, -20.0f };
	const int DAMAGEINTERVAL = (60);
}

//==========================================================
// コンストラクタ
//==========================================================
CEnemyMelee::CEnemyMelee()
{
	// 値のクリア
	m_nAction = 7;
	m_pBody = nullptr;
	m_pLeg = nullptr;
	m_pWaist = nullptr;
	m_nInterVal = 0;
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
	m_nInterVal = 1;

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
	SInfo* pInfo = GetInfo();
	pInfo->posOld = pInfo->pos;
	m_nInterVal--;

	// 攻撃確認
	AttackCheck();

	// 当たり判定確認
	CObjectX::COLLISION_AXIS axis = CObjectX::TYPE_MAX;
	CMeshWall::Collision(pInfo->pos, pInfo->posOld, pInfo->move, COLLIMAX, COLLIMIN, axis);

	//マトリックス設定
	CEnemy::Update();
	BodySet();
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
void CEnemyMelee::Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage)
{
	SInfo* pInfo = GetInfo();

	if (m_nInterVal > 0) {
		return;
	}

	if (pos.x + fRange < pInfo->pos.x + COLLIMIN.x ||
		pos.x - fRange > pInfo->pos.x + COLLIMAX.x)
	{// X範囲外
		return;
	}

	if (pos.z + fRange < pInfo->pos.z + COLLIMIN.z ||
		pos.z - fRange > pInfo->pos.z + COLLIMAX.z)
	{// Z範囲外
		return;
	}

	if (pos.y + fRange < pInfo->pos.y + COLLIMIN.y ||
		pos.y - fRange > pInfo->pos.y + COLLIMAX.y)
	{// Y範囲外
		return;
	}

	// ダメージ処理
	Damage(nDamage);
}

//===============================================
// ダメージ処理
//===============================================
void CEnemyMelee::Damage(const int nDamage)
{
	if (m_nInterVal > 0) {	// インターバルが戻ってきていない
		return;
	}

	// インターバルを変更
	m_nInterVal = DAMAGEINTERVAL;

	// 体力を減らす
	int nLife = GetLife();
	nLife -= nDamage;

	if (nLife <= 0) {	// 体力がなくなった
		Uninit();
	}
	else {	// まだある
		SetLife(nLife);
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

	CModel* pModel = m_pLeg->GetParts(3);
	float fRange = 50.0f;
	int nDamage = 1;
	D3DXVECTOR3 pos = { pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43 };
	CPlayerManager::GetInstance()->Hit(pos, fRange, nDamage);
}