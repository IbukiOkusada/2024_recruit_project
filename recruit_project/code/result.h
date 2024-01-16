//===============================================
//
// リザルト画面の管理処理 [result.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _RESULT_H_		// このマクロが定義されていない場合
#define _RESULT_H_		// 二重インクルード防止用マクロを定義

#include "manager.h"

class CScore;
class CFileLoad;
class CMeshDome;
class CTime;
class CCharacter;
class CObject2D;

#define MAX_RANK	(4)		// ランキングの順位数

//===============================================
// リザルトクラスの定義(派生クラス)
//===============================================
class CResult : public CScene
{
public:
	
	// 種類列挙型
	enum TYPE
	{
		TYPE_MULTI_WIN,		// マルチ勝利
		TYPE_MULTI_LOSE,	// マルチ敗北
		TYPE_MAX			// 通常
	};

public:

	// メンバ関数
	CResult();	// コンストラクタ
	~CResult();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetScore(CPlayer **ppPlayer);
	static void SetType(TYPE type) { m_type = type; }
	static TYPE GetType(void) { return m_type; }
	static void SetQuata(const int nValue) { m_nQuota = nValue; }
	static void SetNumPlayer(int nNum) { m_nNumPlayer = nNum; }

private:

	// メンバ関数
	int SumScore(void);
	void Sort(int *pScore);
	void SetRank(int nNum);
	void SetTopScore(int *pScore);

	// メンバ変数
	CFileLoad *m_pFileLoad;	// ファイル読み込みのポインタ
	CMeshDome *m_pMeshSky;		// 空用
	CTime *m_pTime;			// タイマー
	CScore *m_pTotalScore;		// 合計スコアオブジェクト
	CObject2D **m_ppRank;
	CObject2D *m_pObjClear;	// clearしたかどうか
	static CScore **m_apScore;	// スコアのポインタ(順位分)
	static CPlayer **m_ppPlayer;	// プレイヤーのポインタ
	int m_nTimer;				// 遷移タイマー
	int m_nWorst;				// 最下位の順位
	int *m_pRank;				// ランク
	int m_nNowScore;
	int m_nTotalScore;
	bool m_bClear;			// 達成かどうか
	float m_ClearHeight;
	static int *m_pScore;		// 今回のスコア
	static int m_nTopScore;   // 一位のスコア
	static TYPE m_type;		// 種類
	static int m_nQuota;		// ノルマ
	static int m_nNumPlayer;  // ゴールしたプレイヤーの人数
};

#endif