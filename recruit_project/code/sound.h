//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=============================================================================
// サウンドクラスの定義
//=============================================================================
class CSound
{
public:	// 誰でもアクセス可能な定義

	// 種類列挙
	typedef enum
	{
		LABEL_BGM_TITLE = 0,	// BGMタイトル
		LABEL_BGM_TUTORIAL,		// BGMチュートリアル
		LABEL_BGM_GAME,			// BGMゲーム
		LABEL_BGM_RESULT_CLEAR,	// BGMリザルトクリア
		LABEL_BGM_RESULT_FAILED,// BGMリザルト失敗
		LABEL_BGM_RANKING,		// BGMランキング
		LABEL_SE_CLICK,			// Enter
		LABEL_SE_STEP,			// ステップ音
		LABEL_SE_DAMAGE,		// ダメージ音
		LABEL_SE_SPAWN,			// 復活音
		LABEL_SE_AIR,			// 風の術
		LABEL_SE_KUNAI,			// クナイ
		LABEL_SE_ATTACK,		// 攻撃
		LABEL_SE_CATCH,			// つかむ
		LABEL_SE_DROP,			// 落とす
		LABEL_SE_GET,			// 取得
		LABEL_SE_SELECT,		// アイテム選択
		LABEL_SE_LEVER,			// レバー
		LABEL_SE_JUMP,			// ジャンプ
		LABEL_SE_GROW,			// 拡大
		LABEL_SE_FADE,			// 縮小
		LABEL_SE_SEEK,			// 見つけた
		LABEL_SE_LAND,			// 着地
		LABEL_SE_DEATH,			// 死亡
		LABEL_SE_SPEAR,			// 死亡
		LABEL_SE_OPEN00,		// 協力扉が開く
		LABEL_SE_OPEN01,		// 協力扉が閉まる
		LABEL_SE_OPEN02,		// 開始扉
		LABEL_SE_WARNING,		// 警告音
		LABEL_SE_COIN00,		// コイン
		LABEL_SE_COIN01,		// コイン
		LABEL_SE_BUTTON,		// ボタンを押した
		LABEL_SE_MOVE,			// 壺移動
		LABEL_SE_RAGE,			// バタバタ
		LABEL_SE_ENTRY,			// エントリー
		LABEL_MAX,
	} LABEL;

private: // 自分だけがアクセス可能

	 // サウンド情報の構造体定義
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDINFO;

public:	// 誰でもアクセス可能

	CSound();	// コンストラクタ
	~CSound();	// デストラクタ

	// メンバ関数
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(LABEL label);
	void Stop(LABEL label);
	void Stop(void);

private:	// 自分だけがアクセス可能

	// メンバ関数
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// メンバ変数
	static const SOUNDINFO m_aSoundInfo[LABEL_MAX];	// サウンドファイル名
	IXAudio2 *m_pXAudio2;							// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;		// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX];					// オーディオデータサイズ
};

#endif
