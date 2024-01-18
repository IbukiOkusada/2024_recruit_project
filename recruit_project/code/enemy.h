//==========================================================
//
// 敵の処理 [enemy.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _ENEMY_H_		// このマクロが定義されていない場合
#define _ENEMY_H_		// 二重インクルード防止用マクロを定義

#include "task.h"	// これでファイルインクルードできます

//==========================================================
// サンプルのクラス定義
//==========================================================
class CEnemy : public CTask
{
protected:

	// 種類列挙型
	enum TYPE
	{
		TYPE_MELEE = 0,		// 近接型(近距離)
		TYPE_SHIELD,		// 盾持ち
		TYPE_GUN,			// 銃(遠距離)
		TYPE_MAX
	};

protected:

	// 情報構造体
	struct SInfo
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
		D3DXVECTOR3 move;		// 移動量
		D3DXVECTOR3 posOld;		// 前回の座標
		D3DXMATRIX mtxWorld;	// ワールドマトリックス
	};

public:	// 誰でもアクセス可能

	CEnemy();	// コンストラクタ(オーバーロード)
	~CEnemy();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// メンバ関数(取得)
	CEnemy* GetNext(void) { return m_pNext; }
	CEnemy* GetPrev(void) { return m_pPrev; }

	// メンバ関数(設定)
	void SetNext(CEnemy* pNext) { m_pNext = pNext; }
	void SetPrev(CEnemy* pPrev) { m_pPrev = pPrev; }

	// メンバ関数
	virtual void Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage);

protected:

	// メンバ変数
	TYPE GetType(void) { return m_type; }
	void SetType(TYPE type) { m_type = type; }
	SInfo* GetInfo(void) { return &m_Info; }
	void SetInfo(const SInfo& Info) { m_Info = Info; }
	void SetMatrix(void);
	int GetLife(void) { return m_nLife; }
	void SetLife(const int nLife) { m_nLife = nLife; }

private:	// 自分だけがアクセス可能

	// メンバ変数
	CEnemy* m_pPrev;	// 前のオブジェクトへのポインタ
	CEnemy* m_pNext;	// 次のオブジェクトへのポインタ
	int m_nLife;		// 体力
	TYPE m_type;		// 種類
	SInfo m_Info;		// 情報
};

#endif
