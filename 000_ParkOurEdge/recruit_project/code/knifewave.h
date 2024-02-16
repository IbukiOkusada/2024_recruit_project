//==========================================================
//
// �e [bullet.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _KNIFEWAVE_H_		// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _KNIFEWAVE_H_		// ��d�C���N���[�h�h�~�p�}�N�����`

#include "task.h"	// ����Ńt�@�C���C���N���[�h�ł��܂�

// �O���錾
class CObjectX;

//==========================================================
// �T���v���̃N���X��`
//==========================================================
class CKnifeWave : public CTask
{
public:

	// ��ޗ񋓌^
	enum TYPE {
		TYPE_ENEMY = 0,			// �G
		TYPE_MAX
	};

private:

	// ���\����
	struct SInfo {
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 posOld;	// 
		D3DXVECTOR3 rot;	// ����
		D3DXVECTOR3 move;	// �ړ���
		D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	};

public:	// �N�ł��A�N�Z�X�\

	CKnifeWave();	// �R���X�g���N�^(�I�[�o�[���[�h)
	~CKnifeWave();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CKnifeWave *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &move, const TYPE type);

	// �����o�֐�(�擾)
	D3DXVECTOR3 GetPosition(void) const { return m_Info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_Info.rot; }
	D3DXVECTOR3 GetMove(void) const { return m_Info.move; }
	TYPE GetType(void) { return m_type; }

	// �����o�֐�(�ݒ�)
	void SetPosition(const D3DXVECTOR3 &pos) { m_Info.pos = pos; }
	void SetRotation(const D3DXVECTOR3 &rot) { m_Info.rot = rot; }
	void SetMove(const D3DXVECTOR3 &move) { m_Info.move = move; }
	void BindId(const int nId) { m_nId = nId; }
	void Hit(void);

private:	// �����������A�N�Z�X�\

	// �����o�֐�
	void Controller(void);
	void SetMatrix(void);
	void SetType(const TYPE type) { m_type = type; }

	// �����o�ϐ�
	SInfo m_Info;			// ��{���
	int m_nId;				// ID
	CObjectX *m_pObject;	// �I�u�W�F�N�g�̃|�C���^
	float m_fLife;			// ����
	TYPE m_type;
};

#endif