//==========================================================
//
// 敵移動ポイント [point.h]
// Author : Soma Ishihara
//
//==========================================================
#include "point.h"

// 前方宣言
CPoint *CPoint::m_pTop = nullptr;	// 先頭のオブジェクトへのポインタ
CPoint *CPoint::m_pCur = nullptr;	// 最後尾のオブジェクトへのポインタ
int CPoint::m_nNumAll = 0;

//==========================================================
// コンストラクタ
//==========================================================
CPoint::CPoint()
{
	m_pPosPoint = nullptr;
	m_nPointNum = 0;
	m_nRegistPointNum = 0;

	// 自分自身をリストに追加
	if (m_pTop != nullptr)
	{// 先頭が存在している場合
		m_pCur->m_pNext = this;	// 現在最後尾のオブジェクトのポインタにつなげる
		m_pPrev = m_pCur;
		m_pCur = this;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合
		m_pTop = this;	// 自分自身が先頭になる
		m_pCur = this;	// 自分自身が最後尾になる
	}

	m_nNumAll++;
}

//==========================================================
// デストラクタ
//==========================================================
CPoint::~CPoint()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CPoint::Init(void)
{
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CPoint::Uninit(void)
{
	// リストから自分自身を削除する
	if (m_pTop == this)
	{// 自身が先頭
		if (m_pNext != nullptr)
		{// 次が存在している
			m_pTop = m_pNext;	// 次を先頭にする
			m_pNext->m_pPrev = nullptr;	// 次の前のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else if (m_pCur == this)
	{// 自身が最後尾
		if (m_pPrev != nullptr)
		{// 次が存在している
			m_pCur = m_pPrev;			// 前を最後尾にする
			m_pPrev->m_pNext = nullptr;	// 前の次のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else
	{
		if (m_pNext != nullptr)
		{
			m_pNext->m_pPrev = m_pPrev;	// 自身の次に前のポインタを覚えさせる
		}
		if (m_pPrev != nullptr)
		{
			m_pPrev->m_pNext = m_pNext;	// 自身の前に次のポインタを覚えさせる
		}
	}

	//位置破棄
	if (m_pPosPoint != nullptr)
	{
		delete[] m_pPosPoint;
		m_pPosPoint = nullptr;
	}
	m_nNumAll--;

	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CPoint::Update(void)
{

}

//==========================================================
// 生成
//==========================================================
CPoint *CPoint::Create(void)
{
	CPoint *pSample = nullptr;

	pSample = new CPoint;

	if (pSample != nullptr)
	{
		// 初期化処理
		pSample->Init();
	}

	return pSample;
}

//==========================================================
// 個数設定
//==========================================================
void CPoint::SetNum(const int nNum)
{
	//個数設定して位置の動的確保
	m_nPointNum = nNum;
	if (m_pPosPoint != nullptr)
	{//なんか生成されてる
		delete[] m_pPosPoint;	//消す
		m_nRegistPointNum = 0;
		
	}
	m_pPosPoint = new D3DXVECTOR3[nNum];	//確保
}

//==========================================================
// 位置設定
//==========================================================
void CPoint::AddPoint(const D3DXVECTOR3 pos)
{
	if (m_nRegistPointNum < m_nPointNum)
	{//余裕がある
		m_pPosPoint[m_nRegistPointNum] = pos;
		m_nRegistPointNum++;
	}
}
