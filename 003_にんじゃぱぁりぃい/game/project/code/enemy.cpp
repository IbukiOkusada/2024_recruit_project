//===============================================
//
// 敵の処理 [enemy.cpp]
// Author : Soma Ishihara (Base:Ibuki Okusada)
//
//===============================================
#include "enemy.h"
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
#include "player.h"
#include "item.h"
#include "gimmick.h"
#include "model.h"
#include "object3DFan.h"
#include "waist.h"
#include "point.h"
#include "particle.h"

//無名名前空間
namespace
{
	const float MOVE = 1.3f;			// 移動量
	const float GRAVITY = -0.9f;		// 敵重力
	const float JUMP = 20.0f;			// 敵ジャンプ力
	const float ROT_MULTI = 0.1f;		// 向き補正倍率
	const float INER = 0.3f;			// 慣性
	const float BLOW_INER = 0.1f;		// 吹き飛ばし中慣性
	const int START_LIFE = 4;			// 初期体力
	const int DAMAGE_INTERVAL = 80;		// ダメージ付与間隔
	const int APPEAR_INTERVAL = 120;	// 出現中になっている間隔
	const float DEFAULT_ROTATE = 0.1f;	// プレイヤー探索中の回転量
	const float SEARCH_LENGTH = 300.0f;	// プレイヤー探索範囲
	const float SEARCH_RADIUS = 0.25f;	// プレイヤー探索角度
	const float CHACE_LENGTH = 400.0f;	// 追跡範囲
	const float ATTACK_LENGTH = 50.0f;	// 攻撃モードにする範囲
	const int ATTACK_COOLTIME = 60;		// 攻撃クールタイム
	const float NEXTPOINT_LENGTH = 100.0f;	//次のポイントに切り替える距離
	const D3DXVECTOR3 ENEMY_VTX_MIN = D3DXVECTOR3(-20.0f, 0.0f, -20.0f);	// 当たり判定などのサイズ
	const D3DXVECTOR3 ENEMY_VTX_MAX = D3DXVECTOR3(20.0f, 0.0f, 20.0f);
	const float HIT_RANGE = 100.0f;		// 攻撃が当たる範囲
	const int DROP_COIN = 3;			// コインのドロップ量
	const char* BODY_FILENAME = "data\\TXT\\enemy\\motion_ninjabody.txt";	// 上半身のモーションファイルパス
	const char* LEG_FILENAME = "data\\TXT\\enemy\\motion_ninjaleg.txt";		// 下半身のモーションファイルパス
	const int RETURN_TIME_LIMIT = 120;	//帰って来るまでの制限時間
	const int HEADPARTS_IDX = (1);
}

//===============================================
// マクロ定義
//===============================================
#define FIX_ROT(x)				(fmodf(x + (D3DX_PI * 3), D3DX_PI * 2) - D3DX_PI)	//角度を-PI~PIに修正
#define MINUS_GUARD(x)			((x < 0) ? 0 : x)

// 前方宣言
CEnemy *CEnemy::m_pTop = nullptr;	// 先頭のオブジェクトへのポインタ
CEnemy *CEnemy::m_pCur = nullptr;	// 最後尾のオブジェクトへのポインタ
int CEnemy::m_nNumCount = 0;

//===============================================
// コンストラクタ
//===============================================
CEnemy::CEnemy()
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
	m_nLife = 0;
	m_nCounterAttack = 0;
	m_bChace = false;
	m_bJump = false;
	m_type = TYPE_NONE;
	m_nId = m_nNumCount;
	m_pChase = nullptr;
	m_nPointID = -1;
	m_nPointNum = 0;

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
CEnemy::~CEnemy()
{
	
}

//===============================================
// 初期化処理
//===============================================
HRESULT CEnemy::Init(void)
{
	if (nullptr == m_pBody)
	{
		m_pBody = CCharacter::Create(GetPosition(), GetRotation(), "data\\TXT\\motion_kidsboy.txt");
		m_pBody->SetShadow(true);
	}

	if (nullptr == m_pFov)
	{
		m_pFov = CObject3DFan::Create(m_Info.pos, m_Info.rot, SEARCH_LENGTH, SEARCH_RADIUS * D3DX_PI, 8);
		m_pFov->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.4f));
	}

	m_Info.state = STATE_APPEAR;
	m_type = TYPE_NONE;
	m_nLife = START_LIFE;

	// 煙のパーティクル生成
	CModel *pModel = m_pLeg->GetParts(0);
	CParticle::Create(D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43), CEffect::TYPE_SMAKE);

	return S_OK;
}

//===============================================
// 初期化処理(オーバーロード)
//===============================================
HRESULT CEnemy::Init(const char *pBodyName, const char *pLegName)
{
	// 腰の生成
	if (m_pWaist == NULL)
	{
		m_pWaist = new CWaist;
		m_pWaist->SetParent(&m_Info.mtxWorld);
		m_pWaist->SetMatrix();
	}

	// 胴体の設定
	//if (pBodyName != NULL)
	{// ファイル名が存在している
		m_pBody = CCharacter::Create(BODY_FILENAME);
		m_pBody->SetParent(m_pWaist->GetMtxWorld());
		m_pBody->SetShadow(true);
		m_pBody->SetDraw();

		if (m_pBody->GetMotion() != NULL)
		{
			// 初期モーションの設定
			m_pBody->GetMotion()->InitSet(MOTION_NEUTRAL);
		}
	}

	// 下半身の設定
	//if (pLegName != NULL)
	{// ファイル名が存在している
		m_pLeg = CCharacter::Create(LEG_FILENAME);
		m_pLeg->SetParent(m_pWaist->GetMtxWorld());
		m_pLeg->SetShadow(true);
		m_pLeg->SetDraw();

		if (m_pLeg->GetMotion() != NULL)
		{
			// 初期モーションの設定
			m_pLeg->GetMotion()->InitSet(MOTION_NEUTRAL);
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

	if (nullptr == m_pFov && CManager::GetInstance()->GetMode() != CScene::MODE_TITLE)
	{
		m_pFov = CObject3DFan::Create(m_Info.pos, m_Info.rot, SEARCH_LENGTH, SEARCH_RADIUS * D3DX_PI, 8);
		m_pFov->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.6f));
	}

	m_nLife = START_LIFE;
	m_type = TYPE_NONE;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CEnemy::Uninit(void)
{
	// リストから自分自身を削除する
	if (m_pTop == this)
	{// 自身が先頭
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
	else if (m_pCur == this)
	{// 自身が最後尾
		if (m_pPrev != nullptr)
		{// 次が存在している
			m_pCur = m_pPrev;			// 前を最後尾にする
			m_pPrev->m_pNext = nullptr;	// 前の次のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else
	{
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
	if (m_pWaist != nullptr) {
		delete m_pWaist;
		m_pWaist = nullptr;
	}

	if (nullptr != m_pFov)
	{
		m_pFov->Uninit();
		m_pFov = nullptr;
	}

	m_nNumCount--;

	// 廃棄
	Release();
}

//===============================================
// 更新処理
//===============================================
void CEnemy::Update(void)
{
	// 前回の座標を取得
	m_Info.posOld = GetPosition();	

	m_nCounterAttack--;

	if (m_nPointID == ExPattern::POINTID_GAMESTART) {
		GameStart();
	}
	else{
		if (m_Info.state < STATE_DAMAGE || m_Info.state >= STATE_BLOW)
		{
			// 敵操作
			Controller();
		}
	}

	// モーション設定
	MotionSet();

	// マトリックス設定
	SetMatrix();

	CManager::GetInstance()->GetDebugProc()->Print("体力 [ %d ]\n", m_nLife);

	// 使用オブジェクト更新
	if (nullptr != m_pFov)
	{
		m_pFov->SetPosition(m_Info.pos);
		m_pFov->SetRotation(m_Info.rot + D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	}

	// 階層構造設定
	BodySet();

	// 状態遷移
	StateSet();

	if (m_pLeg != nullptr)
	{// 使用されている場合
		CModel *pModel = m_pLeg->GetParts(0);

		pModel->SetCurrentPosition(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	}
}

//===============================================
// 生成
//===============================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, const char *pBodyName, const char *pLegName, const int nPointID)
{
	CEnemy *pEnemy = nullptr;

	// 敵の生成
	pEnemy = new CEnemy();

	if (nullptr != pEnemy)
	{// 生成できた場合
		// 初期化処理
		pEnemy->Init(pBodyName, pLegName);
		pEnemy->m_nPointID = nPointID;

		// 座標設定
		pEnemy->SetPosition(pos);

		// 向き設定
		pEnemy->SetRotation(rot);

		pEnemy->m_fRotDest = rot.y;

		// 移動量設定
		pEnemy->SetMove(move);

		//煙出す
		CParticle::Create(pos, CEffect::TYPE_SMAKE);
	}
	else
	{// 生成に失敗した場合
		return nullptr;
	}

	return pEnemy;
}

//===============================================
// 操作処理
//===============================================
void CEnemy::Controller(void)
{
	D3DXVECTOR3 pos = GetPosition();	// 座標を取得
	D3DXVECTOR3 rot = GetRotation();	// 向きを取得
	m_fRotMove = rot.y;	//現在の向きを取得
	float fIner = INER;

	// 操作処理
	{
		if (m_Info.state != STATE_BLOW) {	// 吹き飛ばし以外
			Move();		// 移動
		}
		else {
			fIner = BLOW_INER;
		}

		if (m_bChace == false)
		{
			Search();	// 探索
		}
		else
		{
			Chace();	// 追跡
		}
	}

	if (CManager::GetInstance()->GetMode() != CScene::MODE_TUTORIAL && CManager::GetInstance()->GetMode() != CScene::MODE_TITLE)
	{// チュートリアル以外

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
		m_bJump = true;

		// 起伏との当たり判定
		float fHeight = CMeshField::GetHeight(m_Info.pos);
		if (m_Info.pos.y <= fHeight)
		{
			m_Info.pos.y = fHeight;
			m_bJump = false;
		}

		//当たり判定処理前の位置記憶
		pos = m_Info.pos;

		D3DXVECTOR3 vtxMax = D3DXVECTOR3(50.0f, 120.0f, 50.0f);
		D3DXVECTOR3 vtxMin = D3DXVECTOR3(-50.0f, -10.0f, -50.0f);
		if (CObjectX::Collision(m_Info.pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax, 0.3f))
		{
			m_bJump = false;
		}

		CGimmick::Collision(m_Info.pos, m_Info.posOld, m_Info.move, D3DXVECTOR3(0.0f, 0.0f, 0.0f), vtxMin, vtxMax, 0);

		//追跡モードでかつxzどちらか処理前から変化している
		if (m_bChace == true && m_bJump == false && (m_Info.pos.x != pos.x || m_Info.pos.z != pos.z))
		{
			//ジャンプする必要があるか確認
			CPlayer* pPlayerNear = SearchNearPlayer(FLT_MAX);

			if (pPlayerNear != nullptr && CObjectX::CollisionCloss(pPlayerNear->GetPosition(), m_Info.pos))
			{
				m_Info.move.y = JUMP;
				m_bJump = true;
			}
		}

		//敵同士当たり判定
		this->Collision();
	}
	else if (CManager::GetInstance()->GetMode() == CScene::MODE_TITLE && m_nPointID == ExPattern::POINTID_TITLE)
	{
		pos.z += 10.0f * CManager::GetInstance()->GetSlow()->Get();

		m_Info.pos = pos;
		m_fRotDest = D3DX_PI;

		Adjust();

		// 起伏との当たり判定
		float fHeight = CMeshField::GetHeight(m_Info.pos);
		if (m_Info.pos.y <= fHeight)
		{
			m_Info.pos.y = fHeight;
			m_bJump = false;
		}
	}
}

//===============================================
// 移動
//===============================================
void CEnemy::Move(void)
{
	//敵の更新
	m_Info.move.x += sinf(m_fRotDest - D3DX_PI) * MOVE;
	m_Info.move.z += cosf(m_fRotDest - D3DX_PI) * MOVE;
}

//===============================================
// 回転
//===============================================
void CEnemy::Rotation(void)
{
	m_fRotDest = m_fRotMove - DEFAULT_ROTATE;
}

//===============================================
// ポイント移動
//===============================================
void CEnemy::Trace(void)
{
	CPoint* pPoint = CPoint::GetTop();
	D3DXVECTOR3 posPoint;

	//ポイント位置取得
	for (int cnt = 0; cnt < m_nPointID; cnt++)
	{
		pPoint = pPoint->GetNext();
	}
	if (pPoint != nullptr)
	{
		posPoint = pPoint->GetPoint(m_nPointNum);

		//角度調整
		m_fRotDest = FIX_ROT(atan2f(posPoint.x - m_Info.pos.x, posPoint.z - m_Info.pos.z) + D3DX_PI);

		//近くなったら次のポイント
		if (D3DXVec3Length(&(posPoint - m_Info.pos)) < NEXTPOINT_LENGTH)
		{
			m_nPointNum = (m_nPointNum + 1) % pPoint->GetRegistPointNum();

			//ワープ時間カウント中ならカウント停止
			if (m_nLimitReturn > 0)
			{
				m_nLimitReturn = -1;
			}
		}

		//カウント中ならカウント減らす
		if (m_nLimitReturn > 0)
		{
			m_nLimitReturn--;
			if (m_nLimitReturn == 0)
			{//ドロン
				Warp();
			}
		}
	}
	else
	{//一応ぬるぽの時はぐるぐるするようにする
		m_nPointID = ExPattern::POINTID_FREE;
	}
}

//===============================================
// 探索
//===============================================
void CEnemy::Search(void)
{
	float fLengthNear = FLT_MAX;
	CPlayer* pPlayerNear = SearchNearPlayer(SEARCH_RADIUS, &fLengthNear);

	if (pPlayerNear != nullptr && fLengthNear <= SEARCH_LENGTH && pPlayerNear->GetAction() != CPlayer::ACTION_HENGE)
	{//プレイヤー見つけてかつ相手が隠れ身の術
		m_bChace = true;
		if (nullptr != m_pFov)
		{
			m_pFov->SetLength(CHACE_LENGTH);
			m_pFov->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f));

			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SEEK);
		}
	}
	else
	{
		if (m_nPointID != ExPattern::POINTID_FREE && m_nPointID != ExPattern::POINTID_TITLE)
		{//移動パターンあり
			Trace();
		}
		else if (m_nPointID == ExPattern::POINTID_FREE)
		{//適当にぐるぐる
			Rotation();	// 回転
		}
	}
}

//===============================================
// 追跡
//===============================================
void CEnemy::Chace(void)
{
	CPoint* pPoint = CPoint::GetTop();
	int nPointNear = -1;
	float fLengthNear = FLT_MAX;
	CPlayer* pPlayerNear = SearchNearPlayer(SEARCH_RADIUS, &fLengthNear);

	if (pPlayerNear != nullptr && fLengthNear <= ATTACK_LENGTH)
	{//攻撃範囲
		m_Info.move.x = 0.0f;
		m_Info.move.z = 0.0f;
		if (m_nCounterAttack <= 0)
		{//クールタイム終了
			if (m_Info.state == STATE_NORMAL) {	// 通常状態のときのみ
				pPlayerNear->Damage(1);				
			}

			m_nCounterAttack = ATTACK_COOLTIME;
		}
	}
	else if (pPlayerNear != nullptr && fLengthNear <= CHACE_LENGTH)
	{//追跡範囲
		D3DXVECTOR3 posPlayer = pPlayerNear->GetPosition();
		m_fRotDest = FIX_ROT(atan2f(posPlayer.x - m_Info.pos.x, posPlayer.z - m_Info.pos.z) + D3DX_PI);
	}
	else
	{
		//探索状態にする
		m_bChace = false;
		m_pFov->SetLength(SEARCH_LENGTH);
		m_pFov->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.4f));

		//一番近いポイントに移動するようにする
		//ポイント取得
		for (int cnt = 0; cnt < m_nPointID; cnt++)
		{
			pPoint = pPoint->GetNext();
		}
		if (pPoint != nullptr)
		{
			//近いの探索
			fLengthNear = FLT_MAX;
			for (int cnt = 0; cnt < pPoint->GetRegistPointNum(); cnt++)
			{
				D3DXVECTOR3 posPoint = pPoint->GetPoint(cnt);
				float fLength = D3DXVec3Length(&(posPoint - m_Info.pos));
				if (fLengthNear > fLength)
				{
					fLengthNear = fLength;
					nPointNear = cnt;
				}
			}
			//次の移動先を設定
			m_nPointNum = nPointNear;

			//元の軌道に戻るまでの制限時間設定
			m_nLimitReturn = RETURN_TIME_LIMIT;
		}
		else
		{//一応ぬるぽの時はぐるぐるするようにする
			m_nPointID = ExPattern::POINTID_FREE;
		}
	}
}

//===============================================
// 死亡処理
//===============================================
void CEnemy::Death(void)
{
	// 落とした分生成（playerから拝借）
	for (int nCnt = 0; nCnt < DROP_COIN; nCnt++)
	{
		CItem *pItem = CItem::Create(m_Info.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CItem::TYPE_COIN, CItem::STATE_DROP);

		if (nullptr != pItem)
		{
			D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.04f;
			move.y = 18.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.04f;
			pItem->SetMove(move);
		}
	}

	CModel *pModel = m_pLeg->GetParts(0);  // 腰のパーツ

	// 煙のパーティクル生成
	CParticle::Create(D3DXVECTOR3(pModel->GetMtx()->_41, pModel->GetMtx()->_42, pModel->GetMtx()->_43), CEffect::TYPE_BLACKSMAKE);

	//自滅
	Uninit();
}

//===============================================
// 敵同士当たり判定
//===============================================
void CEnemy::Collision(void)
{
	CEnemy *pObj = m_pTop;	// 先頭取得

	//仮置き
	D3DXVECTOR3 posTemp = m_Info.posOld;

	//X
	posTemp.x = m_Info.pos.x;
	CollisionAllEnemy(posTemp);

	//Y
	posTemp.y = m_Info.pos.y;
	CollisionAllEnemy(posTemp);

	//Z
	posTemp.z = m_Info.pos.z;
	CollisionAllEnemy(posTemp);

	m_Info.pos = posTemp;
}

//===============================================
// 敵すべてに当たり判定チェック
//===============================================
D3DXVECTOR3 CEnemy::CollisionAllEnemy(D3DXVECTOR3 pos)
{
	CEnemy *pObj = m_pTop;	// 先頭取得
	while (pObj != nullptr)
	{
		CEnemy *pObjNext = pObj->m_pNext;
		if (pObj != this)
		{
			pObj->CollisionCheck(pos, m_Info.posOld, m_Info.move, ENEMY_VTX_MIN, ENEMY_VTX_MAX);
		}

		pObj = pObjNext;
	}

	return pos;
}

//===============================================
// 復帰用ワープ（ドロン）
//===============================================
void CEnemy::Warp(void)
{
	//ポイント取得
	CPoint* pPoint = CPoint::GetTop();
	for (int cnt = 0; cnt < m_nPointID; cnt++)
	{
		pPoint = pPoint->GetNext();
	}

	//移動（同じものを生成）する
	CEnemy* pNewEnemy = Create(pPoint->GetPoint(m_nPointNum), this->m_Info.rot, this->m_Info.move, nullptr, nullptr, this->m_nPointID);
	pNewEnemy->m_nPointNum = m_nPointNum + 1;
	pNewEnemy->m_nLimitReturn = -1;

	//ドロン（今の敵を破棄）する
	this->Uninit();
	CParticle::Create(this->m_Info.pos, CEffect::TYPE_SMAKE);


}

//===============================================
// 敵単体当たり判定
//===============================================
void CEnemy::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, const float fRefMulti)
{
	//X
	if (pos.z + vtxMax.z > m_Info.pos.z + vtxMin.z
		&& pos.z + vtxMin.z < m_Info.pos.z + vtxMax.z)
	{//範囲内にある
		if (posOld.x + vtxMin.x >= m_Info.pos.x + vtxMax.x
			&& pos.x + vtxMin.x < m_Info.pos.x + vtxMax.x)
		{//右から左にめり込んだ
			move.x *= -1.0f;
			move.x *= fRefMulti;
			pos.x = m_Info.pos.x + vtxMax.x - vtxMin.x + 0.1f + move.x;
		}
		else if (posOld.x + vtxMax.x <= m_Info.pos.x + vtxMin.x
			&& pos.x + vtxMax.x > m_Info.pos.x + vtxMin.x)
		{//左から右にめり込んだ
		 //位置を戻す
			move.x *= -1.0f;
			move.x *= fRefMulti;
			pos.x = m_Info.pos.x + vtxMin.x - vtxMax.x - 0.1f + move.x;
		}
	}

	//Z
	if (pos.x + vtxMax.x > m_Info.pos.x + vtxMin.x
		&& pos.x + vtxMin.x < m_Info.pos.x + vtxMax.x)
	{//範囲内にある
		if (posOld.z + vtxMin.z >= m_Info.pos.z + vtxMax.z
			&& pos.z + vtxMin.z < m_Info.pos.z + vtxMax.z)
		{//奥から手前にめり込んだ
		 //位置を戻す
			move.z *= -1.0f;
			move.z *= fRefMulti;
			pos.z = m_Info.pos.z + vtxMax.z - vtxMin.z + 0.1f + move.z;
		}
		else if (posOld.z + vtxMax.z <= m_Info.pos.z + vtxMin.z
			&& pos.z + vtxMax.z > m_Info.pos.z + vtxMin.z)
		{//手前から奥にめり込んだ
		 //位置を戻す
			move.z *= -1.0f;
			move.z *= fRefMulti;
			pos.z = m_Info.pos.z + vtxMin.z - vtxMax.z - 0.1f + move.z;
		}
	}
}

//===============================================
// 近いプレイヤー探索
//===============================================
CPlayer* CEnemy::SearchNearPlayer(float fRadiusRest, float* pLength)
{
	CPlayer* pPlayer = CPlayer::GetTop();
	CPlayer* pPlayerNear = nullptr;
	float fLengthNear = FLT_MAX;

	while (pPlayer != nullptr)
	{
		if (pPlayer->GetLife() > 0)
		{//生きている奴を計測
			D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
			D3DXVECTOR3 vecPos = posPlayer - this->m_Info.pos;
			float fLength = D3DXVec3Length(&vecPos);
			D3DXVec3Normalize(&vecPos, &vecPos);
			D3DXVECTOR3 vecGaze = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			vecGaze.x = cosf(this->m_Info.rot.y + 0.5f * D3DX_PI);
			vecGaze.z = -sinf(this->m_Info.rot.y + 0.5f * D3DX_PI);

			float fRadius = D3DXVec3Dot(&vecGaze, &vecPos) / (D3DXVec3Length(&vecGaze) * D3DXVec3Length(&vecPos));
			if (fLengthNear > fLength && fRadius >= 1.0f - fRadiusRest 
				&& CObjectX::CollisionCloss(posPlayer,this->m_Info.pos) == false && CGimmick::CollisionCloss(posPlayer, this->m_Info.pos) == false)
			{//一番近いやつ
				pPlayerNear = pPlayer;
				fLengthNear = fLength;
			}
		}

		pPlayer = pPlayer->GetNext();
	}

	if (pLength != nullptr)
	{//距離も欲しい
		*pLength = fLengthNear;
	}

	return pPlayerNear;	//近いプレイヤーのポインタを返す
}

//===============================================
// 調整
//===============================================
void CEnemy::Adjust(void)
{
	m_fRotDest = FIX_ROT(m_fRotDest);

	m_fRotDiff = m_fRotDest - m_fRotMove;	//目標までの移動方向の差分
	m_fRotDiff = FIX_ROT(m_fRotDiff);

	m_Info.rot.y += m_fRotDiff * ROT_MULTI;
	m_Info.rot.y = FIX_ROT(m_Info.rot.y);
}

//===============================================
// 状態管理
//===============================================
void CEnemy::StateSet(void)
{
	m_Info.nStateCounter--;

	if (m_Info.nStateCounter > 0)
	{
		return;
	}

	switch (m_Info.state)
	{
	case STATE_APPEAR:
	{
		m_Info.state = STATE_NORMAL;
	}
		break;

	case STATE_NORMAL:
	{

	}
		break;

	case STATE_DAMAGE:
	{
		m_Info.state = STATE_NORMAL;
	}
		break;

	case STATE_DEATH:
	{
		if (m_pBody == nullptr) {
			return;
		}

		if (m_pBody->GetMotion() == nullptr) {
			return;
		}

		if (m_pLeg == nullptr) {
			return;
		}

		if (m_pLeg->GetMotion() == nullptr) {
			return;
		}

		if (m_pBody->GetMotion()->GetEnd() && m_pBody->GetMotion()->GetNowKey() == m_pBody->GetMotion()->GetNowNumKey() - 1
			&& m_pBody->GetMotion()->GetNowMotion() == MOTION_DEATH) {	// モーション終了かつ死亡モーション
			Death();
		}
		else if (m_pBody->GetMotion()->GetEnd() && m_pBody->GetMotion()->GetNowKey() == m_pBody->GetMotion()->GetNowNumKey() - 1
			&& m_pBody->GetMotion()->GetNowMotion() == MOTION_DOWN) {	// モーション終了かつ死亡モーション
			m_pBody->GetMotion()->BlendSet(MOTION_DEATH);
			m_pLeg->GetMotion()->BlendSet(MOTION_DEATH);
		}
	}

		break;

	case STATE_SPAWN:
	{
		m_Info.state = STATE_APPEAR;
		m_Info.nStateCounter = APPEAR_INTERVAL;
	}
		break;
		
	case STATE_BLOW:
	{
		m_Info.state = STATE_APPEAR;
		m_Info.nStateCounter = APPEAR_INTERVAL;
	}
		break;
	}
}

//===============================================
// ダメージ処理
//===============================================
void CEnemy::Damage(int nDamage) 
{ 
	int nOldLife = m_nLife;
	m_nLife = MINUS_GUARD(m_nLife - nDamage);

	// エフェクトの生成
	if (m_pBody != nullptr) {
		if (m_pBody->GetParts(HEADPARTS_IDX) != nullptr) {
			D3DXVECTOR3 pos = D3DXVECTOR3(m_pBody->GetParts(HEADPARTS_IDX)->GetMtx()->_41, 
				m_pBody->GetParts(HEADPARTS_IDX)->GetMtx()->_42, 
				m_pBody->GetParts(HEADPARTS_IDX)->GetMtx()->_43);
			CParticle::Create(pos, CEffect::TYPE_HIT);
		}
	}

	if (m_nLife <= 0)
	{//死
		m_Info.state = STATE_DEATH;
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DEATH);

		if (m_pBody == nullptr)
		{
			return;
		}

		if (m_pBody->GetMotion() == nullptr) {
			return;
		}

		m_pBody->GetMotion()->Set(MOTION_DOWN);

		if (m_pLeg == nullptr) {
			return;
		}

		if(m_pLeg->GetMotion() == nullptr){
			return;
		}

		m_pLeg->GetMotion()->Set(MOTION_DOWN);

		if (nullptr != m_pFov)
		{
			m_pFov->Uninit();
			m_pFov = nullptr;
		}

		return;
	}

	if (m_nLife != nOldLife)
	{
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DAMAGE);
		m_Info.nStateCounter = DAMAGE_INTERVAL;
		m_Info.state = STATE_DAMAGE;
	}
}

//===============================================
// 体力設定
//===============================================
void CEnemy::SetLife(int nLife)
{
	m_nLife = MINUS_GUARD(nLife);
}

//===============================================
// 攻撃のヒット確認
//===============================================
bool CEnemy::HitCheck(D3DXVECTOR3 pos, float fRange, int nDamage)
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
// モーション設定
//===============================================
void CEnemy::MotionSet(void)
{
	if (m_Info.state == STATE_DEATH) {	// 死亡状態
		return;
	}

	if (m_pBody == nullptr) {	// 胴体無し
		return;
	}

	if (m_pBody->GetMotion() == nullptr) {	// モーション無し
		return;
	}

	if (m_pLeg == nullptr) {	// 下半身無し
		return;
	}

	if(m_pLeg->GetMotion() == nullptr){	// モーション無し
		return;
	}

	if (m_Info.state == STATE_DAMAGE) {	// ダメージ状態
		m_pBody->GetMotion()->Set(MOTION_DAMAGE);
		m_pLeg->GetMotion()->Set(MOTION_DAMAGE);
	}
	else if (m_Info.state == STATE_BLOW) {	// ダメージ状態
		m_pBody->GetMotion()->Set(MOTION_DAMAGE);
		m_pLeg->GetMotion()->Set(MOTION_DAMAGE);
	}
	else if (m_nCounterAttack > 0) {	// 攻撃中
		m_pBody->GetMotion()->Set(MOTION_ATK);
		m_pLeg->GetMotion()->Set(MOTION_ATK);
	}
	else if (m_bJump) {	// ジャンプ状態
		m_pBody->GetMotion()->BlendSet(MOTION_JUMP);
		m_pLeg->GetMotion()->BlendSet(MOTION_JUMP);
	}
	else if (m_bChace) {	// チェイス中
		m_pBody->GetMotion()->BlendSet(MOTION_CHASEMOVE);
		m_pLeg->GetMotion()->BlendSet(MOTION_CHASEMOVE);
	}
	else {	// 待機状態
		if (m_pBody->GetMotion()->GetEnd()) {	// モーションが終了している
			m_pBody->GetMotion()->BlendSet(MOTION_MOVE);
		}

		if (m_pLeg->GetMotion()->GetEnd()) {	// モーションが終了している
			m_pLeg->GetMotion()->BlendSet(MOTION_MOVE);
		}

		if (m_pLeg->GetMotion()->GetNowMotion() == MOTION_MOVE && 
			m_pLeg->GetMotion()->GetNowFrame() == 0 && 
			(m_pLeg->GetMotion()->GetNowKey() == 0 || m_pLeg->GetMotion()->GetNowKey() == 2))
		{
			CParticle::Create(m_Info.pos, CEffect::TYPE_WALK);
		}
	}
}

//===============================================
// 使用階層構造の設定
//===============================================
void CEnemy::BodySet(void)
{
	// 下半身更新
	if (m_pLeg != nullptr)
	{// 使用されている場合

		m_pLeg->Update();

		// 腰の設定
		if (m_pWaist != nullptr)
		{
			CModel *pModel = m_pLeg->GetParts(0);

			// 腰の高さを補填
			m_pWaist->SetPosition(m_pWaist->GetSetPosition() + pModel->GetCurrentPosition());
			m_pWaist->SetMatrix();
		}
	}

	// 胴体更新
	if (m_pBody != nullptr)
	{// 使用されている場合
		m_pBody->Update();
	}
}

//===============================================
// マトリックス設定
//===============================================
void CEnemy::SetMatrix(void)
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
// 吹き飛ばされる
//===============================================
void CEnemy::Blow(void) {
	m_Info.nStateCounter = DAMAGE_INTERVAL;
	m_Info.state = STATE_BLOW;
}

//===============================================
// 開始時の演出追加
//===============================================
void CEnemy::GameStart(void)
{
	m_Info.pos.x += 5.0f * CManager::GetInstance()->GetSlow()->Get();
	m_fRotMove = m_Info.rot.y;	//現在の向きを取得
	m_fRotDest = -D3DX_PI * 0.5f;

	
	m_pBody->GetMotion()->Set(MOTION_MOVE);
	m_pLeg->GetMotion()->Set(MOTION_MOVE);

	Adjust();

	// 起伏との当たり判定
	float fHeight = CMeshField::GetHeight(m_Info.pos);
	if (m_Info.pos.y <= fHeight)
	{
		m_Info.pos.y = fHeight;
		m_bJump = false;
	}
}