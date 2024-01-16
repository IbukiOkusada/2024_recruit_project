//===============================================
//
// ランキング画面の管理処理 [ranking.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _RANKING_H_		// このマクロが定義されていない場合
#define _RANKING_H_		// 二重インクルード防止用マクロを定義

#include "manager.h"

//===============================================
// マクロ定義
//===============================================
#define NUM_RANK	(3)		// ランキングの順位数
#define NUM_RANKING (2)     // 表示するランキングの数
#define MAX_RANKING (4)
#define NUM_NOWSCORE (2)	//スコア

// 前方宣言
class CScore;
class CFileLoad;
class CCarManager;
class CNumber;
class CObject2D;

//===============================================
// ランキングクラスの定義(派生クラス)
//===============================================
class CRanking : public CScene
{
public:

	enum TYPE
	{
		TYPE_ONE = 0,	// 順位
		TYPE_TEAM,		// 新記録
		TYPE_MAX
	};

	// メンバ関数
	CRanking();		// コンストラクタ
	~CRanking();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetScore(int nValue) { m_nScore = nValue; }
	static void SetTotalScore(int nValue) { m_nTotalScore = nValue; }

private:

	// メンバ関数
	void Save(int *pScore, const char *pFileName);
	void Load(int *pScore, const char *pFileName);
	void Sort(int *pScore);
	void RankIn(int *pScore, int nResult, const char *pFileName, int nNew);

	// メンバ変数
	CFileLoad *m_pFileLoad;	// ファイル読み込みのポインタ
	static CScore *m_apScore[NUM_RANKING][NUM_RANK];	// スコアのポインタ(順位分)
	static CScore *m_apNowScore[NUM_NOWSCORE];			// スコアのポインタ(個人、チーム)
	static int m_nScore;		// 今回のスコア
	static int m_nTotalScore;	// 今回のスコア
	int m_nTimer;				// 自動遷移タイマー
	int m_nRank;
	CCarManager *m_pCarManager;		// カーマネージャーのポインタ
	int m_nCounter;
	CObject2D *m_pObjectRank[NUM_RANKING][NUM_RANK];
	CObject2D *m_pObject[MAX_RANKING];

	bool m_bOne;	//トップになったか
	bool m_bTotal;	//トップになったか
	int m_nOne;	//トップになったか
	int m_nTotal;	//トップになったか
};

#endif