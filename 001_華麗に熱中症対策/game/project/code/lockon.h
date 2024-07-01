//==========================================================
//
// ロックオンの処理 [lockon.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _LOCKON_H_
#define _LOCKON_H_

#include "billboard.h"

// 前方宣言
class CModel;
class CEnemy;
class CObject;

//**********************************************************
// ロックオンクラスの定義
//**********************************************************
class CLockOn : public CObjectBillboard
{
public:		// 誰でもアクセス可能な定義

	// ロックオン種類
	typedef enum 
	{
		TYPE_NONE = 0,	// 何もない
		TYPE_TARGET,	// 単体ロックオン
		TYPE_MULTI,		// multiロックオン
		TYPE_MAX
	}TYPE;

public:	// 誰でもアクセス可能

	CLockOn(int nPriOrity = 5);	// コンストラクタ
	~CLockOn();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetParent(D3DXMATRIX *pMtx);
	bool GetLock(void) { return m_bLock; }
	void SetLock(bool bLock) { m_bUse = bLock; }
	static CLockOn *Create(D3DXMATRIX *pMtx, TYPE type = TYPE_TARGET);
	CObject *GetTag(void);
	void SetTag(CEnemy *pEnemy) { m_pObj = pEnemy; }
	static void Check(CEnemy *pObject);
	void DeathCheck(void);
	static void MultiDeath(void);
	static CLockOn *GetTop(void) { return m_pTop; }
	CLockOn *GetNext(void) { return m_pNext; }
	CEnemy *GetEnemy(void) { return m_pObj; }
	TYPE GetType(void) { return m_type; }
	bool GetDeath(void) { return m_bDeath; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void LockOn(void);

	static CLockOn *m_pTop;	// 先頭のオブジェクトへのポインタ
	static CLockOn *m_pCur;	// 最後尾のオブジェクトへのポインタ
	CLockOn *m_pPrev;		// 前のオブジェクトへのポインタ
	CLockOn *m_pNext;		// 次のオブジェクトへのポインタ
	CEnemy *m_pObj;			// ロックオンするモデル
	D3DXMATRIX *m_pMtx;		// 親のマトリックス
	bool m_bLock;			// ロックオンしているかどうか
	bool m_bUse;			// 使用するかどうか
	bool m_bDeath;			// 死亡フラグ
	TYPE m_type;			// ロックオン種類
};

#endif

