//==========================================================
//
// メッシュの水風船処理 [meshballoon.h]
// Author : Ibuki Okusada
//
//==========================================================

#ifndef _MESHBALLOON_H_	// このマクロが定義されていない場合
#define _MESHBALLOON_H_	// 二重インクルード防止用マクロを定義

#include "main.h"
#include "mesh.h"

//**********************************************************
// メッシュドームクラスの定義(派生クラス)
//**********************************************************
class CMeshBalloon : public CObjectMesh
{
public:		// 誰でもアクセス可能な定義

	// 種類
	enum TYPE
	{
		TYPE_PINK = 0,	// ピンク色
		TYPE_BLUE,		// 青色
		TYPE_MAX
	};

	// 風船初期設定情報
	typedef struct
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
		float fLength;		// 半径
		float fHeight;		// 高さ
		int nPriority;		// 優先順位
		int nWidth;			// 幅
		int nHeight;		// 高さ
	}SET;

private:	// 自分だけがアクセス可能な定義

	enum STATE
	{
		STATE_NONE = 0,	// 何もない状態
		STATE_SHRINK,	// 膨張前の縮む状態
		STATE_SWELL,	// 膨らむ状態
		STATE_MAX
	};

public:		// 誰でもアクセス可能

	CMeshBalloon();	// コンストラクタ
	~CMeshBalloon();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtxInfo(void);
	static CMeshBalloon *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, float fLength, float fHeight, const int nPriority = 3, const int nWidth = 1, const int nHeight = 1);
	void SetParent(D3DXMATRIX *pMtx) { m_pMtxParent = pMtx; }
	void AddLength(float fAddLength);
	void SetMove(D3DXVECTOR3 move) { m_move = move;}
	float GetLength(void) { return m_fLength; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void SetSize(float fLength, float fHeight);
	void SetParentMtx(void);
	void UpdateState(void);

	// メンバ関数
	float m_fHeight;	// 高さ
	float m_fLength;	// 距離
	float m_fRot;		// 一頂点の角度
	float m_fHeightRot;	// y軸角度
	D3DXVECTOR3 m_move;	// 移動量
	D3DXMATRIX *m_pMtxParent;	// 親のマトリックス
	STATE m_State;		// 状態
	float m_fStateCnt;	// 状態遷移カウント
};

#endif