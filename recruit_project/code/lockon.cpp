//==========================================================
//
// ���b�N�I���̏��� [lockon.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "lockon.h"
#include "manager.h"
#include "texture.h"
#include "debugproc.h"
#include "renderer.h"
#include "objectX.h"
#include "model.h"
#include "camera.h"
#include "slow.h"
#include "enemy.h"
#include "character.h"
#include "sound.h"
#include "billboard.h"
#include "object_manager.h"
#include "enemy_manager.h"

// �}�N����`
#define MULTIDEF_SIZE	(50.0f)		// �f�t�H���g�T�C�Y
#define MULTISTART_SIZE	(1000.0f)	// �\���J�n�T�C�Y
#define MULTIMINUS_SIZE	(120.0f)	// �T�C�Y�ύX
#define MULTIADD_COLA	(0.1f)		// �F
#define DEF_SIZE		(35.0f)		// �f�t�H���g�T�C�Y
#define START_SIZE		(300.0f)	// �\���J�n�T�C�Y
#define MINUS_SIZE		(30.0f)		// �T�C�Y�ύX
#define ADD_COLA		(0.1f)		// �F
#define SHOT_LENGTH		(1500.0f)
#define MAX_LOCKON		(12)

// �ÓI�����o�ϐ�
CLockOn *CLockOn::m_pTop = NULL;	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
CLockOn *CLockOn::m_pCur = NULL;	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
int CLockOn::m_nNum = 0;

//==========================================================
// �R���X�g���N�^
//==========================================================
CLockOn::CLockOn()
{
	// �l�̃N���A
	m_bLock = false;
	m_pMtx = NULL;
	m_bDeath = false;
	m_pNext = NULL;
	m_pPrev = NULL;
	m_pObj = nullptr;

	// �������g�����X�g�ɒǉ�
	if (m_pTop != NULL)
	{// �擪�����݂��Ă���ꍇ
		m_pCur->m_pNext = this;	// ���ݍŌ���̃I�u�W�F�N�g�̃|�C���^�ɂȂ���
		m_pPrev = m_pCur;
		m_pCur = this;	// �������g���Ō���ɂȂ�
	}
	else
	{// ���݂��Ȃ��ꍇ
		m_pTop = this;	// �������g���擪�ɂȂ�
		m_pCur = this;	// �������g���Ō���ɂȂ�
	}

	m_nNum++;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CLockOn::~CLockOn()
{
	m_nNum--;
}

//==========================================================
// ����������
//==========================================================
HRESULT CLockOn::Init(void)
{
	m_pObj = CObjectBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), NUM_PRIORITY);
	// �F��ύX
	m_pObj->SetVtx(D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f));

	m_pObj->SetSize(MULTISTART_SIZE, MULTISTART_SIZE);

	m_pObj->BindTexture(CTexture::TYPE_LOCKON);

	m_pObj->SetDraw(false);

	m_bUse = false;

	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CLockOn::Uninit(void)
{

	m_bDeath = true;

	DeathCheck();

	if (m_pObj != nullptr) {
		m_pObj->Uninit();
		m_pObj = nullptr;
	}

	Release();
}

//==========================================================
// �X�V����
//==========================================================
void CLockOn::Update(void)
{
	if (m_pObj == nullptr) {
		return;
	}

	if (m_bUse == false && m_type == TYPE_TARGET)
	{
		return;
	}
	else if (m_type != TYPE_TARGET)
	{
		m_pObj->SetPosition(D3DXVECTOR3(m_pTarget->GetMtx()->_41, m_pTarget->GetMtx()->_42 + 50.0f, m_pTarget->GetMtx()->_43));

		float fSize = m_pObj->GetWidth();
		D3DXCOLOR col = m_pObj->GetCol();
		D3DXVECTOR3 rot = m_pObj->GetRotation();

		if (fSize > MULTIDEF_SIZE)
		{// ����̃T�C�Y���傫��
			fSize -= MULTIMINUS_SIZE;
			col.a += MULTIADD_COLA;
			rot.z += D3DX_PI * 0.025f;

			if (fSize < MULTIDEF_SIZE)
			{
				fSize = MULTIDEF_SIZE;
				rot.z = 0.0f;
			}

			if (col.a > 1.0f)
			{
				col.a = 1.0f;
			}

			if (rot.z > D3DX_PI)
			{
				rot.z += -D3DX_PI * 2;
			}

			m_pObj->SetRotation(rot);
			m_pObj->SetSize(fSize, fSize);
			m_pObj->SetCol(col);
		}

		return;
	}

	// ���b�N�I������
	LockOn();

	float fSize = m_pObj->GetWidth();
	D3DXCOLOR col = m_pObj->GetCol();
	D3DXVECTOR3 rot = m_pObj->GetRotation();

	if (fSize > DEF_SIZE)
	{// ����̃T�C�Y���傫��
		fSize -= MINUS_SIZE;
		col.a += ADD_COLA;

		if (fSize < DEF_SIZE)
		{
			fSize = DEF_SIZE;
			col.a = 1.0f;
		}

		if (col.a > 1.0f)
		{
			col.a = 1.0f;
		}

		m_pObj->SetSize(fSize, fSize);
		m_pObj->SetCol(col);
	}
}

//==========================================================
// ����
//==========================================================
CLockOn *CLockOn::Create(D3DXMATRIX *pMtx, TYPE type)
{
	CLockOn *pLock = NULL;

	// ����
	pLock = new CLockOn();

	if (pLock == NULL)
	{// �m�ۂł��Ȃ������ꍇ
		return NULL;
	}

	// ����������
	pLock->Init();

	// �e�̃}�g���b�N�X�ݒ�
	pLock->SetParent(pMtx);

	// ��ސݒ�
	pLock->m_type = type;

	return pLock;
}

//==========================================================
// ���b�N�I���^�[�Q�b�g�ݒ�
//==========================================================
void CLockOn::LockOn(void)
{
	CEnemy* pOldObj = NULL;
	D3DXMATRIX mtxProjection;
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 EnemyPos;
	D3DXVECTOR3 ScreenPos;
	D3DVIEWPORT9 Viewport;
	float fMultiScreen = 0.0f;	// ����͈�

	if (CManager::GetInstance()->GetSlow()->Get() >= 1.0f) {
		return;
	}

	if (m_pObj == nullptr) {
		return;
	}

	if (m_pMtx == nullptr) {
		return;
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetViewport(&Viewport);
	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	CEnemy* pObj = CEnemyManager::GetInstance()->GetTop();	// �擪���擾

	while (pObj != NULL)
	{// �g�p����Ă��Ȃ���Ԃ܂�

		CEnemy* pObjectNext = pObj->GetNext();	// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾

		if (pObj == NULL)
		{// �g�p����Ă��Ȃ��ꍇ
			pObj = pObjectNext;	// ���̃I�u�W�F�N�g�Ɉړ�
			continue;
		}

		if (pObj->GetDeath() == true)
		{
			pObj = pObjectNext;	// ���̃I�u�W�F�N�g�Ɉړ�
			continue;
		}

		EnemyPos.x = pObj->GetMtx()->_41;
		EnemyPos.y = pObj->GetMtx()->_42;
		EnemyPos.z = pObj->GetMtx()->_43;

		D3DXVec3Project(&ScreenPos, &EnemyPos, &Viewport, &mtxProjection, &mtxView, &mtxWorld);

		if (ScreenPos.x < SCREEN_WIDTH * fMultiScreen || ScreenPos.x > SCREEN_WIDTH * (1.0f - fMultiScreen) ||
			ScreenPos.y < SCREEN_HEIGHT * fMultiScreen || ScreenPos.y > SCREEN_HEIGHT * (1.0f - fMultiScreen) || ScreenPos.z >= 1.0f)
		{// ��ʂɕ`�悳��Ă��Ȃ�
			pObj = pObjectNext;	// ���̃I�u�W�F�N�g�Ɉړ�
			pObj = NULL;
			continue;
		}

		CLockOn* pLock = m_pTop;	// �擪���擾

		while (pLock != NULL)
		{// �g�p����Ă���ԌJ��Ԃ�
			CLockOn* pLockNext = pLock->m_pNext;	// ����ێ�

			if (pLock->m_bDeath == false && pLock->m_type == TYPE_MULTI)
			{
				if (pLock->m_pTarget == pObj)
				{// �����W�I�̏ꍇ
					pObj = NULL;
					break;
				}
			}

			pLock = pLockNext;	// ���Ɉړ�
		}

		if (pOldObj != NULL && pObj != nullptr)
		{// ��ԋ߂��I�u�W�F�N�g������ꍇ

			D3DXVECTOR3 OldPos = pOldObj->GetPosition();

			// ��ԋ߂����f���ƃv���C���[�̋��������߂�
			float fOldObjLength =
				sqrtf((OldPos.x - m_pMtx->_41) * (OldPos.x - m_pMtx->_41)
					+ (OldPos.z - m_pMtx->_43) * (OldPos.z - m_pMtx->_43));

			// ����̃��f���ƃv���C���[�̋��������߂�
			float fObjLength =
				sqrtf((EnemyPos.x - m_pMtx->_41) * (EnemyPos.x - m_pMtx->_41)
					+ (EnemyPos.z - m_pMtx->_43) * (EnemyPos.z - m_pMtx->_43));

			if (fObjLength < fOldObjLength)
			{// ����̃��f���̕����߂��ꍇ
				pOldObj = pObj;	// ��ԋ߂����f����ύX
			}
		}
		else
		{// �O�񃂃f�����o���Ă��Ȃ��ꍇ
			if (pObj != NULL)
			{
				pOldObj = pObj;
			}
		}

		pObj = pObjectNext;	// ���̃I�u�W�F�N�g�Ɉړ�
	}

	if (pOldObj != NULL)
	{
		pObj = pOldObj;
	}

	if (pObj != NULL)
	{// �g�p����Ă���ꍇ

		EnemyPos.x = pObj->GetMtx()->_41;
		EnemyPos.y = pObj->GetMtx()->_42;
		EnemyPos.z = pObj->GetMtx()->_43;
		// ����̃��f���ƃv���C���[�̋��������߂�
		float fObjLength =
			sqrtf((EnemyPos.x - m_pMtx->_41) * (EnemyPos.x - m_pMtx->_41)
				+ (EnemyPos.z - m_pMtx->_43) * (EnemyPos.z - m_pMtx->_43));

		if (fObjLength >= SHOT_LENGTH)
		{
			m_bLock = false;
			m_pObj->SetSize(0.0f, 0.0f);
			m_pTarget = NULL;
			return;
		}

		
		{// �X���[���Ă���
			if (CManager::GetInstance()->GetSlow()->Get() < 1.0f)
			{
				m_bLock = false;
				m_pObj->SetSize(0.0f, 0.0f);
				m_pTarget = NULL;

				if (m_nNum < MAX_LOCKON) {
					CLockOn* pLock = CLockOn::Create(pObj->GetMtx(), CLockOn::TYPE_MULTI);
					pLock->SetTag(pObj);
					pLock->GetObj()->SetCol(D3DXCOLOR(m_nNum * 0.1f, 1.0f - m_nNum * 0.1f, 0.5f, 0.3f));
					pLock->SetLock(true);
					pLock->GetObj()->SetDraw(true);
				}
			}
			else
			{
				m_bLock = false;
				m_pObj->SetSize(0.0f, 0.0f);
				m_pTarget = NULL;
			}
		}
	}
	else
	{
		m_bLock = false;
		m_pObj->SetSize(0.0f, 0.0f);
		m_pObj->SetDraw(false);
		m_pTarget = NULL;
	}

	//if (pObj != NULL)
	//{// �g�p����Ă���ꍇ
	//	EnemyPos.x = pObj->GetMtx()->_41;
	//	EnemyPos.y = pObj->GetMtx()->_42;
	//	EnemyPos.z = pObj->GetMtx()->_43;
	//	// ����̃��f���ƃv���C���[�̋��������߂�
	//	float fObjLength =
	//		sqrtf((EnemyPos.x - m_pMtx->_41) * (EnemyPos.x - m_pMtx->_41)
	//			+ (EnemyPos.z - m_pMtx->_43) * (EnemyPos.z - m_pMtx->_43));

	//	if (fObjLength >= SHOT_LENGTH)
	//	{
	//		m_bLock = false;
	//		m_pObj->SetSize(0.0f, 0.0f);
	//		m_pTarget = NULL;
	//		return;
	//	}	
	//	m_pObj->SetPosition(D3DXVECTOR3(pOldObj->GetPosition().x, pOldObj->GetPosition().y + 30.0f, pOldObj->GetPosition().z));
	//	if (m_pTarget != pObj)
	//	{
	//		m_pObj->SetSize(START_SIZE, START_SIZE);
	//		m_pObj->SetCol(D3DXCOLOR(m_pObj->GetCol().r, m_pObj->GetCol().g, m_pObj->GetCol().b, 0.3f));
	//		m_pObj->SetDraw(true);
	//	}
	//	m_bLock = true;
	//	m_pTarget = pObj;
	//}
	//else
	//{
	//	m_bLock = false;
	//	m_pObj->SetSize(0.0f, 0.0f);
	//	m_pObj->SetDraw(false);
	//	m_pTarget = NULL;
	//}
}

//==========================================================
// �g�p����e�}�g���b�N�X��ݒ�
//==========================================================
void CLockOn::SetParent(D3DXMATRIX *pMtx)
{
	// �e�̃}�g���b�N�X���w��
	m_pMtx = pMtx;
}

//===============================================
// ���S�m�F
//===============================================
void CLockOn::DeathCheck(void)
{
	if (m_bDeath == true)
	{
		// ���X�g���玩�����g���폜����
		if (m_pTop == this)
		{// ���g���擪
			if (m_pNext != NULL)
			{// �������݂��Ă���
				m_pTop = m_pNext;	// ����擪�ɂ���
				m_pNext->m_pPrev = NULL;	// ���̑O�̃|�C���^���o���Ă��Ȃ��悤�ɂ���
			}
			else
			{// ���݂��Ă��Ȃ�
				m_pTop = NULL;	// �擪���Ȃ���Ԃɂ���
				m_pCur = NULL;	// �Ō�����Ȃ���Ԃɂ���
			}
		}
		else if (m_pCur == this)
		{// ���g���Ō��
			if (m_pPrev != NULL)
			{// �������݂��Ă���
				m_pCur = m_pPrev;	// �O���Ō���ɂ���
				m_pPrev->m_pNext = NULL;	// �O�̎��̃|�C���^���o���Ă��Ȃ��悤�ɂ���
			}
			else
			{// ���݂��Ă��Ȃ�
				m_pTop = NULL;	// �擪���Ȃ���Ԃɂ���
				m_pCur = NULL;	// �Ō�����Ȃ���Ԃɂ���
			}
		}
		else
		{
			if (m_pNext != NULL)
			{
				m_pNext->m_pPrev = m_pPrev;	// ���g�̎��ɑO�̃|�C���^���o��������
			}
			if (m_pPrev != NULL)
			{
				m_pPrev->m_pNext = m_pNext;	// ���g�̑O�Ɏ��̃|�C���^���o��������
			}
		}
	}
}

//===============================================
// �g�p�ݒ�
//===============================================
void  CLockOn::SetLock(bool bLock)
{
	m_bUse = bLock;

	if (!m_bUse) {
		m_pTarget = nullptr;
		m_bLock = false;

		if (m_pObj != nullptr) {
			m_pObj->SetDraw(false);
		}
	}
}

//===============================================
// ���S�m�F
//===============================================
void CLockOn::Check(CEnemy* pObject)
{
	CLockOn* pLock = m_pTop;	// �擪���擾

	while (pLock != NULL)
	{// �g�p����Ă���ԌJ��Ԃ�
		CLockOn* pLockNext = pLock->m_pNext;	// ����ێ�

		if (pLock->m_bDeath == false && pLock->m_type == TYPE_MULTI)
		{
			if (pLock->m_pTarget == pObject)
			{// �����W�I�̏ꍇ
				pLock->Uninit();
			}
		}

		pLock = pLockNext;	// ���Ɉړ�
	}
}

//===============================================
// multi�^�[�Q�b�g�폜
//===============================================
void CLockOn::MultiDeath(void)
{
	CLockOn* pLock = m_pTop;	// �擪���擾

	while (pLock != NULL)
	{// �g�p����Ă���ԌJ��Ԃ�
		CLockOn* pLockNext = pLock->m_pNext;	// ����ێ�

		if (pLock->m_bDeath == false && pLock->m_type == TYPE_MULTI)
		{
			pLock->Uninit();
		}
		pLock = pLockNext;	// ���Ɉړ�
	}
}