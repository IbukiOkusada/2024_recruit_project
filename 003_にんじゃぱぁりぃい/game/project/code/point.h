//==========================================================
//
// 敵移動ポイント [point.h]
// Author : Soma Ishihara
//
//==========================================================
#ifndef _POINT_H_		// このマクロが定義されていない場合
#define _POINT_H_		// 二重インクルード防止用マクロを定義

#include "task.h"

//==========================================================
// 敵移動ポイントのクラス定義
//==========================================================
class CPoint : public CTask
{

public:	// 誰でもアクセス可能

	CPoint();	// コンストラクタ(オーバーロード)
	~CPoint();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CPoint *Create(void);

	// メンバ関数(取得)
	int GetRegistPointNum(void) { return m_nRegistPointNum; }
	D3DXVECTOR3 GetPoint(const int nNum) { return m_pPosPoint[nNum % m_nRegistPointNum]; }
	static CPoint* GetTop(void) { return m_pTop; }
	static int GetNumAll(void) { return m_nNumAll; }
	CPoint* GetNext(void) { return m_pNext; }

	// メンバ関数(設定)
	void SetNum(const int nNum);
	void AddPoint(const D3DXVECTOR3 pos);

private:	// 自分だけがアクセス可能
	// メンバ変数
	//リスト
	static CPoint* m_pTop;
	static CPoint* m_pCur;
	CPoint* m_pPrev;
	CPoint* m_pNext;
	static int m_nNumAll;	//総数

	//データ
	D3DXVECTOR3* m_pPosPoint;	//移動ポイント（動的確保）
	int m_nPointNum;			//移動ポイント数
	int m_nRegistPointNum;		//登録済み移動ポイント数
};

#endif
