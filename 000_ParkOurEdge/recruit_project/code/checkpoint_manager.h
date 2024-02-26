//==========================================================
//
// �`�F�b�N�|�C���g�}�l�[�W���[ {checkpoint_manager.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _CHECKPOINTMANAGER_H_		// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _CHECKPOINTMANAGER_H_		// ��d�C���N���[�h�h�~�p�}�N�����`

// �O���錾
class CCheckPoint;

//==========================================================
// �T���v���̃N���X��`
//==========================================================
class CCheckPointManager
{
private:

	CCheckPointManager();		// �R���X�g���N�^
	~CCheckPointManager();	// �f�X�g���N�^

public:	// �N�ł��A�N�Z�X�\

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CCheckPointManager* GetInstance(void);
	static void Release(void);
	CCheckPoint* GetTop(void) { return m_pTop; }
	CCheckPoint* GetCur(void) { return m_pCur; }
	void ListIn(CCheckPoint* pCheckPoint);
	void ListOut(CCheckPoint* pCheckPoint);
	int GetNum(void) { return m_nNum; }
	void Hit(const D3DXVECTOR3& pos, D3DXVECTOR3& CheckPointPos);

private:	// �����������A�N�Z�X�\

	// �����o�֐�
	// 
	// �����o�ϐ�
	CCheckPoint* m_pTop;	// �擪
	CCheckPoint* m_pCur;	// �Ō��
	int m_nNum;
	static CCheckPointManager* m_pInstance;	// �C���X�^���X
};

#endif
