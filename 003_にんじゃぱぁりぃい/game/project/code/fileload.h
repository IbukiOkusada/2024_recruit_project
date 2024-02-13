//==========================================================
//
//ファイル、データ読み込み処理
//Author Ibuki Okusada
//
//==========================================================
#ifndef _FILELOAD_H_
#define _FILELOAD_H_

#include "main.h"	//main.hで定義しているものが必要なためinclude

// 前方宣言
class CTitleEnter;

//**********************************************************
// 外部ファイル読み込みクラスの定義
//**********************************************************
class CFileLoad
{
private:	// 自分だけがアクセス可能な定義

	// ファイル情報
	struct File
	{
		int nIdx;			// 読み込み番号
		char aFileName[256];	// ファイル名
	};

public:		// 誰でもアクセス可能

	CFileLoad();	// コンストラクタ
	~CFileLoad();	// デストラクタ

	// メンバ関数
	void Init(void);
	void Uninit(void);
	void OpenFile(const char *pFileName);

	// メンバ関数(取得)
	char *GetTextureFileName(int nCntTexture);
	char *GetModelFileName(int nCntModel);
	int GetTextureNum(const char *pFileName);
	int GetModelNum(const char *pFileName);
	int GetTexNumAll(void) { return m_nTexNumAll; }
	int GetModelNumAll(void) { return m_nModelNumAll; }

	// メンバ関数(設定)
	void SetTitleEnter(CTitleEnter *pEnter) { m_pEnter = pEnter; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void LoadTexNum(FILE *pFile);
	void LoadModelNum(FILE *pFile);
	void LoadTexFile(FILE *pFile);
	void LoadModelFile(FILE *pFile);
	void LoadFileData(FILE *pFile);
	void LoadMeshWallData(FILE *pFile);
	void LoadMeshFieldData(FILE *pFile);
	void LoadModelData(FILE *pFile);
	void LoadItemData(FILE *pFile);
	void LoadDomeData(FILE *pFile);
	void LoadCylinderData(FILE *pFile);
	void LoadVtxMaxData(FILE *pFile, int nIdx);
	void LoadVtxMinData(FILE *pFile, int nIdx);
	void LoadButtonData(FILE *pFile);
	void LoadPullData(FILE *pFile);
	void LoadMultiDoorData(FILE *pFile);
	void LoadPitFallData(FILE *pFile);
	void LoadSpearData(FILE *pFile);
	void LoadRotateDoorData(FILE *pFile);
	void LoadPointData(FILE *pFile);

	// メンバ変数
	File *m_pModelFile;	// モデルファイル情報
	File *m_pTextureFile;	// テクスチャファイル情報
	int m_nTexNumAll;	// テクスチャ総数
	int m_nModelNumAll;	// モデル総数
	CTitleEnter *m_pEnter;
};

#endif
