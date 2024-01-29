//==========================================================
//
// ロックオンの処理 [lockon.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _LOCKON_H_
#define _LOCKON_H_

#include "task.h"

// 前方宣言
class CEnemy;
class CObjectBillboard;

//**********************************************************
// ロックオンクラスの定義
//**********************************************************
class CLockOn : public CTask
{
public:	// 誰でもアクセス可能な定義
	// ロックオン種類
	typedef enum 
	{
		TYPE_NONE = 0,	// 何もない
		TYPE_TARGET,	// 単体ロックオン
		TYPE_MULTI,		// 複数ロックオン
		TYPE_MAX
	}TYPE;

public:	// 誰でもアクセス可能

	CLockOn();	// コンストラクタ
	~CLockOn();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetParent(D3DXMATRIX *pMtx);
	bool GetLock(void) { return m_bLock; }
	void SetLock(bool bLock);
	static CLockOn *Create(D3DXMATRIX *pMtx, TYPE type = TYPE_TARGET);
	void SetTag(CEnemy *pEnemy) { m_pTarget = pEnemy; }
	static CLockOn *GetTop(void) { return m_pTop; }
	CLockOn *GetNext(void) { return m_pNext; }
	CEnemy *GetEnemy(void) { return m_pTarget; }
	TYPE GetType(void) { return m_type; }
	bool GetDeath(void) { return m_bDeath; }
	static void Check(CEnemy* pObject);
	CObjectBillboard* GetObj(void) { return m_pObj; }
	void DeathCheck(void);
	static void MultiDeath(void);

private:	// 自分だけがアクセス可能

	// メンバ関数
	void LockOn(void);

	static CLockOn *m_pTop;	// 先頭のオブジェクトへのポインタ
	static CLockOn *m_pCur;	// 最後尾のオブジェクトへのポインタ
	static int m_nNum;
	CLockOn *m_pPrev;		// 前のオブジェクトへのポインタ
	CLockOn *m_pNext;		// 次のオブジェクトへのポインタ
	CEnemy *m_pTarget;			// ロックオンするモデル
	CObjectBillboard* m_pObj;
	D3DXMATRIX *m_pMtx;		// 親のマトリックス
	bool m_bLock;			// ロックオンしているかどうか
	bool m_bUse;			// 使用するかどうか
	bool m_bDeath;			// 死亡フラグ
	TYPE m_type;			// ロックオン種類
};

#endif

