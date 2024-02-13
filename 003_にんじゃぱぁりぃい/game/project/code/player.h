//==========================================================
//
// プレイヤーの処理 [player.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _PLAYER_H_		// このマクロが定義されていない場合
#define _PLAYER_H_		// 二重インクルード防止用マクロを定義

#include "main.h"
#include "object.h"
#include "objectX.h"
#include "task.h"

// 前方宣言
class CWaist;
class CCharacter;
class CGimmick;
class CScore;
class CCamera;
class CItem;
class CLife;
class CUI;
class CGage;
class CThrowItem;
class CHeadUI;
class CObject2D;

// マクロ定義
#define MAX_ITEM  (1280)  // 所持できるアイテムの最大数

//==========================================================
// プレイヤーのクラス定義(派生クラス)
//==========================================================
class CPlayer : public CTask
{
public:

	// 操作種類列挙型
	enum TYPE
	{
		TYPE_NONE,	// 操作不可能
		TYPE_SEND,	// データ送信
		TYPE_ACTIVE,	// 操作可能
		TYPE_AUTOMOVE,	// 自動移動
		TYPE_MAX
	};


	// アクション列挙型
	enum ACTION
	{
		ACTION_NEUTRAL = 0,	// 待機
		ACTION_WALK,			// 歩行
		ACTION_JUMP,			// ジャンプ
		ACTION_ATK,			// 攻撃
		ACTION_CATCH,			// 持つ
		ACTION_HOLD,			// 保持
		ACTION_THROW,			// 投げる
		ACTION_DAMAGE,		// 攻撃を受けた
		ACTION_FLUTTERING,	// じたばた
		ACTION_HENGE,			// 変化の術
		ACTION_KUNAI,			// クナイの術
		ACTION_AIR,			// 風神の術
		ACTION_MAX
	};

private:	// 自分だけがアクセス可能な定義

	// 状態列挙型
	enum STATE
	{
		STATE_APPEAR = 0,	// 出現状態
		STATE_NORMAL,		// 通常状態
		STATE_DAMAGE,		// ダメージ状態
		STATE_DEATH,		// 死亡状態
		STATE_SPAWN,		// 復活中状態
		STATE_CATCH,		// 掴まれている状態
		STATE_BLOW,		// 吹き飛ばされ状態
		STATE_MAX
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
		float fStateCounter;	// 状態管理カウンター
	};

	// 掴み情報構造体
	struct SCatch
	{
		CGimmick *pGimmick;	// ギミック
		CPlayer *pPlayer;		// プレイヤー
		int nMoveCnt;			// 移動した量
		D3DXVECTOR3 SetPos;	// 設定座標
	};

public:	// 誰でもアクセス可能

	CPlayer();	// コンストラクタ
	~CPlayer();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	HRESULT Init(const char *pBodyName, const char *pLegName);	// オーバーロード
	void Uninit(void);
	void Update(void);
	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move,
		const char *pBodyName, const char *pLegName);
	bool HitCheck(D3DXVECTOR3 pos, float fRange, int nDamage = 1);

	// メンバ関数(設定)
	void SetMove(const D3DXVECTOR3 move) { m_Info.move = move; }
	void SetPosition(const D3DXVECTOR3 pos) { m_Info.pos = pos; m_Info.posDiff = pos; }
	void SetDiffPosition(const D3DXVECTOR3 pos) { m_Info.posDiff = pos; }
	void SetRotation(const D3DXVECTOR3 rot) { m_Info.rot = rot; }
	void BindId(int nId) { m_nId = nId; }
	void SetType(TYPE type);
	void SetCamera(CCamera *pCamera);
	void BindScore(CScore *pScore) { m_pScore = pScore; }
	void BindUI(CUI *pUI);
	void Ninjutsu(void);
	void Blow(void);
	void SetMotion(int nMotion);
	void SetDraw(bool bDraw);

	// メンバ関数(取得)
	D3DXVECTOR3 GetMove(void) { return m_Info.move; }
	D3DXVECTOR3 GetPosition(void) { return m_Info.pos; }
	D3DXVECTOR3 GetRotation(void) { return m_Info.rot; }
	int GetId(void) { return m_nId; }
	static CPlayer *GetTop(void) { return m_pTop; }
	static int GetNum(void) { return m_nNumCount; }
	CPlayer *GetNext(void) { return m_pNext; }
	void Damage(int nDamage);
	void SetLife(int nLife);
	int GetLife(void) { return m_nLife; }
	STATE GetState(void) { return m_Info.state; }
	bool GetGoal(void) { return m_bGoal; }
	bool GetEnd(void) { return m_bEnd; }
	CScore *GetScore(void) { return m_pScore; }
	ACTION GetAction(void) const { return m_action; }
	int GetThrowItemId(void) { return m_nItemId; }
	int GetMotion(void);
	void SetFailedParticle(void);
	TYPE GetType(void) { return m_type; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void SetMatrix(void);
	void StateSet(void);
	void Controller(void);
	void Move(void);
	void Rotation(void);
	void Adjust(void);
	void KeyBoardRotation(void);
	void MoveController(void);
	void Jump(void);
	void MotionSet(void);
	void Attack(void);
	void Catch(void);
	void PlayerCatch(D3DXVECTOR3 pos);
	void SetCatchMatrix(void);
	void Throw(void);
	void Drop(int nDropCnt);
	void DropAll(void);
	void DamageCollision(D3DXVECTOR3 pos);
	void AttackCheck(void);
	void GimmickRelease(void);
	const char *ItemFileName(int type);
	void ItemSort(void);
	void AddItemCount(int type);
	void SubItemCount(int type);
	void SelectItem(void);
	int GetSelectItem(int type);
	void BodySet(void);
	void ChangeBody(void);
	void BulletSet(void);
	void GoalWait(void);
	void SetWarpFadeCol(void);

	// メンバ変数
	static CPlayer *m_pTop;	// 先頭のオブジェクトへのポインタ
	static CPlayer *m_pCur;	// 最後尾のオブジェクトへのポインタ
	CPlayer *m_pPrev;		// 前のオブジェクトへのポインタ
	CPlayer *m_pNext;		// 次のオブジェクトへのポインタ
	SInfo m_Info;			// 自分自身の情報
	SCatch m_Catch;		// 掴みに関する情報
	CWaist *m_pWaist;		// 腰
	CCharacter *m_pBody;	// 上半身
	CCharacter *m_pLeg;		// 下半身
	CScore *m_pScore;       // スコアへのポインタ
	int m_aSaveType[MAX_ITEM];
	float m_fRotMove;		// 現在の角度
	float m_fRotDiff;		// 目的の角度
	float m_fRotDest;		// 角度計算
	float m_fGage;	// 忍術ゲージ
	bool m_bMove;			// 移動したかどうか
	bool m_bJump;			// ジャンプ
	bool m_bGoal;			// ゴールフラグ
	bool m_bEnd;
	int m_nLife;			// 体力
	int m_nId;				// ID
	int m_nNumItemCoin;
	int m_nNumItemBrecetet;
	int m_nNumItemCup;
	int m_nNumItemEmerald;
	int m_nNumItemDiamond;
	int m_nNumItemGold;
	int m_nNumItemJar;
	int m_nNumItemKunai;
	int m_nNumItemRing;
	int m_nNumItemScroll;
	int m_nNumItemShuriken;
	int m_nItemId;
	TYPE m_type;			// 種類
	ACTION m_action;		// アクション
	int m_nItemCnt;		// 
	static int m_nNumCount;
	int m_nQuitCounter;	// 脱出から消えるまでのカウンター
	CCamera *m_pMyCamera;	// 自分用のカメラ
	CUI *m_pUI;
	CGage *m_pGage;		// 忍術ゲージ
	CThrowItem *m_pThrowItem; // 頭の上に出るアイテムのアイコン
	CHeadUI *m_pHeadUI;
	CObject2D *m_pFade;	// ゴール後フェード
	CObject2D *m_pEscape;
	bool m_bAttack;
	bool m_bWarp;
	float m_fFadeCola;	// 透明度変更量
};

#endif
