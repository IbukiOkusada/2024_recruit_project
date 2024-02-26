//==========================================================
//
// チェックポイントマネージャー {checkpoint_manager.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _CHECKPOINTMANAGER_H_		// このマクロが定義されていない場合
#define _CHECKPOINTMANAGER_H_		// 二重インクルード防止用マクロを定義

// 前方宣言
class CCheckPoint;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CCheckPointManager
{
private:

	CCheckPointManager();		// コンストラクタ
	~CCheckPointManager();	// デストラクタ

public:	// 誰でもアクセス可能

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CCheckPointManager* GetInstance(void);
	static void Release(void);
	CCheckPoint* GetTop(void) { return m_pTop; }
	CCheckPoint* GetCur(void) { return m_pCur; }
	void ListIn(CCheckPoint* pCheckPoint);
	void ListOut(CCheckPoint* pCheckPoint);
	int GetNum(void) { return m_nNum; }
	void Hit(const D3DXVECTOR3& pos, D3DXVECTOR3& CheckPointPos);

private:	// 自分だけがアクセス可能

	// メンバ関数
	// 
	// メンバ変数
	CCheckPoint* m_pTop;	// 先頭
	CCheckPoint* m_pCur;	// 最後尾
	int m_nNum;
	static CCheckPointManager* m_pInstance;	// インスタンス
};

#endif
