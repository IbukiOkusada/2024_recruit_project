//==========================================================
//
// ギミック基底 [gimmick.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_H_		// このマクロが定義されていない場合
#define _GIMMICK_H_		// 二重インクルード防止用マクロを定義

#include "task.h"	// これでファイルインクルードできます

// 前方宣言
class CGimmickRotateDoor;
class CGimmickPull;
class CGimmickButton;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CGimmick : public CTask
{

public:

	// 種類列挙型
	enum TYPE
	{
		TYPE_NONE = 0,	// 何もない
		TYPE_BUTTON,		// ボタン
		TYPE_LEVER,		// レバー
		TYPE_ROTATEDOOR,	// 回転ドア
		TYPE_STARTDOOR,	// 開始地点ドア
		TYPE_SPEAR,		// 槍
		TYPE_PITFALL,	// 落とし穴
		TYPE_MAX
	};

public:	// 誰でもアクセス可能

	CGimmick();	// コンストラクタ(オーバーロード)
	virtual ~CGimmick();	// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;

	// メンバ関数(取得)
	static bool Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick = nullptr, bool* bLand = nullptr);
	static bool CollisionCloss(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3* posCollisioned = nullptr);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	D3DXMATRIX *GetMtxWorld(void) { return &m_mtxWorld; }
	TYPE GetType(void) { return m_type; }
	static void SwitchOff(void);
	static void SwitchOn(void);
	static void Buttonoff(void);
	static CGimmick *GetTop(void) { return m_pTop; }
	CGimmick *GetNext(void) { return m_pNext; }
	int GetId(void) { return m_nActionId; }
	virtual void Switch(bool bUse) {}
	virtual void SetMtxParent(D3DXMATRIX *pMtx) {}
	virtual void IdSetButton(void) {}

	// ダウンキャスト用関数
	virtual CGimmickRotateDoor *GetRotateDoor(void) { return nullptr; }
	virtual CGimmickPull *GetPull(void) { return nullptr; }
	virtual CGimmickButton *GetButton(void) { return nullptr; }

	// メンバ関数(設定)
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetActionId(const int nId) { m_nActionId = nId; }

protected:

	// 種類設定
	void BindType(TYPE type) { m_type = type; }
	void ListOut(void);
	void SetMtxWorld(void);

private:	// 自分だけがアクセス可能

	// メンバ関数
	virtual bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand = nullptr) = 0;
	virtual bool CollisionCheckCloss(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3* posCollisioned = nullptr) = 0;

	// メンバ変数
	static CGimmick *m_pTop;	// 先頭のオブジェクトへのポインタ
	static CGimmick *m_pCur;	// 最後尾のオブジェクトへのポインタ
	CGimmick *m_pPrev;		// 前のオブジェクトへのポインタ
	CGimmick *m_pNext;		// 次のオブジェクトへのポインタ
	TYPE m_type;				// 種類
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	int m_nActionId;			// 自分のアクション№
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
};

#endif

