//===================================================
//
// モーションエディター処理全般 [edit.cpp]
// Author : Ibuki Okusada
//
//===================================================
#include "edit.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"
#include "character.h"
#include "player.h"
#include "model.h"

// マクロ定義
#define MAX_DEBUGPARTS	(10)

//***************************************************
// 静的メンバ変数宣言
//***************************************************
// 状態表示文字
const char *CEdit::m_apState[] =
{
	"EDITMODE",
	"PLAYMODE",
};

// 選択部位表示文字
const char *CEdit::m_apType[TYPE_MAX] =
{
	"上半身",
	"下半身",
};

//===================================================
// コンストラクタ
//===================================================
CEdit::CEdit()
{
	// 値のクリア
	m_State = STATE_EDIT;		
	m_Type = TYPE_BODY;
	m_nIdx = 0;		
	m_nNowMotion = 0;	
	m_nNowKey = 0;
	m_Key = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
}

//===================================================
// デストラクタ
//===================================================
CEdit::~CEdit()
{

}

//===================================================
// 初期化処理
//===================================================
void CEdit::Init(void)
{
	m_bMotion = false;
}

//===================================================
// 終了処理
//===================================================
void CEdit::Uninit(void)
{

}

//===================================================
// 更新処理
//===================================================
void CEdit::Update(void)
{
	CDebugProc *pDebug = CManager::GetDebugProc();

	// 操作
	Controller();

	// デバッグ表示
	if (pDebug != NULL)
	{// 使用されている場合
		pDebug->Print("[モード変更]       : [ F2 ] 〔 %s 〕\n", m_apState[m_State]);
		pDebug->Print("[編集部位変更]     : [ ３ ] 〔 %s 〕\n", m_apType[m_Type]);
		pDebug->Print("[モーション変更]   : [ ←, → ]  〔 %d / %d 〕\n", m_nNowMotion, m_pBody->GetMotion()->GetNum());
		pDebug->Print("[キー選択]         : [ 1, 2 ]〔 %d / %d〕\n", m_nNowKey, m_pBody->GetMotion()->GetNumKey());
		pDebug->Print("[キー総数変更]     : [ I, K ]  〔 NUM %d 〕\n", m_pBody->GetMotion()->GetNumKey());
		// 状態別デバッグ表示
		if (m_State == STATE_PLAY)
		{// 動くとき
			pDebug->Print("[キーフレーム数変更]     : [ ↑, ↓ ] 〔 %d 〕\n", m_pBody->GetMotion()->GetFrame(m_nNowKey, m_nNowMotion));
			pDebug->Print("[ 最初から再生]          : [ F4 ]\n");

			pDebug->Print("\n");

			// フレーム情報追加
			pDebug->Print("+------------------------------------+\n");
			pDebug->Print("MOTION : %d\n", m_nNowMotion);
			pDebug->Print("KEY    : %d / %d\n", m_pBody->GetMotion()->GetNowKey(), m_pBody->GetMotion()->GetNumKey());
			pDebug->Print("FRAME  : %d / %d ( %d / %d )\n", 
				m_pBody->GetMotion()->GetNowFrame(), m_pBody->GetMotion()->GetFrame(m_nNowKey, m_nNowMotion),
				m_pBody->GetMotion()->GetFrame(), m_pBody->GetMotion()->GetNumFrame());
			pDebug->Print("+------------------------------------+\n");
		}
		else
		{// 作成時
			pDebug->Print("[ポーズ登録]       : [ F3 ]\n");
			pDebug->Print("[ポーズコピー]     : [ F6 ]\n");
			pDebug->Print("[ポーズペースト]   : [ F7 ]\n");
			pDebug->Print("[ファイル書き出し] : [ F9 ] 〔 data\\motion_info.txt 〕\n");

			// パーツ情報説明
			pDebug->Print("\n+------------------------------------+\n");
			pDebug->Print("| [↑][↓] : パーツ選択              |\n");
			pDebug->Print("+------------------------------------+\n");
			pDebug->Print("|[T][G] : X軸回転 [B] : X軸リセット  |\n");
			pDebug->Print("|[Y][H] : Y軸回転 [N] : Y軸リセット  |\n");
			pDebug->Print("|[U][J] : Z軸回転 [M] : Z軸リセット  |\n");
			pDebug->Print("+------------------------------------+\n");
			pDebug->Print(" [ パーツ数 : %d ]\n", m_pBody->GetNumParts());

			if (m_pBody->GetNumParts() > 0)
			{
				for (int nCnt = 0; nCnt < m_pBody->GetNumParts(); nCnt++)
				{
					if (nCnt > MAX_DEBUGPARTS)
					{
						break;
					}
					else
					{
						int nIdx = (nCnt + m_nIdx) % m_pBody->GetNumParts();
						D3DXVECTOR3 rot = m_pBody->GetParts(nIdx)->GetCurrentRotation();
						
						if (m_nIdx == nIdx)
						{
							pDebug->Print("★");
						}
						else
						{
							pDebug->Print("o");
						}

						pDebug->Print("[%d]: (%f, %f, %f)\n", nIdx,
							rot.x, rot.y, rot.z);
					}
				}
			}

			D3DXVECTOR3 pos = m_pBody->GetParts(0)->GetCurrentPosition();
			pDebug->Print("+----------------------------------------+\n");
			pDebug->Print("|[Q][A] : X座標移動 [Z] : X座標リセット  |\n");
			pDebug->Print("|[W][S] : Y座標移動 [X] : Y座標リセット  |\n");
			pDebug->Print("|[E][D] : Z座標移動 [C] : Z座標リセット  |\n");
			pDebug->Print("+----------------------------------------+\n");
			pDebug->Print("  [0] (%f, %f, %f)\n", pos.x, pos.y, pos.z);
			pDebug->Print("+-----------------------------+\n");
			pDebug->Print("| [ Enter ]回転、移動リセット |\n");
			pDebug->Print("+-----------------------------+\n");
		}
	}
}

//===================================================
// 操作
//===================================================
void CEdit::Controller(void)
{
	CInputKeyboard *pKey = CManager::GetInputKeyboard();	// キーボードのポインタ

	if (pKey == NULL)
	{
		return;
	}

	// 状態切り替え
	if (pKey->GetTrigger(DIK_F2) == true)
	{
		m_State = (STATE)(m_State ^ STATE_PLAY);

		if (m_State == STATE_PLAY)
		{
			m_bMotion = true;
		}
	}

	// 部位切り替え
	if (pKey->GetTrigger(DIK_3) == true)
	{
		m_Type = (TYPE)(m_Type ^ TYPE_LEG);

		// 種類ごとに分ける
		switch (m_Type)
		{
		case TYPE_BODY:
			m_pBody = CManager::GetPlayer()->GetBody();
			break;

		case TYPE_LEG:
			m_pBody = CManager::GetPlayer()->GetLeg();
			break;
		}

		m_nIdx = 0;
		m_nNowMotion = m_pBody->GetMotion()->GetNow();
		m_nNowKey = 0;
		m_pInfo = NULL;

		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		pos = m_pBody->GetParts(m_nIdx)->GetCurrentPosition();
		rot = m_pBody->GetParts(m_nIdx)->GetCurrentRotation();
		m_Key = { pos.x, pos.y, pos.z, rot.x, rot.y, rot.z };
	}

	// モーション番号切り替え
	if (pKey->GetTrigger(DIK_LEFT) == true)
	{
		m_nNowMotion = (m_nNowMotion + m_pBody->GetMotion()->GetNum() - 1) % m_pBody->GetMotion()->GetNum();
		m_nNowKey = 0;
		m_pBody->GetMotion()->Set(m_nNowMotion);
		m_Key = m_pBody->GetMotion()->GetKey(m_nNowKey, m_nNowMotion, m_nIdx);
	}
	else if (pKey->GetTrigger(DIK_RIGHT) == true)
	{
		m_nNowMotion = (m_nNowMotion + 1) % m_pBody->GetMotion()->GetNum();
		m_nNowKey = 0;
		m_pBody->GetMotion()->Set(m_nNowMotion);
		m_Key = m_pBody->GetMotion()->GetKey(m_nNowKey, m_nNowMotion, m_nIdx);
	}

	// キー番号切り替え
	if (pKey->GetTrigger(DIK_1) == true)
	{
		m_nNowKey = (m_nNowKey + m_pBody->GetMotion()->GetNumKey() - 1) % m_pBody->GetMotion()->GetNumKey();
		m_pBody->GetMotion()->SetKey(m_nNowKey);
		m_Key = m_pBody->GetMotion()->GetKey(m_nNowKey, m_nNowMotion, m_nIdx);
	}
	else if (pKey->GetTrigger(DIK_2) == true)
	{
		m_nNowKey = (m_nNowKey + 1) % m_pBody->GetMotion()->GetNumKey();
		m_pBody->GetMotion()->SetKey(m_nNowKey);
		m_Key = m_pBody->GetMotion()->GetKey(m_nNowKey, m_nNowMotion, m_nIdx);
	}

	// キー総数変更
	if (pKey->GetTrigger(DIK_I) == true)
	{// 減らす場合
		int nNumKey = m_pBody->GetMotion()->GetNumKey();
		nNumKey--;

		if (nNumKey < 1)
		{
			nNumKey = 1;
		}

		m_pBody->GetMotion()->SetNumKey(nNumKey);
	}
	else if (pKey->GetTrigger(DIK_K) == true)
	{// 増やす場合
		int nNumKey = m_pBody->GetMotion()->GetNumKey();
		nNumKey++;

		if (nNumKey >= MAX_KEY)
		{
			nNumKey = MAX_KEY - 1;
		}

		m_pBody->GetMotion()->SetNumKey(nNumKey);
	}

	// 状態ごとの操作
	switch (m_State)
	{
	case STATE_EDIT:
		EditController();
		break;
		
	case STATE_PLAY:
		PlayController();
		break;
	}
}

//===================================================
// エディット状態操作
//===================================================
void CEdit::EditController(void)
{
	CInputKeyboard *pKey = CManager::GetInputKeyboard();	// キーボードのポインタ

	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;

	// パーツ情報変更
	if (pKey->GetRepeat(DIK_T) == true || pKey->GetTrigger(DIK_T) == true)
	{// 減らす場合
		m_Key.fRotX -= D3DX_PI * 0.01f;

		if (m_Key.fRotX < -D3DX_PI)
		{
			m_Key.fRotX += D3DX_PI * 2;
		}

		m_pBody->GetParts(m_nIdx)->SetCurrentRotation(D3DXVECTOR3(m_Key.fRotX, m_Key.fRotY, m_Key.fRotZ));

		pos = m_pBody->GetParts(m_nIdx)->GetCurrentPosition();
		rot = m_pBody->GetParts(m_nIdx)->GetCurrentRotation();
		m_Key = { pos.x, pos.y, pos.z, rot.x, rot.y, rot.z };
	}
	else if (pKey->GetRepeat(DIK_G) == true || pKey->GetTrigger(DIK_G) == true)
	{// 増やす場合
		m_Key.fRotX += D3DX_PI * 0.01f;

		if (m_Key.fRotX > D3DX_PI)
		{
			m_Key.fRotX += -D3DX_PI * 2;
		}

		m_pBody->GetParts(m_nIdx)->SetCurrentRotation(D3DXVECTOR3(m_Key.fRotX, m_Key.fRotY, m_Key.fRotZ));

		pos = m_pBody->GetParts(m_nIdx)->GetCurrentPosition();
		rot = m_pBody->GetParts(m_nIdx)->GetCurrentRotation();
		m_Key = { pos.x, pos.y, pos.z, rot.x, rot.y, rot.z };
	}
	else if (pKey->GetTrigger(DIK_B) == true)
	{// リセット
		m_Key.fRotX = 0.0f;

		m_pBody->GetParts(m_nIdx)->SetCurrentRotation(D3DXVECTOR3(m_Key.fRotX, m_Key.fRotY, m_Key.fRotZ));

		pos = m_pBody->GetParts(m_nIdx)->GetCurrentPosition();
		rot = m_pBody->GetParts(m_nIdx)->GetCurrentRotation();
		m_Key = { pos.x, pos.y, pos.z, rot.x, rot.y, rot.z };
	}
	if (pKey->GetRepeat(DIK_Y) == true || pKey->GetTrigger(DIK_Y) == true)
	{// 減らす場合
		m_Key.fRotY -= D3DX_PI * 0.01f;

		if (m_Key.fRotY < -D3DX_PI)
		{
			m_Key.fRotY += D3DX_PI * 2;
		}

		m_pBody->GetParts(m_nIdx)->SetCurrentRotation(D3DXVECTOR3(m_Key.fRotX, m_Key.fRotY, m_Key.fRotZ));

		pos = m_pBody->GetParts(m_nIdx)->GetCurrentPosition();
		rot = m_pBody->GetParts(m_nIdx)->GetCurrentRotation();
		m_Key = { pos.x, pos.y, pos.z, rot.x, rot.y, rot.z };
	}
	else if (pKey->GetRepeat(DIK_H) == true || pKey->GetTrigger(DIK_H) == true)
	{// 増やす場合
		m_Key.fRotY += D3DX_PI * 0.01f;

		if (m_Key.fRotY > D3DX_PI)
		{
			m_Key.fRotY += -D3DX_PI * 2;
		}

		m_pBody->GetParts(m_nIdx)->SetCurrentRotation(D3DXVECTOR3(m_Key.fRotX, m_Key.fRotY, m_Key.fRotZ));

		pos = m_pBody->GetParts(m_nIdx)->GetCurrentPosition();
		rot = m_pBody->GetParts(m_nIdx)->GetCurrentRotation();
		m_Key = { pos.x, pos.y, pos.z, rot.x, rot.y, rot.z };
	}
	else if (pKey->GetTrigger(DIK_N) == true)
	{// リセット
		m_Key.fRotY = 0.0f;

		m_pBody->GetParts(m_nIdx)->SetCurrentRotation(D3DXVECTOR3(m_Key.fRotX, m_Key.fRotY, m_Key.fRotZ));

		pos = m_pBody->GetParts(m_nIdx)->GetCurrentPosition();
		rot = m_pBody->GetParts(m_nIdx)->GetCurrentRotation();
		m_Key = { pos.x, pos.y, pos.z, rot.x, rot.y, rot.z };
	}
	if (pKey->GetRepeat(DIK_U) == true || pKey->GetTrigger(DIK_U) == true)
	{// 減らす場合
		m_Key.fRotZ -= D3DX_PI * 0.01f;

		if (m_Key.fRotZ < -D3DX_PI)
		{
			m_Key.fRotZ += D3DX_PI * 2;
		}

		m_pBody->GetParts(m_nIdx)->SetCurrentRotation(D3DXVECTOR3(m_Key.fRotX, m_Key.fRotY, m_Key.fRotZ));
	}
	else if (pKey->GetRepeat(DIK_J) == true || pKey->GetTrigger(DIK_J) == true)
	{// 増やす場合
		m_Key.fRotZ += D3DX_PI * 0.01f;

		if (m_Key.fRotZ > D3DX_PI)
		{
			m_Key.fRotZ += -D3DX_PI * 2;
		}

		m_pBody->GetParts(m_nIdx)->SetCurrentRotation(D3DXVECTOR3(m_Key.fRotX, m_Key.fRotY, m_Key.fRotZ));
	}
	else if (pKey->GetTrigger(DIK_M) == true)
	{// リセット
		m_Key.fRotZ = 0.0f;

		m_pBody->GetParts(m_nIdx)->SetCurrentRotation(D3DXVECTOR3(m_Key.fRotX, m_Key.fRotY, m_Key.fRotZ));

		pos = m_pBody->GetParts(m_nIdx)->GetCurrentPosition();
		rot = m_pBody->GetParts(m_nIdx)->GetCurrentRotation();
		m_Key = { pos.x, pos.y, pos.z, rot.x, rot.y, rot.z };
	}

	// 位置変更
	if (pKey->GetRepeat(DIK_Q) == true || pKey->GetTrigger(DIK_Q) == true)
	{// 減らす場合
		
		pos = m_pBody->GetParts(0)->GetCurrentPosition();
		pos.x += 0.1f;
		m_pBody->GetParts(0)->SetCurrentPosition(pos);
	}
	else if (pKey->GetRepeat(DIK_A) == true || pKey->GetTrigger(DIK_A) == true)
	{// 増やす場合
		pos = m_pBody->GetParts(0)->GetCurrentPosition();
		pos.x -= 0.1f;
		m_pBody->GetParts(0)->SetCurrentPosition(pos);
	}
	if (pKey->GetRepeat(DIK_W) == true || pKey->GetTrigger(DIK_W) == true)
	{// 減らす場合
		pos = m_pBody->GetParts(0)->GetCurrentPosition();
		pos.y += 0.1f;
		m_pBody->GetParts(0)->SetCurrentPosition(pos);
	}
	else if (pKey->GetRepeat(DIK_S) == true || pKey->GetTrigger(DIK_S) == true)
	{// 増やす場合
		pos = m_pBody->GetParts(0)->GetCurrentPosition();
		pos.y -= 0.1f;
		m_pBody->GetParts(0)->SetCurrentPosition(pos);
	}
	if (pKey->GetRepeat(DIK_E) == true || pKey->GetTrigger(DIK_E) == true)
	{// 減らす場合
		pos = m_pBody->GetParts(0)->GetCurrentPosition();
		pos.z += 0.1f;
		m_pBody->GetParts(0)->SetCurrentPosition(pos);
	}
	else if (pKey->GetRepeat(DIK_D) == true || pKey->GetTrigger(DIK_D) == true)
	{// 増やす場合
		pos = m_pBody->GetParts(0)->GetCurrentPosition();
		pos.z -= 0.1f;
		m_pBody->GetParts(0)->SetCurrentPosition(pos);
	}

	// パーツ番号変更
	if (pKey->GetTrigger(DIK_UP) == true)
	{
		m_nIdx = (m_nIdx + m_pBody->GetNumParts() - 1) % m_pBody->GetNumParts();
		pos = m_pBody->GetParts(m_nIdx)->GetCurrentPosition();
		rot = m_pBody->GetParts(m_nIdx)->GetCurrentRotation();
		m_Key = {pos.x, pos.y, pos.z, rot.x, rot.y, rot.z};
	}
	else if (pKey->GetTrigger(DIK_DOWN) == true)
	{
		m_nIdx = (m_nIdx + 1) % m_pBody->GetNumParts();
		pos = m_pBody->GetParts(m_nIdx)->GetCurrentPosition();
		rot = m_pBody->GetParts(m_nIdx)->GetCurrentRotation();
		m_Key = { pos.x, pos.y, pos.z, rot.x, rot.y, rot.z };
	}

	// 作成キー情報登録
	if (pKey->GetTrigger(DIK_F3) == true)
	{
		for (int nCnt = 0; nCnt < m_pBody->GetNumParts(); nCnt++)
		{
			m_pBody->GetMotion()->SetKey(m_nNowKey, m_nNowMotion, nCnt);
		}
	}

	// コピー
	if (pKey->GetTrigger(DIK_F6) == true)
	{
		m_pInfo = m_pBody->GetMotion()->GetKeyInfo(m_nNowKey, m_nNowMotion);
	}

	// ペースト
	if (pKey->GetTrigger(DIK_F7) == true)
	{
		if (m_pInfo != NULL)
		{
			for (int nCnt = 0; nCnt < m_pBody->GetNumParts(); nCnt++)
			{
				m_pBody->GetParts(nCnt)->SetCurrentRotation(D3DXVECTOR3(m_pInfo->aKey[nCnt].fRotX,
					m_pInfo->aKey[nCnt].fRotY, m_pInfo->aKey[nCnt].fRotZ));
				m_pBody->GetParts(nCnt)->SetCurrentPosition(D3DXVECTOR3(m_pInfo->aKey[nCnt].fPosX + m_pBody->GetParts(nCnt)->GetPosition().x,
					m_pInfo[nCnt].aKey[nCnt].fPosY + m_pBody->GetParts(nCnt)->GetPosition().y, m_pInfo->aKey[nCnt].fPosZ + m_pBody->GetParts(nCnt)->GetPosition().z));
			}
		}
	}

	// 書き出し
	if (pKey->GetTrigger(DIK_F9) == true)
	{
		m_pBody->GetMotion()->Save();
	}

	// キー情報リセット
	if (pKey->GetTrigger(DIK_RETURN) == true)
	{
		m_pBody->GetMotion()->Reset();
		pos = m_pBody->GetParts(m_nIdx)->GetCurrentPosition();
		rot = m_pBody->GetParts(m_nIdx)->GetCurrentRotation();
		m_Key = { pos.x, pos.y, pos.z, rot.x, rot.y, rot.z };
	}
}

//===================================================
// プレイ状態操作
//===================================================
void CEdit::PlayController(void)
{
	CInputKeyboard *pKey = CManager::GetInputKeyboard();	// キーボードのポインタ
	int nFrame = m_pBody->GetMotion()->GetFrame(m_nNowKey, m_nNowMotion);

	// フレーム数を変更
	if (pKey->GetTrigger(DIK_UP) == true || pKey->GetRepeat(DIK_UP) == true)
	{
		nFrame--;

		if (nFrame < 1)
		{
			nFrame = 1;
		}
	}
	else if (pKey->GetTrigger(DIK_DOWN) == true || pKey->GetRepeat(DIK_DOWN) == true)
	{
		nFrame++;
	}

	// 再生やり直し
	if (pKey->GetTrigger(DIK_F4) == true)
	{
		CManager::GetPlayer()->GetBody()->GetMotion()->Set(m_nNowMotion, false);
		CManager::GetPlayer()->GetLeg()->GetMotion()->Set(m_nNowMotion, false);
	}

	// フレーム数設定
	m_pBody->GetMotion()->SetFrame(m_nNowKey, m_nNowMotion, nFrame);
}