//==========================================================
//
// 敵マネージャー [Enemy_manager.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _ENEMYMANAGER_H_		// このマクロが定義されていない場合
#define _ENEMYMANAGER_H_		// 二重インクルード防止用マクロを定義

// 前方宣言
class CEnemy;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CEnemyManager
{
private:

	CEnemyManager();		// コンストラクタ
	~CEnemyManager();	// デストラクタ

public:	// 誰でもアクセス可能

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CEnemyManager* GetInstance(void);
	static void Release(void);
	CEnemy* GetTop(void) { return m_pTop; }
	CEnemy* GetCur(void) { return m_pCur; }
	void ListIn(CEnemy* pEnemy);
	void ListOut(CEnemy* pEnemy);
	int GetNum(void) { return m_nNum; }
	bool Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage);

private:	// 自分だけがアクセス可能

	// メンバ関数
	// 
	// メンバ変数
	CEnemy* m_pTop;	// 先頭
	CEnemy* m_pCur;	// 最後尾
	int m_nNum;
	static CEnemyManager* m_pInstance;	// インスタンス
};

#endif
