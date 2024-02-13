//==========================================================
//
// 仮想の腰処理全般 [waist.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _WAIST_H_	// このマクロが定義されていない場合
#define _WAIST_H_	// 二重インクルード防止用マクロを定義

#include "main.h"		//main.hで定義しているものが必要なためinclude

//==========================================================
// 腰クラスの定義
//==========================================================
class CWaist
{
public:	// 誰でもアクセス可能

	CWaist();	// コンストラクタ
	~CWaist();	// デストラクタ

	// メンバ関数
	void SetMatrix(void);
	void SetParent(D3DXMATRIX *pMtx) { m_pParentMtx = pMtx; }
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetHeight(D3DXVECTOR3& pPos);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	D3DXVECTOR3 GetSetPosition(void) { return m_SetPos; }
	D3DXMATRIX *GetMtxWorld(void) { return &m_mtxWorld; }
	static CWaist *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:	// 自分だけがアクセス可能

	// メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 m_SetPos;	// 設定位置
	D3DXMATRIX *m_pParentMtx;	// 親のワールドマトリックス
};
#endif