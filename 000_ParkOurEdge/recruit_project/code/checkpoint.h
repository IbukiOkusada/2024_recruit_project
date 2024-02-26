//==========================================================
//
// �`�F�b�N�|�C���g���� [checkpoint.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _CHECKPOINT_H_		// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _CHECKPOINT_H_		// ��d�C���N���[�h�h�~�p�}�N�����`

#include "task.h"	// ����Ńt�@�C���C���N���[�h�ł��܂�

//==========================================================
// �T���v���̃N���X��`
//==========================================================
class CCheckPoint : public CTask
{
public:

	// ��ޗ񋓌^
	enum TYPE
	{
		TYPE_MELEE = 0,		// �ߐڌ^(�ߋ���)
		TYPE_GUN,			// �e(������)
		TYPE_BOSS,			// �{�X
		TYPE_MAX
	};

protected:

	// ���\����
	struct SInfo
	{
		D3DXVECTOR3 pos;		// �ʒu
	};

public:	// �N�ł��A�N�Z�X�\

	CCheckPoint();	// �R���X�g���N�^(�I�[�o�[���[�h)
	~CCheckPoint();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// �����o�֐�(�擾)
	CCheckPoint* GetNext(void) { return m_pNext; }
	CCheckPoint* GetPrev(void) { return m_pPrev; }
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	static CCheckPoint* Create(const D3DXVECTOR3& pos);

	// �����o�֐�(�ݒ�)
	void SetNext(CCheckPoint* pNext) { m_pNext = pNext; }
	void SetPrev(CCheckPoint* pPrev) { m_pPrev = pPrev; }
	void SetPosition(const D3DXVECTOR3& pos) { m_pos = pos; }

	// �����o�֐�
	void Hit(const D3DXVECTOR3& pos, D3DXVECTOR3& CheckPointPos);

private:	// �����������A�N�Z�X�\

	// �����o�ϐ�
	CCheckPoint* m_pPrev;	// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CCheckPoint* m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^
	D3DXVECTOR3 m_pos;		// �ʒu
};

#endif