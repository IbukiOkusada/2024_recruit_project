//===================================================
//
// 階層構造によるモーション処理 [motion.h]
// Author Ibuki Okusada
//
//===================================================
#ifndef _MOITON_H_
#define _MOITON_H_

#include "main.h"		//main.hで定義しているものが必要なためinclude

#define MAX_MOTION		(64)	// モーション最大数
#define MAX_KEY		(64)		// キーの最大数

// 前方宣言
class CModel;

//**********************************************************
// モーションクラスの定義
//**********************************************************
class CMotion
{
public: // 誰でもアクセス可能な定義

	// キー
	typedef struct
	{
		float fPosX;
		float fPosY;
		float fPosZ;
		float fRotX;
		float fRotY;
		float fRotZ;
	}KEY;

	// キー情報
	typedef struct
	{
		int nFrame;			// 再生フレーム
		KEY *aKey;			// パーツの総数
	}KEY_INFO;

private:	// 自分だけがアクセス可能な定義

	// モーション情報
	typedef struct
	{
		bool bLoop;			// ループするかどうか
		int nNumKey;		// キー数
		KEY_INFO aKeyInfo[MAX_KEY];	// キーの総数
	}INFO;

	//胴体ファイル情報
	typedef struct
	{
		CModel **ppParts;		// パーツの情報
		int nNumParts;			// パーツ数
	}BodyFileData;

public:		// 誰でもアクセス可能
	CMotion();	// コンストラクタ
	~CMotion();	// デストラクタ

	// メンバ関数
	void Uninit(void);
	void Update(void);
	void Set(int nType);
	void Set(int nType, bool b);
	void SetModel(CModel **pModel, int nNumParts);
	void SetNumKey(int nNumKey) { aInfo[m_nNowMotion].nNumKey = nNumKey; }
	void SetFrame(int nKey, int nMotion, int nFrame) { aInfo[nMotion].aKeyInfo[nKey].nFrame = nFrame; }
	void LoadMotionData(FILE *pFile);
	int GetNumKey(void) { return aInfo[m_nNowMotion].nNumKey; }
	int GetNum(void) { return m_nNumMotion; }
	int GetNow(void) { return m_nNowMotion; }
	KEY GetKey(int nKey, int nMotion, int nParts);
	KEY_INFO *GetKeyInfo(int nKey, int nMotion) { return &aInfo[nMotion].aKeyInfo[nKey]; }
	void SetKey(int nKey, int nMotion, int nParts);
	void SetKey(int nKey);
	void Reset(void);
	void Save(void);
	int GetFrame(int nKey, int nMotion) { return aInfo[nMotion].aKeyInfo[nKey].nFrame; }
	int GetFrame(void);
	int GetNumFrame(void);
	int GetNowKey(void) { return m_nNowKey;}
	int GetNowFrame(void) { return m_nNowFrame; }

private:	// 自分だけがアクセス可能

	// メンバ変数
	KEY OldKey;				// 前回のキー情報
	INFO aInfo[MAX_MOTION];	// モーション情報
	int m_nNumMotion;		// モーション数
	int m_nNowFrame;		// 現在のフレーム数
	int m_nNowMotion;		// 現在のモーション番号
	int m_nNowKey;			// 現在のキー数
	bool m_bFinish;			// 終了したかどうか
	BodyFileData m_FileData;// ファイル情報
};

#endif
