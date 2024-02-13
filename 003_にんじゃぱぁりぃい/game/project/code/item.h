//==========================================================
//
// アイテムの処理 [item.h]
// Author : Yuuka Sakuma
//
//==========================================================
#ifndef _ITEM_H_	// このマクロが定義されていない場合
#define _ITEM_H_	// 二重インクルード防止用マクロを定義

#include "task.h"

// 前方宣言
class CModel;

//==========================================================
// アイテムクラスの定義(派生クラス)
//==========================================================
class CItem : public CTask
{
public:

	enum STATE
	{
		STATE_NORMAL = 0,	// 通常
		STATE_DROP,		// ドロップしたもの
		STATE_CRASH,			//崩れる
		STATE_DOWN,			//落下
		STATE_MAX
	};

	enum TYPE
	{
		TYPE_COIN,         // コイン
		TYPE_BRECELET,     // ブレスレット
		TYPE_CUP,          // 盃
		TYPE_GEM00,        // エメラルド
		TYPE_GEM01,        // ダイヤモンド
		TYPE_GOLDBAR,      // 金塊
		TYPE_JAR,          // 瓶
		TYPE_KUNAI,        // クナイ
		TYPE_RING00,       // 腕輪
		TYPE_SCROLL,       // 巻物
		TYPE_SHURIKEN,     //手裏剣
		TYPE_MAX
	};

public:	// 誰でもアクセス可能

	CItem();		// コンストラクタ
	~CItem();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	HRESULT Init(int type);
	void Uninit(void);
	void Update(void);

	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type, int nType = TYPE_COIN);
	static CItem *Collision(D3DXVECTOR3 &pos);

	// メンバ関数(設定)
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetOldPos(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetState(STATE state) { m_nState = state; }
	void SetType(int type) { m_type = type; }
	void SetNext(CItem *pNext) { m_pNext = pNext; }
	void SetPrev(CItem *pPrev) { m_pPrev = pPrev; }

	// メンバ関数(取得)
	static CItem *GetTop(void) { return m_pTop; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	D3DXVECTOR3 GetOldPos(void) { return m_posOld; }
	int GetState(void) { return m_nState; }
	int GetType(void) { return m_type; }
	int GetEachScore(void);
	CItem *GetNext(void) { return m_pNext; }
	CItem *GetPrev(void) { return m_pPrev; }
	CModel *GetModel(void) { return m_pObject; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	bool CollisionCheck(D3DXVECTOR3 &pos);

	// メンバ変数
	static CItem *m_pTop;	// 先頭のオブジェクトへのポインタ
	static CItem *m_pCur;	// 最後尾のオブジェクトへのポインタ
	CItem *m_pPrev;	// 前のオブジェクトへのポインタ
	CItem *m_pNext;	// 次のオブジェクトへのポインタ
	CModel *m_pObject;
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_posOld;	// 前回の座標
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	int m_type;
	int m_nBound;		// bound回数
	float m_fCurve;
	int m_nState;
};

#endif