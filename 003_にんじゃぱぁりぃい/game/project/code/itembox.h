//==========================================================
//
// アイテムボックス [itembox.h]
// Author : Soma Ishihara
//
//==========================================================
#ifndef _ITEMBOX_H_		// このマクロが定義されていない場合
#define _ITEMBOX_H_		// 二重インクルード防止用マクロを定義

#include "gimmick.h"

//前方宣言
class CObjectX;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CItemBox : public CGimmick
{
public:	// 誰でもアクセス可能

	CItemBox();	// コンストラクタ(オーバーロード)
	~CItemBox();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CItemBox *Create(const D3DXVECTOR3 pos,const D3DXVECTOR3 rot);
	void Emission(void);
	bool CollisionCheckCloss(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3* posCollisioned) { return false; }

	// メンバ関数(取得)
	static CItemBox* GetTop(void) { return m_pTop; }
	CItemBox* GetNext(void) { return m_pNext; }

	// メンバ関数(設定)

private:	// 自分だけがアクセス可能

	// メンバ関数
	bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand = nullptr);

	// メンバ変数
	static CItemBox *m_pTop;	// 先頭のオブジェクトへのポインタ
	static CItemBox *m_pCur;	// 最後尾のオブジェクトへのポインタ
	CItemBox *m_pPrev;		// 前のオブジェクトへのポインタ
	CItemBox *m_pNext;		// 次のオブジェクトへのポインタ
	CObjectX* m_pObj;			// オブジェクト

	int m_nCounter;				//排出CT
	int m_nParticleCounter;		// パーティクルカウンター
};

#endif

