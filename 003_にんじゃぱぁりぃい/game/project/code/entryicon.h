//==========================================================
//
// チュートリアルのエントリーのアイコン [entryicon.h]
// Author : Ryosuke Ohara
//
//==========================================================
#ifndef _ENTRYICON_H_		// このマクロが定義されていない場合
#define _ENTRYICON_H_		// 二重インクルード防止用マクロを定義

#include "task.h"

// 前方宣言
class CObject2D;

//===============================================
// ナンバークラスの定義
//===============================================
class CEntryIcon : public CTask
{
public:	// 誰でもアクセス可能

	CEntryIcon();	// コンストラクタ
	~CEntryIcon();	// デストラクタ

	enum STATE
	{
		STATE_STANDBY = 0,          // エントリー受付中
		STATE_ENTRY,                // エントリー完了
		STATE_MAX
	};

	// 情報
	struct Info
	{
		D3DXVECTOR3 pos;            // 位置
		D3DXCOLOR col;              // 色
		STATE state;                // 状態
		float fPolyWidth;	        // オブジェクトの幅
		float fPolyHeight;	        // オブジェクトの高さ
		bool bEntry;                // エントリーしたかどうか
	};

	// メンバ関数
	HRESULT Init(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CEntryIcon *Create(void);
	static CEntryIcon *Create(D3DXVECTOR3 Pos, const int nIdx, const float fPolyWidth, const float fPolyHeight);
	void Entryed(void);
	void NoEntry(void);

	// 設定
	void SetState(STATE state) { m_Info.state = state; }
	void SetbEntry(bool bEntry) { m_Info.bEntry = bEntry; }
	void SetCol(D3DXCOLOR col) { 
		m_Info.col = col; 
	}

	// 取得
	CObject2D *GetObjectBill(void) { return m_pObject; }
	static float GetCol(void) { return m_Allcol; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void SetPolySize(const float fWidth, const float fHeight);

	// メンバ変数
	D3DXCOLOR col;
	Info m_Info;                        // 情報
	CObject2D *m_pObject;	            // オブジェクト2Dのポインタ
	int m_nIdxPlayer;                   // プレイヤーの番号
	static float m_Allcol;
	bool m_bChangeTex;                  // テクスチャが変わっている
};

#endif