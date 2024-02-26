//==========================================================
//
// �`�F�b�N�|�C���g�}�l�[�W���[ [checkpoint_manager.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "checkpoint_manager.h"
#include "checkpoint.h"

// �ÓI�����o�ϐ��錾
CCheckPointManager* CCheckPointManager::m_pInstance = nullptr;	// �C���X�^���X

//==========================================================
// �R���X�g���N�^
//==========================================================
CCheckPointManager::CCheckPointManager()
{
	// �l�̃N���A
	m_pCur = nullptr;
	m_pTop = nullptr;
	m_nNum = 0;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CCheckPointManager::~CCheckPointManager()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CCheckPointManager::Init(void)
{
	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CCheckPointManager::Uninit(void)
{

}

//==========================================================
// �X�V����
//==========================================================
void CCheckPointManager::Update(void)
{

}

//==========================================================
// �C���X�^���X���m��
//==========================================================
CCheckPointManager* CCheckPointManager::GetInstance(void)
{
	if (m_pInstance == nullptr) {	// �g���Ă��Ȃ�
		m_pInstance = new CCheckPointManager;
	}

	return m_pInstance;
}

//==========================================================
// �C���X�^���X�������[�X
//==========================================================
void CCheckPointManager::Release(void)
{
	if (m_pInstance != nullptr) {	// �C���X�^���X���m�ۂ���Ă���
		m_pInstance->Uninit();
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

//==========================================================
// ���X�g�ɑ}��
//==========================================================
void CCheckPointManager::ListIn(CCheckPoint* pCheckPoint)
{
	if (m_pTop != nullptr)
	{// �擪�����݂��Ă���ꍇ
		m_pCur->SetNext(pCheckPoint);	// ���ݍŌ���̃I�u�W�F�N�g�̃|�C���^�ɂȂ���
		pCheckPoint->SetPrev(m_pCur);
		m_pCur = pCheckPoint;	// �������g���Ō���ɂȂ�
	}
	else
	{// ���݂��Ȃ��ꍇ
		m_pTop = pCheckPoint;	// �������g���擪�ɂȂ�
		m_pCur = pCheckPoint;	// �������g���Ō���ɂȂ�
	}

	m_nNum++;
}

//==========================================================
// ���X�g����O��
//==========================================================
void CCheckPointManager::ListOut(CCheckPoint* pCheckPoint)
{
	// ���X�g���玩�����g���폜����
	if (m_pTop == pCheckPoint)
	{// ���g���擪
		if (pCheckPoint->GetNext() != nullptr)
		{// �������݂��Ă���
			m_pTop = pCheckPoint->GetNext();	// ����擪�ɂ���
			m_pTop->SetPrev(nullptr);	// ���̑O�̃|�C���^���o���Ă��Ȃ��悤�ɂ���
		}
		else
		{// ���݂��Ă��Ȃ�
			m_pTop = nullptr;	// �擪���Ȃ���Ԃɂ���
			m_pCur = nullptr;	// �Ō�����Ȃ���Ԃɂ���
		}
	}
	else if (m_pCur == pCheckPoint)
	{// ���g���Ō��
		if (pCheckPoint->GetPrev() != nullptr)
		{// �������݂��Ă���
			m_pCur = pCheckPoint->GetPrev();		// �O���Ō���ɂ���
			m_pCur->SetNext(nullptr);			// �O�̎��̃|�C���^���o���Ă��Ȃ��悤�ɂ���
		}
		else
		{// ���݂��Ă��Ȃ�
			m_pTop = nullptr;	// �擪���Ȃ���Ԃɂ���
			m_pCur = nullptr;	// �Ō�����Ȃ���Ԃɂ���
		}
	}
	else
	{
		if (pCheckPoint->GetNext() != nullptr)
		{
			pCheckPoint->GetNext()->SetPrev(pCheckPoint->GetPrev());	// ���g�̎��ɑO�̃|�C���^���o��������
		}
		if (pCheckPoint->GetPrev() != nullptr)
		{
			pCheckPoint->GetPrev()->SetNext(pCheckPoint->GetNext());	// ���g�̑O�Ɏ��̃|�C���^���o��������
		}
	}

	m_nNum--;
}

//==========================================================
// �U���q�b�g�m�F
//==========================================================
void CCheckPointManager::Hit(const D3DXVECTOR3& pos, D3DXVECTOR3& CheckPointPos)
{
	CCheckPoint* pCheckPoint = m_pTop;

	//�ʔ���
	while (pCheckPoint != nullptr) {
		CCheckPoint* pCheckPointNext = pCheckPoint->GetNext();
		pCheckPoint->Hit(pos, CheckPointPos);
		pCheckPoint = pCheckPointNext;
	}
}