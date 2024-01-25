//==========================================================
//
// �G�}�l�[�W���[ [Enemy_manager.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _ENEMYMANAGER_H_		// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _ENEMYMANAGER_H_		// ��d�C���N���[�h�h�~�p�}�N�����`

// �O���錾
class CEnemy;

//==========================================================
// �T���v���̃N���X��`
//==========================================================
class CEnemyManager
{
private:

	CEnemyManager();		// �R���X�g���N�^
	~CEnemyManager();	// �f�X�g���N�^

public:	// �N�ł��A�N�Z�X�\

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CEnemyManager* GetInstance(void);
	static void Release(void);
	CEnemy* GetTop(void) { return m_pTop; }
	CEnemy* GetCur(void) { return m_pCur; }
	void ListIn(CEnemy* pEnemy);
	void ListOut(CEnemy* pEnemy);
	int GetNum(void) { return m_nNum; }
	bool Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage);

private:	// �����������A�N�Z�X�\

	// �����o�֐�
	// 
	// �����o�ϐ�
	CEnemy* m_pTop;	// �擪
	CEnemy* m_pCur;	// �Ō��
	int m_nNum;
	static CEnemyManager* m_pInstance;	// �C���X�^���X
};

#endif
