//==========================================================
//
// �|��Ă��鋴 [bridge.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _BRIDGE_H_		// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _BRIDGE_H_		// ��d�C���N���[�h�h�~�p�}�N�����`

#include "task.h"	// ����Ńt�@�C���C���N���[�h�ł��܂�

class CObjectX;

//==========================================================
// �T���v���̃N���X��`
//==========================================================
class CBridge : public CTask
{

public:	// �N�ł��A�N�Z�X�\

	CBridge();	// �R���X�g���N�^(�I�[�o�[���[�h)
	~CBridge();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetClose(bool bValue) { m_bClose = bValue; }
	static CBridge *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

	// �����o�֐�(�擾)

	// �����o�֐�(�ݒ�)

private:	// �����������A�N�Z�X�\

	// �����o�֐�
	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

	// �����o�ϐ�
	CObjectX* m_pObj;
	bool m_bClose;
};

#endif
