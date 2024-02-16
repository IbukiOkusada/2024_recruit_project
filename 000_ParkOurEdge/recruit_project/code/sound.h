//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=============================================================================
// �T�E���h�N���X�̒�`
//=============================================================================
class CSound
{
public:	// �N�ł��A�N�Z�X�\�Ȓ�`

	// ��ޗ�
	typedef enum
	{
		LABEL_BGM_TITLE = 0,	// BGM�^�C�g��
		LABEL_BGM_TUTORIAL,		// BGM�`���[�g���A��
		LABEL_BGM_GAME,			// BGM�Q�[��
		LABEL_BGM_RESULT_CLEAR,	// BGM���U���g�N���A
		LABEL_BGM_RESULT_FAILED,// BGM���U���g���s
		LABEL_BGM_RANKING,		// BGM�����L���O
		LABEL_SE_CLICK,			// Enter
		LABEL_SE_STEP,			// �X�e�b�v��
		LABEL_SE_DAMAGE,		// �_���[�W��
		LABEL_SE_SPAWN,			// ������
		LABEL_SE_AIR,			// ���̏p
		LABEL_SE_KUNAI,			// �N�i�C
		LABEL_SE_ATTACK,		// �U��
		LABEL_SE_CATCH,			// ����
		LABEL_SE_DROP,			// ���Ƃ�
		LABEL_SE_GET,			// �擾
		LABEL_SE_SELECT,		// �A�C�e���I��
		LABEL_SE_LEVER,			// ���o�[
		LABEL_SE_JUMP,			// �W�����v
		LABEL_SE_GROW,			// �g��
		LABEL_SE_FADE,			// �k��
		LABEL_SE_SEEK,			// ������
		LABEL_SE_LAND,			// ���n
		LABEL_SE_DEATH,			// ���S
		LABEL_SE_SPEAR,			// ���S
		LABEL_SE_OPEN00,		// ���͔����J��
		LABEL_SE_OPEN01,		// ���͔����܂�
		LABEL_SE_OPEN02,		// �J�n��
		LABEL_SE_WARNING,		// �x����
		LABEL_SE_COIN00,		// �R�C��
		LABEL_SE_COIN01,		// �R�C��
		LABEL_SE_BUTTON,		// �{�^����������
		LABEL_SE_MOVE,			// ��ړ�
		LABEL_SE_RAGE,			// �o�^�o�^
		LABEL_SE_ENTRY,			// �G���g���[
		LABEL_MAX,
	} LABEL;

private: // �����������A�N�Z�X�\

	 // �T�E���h���̍\���̒�`
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDINFO;

public:	// �N�ł��A�N�Z�X�\

	CSound();	// �R���X�g���N�^
	~CSound();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(LABEL label);
	void Stop(LABEL label);
	void Stop(void);

private:	// �����������A�N�Z�X�\

	// �����o�֐�
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// �����o�ϐ�
	static const SOUNDINFO m_aSoundInfo[LABEL_MAX];	// �T�E���h�t�@�C����
	IXAudio2 *m_pXAudio2;							// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;		// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[LABEL_MAX];					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y
};

#endif