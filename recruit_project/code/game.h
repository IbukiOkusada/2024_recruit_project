//===============================================
//
// ゲーム画面の管理処理 [game.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _GAME_H_		// このマクロが定義されていない場合
#define _GAME_H_		// 二重インクルード防止用マクロを定義

#include "manager.h"
#include <mutex>

// 前方宣言
class CScore;
class CTime;
class CPlayer;
class CFileLoad;
class CClient;
class CMeshDome;
class CMiniMap;
class CPause;
class CQuataUI;
class CGimmickLever;
class CGimmickStartDoor;
class CEnemy;
class CObject2D;

// マクロ定義
#define NUM_FILTER	(2)

//===============================================
// ゲームクラスの定義(派生クラス)
//===============================================
class CGame : public CScene
{
public:

	enum STATE
	{
		STATE_LOCAL = 0,	// ローカル通信
		STATE_ONLINE,		// TCPオンライン
		STATE_END,
		STATE_PAUSE,
		STATE_MAX
	};

public:

	// メンバ関数
	CGame();	// コンストラクタ
	CGame(int nNumPlayer);	// コンストラクタ
	~CGame();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetState(STATE state) { m_state = state; }
	void SendPosition(D3DXVECTOR3& pos);
	void SendRotation(D3DXVECTOR3& rot);
	void SendDamage(int nDamage);
	void SendLife(int nLife);
	void SendSetUp(void);
	void SendGoal(void);
	static void SetNumPlayer(int nNum) { m_nNumPlayer = nNum; }
	static int GetNumPlayer(void) { return m_nNumPlayer; }
	CMiniMap* GetMiniMap(void) { return m_pMiniMap; }
	bool GetbopenDoor(void) { return m_bOpenStartDoor; }

	// メンバ関数(ポインタ)
	CPlayer *GetPlayer(void);
	CFileLoad *GetFileLoad(void);

private:

	// TCP通信用関数
	void Online(void);
	void ByteCheck(char *pRecvData, int nRecvByte);
	void OnlineEnd(void);
	void AddressLoad(char *pAddrss);
	bool EndCheck(void);
	bool StartDirection(void);

	CFileLoad *m_pFileLoad;		// ファイル読み込みのポインタ
	CPlayer **m_ppPlayer;		// プレイヤーのポインタ
	CMultiCamera **m_ppCamera;	// カメラのポインタ
	CMeshDome *m_pMeshDome;		// メッシュドームのポインタ
	CTime *m_pTimer;			// タイマー
	CMiniMap* m_pMiniMap;		// ミニマップ
	CClient *m_pClient;			// クライアントのポインタ
	CScore *m_QuataScore;       // ノルマのスコア
	CQuataUI *m_QuataUI;        // ノルマのUI
	CGimmickLever **m_ppLever;  // レバーのポインタ
	CGimmickStartDoor **m_ppMultiDoor;  // 協力扉のポインタ
	CEnemy *m_pEnemy;			// 敵のポインタ
	CObject2D *m_pAllOpen;      // オブジェクト2Dのポインタ
	char m_aAddress[30];		// 接続先サーバーのアドレス
	static STATE m_state;		// 状態
	int m_nSledCnt;				// 現在動作しているスレッド数
	static int m_nNumPlayer;	// プレイ人数
	CPause *m_pPause;			// ポーズ画面
	WSADATA m_wsaData;
	std::mutex m_mutex;
	bool m_bEnd;
	int m_nStartCnt;			// 開始タイマー
	int m_nCntLostQuataUI;      // ノルマのUIが消えるまでのカウント
	int m_nCntGoal;             // ゴールした人数
	int m_nCntLookGoal;         //
	int m_nOldOpenDoor;         // 
	float m_fOpenDoorUISin;
	bool m_bPause;              // ポーズ
	bool m_bQuota;              // ノルマ達成しているどうか
	bool m_bDispQuataUI;        // ノルマのUIができるかどうか
	bool m_bSetOnceAngle;		// 1度だけの角度設定をしたか
	bool m_bOpenStartDoor;      // スタートのドアがすべて開いているかどうか
	bool m_bAllOpenUI;
};

#endif