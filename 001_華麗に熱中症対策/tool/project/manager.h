//===============================================
//
// マネージャーの処理 [manager.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _MANAGER_H_		// このマクロが定義されていない場合
#define _MANAGER_H_		// 二重インクルード防止用マクロを定義

#include "main.h"

// 前方宣言
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CDebugProc;
class CPlayer;
class CCamera;
class CLight;
class CTexture;
class CXFile;
class CFileLoad;
class CEdit;

//===============================================
// マネージャークラスの定義
//===============================================
class CManager
{
public:	// 誰でもアクセス可能

	CManager();	// コンストラクタ
	~CManager();	// デストラクタ

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetInputKeyboard(void);
	static CInputMouse *GetInputMouse(void);
	static CDebugProc *GetDebugProc(void);
	static CPlayer *GetPlayer(void);
	static CCamera *GetCamera(void);
	static CLight *GetLight(void);
	static CTexture *GetTexture(void);
	static CXFile *GetModelFile(void);
	static CFileLoad *GetFileLoad(void);
	static CEdit *GetEdit(void);

private:	// 自分だけがアクセス可能

	// メンバ関数
	void ItemReset(void);
	void SystemLoad(char *pBody, char *pLeg);

	// 静的メンバ変数
	static CRenderer *m_pRenderer;	// レンダラーのポインタ
	static CInputKeyboard *m_pInputKeyboard;	// キーボードのポインタ
	static CInputMouse *m_pInputMouse;	// マウスのポインタ
	static CDebugProc *m_pDebugProc;	// デバッグ表示のポインタ
	static CPlayer *m_pPlayer;		// プレイヤーのポインタ
	static CCamera *m_pCamera;		// カメラのポインタ
	static CLight *m_pLight;		// ライトのポインタ
	static CTexture *m_pTexture;	// テクスチャのポインタ
	static CXFile *m_pModelFile;	// Xファイルのポインタ
	static CFileLoad *m_pFileLoad;	// ファイル読み込みのポインタ
	static CEdit *m_pEdit;			// エディットのポインタ
};


#endif

