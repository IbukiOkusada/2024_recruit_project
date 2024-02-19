//==========================================================
//
// �G�}�l�[�W���[ [enemy_manager.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "enemy_manager.h"
#include "enemy.h"
#include "lockon.h"

// �ÓI�����o�ϐ��錾
CEnemyManager* CEnemyManager::m_pInstance = nullptr;	// �C���X�^���X

//==========================================================
// �R���X�g���N�^
//==========================================================
CEnemyManager::CEnemyManager()
{
	// �l�̃N���A
	m_pCur = nullptr;
	m_pTop = nullptr;
	m_nNum = 0;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CEnemyManager::~CEnemyManager()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CEnemyManager::Init(void)
{
	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CEnemyManager::Uninit(void)
{

}

//==========================================================
// �X�V����
//==========================================================
void CEnemyManager::Update(void)
{

}

//==========================================================
// �C���X�^���X���m��
//==========================================================
CEnemyManager* CEnemyManager::GetInstance(void)
{
	if (m_pInstance == nullptr) {	// �g���Ă��Ȃ�
		m_pInstance = new CEnemyManager;
	}

	return m_pInstance;
}

//==========================================================
// �C���X�^���X�������[�X
//==========================================================
void CEnemyManager::Release(void)
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
void CEnemyManager::ListIn(CEnemy* pEnemy)
{
	if (m_pTop != nullptr)
	{// �擪�����݂��Ă���ꍇ
		m_pCur->SetNext(pEnemy);	// ���ݍŌ���̃I�u�W�F�N�g�̃|�C���^�ɂȂ���
		pEnemy->SetPrev(m_pCur);
		m_pCur = pEnemy;	// �������g���Ō���ɂȂ�
	}
	else
	{// ���݂��Ȃ��ꍇ
		m_pTop = pEnemy;	// �������g���擪�ɂȂ�
		m_pCur = pEnemy;	// �������g���Ō���ɂȂ�
	}

	m_nNum++;
}

//==========================================================
// ���X�g����O��
//==========================================================
void CEnemyManager::ListOut(CEnemy* pEnemy)
{
	// ���X�g���玩�����g���폜����
	if (m_pTop == pEnemy)
	{// ���g���擪
		if (pEnemy->GetNext() != nullptr)
		{// �������݂��Ă���
			m_pTop = pEnemy->GetNext();	// ����擪�ɂ���
			m_pTop->SetPrev(nullptr);	// ���̑O�̃|�C���^���o���Ă��Ȃ��悤�ɂ���
		}
		else
		{// ���݂��Ă��Ȃ�
			m_pTop = nullptr;	// �擪���Ȃ���Ԃɂ���
			m_pCur = nullptr;	// �Ō�����Ȃ���Ԃɂ���
		}
	}
	else if (m_pCur == pEnemy)
	{// ���g���Ō��
		if (pEnemy->GetPrev() != nullptr)
		{// �������݂��Ă���
			m_pCur = pEnemy->GetPrev();		// �O���Ō���ɂ���
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
		if (pEnemy->GetNext() != nullptr)
		{
			pEnemy->GetNext()->SetPrev(pEnemy->GetPrev());	// ���g�̎��ɑO�̃|�C���^���o��������
		}
		if (pEnemy->GetPrev() != nullptr)
		{
			pEnemy->GetPrev()->SetNext(pEnemy->GetNext());	// ���g�̑O�Ɏ��̃|�C���^���o��������
		}
	}

	m_nNum--;
}

//==========================================================
// �U���q�b�g�m�F
//==========================================================
bool CEnemyManager::Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage, CEnemy* pTarget)
{
	CEnemy* pEnemy = m_pTop;
	bool bhit = false;

	//�ʔ���
	while (pEnemy != nullptr) {
		CEnemy* pEnemyNext = pEnemy->GetNext();
		if (pTarget != nullptr && pTarget != pEnemy) { // �ڕW�����݂��Ă��Ă��m�F���Ă���̂ƈ�v���Ă��Ȃ�
			pEnemy = pEnemyNext;
			continue;
		}
		if (pEnemy->Hit(pos, fRange, nDamage)) { // ��������
			bhit = true; 
			CLockOn::Check(pEnemy);
		}	
		pEnemy = pEnemyNext;
	}

	return bhit;
}

//==========================================================
// �Փ˔���
//==========================================================
void CEnemyManager::Bump(D3DXVECTOR3& pos, const float fRange, CEnemy* pMy)
{
	CEnemy* pEnemy = m_pTop;

	//�ʔ���
	while (pEnemy != nullptr) {
		CEnemy* pEnemyNext = pEnemy->GetNext();
		if (pMy == pEnemy) { // �������g�̏ꍇ
			pEnemy = pEnemyNext;
			continue;
		}

		// �������擾
		D3DXVECTOR3 EnemyPos = pEnemy->GetPosition();
		float fLength = sqrtf((pos.x - EnemyPos.x) * (pos.x - EnemyPos.x)
			+ (pos.z - EnemyPos.z) * (pos.z - EnemyPos.z));
		float fFusionRange = fRange + pEnemy->GetBumpSize();

		if (fLength > fFusionRange) {	// �������߂�
			pEnemy = pEnemyNext;
			continue;
		}

		// �G��Ă���̂ł߂荞�݂𒼂�
		D3DXVECTOR3 vec = D3DXVECTOR3(pos.x - EnemyPos.x, pos.y - EnemyPos.y, pos.z - EnemyPos.z);
		D3DXVec3Normalize(&vec, &vec);
		pos = D3DXVECTOR3(EnemyPos.x + fFusionRange * vec.x, pos.y, EnemyPos.z + fFusionRange * vec.z);

		pEnemy = pEnemyNext;
	}
}