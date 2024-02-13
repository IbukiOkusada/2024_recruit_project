//==========================================================
//
// 3D扇の処理 [object3DFan.h]
// Author : Soma Ishihara
//
//==========================================================
#ifndef _OBJECT3D_FAN_H_	 // このマクロが定義されていない場合
#define _OBJECT3D_FAN_H_	 // 二重インクルード防止用マクロを定義

#include "main.h"
#include "object.h"

//**********************************************************
// オブジェクト3Dクラスの定義
//**********************************************************
class CObject3DFan : public CObject
{
public:	// 誰でもアクセス可能

	//頂点変更種類列挙（複数使用時AND演算して入れて）
	enum CHANGE
	{
		CHANGE_POS = 0b0001,
		CHANGE_NOR = 0b0010,
		CHANGE_COL = 0b0100,
		CHANGE_TEX = 0b1000,
		CHANGE_ALL = 0b1111
	};

	CObject3DFan(int nPriority = 6);	// コンストラクタ
	~CObject3DFan();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Draw(void);
	static CObject3DFan *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLength, float fRadius, int nDivision);
	void SetpVtx(const int nChangeVtx);

	// メンバ関数(設定)
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetLength(const float fLength);
	void SetRadius(const float fRadius);
	void SetDivision(const int nDiv);
	void SetColor(const D3DXCOLOR col);
	void SetMtx(void);
	D3DXMATRIX *GetMtx(void) { return &m_mtxWorld; }

	// メンバ関数(取得)
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	float GetLength(void) { return m_fLength; }

private:	// 自分だけがアクセス可能
	//メンバ関数
	HRESULT ResetpVtx(void);

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_rot;		//向き
	D3DXCOLOR m_col;		//色
	float m_fLength;		//距離
	float m_fRadius;		//角度
	int m_nDivision;		//分割数
};

#endif
