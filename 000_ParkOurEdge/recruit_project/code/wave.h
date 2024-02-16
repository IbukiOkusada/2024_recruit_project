//==========================================================
//
// ���̏p [wave.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _WAVE_H_		// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _WAVE_H_		// ��d�C���N���[�h�h�~�p�}�N�����`

#include "task.h"	// ����Ńt�@�C���C���N���[�h�ł��܂�

// �O���錾
class CMeshCylinder;

//==========================================================
// �T���v���̃N���X��`
//==========================================================
class CWave : public CTask
{
public:

	// �g�p�I�u�W�F�N�g�񋓌^
	enum TYPE {
		TYPE_NORMAL = 0,	// �ʏ�
		TYPE_ANOTHER,	// ����
		TYPE_MAX
	};

private:

	// ���\����
	struct SInfo
	{
		D3DXVECTOR3 pos;	// ���W
		D3DXVECTOR3 rot;	// ����
		D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
		float fRange;		// ���a
		float fRangeOld;	// �O��̔��a
	};

public:	// �N�ł��A�N�Z�X�\

	CWave();	// �R���X�g���N�^(�I�[�o�[���[�h)
	~CWave();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CWave *Create(const D3DXVECTOR3& pos, const int nId, const float fLength = 500.0f);

	// �����o�֐�(�擾)
	D3DXVECTOR3 GetPosition(void) { return m_Info.pos; }
	int GetId(void) { return m_nId; }

	// �����o�֐�(�ݒ�)
	void SetPosition(const D3DXVECTOR3& pos) { m_Info.pos = pos; }
	void Collision(void);
	void BindId(int nId) { m_nId = nId; }

private:	// �����������A�N�Z�X�\

	// �����o�֐�

	// �����o�ϐ�
	int m_nId;	// �g�p��ID
	SInfo m_Info;	// ���
	float m_fLength;	// ����
	int m_nParticleCounter;	// �p�[�e�B�N���J�E���^�[
	CMeshCylinder *m_apObject[TYPE_MAX];	// �I�u�W�F�N�g
};

#endif