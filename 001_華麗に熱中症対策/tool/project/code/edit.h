//==========================================================
//
// モーションエディター処理全般 [edit.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _EDIT_H_		// このマクロが定義されていない場合
#define _EDIT_H_		// 二重インクルード防止用マクロを定義

#include "motion.h"

// 前方宣言
class CCharacter;

// クラス定義
class CEdit
{
public:		// 誰でもアクセス可能な定義

	// 状態
	enum STATE
	{
		STATE_EDIT = 0,	// エディット状態
		STATE_PLAY,		// プレイ状態
		STATE_MAX
	};

	// 部位種類
	typedef enum
	{
		TYPE_BODY = 0,	// 上半身
		TYPE_LEG,		// 下半身
		TYPE_MAX
	}TYPE;

public:		// 誰でもアクセス可能

	CEdit();	// コンストラクタ
	~CEdit();	// デストラクタ

	// メンバ関数
	void Init(void);
	void Uninit(void);
	void Update(void);
	STATE GetState(void) { return m_State; }
	TYPE GetType(void) { return m_Type; }
	void SetBody(CCharacter *pBody) { m_pBody = pBody; }
	void SetInfo(CMotion::KEY_INFO *pInfo) { m_pInfo = pInfo; }
	int GetIdx(void) { return m_nIdx; }
	int GetKey(void) { return m_nNowKey; }
	int GetMotion(void) { return m_nNowMotion; }
	bool GetMotionNow(void) { return m_bMotion; }
	void SetMotionNow(bool bNow) { m_bMotion = bNow; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void Controller(void);
	void EditController(void);
	void PlayController(void);

	// メンバ引数
	const static char *m_apState[];			// エディット状態
	const static char *m_apType[TYPE_MAX];	// 選択部位
	CMotion::KEY m_Key;	// 設定キー情報
	bool m_bMotion;		// 設定したモーションをするか否か
	CMotion::KEY_INFO *m_pInfo;	// コピー用キー情報
	STATE m_State;		// エディット状態かどうか
	TYPE m_Type;		// 選択部位
	int m_nIdx;			// 選択しているパーツ番号
	int m_nNowMotion;	// モーション番号
	int m_nNowKey;		// キー番号
	CCharacter *m_pBody;	// 編集部位情報
};

#endif
