//===============================================
//
// パーティクルの処理 [particle.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"
#include "billboard.h"

//===============================================
// マクロ定義
//===============================================
#define LIFE	(30)		// 寿命

//===============================================
// コンストラクタ
//===============================================
CParticle::CParticle()
{

}

//===============================================
// デストラクタ
//===============================================
CParticle::~CParticle()
{

}

//===============================================
// 生成
//===============================================
void CParticle::Create(const D3DXVECTOR3& pos, CEffect::TYPE type)
{
	// オブジェクトの種類の設定
	Set(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);
}

//===============================================
// 頂点情報設定
//===============================================
void CParticle::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, CEffect::TYPE type)
{
	// オブジェクトの種類の設定
	Set(pos, move, type);
}

//===============================================
// 設定
//===============================================
void CParticle::Set(const D3DXVECTOR3& Defpos, const D3DXVECTOR3& Defmove, CEffect::TYPE type)
{
	D3DXVECTOR3 pos = {};
	D3DXVECTOR3 move = {};	// 移動量
	D3DXCOLOR col = {};	// 色
	CEffect::TYPE ParType = type;
	float fRadius = 0.0f;
	float fLife = 0;
	D3DXVECTOR3 nor;
	float fCol = (float)(rand() % 3) / 10.0f + 0.3f;

	// 移動ベクトルを求める
	D3DXVec3Normalize(&nor, &Defmove);	// ベクトルを正規化する

	switch (type)
	{
	case CEffect::TYPE_NONE:	// 何もない

		break;

	case CEffect::TYPE_SMAKE:	// 煙

		for (int nCnt = 0; nCnt < 20; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.09f;
			move.y = ((float)(rand() % 50)) * 0.15f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.09f;

			//色の設定
			col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

			//半径の設定
			fRadius = 100.0f;

			//寿命の設定
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}

		break;

	case CEffect::TYPE_ITEMGET:	// 煙
	{
		for (int nCnt = 0; nCnt < 20; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.05f;
			move.y = ((float)(rand() % 50)) * 0.2f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.05f;

			//色の設定
			col = D3DXCOLOR(1.0f, rand() % 10 * 0.1f + 0.0f, 0.0f, 1.0f);

			//半径の設定
			fRadius = 10.0f;

			//寿命の設定
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}

	case CEffect::TYPE_LANDCLEAR:	// 煙
	{
		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.05f;
			move.y = ((float)(rand() % 50)) * 0.2f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.05f;

			//色の設定
			col = D3DXCOLOR(0.3f, 0.3f, 0.1f, 1.0f);

			//半径の設定
			fRadius = 10.0f;

			//寿命の設定
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}

	case CEffect::TYPE_LANDFAILED:	// 煙
	{
		for (int nCnt = 0; nCnt < 60; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.1f;
			move.y = 0.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.1f;

			//色の設定
			col = D3DXCOLOR(0.025f, 0.02f, 0.015f, 1.0f);

			//半径の設定
			fRadius = 30.0f;

			//寿命の設定
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_HIT:	// ヒット
	{
		for (int nCnt = 0; nCnt < 15; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.15f;
			move.y = sinf((float)(rand() % 629 - 314) * 0.01f) * 4.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.15f;

			//色の設定
			col = D3DXCOLOR(rand() % 5 * 0.1f + 0.6f, 1.0f, 0.3f, 1.0f);

			//半径の設定
			fRadius = 7.0f;

			//寿命の設定
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_SPEAR:	// ヒット
	{
		for (int nCnt = 0; nCnt < 25; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.075f;
			move.y = rand() % 100 * 0.01f * 20.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.075f;

			//色の設定
			col = D3DXCOLOR(rand() % 2 * 0.1f + 0.9f, 1.0f, 1.0f, 1.0f);

			//半径の設定
			fRadius = 17.0f;

			//寿命の設定
			fLife = 250.0f + rand() % 50;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_BLACKSMAKE:	// 煙
	{
		for (int nCnt = 0; nCnt < 25; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.09f;
			move.y = ((float)(rand() % 50)) * 0.15f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.09f;

			//色の設定
			col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);

			//半径の設定
			fRadius = 100.0f;

			//寿命の設定
			fLife = 30.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
		break;

	case CEffect::TYPE_WALK:	// 煙
	{
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.015f;
			move.y = rand() % 100 * 0.01f * 5.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.015f;

			//色の設定
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

			//半径の設定
			fRadius = 17.0f;

			//寿命の設定
			fLife = 250.0f + rand() % 50;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
		break;

	case CEffect::TYPE_KUNAI:	// 煙
	{
		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;
			move.y = ((float)(rand() % 50)) * 0.01f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;

			//色の設定
			col = D3DXCOLOR(0.1f, 0.1f, 0.8f, 1.0f);

			//半径の設定
			fRadius = 7.0f;

			//寿命の設定
			fLife = 30.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_BUTTON:	// 煙
	{
		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.03f;
			move.y = ((float)(rand() % 50)) * 0.01f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.03f;

			//色の設定
			col = D3DXCOLOR(0.1f, 0.6f, 0.6f, 0.3f);

			//半径の設定
			fRadius = 70.0f;

			//寿命の設定
			fLife = 30.0f + rand() % 10;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_ROTATEDOOR:	// 煙
	{
		for (int nCnt = 0; nCnt < 12; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.15f;
			move.y = sinf((float)(rand() % 629 - 314) * 0.01f) * 4.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.15f;

			float frand = rand() % 5 * 0.1f;

			//色の設定
			col = D3DXCOLOR(frand + 0.6f, frand + 0.6f, 1.0f, 1.0f);

			//半径の設定
			fRadius = 15.0f;

			//寿命の設定
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_ITEMBOX:	// 煙
	{
		for (int nCnt = 0; nCnt < 12; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.05f;
			move.y = ((float)(rand() % 50)) * 0.2f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.05f;

			float frand = rand() % 2 * 0.1f;

			//色の設定
			col = D3DXCOLOR(frand + 0.9f, frand + 0.9f, 1.0f, 1.0f);

			//半径の設定
			fRadius = 6.0f;

			//寿命の設定
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_ITEMBOXSTAR:	// 煙
	{
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;
			move.y = ((float)(rand() % 3)) * 2.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;

			float frand = rand() % 6 * 0.1f;

			//色の設定
			col = D3DXCOLOR(1.0f, frand, frand * 0.5f, 1.0f);

			//半径の設定
			fRadius = 18.0f;

			//寿命の設定
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_RESULTZITABATA:	// 煙
	{
		for (int nCnt = 0; nCnt < 20; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.03f;
			move.y = ((float)(rand() % 2)) * 0.5f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.03f;

			//色の設定
			col = D3DXCOLOR(0.05f, 0.04f, 0.03f, 1.0f);

			//半径の設定
			fRadius = 30.0f;

			//寿命の設定
			fLife = 120.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_PULLSTAR:	// 煙
	{
		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;
			move.y = ((float)(rand() % 3 + 1)) * 3.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;

			float frand = rand() % 8 * 0.1f;

			//色の設定
			col = D3DXCOLOR(1.0f, frand + 0.3f, frand, 1.0f);

			//半径の設定
			fRadius = 30.0f;

			//寿命の設定
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_PULLNOW:	// 煙
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;
			move.y = ((float)(rand() % 3 + 1)) * 0.1f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;

			float frand = rand() % 2 * 0.1f;

			//色の設定
			col = D3DXCOLOR(frand + 0.9f, frand + 0.9f, frand + 0.9f, 1.0f);

			//半径の設定
			fRadius = 30.0f;

			//寿命の設定
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_AIR:	// 煙
	{
		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.1f;
			move.y = ((float)(rand() % 3 + 1)) * 2.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.1f;

			float frand = rand() % 2 * 0.1f;

			//色の設定
			col = D3DXCOLOR(frand + 0.9f, frand + 0.9f, frand + 0.9f, 1.0f);

			//半径の設定
			fRadius = 15.0f;

			//寿命の設定
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_CATCH:	// 煙
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.03f;
			move.y = ((float)(rand() % 3 + 1)) * 0.95f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.03f;

			float frand = rand() % 2 * 0.1f;

			//色の設定
			col = D3DXCOLOR(frand + 0.7f, frand + 0.7f, 1.0f, 1.0f);

			//半径の設定
			fRadius = 8.0f;

			//寿命の設定
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_JUMP:	// 煙
	{
		for (int nCnt = 0; nCnt < 12; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.055f;
			move.y = rand() % 100 * 0.01f * 11.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.055f;

			//色の設定
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

			//半径の設定
			fRadius = 8.0f + rand() % 10;

			//寿命の設定
			fLife = 250.0f + rand() % 50;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_LAND:	// 煙
	{
		for (int nCnt = 0; nCnt < 12; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.045f;
			move.y = rand() % 20 * 0.01f * -1.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.045f;

			//色の設定
			col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

			//半径の設定
			fRadius = 30.0f + rand() % 10;

			//寿命の設定
			fLife = 100.0f + rand() % 50;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_PARTY:	// 煙
	{
		for (int nCnt = 0; nCnt < 15; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.045f;
			move.y = rand() % 20 * 0.01f * -1.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.045f;

			//色の設定
			col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

			//半径の設定
			fRadius = 30.0f + rand() % 10;

			//寿命の設定
			fLife = 100.0f + rand() % 50;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_TUTORIAL:	// 煙
	{
		for (int nCnt = 0; nCnt < 15; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.045f;
			move.y = 0.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.045f;

			//色の設定
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//半径の設定
			fRadius = 10.0f + rand() % 10;

			//寿命の設定
			fLife = 100.0f + rand() % 50;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_PULLSMAKE:	// 煙
	{
		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;
			move.y = ((float)(rand() % 3 + 1)) * 3.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;

			float frand = rand() % 8 * 0.1f;

			//色の設定
			col = D3DXCOLOR(frand + 0.1f, frand, 1.0f, 1.0f);

			//半径の設定
			fRadius = 40.0f;

			//寿命の設定
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;
	}
}