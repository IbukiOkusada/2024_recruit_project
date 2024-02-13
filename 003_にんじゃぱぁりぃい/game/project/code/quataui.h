//==========================================================
//
// ノルマのUI [quataui.h]
// Author : Ryosuke Ohara
//
//==========================================================
#ifndef _QUATAUI_H_		// このマクロが定義されていない場合
#define _QUATAUI_H_		// 二重インクルード防止用マクロを定義

//#include "../task.h"
#include "task.h"	// これでファイルインクルードできます

//前方宣言
class CObject2D;
class CLife;

//==========================================================
// UIのクラス定義
//==========================================================
class CQuataUI : public CTask
{

public:	// 誰でもアクセス可能

	// UIの種類
	enum QUATATYPE
	{
		TYPE_START = 0,	// 〇〇点盗め
		TYPE_FRAME,		// ノルマ達成
		TYPE_MAX
	};

	enum STATE
	{
		STATE_NONE = 0, // なんもない
		STATE_MOVE,     // 動いている
		STATE_SCALING,  // 拡縮
		STATE_UP,       // 上に上がる
		STATE_CLEAR,    // α値が下がる
		STATE_CHANGE,   // テクスチャ切り替え
		STATE_SET,      // タイマーの下
		STATE_MAX
	};

	struct INFO
	{
		D3DXVECTOR3 m_pos;
	    D3DXVECTOR3 m_rot;
		D3DXCOLOR m_col;
	    QUATATYPE m_type;
		STATE m_state;
		float m_fHeight;
		float m_fWidht;
		float fStateCounter;
	};

	CQuataUI(int nPriority = 6);	// コンストラクタ(オーバーロード)
	~CQuataUI();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	HRESULT Init(float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	static CQuataUI *Create(D3DXVECTOR3 pos, QUATATYPE type, STATE state, float fWidth, float fHeight);

	// メンバ関数(取得)
	D3DXVECTOR3 GetPosition(void) { return m_Info.m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_Info.m_rot; }
	D3DXCOLOR GetColor(void) { return m_Info.m_col; }
	STATE GetState(void) { return m_Info.m_state; }
	float GetHeight(void) { return m_Info.m_fHeight; }
	float GetWight(void) { return m_Info.m_fWidht; }
	float GetCounter(void) { return m_Info.fStateCounter; }
	QUATATYPE GetType(void) { return m_Info.m_type; }

	// メンバ関数(設定)
	void SetPosition(D3DXVECTOR3 pos) { m_Info.m_pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_Info.m_rot = rot; }
	void SetColor(D3DXCOLOR col) { m_Info.m_col = col; }
	void SetState(STATE state) { m_Info.m_state = state; }
	void SetHeight(float fHeight) { m_Info.m_fHeight = fHeight; }
	void SetWight(float fWight) { m_Info.m_fWidht = fWight; }
	void SetType(QUATATYPE type) { m_Info.m_type = type; }

private:	// 自分だけがアクセス可能

	// メンバ関数

	// メンバ変数
	CObject2D *m_pObject;
	INFO m_Info;
	const static char *m_apQuataFileName[TYPE_MAX];	// 初期読み込みファイル名
	int m_nNumPlayer;
};

#endif