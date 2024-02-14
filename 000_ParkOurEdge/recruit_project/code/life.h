//==========================================================
//
// �̗�UI [life.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _LIFE_H_		// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _LIFE_H_		// ��d�C���N���[�h�h�~�p�}�N�����`

#include "task.h"	// ����Ńt�@�C���C���N���[�h�ł��܂�

// �O���錾
class CObject2D;

//==========================================================
// �T���v���̃N���X��`
//==========================================================
class CLife : public CTask
{

public:	// �N�ł��A�N�Z�X�\

	CLife();	// �R���X�g���N�^(�I�[�o�[���[�h)
	~CLife();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CLife *Create(const D3DXVECTOR3& pos);

	// �����o�֐�(�擾)

	// �����o�֐�(�ݒ�)

private:	// �����������A�N�Z�X�\

	CObject2D* m_pObj;	// �I�u�W�F�N�g�̃|�C���^
};

#endif
