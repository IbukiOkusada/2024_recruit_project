//==========================================================
//
// 敵の出現管理処理 [enemy_manager.h]
// Author : Ibuki Okusada
//
//==========================================================
#include "enemy_manager.h"
#include "manager.h"
#include "object.h"
#include "time.h"
#include "enemy.h"
#include "meshfield.h"

// マクロ定義
#define NEXTWAVECNT	(12)	// 各wave開始カウント

const char *CEnemyManager::m_apEnemyFileName[ENEMY_MAX] =
{
	"data\\TXT\\motion_society.txt",
	"data\\TXT\\motion_society_yellow.txt",
	"data\\TXT\\motion_kidsboy.txt",
	"data\\TXT\\motion_kidsgirl.txt",
};

//==========================================================
// コンストラクタ
//==========================================================
CEnemyManager::CEnemyManager()
{
	// 値のクリア
	m_nMaxEnemy = 0;
	m_nTimer = 0;
	m_nDeadCounter = 0;
	m_nSetTimer = 0;
	m_nSuvCounter = 0;
	m_nRouteSetPlus = 0;
	m_nSetNum = 0;
	m_nRouteRandRange = 0;
	m_nTypeSetPlus = 0;
	m_nTypeRandRange = 0;
}

//==========================================================
// デストラクタ
//==========================================================
CEnemyManager::~CEnemyManager()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CEnemyManager::Init(void)
{
	CEnemy::Create("data\\TXT\\motion_society_yellow.txt");
	m_nSetNum = 3;
	m_nSetTimer = NEXTWAVECNT;
	m_nRouteRandRange = NUM_ROUTE;
	m_nTypeRandRange = ENEMY_MAX;

	// 現在の最大人数を覚える
	m_nMaxEnemy = CObject::GetNumEnemAll();

	return true;
}

//==========================================================
// 終了処理
//==========================================================
void CEnemyManager::Uninit(void)
{

}

//==========================================================
// 更新処理
//==========================================================
void CEnemyManager::Update(void)
{
	CTime *pTime = CManager::GetScene()->GetTime();

	if (pTime == NULL)
	{
		return;
	}

	if (pTime->GetAnim() == 0 && pTime->GetNum() % m_nSetTimer == 0 && pTime->GetNum() > pTime->GetStartNum() && pTime->GetNum() > 0)
	{// 次のウェーブ
		// 出現
		int nNextNumEnemy = CObject::GetNumEnemAll() + m_nSetNum;
		Spawn(nNextNumEnemy);
	}
}

//==========================================================
// 出現処理
//==========================================================
void CEnemyManager::Spawn(int nSetNum)
{
	// 敵の配置
	while (1)
	{
		if (CObject::GetNumEnemAll() < nSetNum)
		{
			if (CManager::GetMode() != CScene::MODE_TUTORIAL)
			{
				CEnemy *pEnemy = CEnemy::Create(m_apEnemyFileName[rand() % m_nTypeRandRange + m_nTypeSetPlus]);
				pEnemy->SetRoute(rand() % m_nRouteRandRange + m_nRouteSetPlus);
			}
			else
			{
				CEnemy::Create("data\\TXT\\motion_murabito.txt");
			}
		}
		else
		{
			break;
		}
	}
}

//==========================================================
// チュートリアル出現処理
//==========================================================
void CEnemyManager::UpdateTutorial(void)
{
	m_nFrameTimer++;

	if (m_nFrameTimer % 60 == 0)
	{
		m_nFrameTimer = 0;
		m_nTimer++;
	}

	if (m_nTimer % (int)(NEXTWAVECNT * 0.25f) == 0 && m_nFrameTimer == 0)
	{// 次のウェーブ
		m_nTimer = 0;
		// 出現
		int NextNumEnemy = CObject::GetNumEnemAll() + 1;
		Spawn(NextNumEnemy);
	}
}

//==========================================================
// ルートランダム範囲設定
//==========================================================
void CEnemyManager::SetRouteRange(int nRange, int nSetPlus)
{
	m_nRouteRandRange = nRange;
	m_nRouteSetPlus = nSetPlus;
}

//==========================================================
// 種類ランダム設定
//==========================================================
void CEnemyManager::SetTypeRange(int nRange, int nSetPlus)
{
	m_nTypeRandRange = nRange;
	m_nTypeSetPlus = nSetPlus;
}

//==========================================================
// 配置数設定
//==========================================================
void CEnemyManager::SetNum(int nNum)
{
	m_nSetNum = nNum;
}