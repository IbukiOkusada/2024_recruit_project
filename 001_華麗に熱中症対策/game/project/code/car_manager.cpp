//===============================================
//
// 車全般の管理処理(car_manager.cpp)
// Author : Ibuki Okusada
//
//===============================================
#include "car_manager.h"
#include "car.h"
#include "manager.h"
#include "slow.h"
#include "shadow.h"
#include "Xfile.h"
#include "sound.h"

// 定数定義
namespace
{
	const float LOAD_POSX = (-200.0f);	// 生成X座標
	const float LOAD_POSY = (-30.0f);	// 生成Y座標
	const float LOAD_POSZ = (-4200.0f);	// 生成Z座標
	const float MOVE_Z = (11.0f);		// Z移動量
	const int SPAWN_DEF = (120);		// 基本生成カウント
	const int SPAWN_RAND = (180);		// ランダム分生成カウント
	const int SET_RANKNUM = (6);		// ランキング画面初期配置数
}

//===============================================
// 静的メンバ変数
//===============================================
const char *CCarManager::m_apFileName[MAX_CAR] = 
{
	"data\\MODEL\\car000.x",
	"data\\MODEL\\car001.x",
	"data\\MODEL\\car002.x",
	"data\\MODEL\\car003.x",
	"data\\MODEL\\car004.x",
	"data\\MODEL\\car005.x",
};

//===============================================
// 静的メンバ変数
//===============================================
const D3DXVECTOR2 CCarManager::m_aShadowSize[MAX_CAR] =
{
	{ 100.0f, 200.0f },
	{ 100.0f, 200.0f },
	{ 100.0f, 200.0f },
	{ 100.0f, 200.0f },
	{ 180.0f, 720.0f },
	{ 180.0f, 720.0f },
};

//===============================================
// コンストラクタ
//===============================================
CCarManager::CCarManager()
{
	// 値のクリア
	for (int nCnt = 0; nCnt < LOAD_MAX; nCnt++)
	{
		m_aSpawn[nCnt].fCount = 0.0f;
		m_aSpawn[nCnt].fDef = 0.0f;
		m_aSpawn[nCnt].nNum = 0;
		m_apTop[nCnt] = NULL;
		m_apCur[nCnt] = NULL;
	}
}

//===============================================
// デストラクタ
//===============================================
CCarManager::~CCarManager()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CCarManager::Init(void)
{
	for (int nCnt = 0; nCnt < LOAD_MAX; nCnt++)
	{
		m_aSpawn[nCnt].fDef = SPAWN_DEF;
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CCarManager::Uninit(void)
{

}

//===============================================
// 更新処理
//===============================================
void CCarManager::Update(void)
{
	// 生成管理
	Spawn();

	// 廃棄
	Delete();
}

//===============================================
// ランキングの初期配置
//===============================================
void CCarManager::SetRanking(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(LOAD_POSX, LOAD_POSY, LOAD_POSZ);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, MOVE_Z);
	float fAddPos = 1400.0f;

	// 車線分繰り返し
	for (int nCnt = 0; nCnt < LOAD_MAX; nCnt++)
	{
		for (int nCntSet = 0; nCntSet < SET_RANKNUM; nCntSet++)
		{
			int nRand = rand() % MAX_CAR;

			CCar *pCar = NULL;
			rot.y = D3DX_PI * nCnt;

			pCar = CCar::Create(pos, rot, m_apFileName[nRand]);
			pCar->SetMove(move);
			pCar->GetShadow()->SetpVtx(m_aShadowSize[nRand].x, m_aShadowSize[nRand].y);

			// リストに設定
			BindList(pCar, (LOAD)nCnt);

			pos.z += fAddPos;
		}

		pos = D3DXVECTOR3(LOAD_POSX, LOAD_POSY, LOAD_POSZ);
		pos.x *= -1.0f;
		pos.z *= -1.0f;
		move.z *= -1.0f;
		fAddPos *= -1.0f;
	}
}

//===============================================
// リストにセット
//===============================================
bool CCarManager::BindList(CCar *pCar, LOAD load)
{
	if (load >= LOAD_RIGHT && load < LOAD_MAX)
	{// 範囲内
		// 自分自身をリストに追加
		if (m_apTop[load] != NULL)
		{// 先頭が存在している場合
			m_apCur[load]->SetNext(pCar);	// 現在最後尾のオブジェクトのポインタにつなげる
			pCar->SetPrev(m_apCur[load]);
			m_apCur[load] = pCar;	// 自分自身が最後尾になる
		}
		else
		{// 存在しない場合
			m_apTop[load] = pCar;	// 自分自身が先頭になる
			m_apCur[load] = pCar;	// 自分自身が最後尾になる
		}
	}
	else
	{// リスト追加失敗
		return false;
	}

	// 総数増加
	m_aSpawn[load].nNum++;

	return true;
}

//===============================================
// リストから外す
//===============================================
void CCarManager::DetachList(CCar *pCar, LOAD load)
{
	// リストから自分自身を削除する
	if (m_apTop[load] == pCar)
	{// 自身が先頭
		if (m_apTop[load]->GetNext() != NULL)
		{// 次が存在している
			m_apTop[load] = m_apTop[load]->GetNext();	// 次を先頭にする
			m_apTop[load]->SetPrev(NULL);	// 次の前のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_apTop[load] = NULL;	// 先頭がない状態にする
			m_apCur[load] = NULL;	// 最後尾がない状態にする
		}
	}
	else if (m_apCur[load] == pCar)
	{// 自身が最後尾
		if (m_apCur[load]->GetPrev() != NULL)
		{// 手前が存在している
			m_apCur[load] = pCar->GetPrev();	// 前を最後尾にする
			m_apCur[load]->SetNext(NULL);		// 前の次のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_apTop[load] = NULL;	// 先頭がない状態にする
			m_apCur[load] = NULL;	// 最後尾がない状態にする
		}
	}
	else
	{// 間
		if (pCar->GetNext() != NULL)
		{// 次が存在している
			pCar->GetNext()->SetPrev(pCar->GetPrev());	// 自身の次に前のポインタを覚えさせる
		}
		if (pCar->GetPrev() != NULL)
		{// 手前が存在している
			pCar->GetPrev()->SetNext(pCar->GetNext());	// 自身の前に次のポインタを覚えさせる
		}
	}

	// 総数減少
	m_aSpawn[load].nNum--;
}

//===============================================
// 生成処理
//===============================================
void CCarManager::Spawn(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(LOAD_POSX, LOAD_POSY, LOAD_POSZ);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, MOVE_Z);

	// 車線分繰り返し
	for (int nCnt = 0; nCnt < LOAD_MAX; nCnt++)
	{
		m_aSpawn[nCnt].fCount += CManager::GetSlow()->Get();

		if (m_aSpawn[nCnt].fCount < m_aSpawn[nCnt].fDef)
		{// 規定値に達していない
			pos.x *= -1.0f;
			pos.z *= -1.0f;
			move.z *= -1.0f;
			continue;
		}

		int nRand = rand() % MAX_CAR;

		CCar *pCar = NULL;
		rot.y = D3DX_PI * nCnt;
		if (CManager::GetMode() != CScene::MODE_TUTORIAL)
		{
			pCar = CCar::Create(pos, rot, m_apFileName[nRand], nRand);
			pCar->SetMove(move);
			pCar->GetShadow()->SetpVtx(m_aShadowSize[nRand].x, m_aShadowSize[nRand].y);
		}
		else
		{
			pCar = CCar::Create(pos, rot, m_apFileName[4], nRand);
			pCar->SetMove(move);
			pCar->GetShadow()->SetpVtx(m_aShadowSize[4].x, m_aShadowSize[4].y);
		}

		// リストに設定
		BindList(pCar, (LOAD)nCnt);
		m_aSpawn[nCnt].fCount = 0.0f;
		m_aSpawn[nCnt].fDef = SPAWN_DEF + (float)(rand() % SPAWN_RAND);

		pos.x *= -1.0f;
		pos.z *= -1.0f;
		move.z *= -1.0f;
	}
}

//===============================================
// 生成
//===============================================
void CCarManager::Delete(void)
{
	// 左車線
	{
		CCar *pCar = m_apTop[LOAD_LEFT];

		while (pCar != NULL)
		{
			CCar *pCarNext = pCar->GetNext();
			float fMoveZ = pCar->GetMove().z;
			float fPosZ = pCar->GetPosition().z;

			if (fPosZ < LOAD_POSZ)
			{// 端まで行った
				// 終了
				DetachList(pCar, LOAD_LEFT);
				pCar->Uninit();
			}

			pCar = pCarNext;
		}
	}

	// 右車線
	{
		CCar *pCar = m_apTop[LOAD_RIGHT];

		while (pCar != NULL)
		{
			CCar *pCarNext = pCar->GetNext();
			float fMoveZ = pCar->GetMove().z;
			float fPosZ = pCar->GetPosition().z;

			if (fPosZ > -LOAD_POSZ)
			{// 端まで行った
				// 終了
				DetachList(pCar, LOAD_RIGHT);
				pCar->Uninit();
			}

			pCar = pCarNext;
		}
	}
}

//===============================================
// 当たり判定
//===============================================
CCar *CCarManager::Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, bool *pJump)
{
	CXFile *pFile = CManager::GetModelFile();
	bool bLand = true;	// 着地したか否か
	*pJump = true;

	COLLISION target(pos, posOld, vtxMax, vtxMin, move);

	// 車線分繰り返し
	for (int nCnt = 0; nCnt < LOAD_MAX; nCnt++)
	{
		CCar *pCar = m_apTop[nCnt];

		while (pCar != NULL)
		{
			CCar *pCarNext = pCar->GetNext();
			D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// 向きを反映
			pCar->SetRotSize(vtxObjMax,
				vtxObjMin,
				pFile->GetMax(pCar->GetIdx()),
				pFile->GetMin(pCar->GetIdx()),
				pCar->GetRotation().y);

			COLLISION car(pCar->GetPosition(), pCar->GetOldPos(), vtxObjMax, vtxObjMin, pCar->GetMove());

			if (pos.x + vtxMax.x > pCar->GetPosition().x + vtxObjMin.x
				&& pos.x + vtxMin.x < pCar->GetPosition().x + vtxObjMax.x
				&& pos.z + vtxMax.z > pCar->GetPosition().z + vtxObjMin.z
				&& pos.z + vtxMin.z < pCar->GetPosition().z + vtxObjMax.z)
			{//範囲内にある
			 //上からの判定
				if (posOld.y + vtxMin.y >= pCar->GetPosition().y + vtxObjMax.y
					&& pos.y + vtxMin.y < pCar->GetPosition().y + vtxObjMax.y)
				{//上からめり込んだ
				 //上にのせる
					pos.y = pCar->GetPosition().y + vtxObjMax.y - vtxMin.y;
					pos += pCar->GetMove();
					move.y = 0.0f;
					*pJump = false;

					return pCar;
				}
			}

			if (pos.y + vtxMax.y > pCar->GetPosition().y + vtxObjMin.y
				&& pos.y + vtxMin.y <= pCar->GetPosition().y + vtxObjMax.y)
			{//プレイヤーとモデルが同じ高さにある
				// 衝突判定を取る
				if (CollisionCheck(&car, &target, pCar->GetType()))
				{
					pos = target.pos;
					posOld = target.posOld;
					move = target.move;
					return nullptr;
				}
			}

			pCar = pCarNext;
		}
	}

	return NULL;
}

//===============================================
// 当たり判定
//===============================================
bool CCarManager::CollisionCheck(COLLISION* car, COLLISION* target, const int nType)
{
	if (target->posOld.x + target->vtxMin.x >= car->pos.x + car->vtxMax.x
		&& target->pos.x + target->vtxMin.x < car->pos.x + car->vtxMax.x
		&& target->pos.z + target->vtxMax.z > car->pos.z + car->vtxMin.z
		&& target->pos.z + target->vtxMin.z < car->pos.z + car->vtxMax.z)
	{//右から左にめり込んだ
		target->pos.x = car->pos.x + car->vtxMax.x - target->vtxMin.x + 0.1f;
		target->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		target->pos.y += 5.0f;
		target->move.x = rand() % 20 * 0.1f + 15.0f;
		target->move.y = 7.0f;
		CManager::GetSound()->Play(CSound::LABEL_SE_CARHIT);

		if (nType >= 0 && nType < MAX_CAR - 2)
		{
			CManager::GetSound()->Play(CSound::LABEL_SE_CAR);
		}
		else if (nType == MAX_CAR - 2)
		{
			CManager::GetSound()->Play(CSound::LABEL_SE_TRACK);
		}
		else if (nType == MAX_CAR - 1)
		{
			CManager::GetSound()->Play(CSound::LABEL_SE_BUS);
		}

		return true;
	}
	else if (target->posOld.x + target->vtxMax.x <= car->pos.x + car->vtxMin.x
		&& target->pos.x + target->vtxMax.x > car->pos.x + car->vtxMin.x
		&& target->pos.z + target->vtxMax.z > car->pos.z + car->vtxMin.z
		&& target->pos.z + target->vtxMin.z < car->pos.z + car->vtxMax.z)
	{//左から右にめり込んだ
	 //位置を戻す
		target->pos.x = car->pos.x + car->vtxMin.x - target->vtxMax.x - 0.1f;
		target->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		target->pos.y += 5.0f;
		target->move.x = rand() % 20 * -0.1f + -15.0f;
		target->move.y = 7.0f;
		CManager::GetSound()->Play(CSound::LABEL_SE_CARHIT);

		if (nType >= 0 && nType < MAX_CAR - 2)
		{
			CManager::GetSound()->Play(CSound::LABEL_SE_CAR);
		}
		else if (nType == MAX_CAR - 2)
		{
			CManager::GetSound()->Play(CSound::LABEL_SE_TRACK);
		}
		else if (nType == MAX_CAR - 1)
		{
			CManager::GetSound()->Play(CSound::LABEL_SE_BUS);
		}

		return true;
	}
	else if ((target->posOld.z + target->vtxMin.z >= car->posOld.z + car->vtxMax.z
		&& target->pos.z + target->vtxMin.z < car->pos.z + car->vtxMax.z)
		&& (target->pos.x + target->vtxMax.x > car->pos.x + car->vtxMin.x
			&& target->pos.x + target->vtxMin.x < car->pos.x + car->vtxMax.x))
	{//奥から手前にめり込んだ
		//位置を戻す
		target->pos.z = car->pos.z + car->vtxMax.z - target->vtxMin.z + 0.1f;
		target->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		target->pos.y += 5.0f;
		target->move.y = 7.0f;
		target->move.x = rand() % (200 - 100) * 0.1f;
		if (car->move.z > 0.0f)
		{
			if (nType >= 0 && nType < MAX_CAR - 2)
			{
				CManager::GetSound()->Play(CSound::LABEL_SE_CAR);
			}
			else if (nType == MAX_CAR - 2)
			{
				CManager::GetSound()->Play(CSound::LABEL_SE_TRACK);
			}
			else if (nType == MAX_CAR - 1)
			{
				CManager::GetSound()->Play(CSound::LABEL_SE_BUS);
			}

			target->move.z = car->move.z;
		}
		CManager::GetSound()->Play(CSound::LABEL_SE_CARHIT);
		return true;
	}
	else if ((target->posOld.z + target->vtxMax.z <= car->posOld.z + car->vtxMin.z
		&& target->pos.z + target->vtxMax.z > car->pos.z + car->vtxMin.z)
		&& (target->pos.x + target->vtxMax.x > car->pos.x + car->vtxMin.x
			&& target->pos.x + target->vtxMin.x < car->pos.x + car->vtxMax.x))
	{//手前から奥にめり込んだ
		//位置を戻す
		target->pos.z = car->pos.z + car->vtxMin.z - target->vtxMax.z - 0.1f;
		target->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		target->pos.y += 5.0f;
		target->move.y = 7.0f;
		target->move.x = rand() % (200 - 100) * 0.1f;
		if (car->move.z < 0.0f)
		{
			if (nType >= 0 && nType < MAX_CAR - 2)
			{
				CManager::GetSound()->Play(CSound::LABEL_SE_CAR);
			}
			else if (nType == MAX_CAR - 2)
			{
				CManager::GetSound()->Play(CSound::LABEL_SE_TRACK);
			}
			else if (nType == MAX_CAR - 1)
			{
				CManager::GetSound()->Play(CSound::LABEL_SE_BUS);
			}

			target->move.z = car->move.z;
		}
		CManager::GetSound()->Play(CSound::LABEL_SE_CARHIT);
		return true;
	}

	return false;
}