//==========================================================
//
// チェックポイント処理 [checkpoint.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _CHECKPOINT_H_		// このマクロが定義されていない場合
#define _CHECKPOINT_H_		// 二重インクルード防止用マクロを定義

#include "task.h"	// これでファイルインクルードできます

//==========================================================
// サンプルのクラス定義
//==========================================================
class CCheckPoint : public CTask
{
public:

	// 種類列挙型
	enum TYPE
	{
		TYPE_MELEE = 0,		// 近接型(近距離)
		TYPE_GUN,			// 銃(遠距離)
		TYPE_BOSS,			// ボス
		TYPE_MAX
	};

protected:

	// 情報構造体
	struct SInfo
	{
		D3DXVECTOR3 pos;		// 位置
	};

public:	// 誰でもアクセス可能

	CCheckPoint();	// コンストラクタ(オーバーロード)
	~CCheckPoint();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// メンバ関数(取得)
	CCheckPoint* GetNext(void) { return m_pNext; }
	CCheckPoint* GetPrev(void) { return m_pPrev; }
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	static CCheckPoint* Create(const D3DXVECTOR3& pos);

	// メンバ関数(設定)
	void SetNext(CCheckPoint* pNext) { m_pNext = pNext; }
	void SetPrev(CCheckPoint* pPrev) { m_pPrev = pPrev; }
	void SetPosition(const D3DXVECTOR3& pos) { m_pos = pos; }

	// メンバ関数
	void Hit(const D3DXVECTOR3& pos, D3DXVECTOR3& CheckPointPos);

private:	// 自分だけがアクセス可能

	// メンバ変数
	CCheckPoint* m_pPrev;	// 前のオブジェクトへのポインタ
	CCheckPoint* m_pNext;	// 次のオブジェクトへのポインタ
	D3DXVECTOR3 m_pos;		// 位置
};

#endif