//==========================================================
//
// 弾の処理 [bullet.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _BULLET_H_		// このマクロが定義されていない場合
#define _BULLET_H_		// 二重インクルード防止用マクロを定義

#include "main.h"
#include "object3D.h"

// 前方宣言
class CMeshOrbit;

//==========================================================
// 弾のクラス定義(派生クラス)
//==========================================================
class CBullet : public CObject3D
{
private:	// 自分だけがアクセス可能な定義

	// 構造体の定義
	typedef struct
	{
		CObject *pObj;
		float fSpeed;		// 速さ
	}LockOn;

public:	// 誰でもアクセス可能

	// 種類列挙型の定義
	typedef enum
	{
		TYPE_NONE = 0,		// 通常
		TYPE_GRAVITY,		// 重力
		TYPE_SHOWER,		// シャワー
		TYPE_EXPLOSION,		// 爆発
		TYPE_MAX
	}TYPE;

	CBullet();	// コンストラクタ
	CBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move);	// コンストラクタ(オーバーロード)
	~CBullet();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE type);
	void SetLife(float fLife) { m_fLife = fLife; }
	void SetHom(CObject *pObj, float fSpeed);
	static void Check(CObject *pObject, CBullet *pBullet = NULL);
	void DeathCheck(void);
	void SetInerMove(D3DXVECTOR3 move);

	static int GetNumAll(void) { return m_nNumAll; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void Controller(void);
	bool Collision(D3DXVECTOR3 pos, CObject::TYPE ObjType);

	// メンバ変数
	static CBullet *m_pTop;	// 先頭のオブジェクトへのポインタ
	static CBullet *m_pCur;	// 最後尾のオブジェクトへのポインタ
	static int m_nNumAll;	// 弾の総数
	CBullet *m_pPrev;		// 前のオブジェクトへのポインタ
	CBullet *m_pNext;		// 次のオブジェクトへのポインタ
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_Inermove;	// 完成をつけた移動量
	LockOn *m_pTarget;		// ホーミング用
	float m_fLife;			// 寿命
	float m_fParTimer;		// パーティクルタイマー
	int m_nType;			// 種類
	int m_nChangeTimer;		// 種類変更タイマー
	int m_nIdxTexture;		// テクスチャ番号
	bool m_bDeath;
	CMeshOrbit *m_pOrbit;	// 軌跡のポインタ
	CMeshOrbit *m_pOrbit2;	// 軌跡のポインタ
};

#endif
