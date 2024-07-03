//===================================================
//
// 階層構造によるモーション処理 [motion.cpp]
// Author Ibuki Okusada
//
//===================================================
#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "texture.h"
#include "Xfile.h"
#include "model.h"
#include <string.h>

// マクロ定義
#define DEST_MAG	(0.1f)	// 差分補正倍率

//===================================================
// コンストラクタ
//===================================================
CMotion::CMotion()
{
	m_nNowFrame = 0;
	m_nNowKey = 0;
	m_nNowMotion = 1;
	m_nNumMotion = 0;
	m_FileData.nNumParts = 0;
	m_FileData.ppParts = NULL;
	m_bFinish = false;

	for (int nCnt = 0; nCnt < MAX_MOTION; nCnt++)
	{
		aInfo[nCnt].nNumKey = 0;
		memset(&aInfo[nCnt].aKeyInfo, NULL, sizeof(CMotion::KEY_INFO));
		aInfo[m_nNumMotion].aKeyInfo[nCnt].nFrame = 1;
		aInfo[nCnt].bLoop = false;
	}

	OldKey = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
}

//===================================================
// デストラクタ
//===================================================
CMotion::~CMotion()
{

}

//===================================================
// 終了処理
//===================================================
void CMotion::Uninit(void)
{
	// モーション情報の廃棄
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{
		if (aInfo[nCntMotion].aKeyInfo != NULL)
		{
			for (int nCntKey = 0; nCntKey < aInfo[nCntMotion].nNumKey; nCntKey++)
			{

				delete[] aInfo[nCntMotion].aKeyInfo[nCntKey].aKey;
				aInfo[nCntMotion].aKeyInfo[nCntKey].aKey = NULL;
			}
		}
	}
}

//===================================================
// 更新処理
//===================================================
void CMotion::Update(void)
{
	if (m_FileData.ppParts != NULL)
	{// 使用されていない場合
		if (aInfo[m_nNowMotion].nNumKey > 0)
		{//キー数が存在している場合
			int nNowMotion = m_nNowMotion;	// 現在のモーション番号
			int nNowKey = m_nNowKey;		// 現在のキー
			int nNextkey = (nNowKey + 1) % aInfo[nNowMotion].nNumKey;	//次のキー
			int nFrame = aInfo[nNowMotion].aKeyInfo[nNowKey].nFrame;

			float fFrame = ((float)m_nNowFrame / (float)nFrame);	// フレーム間の差分

			for (int nCntParts = 0; nCntParts < m_FileData.nNumParts; nCntParts++)
			{
				// 前回の値を取得
				OldKey.fPosX = m_FileData.ppParts[nCntParts]->GetPosition().x;
				OldKey.fPosY = m_FileData.ppParts[nCntParts]->GetPosition().y;
				OldKey.fPosZ = m_FileData.ppParts[nCntParts]->GetPosition().z;
				OldKey.fRotX = m_FileData.ppParts[nCntParts]->GetPosition().x;
				OldKey.fRotY = m_FileData.ppParts[nCntParts]->GetRotation().y;
				OldKey.fRotZ = m_FileData.ppParts[nCntParts]->GetRotation().z;

				D3DXVECTOR3 setPos = m_FileData.ppParts[nCntParts]->GetPosition();	// 初期位置
				D3DXVECTOR3 setRot = m_FileData.ppParts[nCntParts]->GetRotation();	// 初期向き

				//差分を算出
				float fPosDiffX = aInfo[nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fPosX	// X座標
					- aInfo[nNowMotion].aKeyInfo[nNowKey].aKey[nCntParts].fPosX;
				float fPosDiffY = aInfo[nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fPosY	// Y座標
					- aInfo[nNowMotion].aKeyInfo[nNowKey].aKey[nCntParts].fPosY;
				float fPosDiffZ = aInfo[nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fPosZ	// Z座標
					- aInfo[nNowMotion].aKeyInfo[nNowKey].aKey[nCntParts].fPosZ;
				float fRotDiffX = aInfo[nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fRotX	// X向き
					- aInfo[nNowMotion].aKeyInfo[nNowKey].aKey[nCntParts].fRotX;
				float fRotDiffY = aInfo[nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fRotY	// Y向き
					- aInfo[nNowMotion].aKeyInfo[nNowKey].aKey[nCntParts].fRotY;
				float fRotDiffZ = aInfo[nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fRotZ	// Z向き
					- aInfo[nNowMotion].aKeyInfo[nNowKey].aKey[nCntParts].fRotZ;

				if (fRotDiffZ < -D3DX_PI)
				{// z座標角度限界
					fRotDiffZ += D3DX_PI * 2;
				}
				else if (fRotDiffZ > D3DX_PI)
				{// z座標角度限界
					fRotDiffZ += -D3DX_PI * 2;
				}

				if (fRotDiffX < -D3DX_PI)
				{// x座標角度限界
					fRotDiffX += D3DX_PI * 2;
				}
				else if (fRotDiffX > D3DX_PI)
				{// x座標角度限界
					fRotDiffX += -D3DX_PI * 2;
				}

				if (fRotDiffY < -D3DX_PI)
				{// x座標角度限界
					fRotDiffY += D3DX_PI * 2;
				}
				else if (fRotDiffY > D3DX_PI)
				{// x座標角度限界
					fRotDiffY += -D3DX_PI * 2;
				}

				//現在のフレームの座標を求める
				float fPosDestX = setPos.x	// X座標
					+ aInfo[nNowMotion].aKeyInfo[nNowKey].aKey[nCntParts].fPosX
					+ fPosDiffX * fFrame;
				float fPosDestY = setPos.y	// Y座標
					+ aInfo[nNowMotion].aKeyInfo[nNowKey].aKey[nCntParts].fPosY
					+ fPosDiffY * fFrame;
				float fPosDestZ = setPos.z	// Z座標
					+ aInfo[nNowMotion].aKeyInfo[nNowKey].aKey[nCntParts].fPosZ
					+ fPosDiffZ * fFrame;

				//現在のフレームの向きを求める
				float fRotDestX = setRot.x	// X向き
					+ aInfo[nNowMotion].aKeyInfo[nNowKey].aKey[nCntParts].fRotX
					+ fRotDiffX * fFrame;
				float fRotDestY = setRot.y	// Y向き
					+ aInfo[nNowMotion].aKeyInfo[nNowKey].aKey[nCntParts].fRotY
					+ fRotDiffY * fFrame;
				float fRotDestZ = setRot.z	// Z向き
					+ aInfo[nNowMotion].aKeyInfo[nNowKey].aKey[nCntParts].fRotZ
					+ fRotDiffZ * fFrame;

				// 設定
				m_FileData.ppParts[nCntParts]->SetCurrentPosition(D3DXVECTOR3(fPosDestX, fPosDestY, fPosDestZ));
				m_FileData.ppParts[nCntParts]->SetCurrentRotation(D3DXVECTOR3(fRotDestX, fRotDestY, fRotDestZ));
			}

			m_nNowFrame++;

			if (aInfo[nNowMotion].aKeyInfo[nNowKey].nFrame != 0)
			{//フレームが0ではない場合
				if (m_nNowFrame >= nFrame)
				{//現在のフレーム数が到達した場合
					m_nNowKey = nNextkey;	//次のキーに移動
					m_nNowFrame = 0;
				}
			}
			else
			{//0の場合
				m_nNowKey = 0;
				m_nNowFrame = 0;
			}
		}
	}
}

//==========================================================
// モーション情報読み込み
//==========================================================
void CMotion::LoadMotionData(FILE *pFile)
{
	char aStr[128] = {};	//配置数文字格納用
	int nCntParts = 0;
	int nCntKey = 0;
	bool bKey = false;

	while (1)
	{
		//テキスト読み込み
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (nCntKey >= aInfo[m_nNumMotion].nNumKey && bKey == true)
		{//キー数に来た場合
			m_nNumMotion++;	// モーション数カウントアップ
			break;
		}
		if (strcmp(&aStr[0], "LOOP") == 0)
		{//ループ確認
			int nLoop = 0;
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &nLoop);	//(=)読み込み

			if (nLoop == 0)
			{//ループしない場合
				aInfo[m_nNumMotion].bLoop = false;	//ループしないようにする
			}
			else
			{
				aInfo[m_nNumMotion].bLoop = true;	//ループするようにする
			}
		}
		else if (strcmp(&aStr[0], "NUM_KEY") == 0)
		{//キー数
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &aInfo[m_nNumMotion].nNumKey);	//キー数読み込み

			// パーツ数分キーを確保
			for (int nCnt = 0; nCnt < MAX_KEY; nCnt++)
			{
				aInfo[m_nNumMotion].aKeyInfo[nCnt].aKey = new KEY[m_FileData.nNumParts];

				for (int i = 0; i < m_FileData.nNumParts; i++)
				{
					aInfo[m_nNumMotion].aKeyInfo[nCnt].aKey[i].fPosX = 0.0f;
					aInfo[m_nNumMotion].aKeyInfo[nCnt].aKey[i].fPosY = 0.0f;
					aInfo[m_nNumMotion].aKeyInfo[nCnt].aKey[i].fPosZ = 0.0f;
					aInfo[m_nNumMotion].aKeyInfo[nCnt].aKey[i].fRotX = 0.0f;
					aInfo[m_nNumMotion].aKeyInfo[nCnt].aKey[i].fRotY = 0.0f;
					aInfo[m_nNumMotion].aKeyInfo[nCnt].aKey[i].fRotZ = 0.0f;
				}
			}

			bKey = true;	//キー数を取得した状態にする
		}
		else if (strcmp(&aStr[0], "KEYSET") == 0)
		{//パーツ情報確認文字あった場合
			bool bFrame = false;
			nCntParts = 0;
			while (1)
			{
				//読み込み
				fscanf(pFile, "%s", &aStr[0]);

				if (nCntParts >= m_FileData.nNumParts)
				{//パーツ総数に到達した場合
					break;
				}
				else if (strcmp(&aStr[0], "FRAME") == 0)
				{//フレーム数
					fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
					fscanf(pFile, "%d", &aInfo[m_nNumMotion].aKeyInfo[nCntKey].nFrame);	//読み込み
					bFrame = true;
				}
				else if (strcmp(&aStr[0], "KEY") == 0 && bFrame == true)
				{//親番号
					while (1)
					{
						//読み込み
						fscanf(pFile, "%s", &aStr[0]);

						if (strcmp(&aStr[0], "END_KEY") == 0)
						{//フレーム数
							break;
						}
						else if (strcmp(&aStr[0], "POS") == 0)
						{//フレーム数
							fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
							fscanf(pFile, "%f", &aInfo[m_nNumMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosX);	//x座標読み込み
							fscanf(pFile, "%f", &aInfo[m_nNumMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosY);	//y座標読み込み
							fscanf(pFile, "%f", &aInfo[m_nNumMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosZ);	//z座標読み込み
						}
						else if (strcmp(&aStr[0], "ROT") == 0)
						{//フレーム数																   
							fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み								
							fscanf(pFile, "%f", &aInfo[m_nNumMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotX);	//x座標読み込み
							fscanf(pFile, "%f", &aInfo[m_nNumMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotY);	//y座標読み込み
							fscanf(pFile, "%f", &aInfo[m_nNumMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotZ);	//z座標読み込み
						}
					}
					nCntParts++;
				}
				else if (strcmp(&aStr[0], "END_KEYSET") == 0)
				{//パーツ情報終了文字確認できた場合
					break;
				}
			}
			nCntKey++;	//キーを移動
		}
		if (strcmp(&aStr[0], "END_MOTIONSET") == 0)
		{//キャラクター情報終了文字確認できた場合
			m_nNumMotion++;	// モーション数カウントアップ
			break;
		}
	}
}

//==========================================================
// モーション番号設定
//==========================================================
void CMotion::Set(int nType)
{
	if (nType >= 0 && nType < m_nNumMotion)
	{// モーション範囲内
		if (m_nNowMotion != nType)
		{// 現在のモーションと違う場合

			m_nNowMotion = nType;	// 種類を設定
			m_nNowKey = 0;
			m_nNowFrame = 0;

			for (int nCntParts = 0; nCntParts < m_FileData.nNumParts; nCntParts++)
			{
				D3DXVECTOR3 setPos = m_FileData.ppParts[nCntParts]->GetPosition();	// 初期位置
				D3DXVECTOR3 setRot = m_FileData.ppParts[nCntParts]->GetRotation();	// 初期向き

				//現在のフレームの座標を求める
				float fPosDestX = setPos.x	// X座標
					+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fPosX;
				float fPosDestY = setPos.y	// Y座標
					+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fPosY;
				float fPosDestZ = setPos.z	// Z座標
					+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fPosZ;

				//現在のフレームの向きを求める
				float fRotDestX = setRot.x	// X向き
					+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fRotX;
				float fRotDestY = setRot.y	// Y向き
					+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fRotY;
				float fRotDestZ = setRot.z	// Z向き
					+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fRotZ;

				// 設定
				m_FileData.ppParts[nCntParts]->SetCurrentPosition(D3DXVECTOR3(fPosDestX, fPosDestY, fPosDestZ));
				m_FileData.ppParts[nCntParts]->SetCurrentRotation(D3DXVECTOR3(fRotDestX, fRotDestY, fRotDestZ));
			}
		}
	}
}

void CMotion::Set(int nType, bool b)
{
	if (nType >= 0 && nType < m_nNumMotion)
	{// モーション範囲内

		m_nNowMotion = nType;	// 種類を設定
		m_nNowKey = 0;
		m_nNowFrame = 0;

		for (int nCntParts = 0; nCntParts < m_FileData.nNumParts; nCntParts++)
		{
			D3DXVECTOR3 setPos = m_FileData.ppParts[nCntParts]->GetPosition();	// 初期位置
			D3DXVECTOR3 setRot = m_FileData.ppParts[nCntParts]->GetRotation();	// 初期向き

			//現在のフレームの座標を求める
			float fPosDestX = setPos.x	// X座標
				+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fPosX;
			float fPosDestY = setPos.y	// Y座標
				+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fPosY;
			float fPosDestZ = setPos.z	// Z座標
				+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fPosZ;

			//現在のフレームの向きを求める
			float fRotDestX = setRot.x	// X向き
				+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fRotX;
			float fRotDestY = setRot.y	// Y向き
				+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fRotY;
			float fRotDestZ = setRot.z	// Z向き
				+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fRotZ;

			// 設定
			m_FileData.ppParts[nCntParts]->SetCurrentPosition(D3DXVECTOR3(fPosDestX, fPosDestY, fPosDestZ));
			m_FileData.ppParts[nCntParts]->SetCurrentRotation(D3DXVECTOR3(fRotDestX, fRotDestY, fRotDestZ));
		}
	}
}

//==========================================================
// モーションキー設定
//==========================================================
void CMotion::SetKey(int nKey)
{
	m_nNowKey = nKey;

	for (int nCntParts = 0; nCntParts < m_FileData.nNumParts; nCntParts++)
	{
		D3DXVECTOR3 setPos = m_FileData.ppParts[nCntParts]->GetPosition();	// 初期位置
		D3DXVECTOR3 setRot = m_FileData.ppParts[nCntParts]->GetRotation();	// 初期向き

		//現在のフレームの座標を求める
		float fPosDestX = setPos.x	// X座標
			+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fPosX;
		float fPosDestY = setPos.y	// Y座標
			+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fPosY;
		float fPosDestZ = setPos.z	// Z座標
			+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fPosZ;

		//現在のフレームの向きを求める
		float fRotDestX = setRot.x	// X向き
			+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fRotX;
		float fRotDestY = setRot.y	// Y向き
			+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fRotY;
		float fRotDestZ = setRot.z	// Z向き
			+ aInfo[m_nNowMotion].aKeyInfo[m_nNowKey].aKey[nCntParts].fRotZ;

		// 設定
		m_FileData.ppParts[nCntParts]->SetCurrentPosition(D3DXVECTOR3(fPosDestX, fPosDestY, fPosDestZ));
		m_FileData.ppParts[nCntParts]->SetCurrentRotation(D3DXVECTOR3(fRotDestX, fRotDestY, fRotDestZ));
	}
}

//==========================================================
// パーツ数取得
//==========================================================
void CMotion::SetModel(CModel **pModel, int nNumParts)
{
	m_FileData.ppParts = pModel;
	m_FileData.nNumParts = nNumParts;
}

//==========================================================
// パーツ数取得
//==========================================================
CMotion::KEY CMotion::GetKey(int nKey, int nMotion, int nParts)
{
	KEY key = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

	if (nParts >= 0 && nParts < m_FileData.nNumParts)
	{
		key = aInfo[nMotion].aKeyInfo[nKey].aKey[nParts];
	}

	return key;
}

//==========================================================
// キー情報を設定
//==========================================================
void CMotion::SetKey(int nKey, int nMotion, int nParts)
{
	if (nParts >= 0 && nParts < m_FileData.nNumParts)
	{
		D3DXVECTOR3 pos = m_FileData.ppParts[nParts]->GetCurrentPosition() - m_FileData.ppParts[nParts]->GetPosition();
		D3DXVECTOR3 rot = m_FileData.ppParts[nParts]->GetCurrentRotation() - m_FileData.ppParts[nParts]->GetRotation();
		aInfo[nMotion].aKeyInfo[nKey].aKey[nParts] = {pos.x, pos.y, pos.z, rot.x, rot.y, rot.z};
	}
}

//==========================================================
// 初期化
//==========================================================
void CMotion::Reset(void)
{
	for (int nCntParts = 0; nCntParts < m_FileData.nNumParts; nCntParts++)
	{
		D3DXVECTOR3 setPos = m_FileData.ppParts[nCntParts]->GetPosition();	// 初期位置
		D3DXVECTOR3 setRot = m_FileData.ppParts[nCntParts]->GetRotation();	// 初期向き

		// 設定
		m_FileData.ppParts[nCntParts]->SetCurrentPosition(setPos);
		m_FileData.ppParts[nCntParts]->SetCurrentRotation(setRot);
	}
}

//==========================================================
// 初期化
//==========================================================
int CMotion::GetFrame(void)
{
	int nFrame = 0;

	for (int nCntKey = 0; nCntKey < m_nNowKey; nCntKey++)
	{
		nFrame += aInfo[m_nNowMotion].aKeyInfo[nCntKey].nFrame;
	}

	return nFrame + m_nNowFrame;
}

//==========================================================
// 初期化
//==========================================================
int CMotion::GetNumFrame(void)
{
	int nFrame = 0;

	for (int nCntKey = 0; nCntKey < aInfo[m_nNowMotion].nNumKey; nCntKey++)
	{
		nFrame += aInfo[m_nNowMotion].aKeyInfo[nCntKey].nFrame;
	}

	return nFrame;
}

//==========================================================
// モーション情報書き出し
//==========================================================
void CMotion::Save(void)
{
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data\\motion_info.txt", "w");

	if (pFile == NULL)
	{// 開けなかった場合
		return;
	}

	// 書き出し開始(見出し)
	fprintf(pFile, "+=========================================+\n");
	fprintf(pFile, "|                                         |\n");
	fprintf(pFile, "|              モーション情報             |\n");
	fprintf(pFile, "|           [ モーション番号 : %d]         |\n", m_nNowMotion);
	fprintf(pFile, "|                                         |\n");
	fprintf(pFile, "+=========================================+\n\n");

	// 確認文字
	fprintf(pFile, "MOTIONSET\n");

	// ループ確認
	fprintf(pFile, "	LOOP = ");
	if (aInfo[m_nNowMotion].bLoop == false)
	{// しない
		fprintf(pFile, "0");
	}
	else
	{// する
		fprintf(pFile, "1");
	}
	fprintf(pFile, "		# ループするかどうか[0:ループしない / 1:ループする]\n");

	// キー数
	fprintf(pFile, "	NUM_KEY = %d		# キー数\n\n", aInfo[m_nNowMotion].nNumKey);

	// キー情報書き出し
	for (int nCntKey = 0; nCntKey < aInfo[m_nNowMotion].nNumKey; nCntKey++)
	{
		// キー情報開始文字
		fprintf(pFile, "	KEYSET			# --- << KEY : %d / %d >> ---\n", nCntKey, aInfo[m_nNowMotion].nNumKey);

		// フレーム数
		fprintf(pFile, "		FRAME = %d\n", aInfo[m_nNowMotion].aKeyInfo[nCntKey].nFrame);

		for (int nCntParts = 0; nCntParts < m_FileData.nNumParts; nCntParts++)
		{// パーツ分繰り返し
			KEY key = aInfo[m_nNowMotion].aKeyInfo[nCntKey].aKey[nCntParts];	// キー情報を取得

			// 情報書き出し
			fprintf(pFile, "		KEY	# ----- [ %d ] -----\n", nCntParts);
			fprintf(pFile, "			POS = %.2f %.2f %.2f\n", key.fPosX, key.fPosY, key.fPosZ);
			fprintf(pFile, "			ROT = %.2f %.2f %.2f\n", key.fRotX, key.fRotY, key.fRotZ);
			fprintf(pFile, "		END_KEY\n");

			if (nCntParts + 1 < m_FileData.nNumParts)
			{// 最後の手前まで
				fprintf(pFile, "\n");
			}
		}

		fprintf(pFile, "	END_KEYSET\n");	// 終了文字

		if (nCntKey + 1 < aInfo[m_nNowMotion].nNumKey)
		{// 最後の手前まで
			fprintf(pFile, "\n");
		}
	}

	fprintf(pFile, "END_MOTIONSET\n");	// 終了文字

	// ファイルを閉じる
	fclose(pFile);
}