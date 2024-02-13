//==========================================================
//
// 敵の処理 [enemy.h]
// Author : Soma Ishihara (Base:Ibuki Okusada)
//
//==========================================================
#ifndef _ENEMY_H_		// このマクロが定義されていない場合
#define _ENEMY_H_		// 二重インクルード防止用マクロを定義

#include "main.h"
#include "object.h"
#include "objectX.h"
#include "task.h"

// 前方宣言
class CPlayer;
class CWaist;
class CCharacter;
class CObject3DFan;

namespace ExPattern
{
	const int POINTID_FREE = -1;
	const int POINTID_TITLE = -2;
	const int POINTID_GAMESTART = -3;
}

//==========================================================
// 敵のクラス定義(派生クラス)
//==========================================================
class CEnemy : public CTask
{
private:	// 自分だけがアクセス可能な定義

	// 状態列挙型
	enum STATE
	{
		STATE_APPEAR = 0,	// 出現状態
		STATE_NORMAL,		// 通常状態
		STATE_SPAWN,		// 生成状態
		STATE_DAMAGE,		// ダメージ状態
		STATE_DEATH,		// 死亡状態
		STATE_BLOW,		// 吹き飛ばされ状態
		STATE_MAX
	};

	// モーション列挙型
	enum MOTION {
		MOTION_NEUTRAL = 0,	// 待機
		MOTION_MOVE,			// 移動
		MOTION_CHASEMOVE,		// チェイス中の移動
		MOTION_JUMP,			// ジャンプ状態
		MOTION_ATK,			// 攻撃
		MOTION_DAMAGE,		// ダメージ
		MOTION_DOWN,			// 吹っ飛びダウン
		MOTION_DEATH,			// 死亡
		MOTION_MAX
	};

	// 情報構造体
	struct SInfo
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
		D3DXVECTOR3 move;		// 移動量
		D3DXVECTOR3 posOld;	// 設定位置
		D3DXMATRIX mtxWorld;	// ワールドマトリックス
		D3DXVECTOR3 posDiff;	// 目標の座標
		STATE state;			// 状態
		int nStateCounter;	// 状態管理カウンター
	};

public:	// 誰でもアクセス可能

	CEnemy();	// コンストラクタ
	~CEnemy();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	HRESULT Init(const char *pBodyName, const char *pLegName);	// オーバーロード
	void Uninit(void);
	void Update(void);
	static CEnemy *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move,
		const char *pBodyName, const char *pLegName, const int nPointID = -1);
	bool HitCheck(D3DXVECTOR3 pos, float fRange, int nDamage = 1);

	// メンバ関数(設定)
	void SetMove(const D3DXVECTOR3 move) { m_Info.move = move; }
	void SetPosition(const D3DXVECTOR3 pos) { m_Info.pos = pos; m_Info.posDiff = pos; }
	void SetDiffPosition(const D3DXVECTOR3 pos) { m_Info.posDiff = pos; }
	void SetRotation(const D3DXVECTOR3 rot) { m_Info.rot = rot; }
	void BindId(int nId) { m_nId = nId; }
	void SetType(TYPE type){ m_type = type; }
	void SetPointID(const int nID) { m_nPointID = nID; }

	// メンバ関数(取得)
	D3DXVECTOR3 GetMove(void) { return m_Info.move; }
	D3DXVECTOR3 GetPosition(void) { return m_Info.pos; }
	D3DXVECTOR3 GetRotation(void) { return m_Info.rot; }
	int GetId(void) { return m_nId; }
	static CEnemy *GetTop(void) { return m_pTop; }
	CEnemy *GetNext(void) { return m_pNext; }
	void Damage(int nDamage);
	void SetLife(int nLife);
	void Blow(void);
	static int GetNum(void) { return m_nNumCount; }
	int GetLife(void) { return m_nLife; }
	int GetPointID(void) { return m_nPointID; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void SetMatrix(void);
	void StateSet(void);
	void Controller(void);
	void Move(void);
	void Rotation(void);
	void Trace(void);		//ポイントIDがある
	void Adjust(void);		//ない
	void Search(void);
	void Chace(void);
	void Death(void);
	void MotionSet(void);
	void BodySet(void);
	void Collision(void);
	void GameStart(void);
	void CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, const float fRefMulti = 0.5f);
	CPlayer* SearchNearPlayer(float fRadiusRest, float* pLength = nullptr);
	D3DXVECTOR3 CollisionAllEnemy(D3DXVECTOR3 pos);
	void Warp(void);

	// メンバ変数
	static CEnemy *m_pTop;	// 先頭のオブジェクトへのポインタ
	static CEnemy *m_pCur;	// 最後尾のオブジェクトへのポインタ
	CEnemy *m_pPrev;		// 前のオブジェクトへのポインタ
	CEnemy *m_pNext;		// 次のオブジェクトへのポインタ
	SInfo m_Info;			// 自分自身の情報
	CWaist *m_pWaist;		// 腰
	CCharacter *m_pBody;	// 上半身
	CCharacter *m_pLeg;	// 下半身
	CObject3DFan* m_pFov;	// 視野オブジェクト
	CPlayer *m_pChase;	// 追跡相手
	float m_fRotMove;		// 現在の角度
	float m_fRotDiff;		// 目的の角度
	float m_fRotDest;		// 角度計算
	bool m_bJump;			// ジャンプ
	int m_nLife;	// 体力
	int m_nCounterAttack;	//攻撃カウンター
	bool m_bChace;	//追跡モードか否か
	int m_nId;	// ID
	TYPE m_type;	// 種類
	int m_nPointID;	//ポイントID
	int m_nPointNum;	//ポイントNo
	int m_nLimitReturn;	//帰って来るまでの制限時間
	static int m_nNumCount;

};

#endif
