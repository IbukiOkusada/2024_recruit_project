//==========================================================
//
// UI [ui.h]
// Author : Yuuka Sakuma
//
//==========================================================
#ifndef _UI_H_		// このマクロが定義されていない場合
#define _UI_H_		// 二重インクルード防止用マクロを定義

//#include "../task.h"
#include "task.h"	// これでファイルインクルードできます

//前方宣言
class CObject2D;
class CLife;

//==========================================================
// UIのクラス定義
//==========================================================
class CUI : public CTask
{

public:	// 誰でもアクセス可能

	enum TYPE
	{
		TYPE_NORMAL = 0,	// なんもない
		TYPE_LEFTUP,		// 左上
		TYPE_RIGHTUP,		// 右上
		TYPE_LEFTDOWN,		// 左下
		TYPE_RIGHTDOWN,		// 右下
		TYPE_MAX
	};

	// フレームの種類
	enum FRAME
	{
		FRAME_GREEN = 0,		// 1P
		FRAME_BLUE,				// 2P
		FRAME_RED,				// 3P
		FRAME_YELLOW,			// 4P
		FRAME_MAX
	};
	// アイコンの種類
	enum ICON
	{
		ICON_GREEN = 0,		// 1P
		ICON_BLUE,			// 2P
		ICON_RED,			// 3P
		ICON_YELLOW,		// 4P
		ICON_MAX
	};

	CUI();	// コンストラクタ(オーバーロード)
	~CUI();	// デストラクタ

					// メンバ関数
	HRESULT Init(void);
	HRESULT Init(int nFrame, int nIcon);
	void Uninit(void);
	void Update(void);
	static CUI *Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot, int nFrame, int nIcon,TYPE type);

	// メンバ関数(取得)
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	TYPE GetType(void) { return m_type; }
	int GetLife(void) { return m_nLife; }

	// メンバ関数(設定)
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetType(TYPE type) { m_type = type; }
	void SetLife(int nLife);

private:	// 自分だけがアクセス可能

	#define NUM_OBJ			(3)			//オブジェクトの数

			// メンバ関数

			// メンバ変数
	CObject2D *m_pObject[NUM_OBJ];
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	TYPE m_type;
	const static char *m_apFrameFileName[FRAME_MAX];	// 初期読み込みファイル名
	const static char *m_apIconFileName[ICON_MAX];	// 初期読み込みファイル名
	int m_nLife;
	int m_nNumPlayer;
	CLife *m_pLife;

};

#endif


