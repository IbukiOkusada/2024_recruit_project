//==========================================================
//
// 多重スクロールの背景の処理 [bg.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _BG_H_		// このマクロが定義されていない場合
#define _BG_H_		// 二重インクルード防止用マクロを定義

#include "main.h"
#include "object.h"

// 前方宣言
class CObject2D;

//==========================================================
// テクスチャスライドオブジェクト(2D)のクラス定義(派生クラス)
//==========================================================
class CMultiBg : public CObject
{
private:	// 自分しか見ない定義

	// 構造体の定義 (東先生のサポート有り)
	struct STex
	{
		float fTexU;		// テクスチャU座標
		float fTexV;		// テクスチャV座標
		float fMoveU;		// U座標移動量
		float fMoveV;		// V座標移動量
	};

	// 構造体の定義(同じものは構造体でまとめよう!!!!!!) (東先生のサポート有り)
	struct SBg	// :  STex 構造体を継承もできるけどconstであらかじめ決めたサイズでは入れられない
	{
		CObject2D *pObject2D;	// オブジェクト
		STex sTex;			// テクスチャ情報
	};

public:	// 誰でもアクセス可能な定義

	// 種類列挙型の定義
	enum TYPE
	{
		TYPE_BACK = 0,	// 一番奥
		TYPE_NORMAL,	// 真ん中
		TYPE_FRONT,		// 手前
		TYPE_MAX
	};

public:	// ここから関数(誰でもアクセス可能)

	CMultiBg();	// コンストラクタ
	~CMultiBg();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMultiBg *Create();
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(const D3DXVECTOR3 rot){}
	void SetPlayerMove(const D3DXVECTOR3 move);
	CObject2D *GetObject2D(void) { return NULL; }

private:	// 自分だけがアクセス可能

	// メンバ変数
	float m_aTexU[TYPE_MAX];
	int m_aIdxTexture[TYPE_MAX];	// テクスチャ番号
	static const char *m_apTextureFileName[];	// ファイル名
	static const STex apMoveTex[TYPE_MAX];	// 頂点情報の移動量
	D3DXVECTOR3 m_pos;		// 座標
	SBg m_aBg[TYPE_MAX];	// 背景の配列
};

#endif
