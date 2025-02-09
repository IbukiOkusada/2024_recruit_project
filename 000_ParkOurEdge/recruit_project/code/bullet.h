//==========================================================
//
// 弾 [bullet.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _BULLET_H_		// このマクロが定義されていない場合
#define _BULLET_H_		// 二重インクルード防止用マクロを定義

#include "task.h"	// これでファイルインクルードできます

// 前方宣言
class CObjectBillboard;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CBullet : public CTask
{
public:

	// 種類列挙型
	enum TYPE {
		TYPE_ENEMY = 0,			// 敵
		TYPE_BOSS,
		TYPE_MAX
	};

private:

	// 情報構造体
	struct SInfo {
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 posOld;	// 
		D3DXVECTOR3 rot;	// 向き
		D3DXVECTOR3 move;	// 移動量
		D3DXMATRIX mtxWorld;	// ワールドマトリックス
	};

public:	// 誰でもアクセス可能

	CBullet();	// コンストラクタ(オーバーロード)
	~CBullet();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CBullet *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &move, const TYPE type);

	// メンバ関数(取得)
	D3DXVECTOR3 GetPosition(void) const { return m_Info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_Info.rot; }
	D3DXVECTOR3 GetMove(void) const { return m_Info.move; }
	TYPE GetType(void) { return m_type; }

	// メンバ関数(設定)
	void SetPosition(const D3DXVECTOR3 &pos) { m_Info.pos = pos; }
	void SetRotation(const D3DXVECTOR3 &rot) { m_Info.rot = rot; }
	void SetMove(const D3DXVECTOR3 &move) { m_Info.move = move; }
	void BindId(const int nId) { m_nId = nId; }
	void Hit(void);

private:	// 自分だけがアクセス可能

	// メンバ関数
	void Controller(void);
	void SetMatrix(void);
	void SetType(const TYPE type) { m_type = type; }

	// メンバ変数
	SInfo m_Info;			// 基本情報
	int m_nId;				// ID
	CObjectBillboard *m_pObject;	// オブジェクトのポインタ
	float m_fInterVal;		// インターバル
	float m_fLife;			// 寿命
	TYPE m_type;
};

#endif
