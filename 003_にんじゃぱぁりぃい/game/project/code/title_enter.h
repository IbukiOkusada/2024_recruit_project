//==========================================================
//
// タイトルエンター [title_enter.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _TITLE_ENTER_H_		// このマクロが定義されていない場合
#define _TITLE_ENTER_H_		// 二重インクルード防止用マクロを定義

// 前方宣言
class CItem;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CTitleEnter
{
public:	// 誰でもアクセス可能
	CTitleEnter();	// コンストラクタ(オーバーロード)
	~CTitleEnter();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// メンバ関数(取得)

	// メンバ関数(設定)
	void SetItemParent(CItem *pItem);
	void SetPosition(D3DXVECTOR3 &pos) { m_pos = pos; }
	void SetRotation(D3DXVECTOR3 &rot) { m_rot = rot; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void SetMatrix(void);

	// メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 向き
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
};

#endif

