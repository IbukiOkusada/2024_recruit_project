//==========================================================
//
// プレコンパイル[precompile.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _PRECOMPILE_H_
#define _PRECOMPILE_H_

//DirectXの初期設定
#include <stdio.h>
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告文対処用マクロ
#include "dinput.h"
#include "xaudio2.h"					//サウンド処理に必要

// 名前空間
namespace MYDEFAULT
{
	const D3DXVECTOR3 VECTOR3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

#endif

