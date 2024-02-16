//===============================================
//
// �Q�[����ʂ̊Ǘ����� [game.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "objectX.h"
#include "Xfile.h"
#include "input.h"
#include "fade.h"
#include "result.h"
#include "sound.h"
#include "fileload.h"
#include "debugproc.h"
#include "time.h"
#include "meshdome.h"
#include "tcp_client.h"
#include <thread>
#include "protocol_online.h"
#include "object2D.h"
#include "player.h"
#include "score.h"
#include "pause.h"
#include <assert.h>
#include "particle.h"
#include "meshdome.h"
#include "enemy_melee.h"
#include "enemy_manager.h"
#include "player_manager.h"
#include "enemy_gun.h"
#include "enemy_boss.h"

// �������O��Ԃ��`
namespace {
    const D3DXVECTOR3 STARTDOORPOS = { -1160.0f, 0.0f, 950.0f };	// �X�^�[�g�n�_�h�A��{���W
	const D3DXVECTOR3 PLAYERSTARTPOS = { 0.0f, 0.0f, -2300.0f };  // �v���C���[�̃X�^�[�g�ʒu
    const D3DXVECTOR3 LEVERPOS[4] =
    {
        D3DXVECTOR3(130.0f, 100.0f, -5130.0f),
        D3DXVECTOR3(-1000.0f, 100.0f, -4440.0f),
        D3DXVECTOR3(470.0f, 100.0f, -560.0f),
        D3DXVECTOR3(360.0f, 100.0f, -1900.0f),
    };

    const D3DXVECTOR3 LEVERROT[4] =
    {
        D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
        D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
        D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f),
    };


	const char* TEXPASS[4] =
	{
		"data\\TEXTURE\\opendoor00.png",
		"data\\TEXTURE\\opendoor01.png",
		"data\\TEXTURE\\opendoor02.png",
		"data\\TEXTURE\\opendoor03.png",
	};

	const D3DXVECTOR3 OPEN_SETPOS = { SCREEN_WIDTH * 1.3f, SCREEN_HEIGHT * 0.5f, 0.0f };  // �X�^�[�g�h�A�J�����Ƃ���UI�̐����ʒu
	const D3DXVECTOR3 OPEN_SETROT = { 0.0f, 0.0f, D3DX_PI * 0.0f };                       // ����
	const D3DXVECTOR2 OPEN_SIZE = { 470.0f, 150.0f };                                     // �T�C�Y
	const float OPEN_MOVESPEED = (-3.3f);                                                 // �ړ��X�s�[�h
	const float OPEN_MOVESIN = (0.05f);
	const float OPEN_MOVESIZE = (30.0f);
    const D3DXVECTOR2 QUATAUI_SIZE = { 100.0f, 50.0f };	// �m���}��UI�̃T�C�Y
    const D3DXVECTOR2 SCORE_SIZE = { 14.0f, 18.0f };	// �X�R�A�̃T�C�Y
    const float DOOR_SPACE = (-20.0f);			// �e�X�^�[�g�n�_�h�A�̊�
    const char* FILEPASS = "data\\TXT\\player";	// �t�@�C���̃p�X
    const char* FILEEXT = ".txt";				// �t�@�C���̊g���q
    const int FILEPASS_SIZE = (200);			// �t�@�C���̃p�X�T�C�Y
    const int START_TIMER = (210);				// �J�n��������
    const int START_WAITCNT = (430);            // �X�^�[�g���̑����Ă鎞��
	const int PLAYER_MOVESTART = (180);
	const int CAMERA_ROTATESTART = (240);
	const D3DXVECTOR3 START_CAMERAROT = {0.0f, D3DX_PI * 0.0f, D3DX_PI * 0.38f};
    const int SCORE = (15000);                   // �����̃X�R�A
    const int UNINITCOUNT = (120);              // �m���}��UI��������܂ł̎���
	const int PLAYER_SPWANSTART = (240);
}

//===============================================
// �}�N����`
//===============================================
#define START_TIME	(60 * 4)	// ��������
#define START_SCORE	(0)		// �J�n�X�R�A
#define STARTSET_NUMENEMY	(3)	// �J�n���ɔz�u����G�̐�
#define MAX_TIME	(60 * 20 + 10)	// �ő厞��
#define SPEED_UP	(30.0f)
#define DEF_PORT	(22333)	// �|�[�g�ԍ�
#define MAX_STRING	(2048)
#define ADDRESSFILE	"data\\TXT\\address.txt"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CGame::STATE CGame::m_state = CGame::STATE_LOCAL;	// ���
int CGame::m_nNumPlayer = 0;

//===============================================
// �R���X�g���N�^
//===============================================
CGame::CGame()
{
    // �l�̃N���A
    m_ppCamera = nullptr;
    m_ppPlayer = nullptr;
    m_pFileLoad = nullptr;
    m_pMeshDome = nullptr;
    m_pClient = nullptr;
    m_pTimer = nullptr;
    m_nSledCnt = 0;
    m_bEnd = false;
	m_fOpenDoorUISin = 0.0f;
    m_bPause = false;
    m_pPause = nullptr;
}

//===============================================
// �R���X�g���N�^(�l���ݒ�)
//===============================================
CGame::CGame(int nNumPlayer)
{
    // �l�̃N���A
    m_ppCamera = nullptr;
    m_ppPlayer = nullptr;
    m_pFileLoad = nullptr;
    m_pMeshDome = nullptr;
    m_pClient = nullptr;
    m_pTimer = nullptr;
    m_nSledCnt = 0;
    m_bEnd = false;
	m_fOpenDoorUISin = 0.0f;
    m_bPause = false;
    m_pPause = nullptr;

    // �l���ݒ�
    m_nNumPlayer = nNumPlayer;
}

//===============================================
// �f�X�g���N�^
//===============================================
CGame::~CGame()
{
   
}

//===============================================
// ����������
//===============================================
HRESULT CGame::Init(void)
{
    memset(&m_aAddress[0], '\0', sizeof(m_aAddress));
    int nErr = WSAStartup(WINSOCK_VERSION, &m_wsaData);	// winsock�̏������֐�

    if (nErr != 0)
    {// �������Ɏ��s�����ꍇ
        
    }

    // �O���t�@�C���ǂݍ��݂̐���
    if (nullptr == m_pFileLoad)
    {// �g�p���Ă��Ȃ��ꍇ
        m_pFileLoad = new CFileLoad;

        if (m_pFileLoad != NULL)
        {
            m_pFileLoad->Init();
            m_pFileLoad->OpenFile("data\\TXT\\model.txt");			//���f����
        }
    }

    // �J�����̏����ʒu�ݒ�
    CCamera* pCamera = CManager::GetInstance()->GetCamera();
    pCamera->SetPositionV(D3DXVECTOR3(-874.3f, 1124.15f, 1717.2f));
    pCamera->SetPositionR(D3DXVECTOR3(-320.3f, 1.0f, -91.6f));
    pCamera->SetLength(300.0f);
    pCamera->SetRotation(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, D3DX_PI * 0.41f));
    pCamera->SetActive(true);

    switch (m_state)
    {
    case STATE_LOCAL:
    {// ���[�J���̏ꍇ
        if (m_nNumPlayer <= 0)
        {// �l�����w�肳��Ă��Ȃ�
            m_nNumPlayer = 1;
        }

        char aBodyPass[FILEPASS_SIZE] = "";		// ���̃p�X
        char aLegPass[FILEPASS_SIZE] = "";		// �����g�p�X

        sprintf(&aBodyPass[0], "%s\\motion_body%s", FILEPASS, FILEEXT);
        sprintf(&aLegPass[0], "%s\\motion_leg%s", FILEPASS, FILEEXT);

        CPlayer *pPlayer = CPlayer::Create(D3DXVECTOR3(PLAYERSTARTPOS.x, PLAYERSTARTPOS.y, PLAYERSTARTPOS.z), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 
            D3DXVECTOR3(0.0f, 0.0f, 0.0f), &aBodyPass[0], &aLegPass[0]);
        pPlayer->SetCamera(CManager::GetInstance()->GetCamera());
        pPlayer->BindId(0);
        pPlayer->SetType(CPlayer::TYPE_ACTIVE);
        pPlayer->SetDraw(true);

        if (m_pFileLoad != NULL)
		{
			m_pFileLoad->OpenFile("data\\TXT\\gimmick.txt");	//�G����|�C���g
		}
    }
        break;

    case STATE_ONLINE:
    {// �I�����C���ʐM�̏ꍇ
        m_pClient = new CClient;
        AddressLoad(&m_aAddress[0]);

        if (m_pClient->Init(&m_aAddress[0], DEF_PORT))
        {// �����ڑ�����
            // �I�����C���֐����}���`�X���b�h
            std::thread th(&CGame::Online, this);
            th.detach();
        }
    }
        break;

    default:

        break;
    }

    CEnemyBoss::Create(D3DXVECTOR3(-1200.0f, 1000.0f, 3950.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    CEnemyMelee::Create(D3DXVECTOR3(100.0f, 600.0f, 2250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    CEnemyMelee::Create(D3DXVECTOR3(300.0f, 600.0f, 2250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    CEnemyMelee::Create(D3DXVECTOR3(80.0f, 600.0f, 2250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    CEnemyMelee::Create(D3DXVECTOR3(-430.0f, 600.0f, 2250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    CEnemyGun::Create(D3DXVECTOR3(500.0f, 600.0f, 2350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    CEnemyGun::Create(D3DXVECTOR3(-500.0f, 600.0f, 2350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

    //�h�[���ǉ�
    CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9000.0f, 3000.0f, 3, 8, 8);
    CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f), 9000.0f, 3000.0f, 3, 8, 8);

    // �^�C�}�[�ǉ�
    m_pTimer = CTime::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT * 0.05f, 0.0f));
    m_pTimer->Set(6000 * 3);
    m_pTimer->SetActive(false);

    m_pPause = CPause::Create();

    return S_OK;
}

//===============================================
// �I������
//===============================================
void CGame::Uninit(void)
{
    m_bEnd = true;

    while (1)
    {
        OnlineEnd();
        if (m_nSledCnt <= 0)
        {
            break;
        }
    }

    if (m_pPause != nullptr) {
        m_pPause->Uninit();
        delete m_pPause;
        m_pPause = nullptr;
    }

    if (m_pFileLoad != nullptr)
    {
        m_pFileLoad->Uninit();

        delete m_pFileLoad;		// �������̊J��
        m_pFileLoad = nullptr;
    }

    if (m_pClient != nullptr)
    {
        m_pClient->Uninit();
        delete m_pClient;
        m_pClient = nullptr;
    }

    if (m_ppPlayer != nullptr) { // �g�p���Ă����ꍇ
        for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
        {
            // �I������
            m_ppPlayer[nCnt]->Uninit();
            m_ppPlayer[nCnt] = nullptr;	// �g�p���Ă��Ȃ���Ԃɂ���
        }

        delete[] m_ppPlayer;	// �|�C���^�̊J��
        m_ppPlayer = nullptr;	// �g�p���Ă��Ȃ���Ԃɂ���
    }

    if (m_ppCamera != nullptr) { // �g�p���Ă����ꍇ
        for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
        {
            // �I������
            m_ppCamera[nCnt]->Uninit();
            delete m_ppCamera[nCnt];
            m_ppCamera[nCnt] = nullptr;	// �g�p���Ă��Ȃ���Ԃɂ���
        }

        delete[] m_ppCamera;	// �|�C���^�̊J��
        m_ppCamera = nullptr;	// �g�p���Ă��Ȃ���Ԃɂ���
    }

    if (m_pTimer != nullptr) {
        m_pTimer->Uninit();
        delete m_pTimer;
        m_pTimer = nullptr;
    }

    // default�J�����I��
    CManager::GetInstance()->GetCamera()->SetDraw(true);

    //Winsock�I������
    WSACleanup();	// WSACleanup�֐� : winsock�̏I������

    m_state = STATE_LOCAL;
}

//===============================================
// �X�V����
//===============================================
void CGame::Update(void)
{
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKey->GetTrigger(DIK_P) == true || pInputPad->GetTrigger(CInputPad::BUTTON_START, 0))
	{//�|�[�Y�L�[(P�L�[)�������ꂽ
		m_bPause = m_bPause ? false : true;

		if (m_pPause != nullptr) {
			m_pPause->SetDraw(m_bPause);

            if (!m_bPause) {    // �|�[�Y����
                m_pPause->MeasureEnd();
            }
            else {
                m_pPause->MeasureStart();
            }
		}
	}

    if (m_bPause == true)
    {
        if (m_pPause != nullptr) {
            if (m_pPause->Update()) {
                m_bPause = false;
                m_pPause->SetDraw(m_bPause);
            }
        }
        return;
    }

    if (m_pTimer != nullptr) {
        m_pTimer->Update();

        if (m_pTimer->GetNum() <= 0) {
            CManager::GetInstance()->GetFade()->Set(CScene::MODE_RESULT);
        }
    }

    if (CPlayerManager::GetInstance()->GetTop()->GetPosition().x <= -3000.0f) {
        CManager::GetInstance()->GetFade()->Set(CScene::MODE_RANKING);
    }

    CScene::Update();
}

//===============================================
// �`�揈��
//===============================================
void CGame::Draw(void)
{
    CScene::Draw();
}

//===================================================
// �v���C���[�̎擾
//===================================================
CPlayer *CGame::GetPlayer(void)
{
    return *m_ppPlayer;
}

//===================================================
// �t�@�C���ǂݍ��݂̎擾
//===================================================
CFileLoad *CGame::GetFileLoad(void)
{
    return m_pFileLoad;
}

//===================================================
// �I�����C���ʐM
//===================================================
void CGame::Online(void)
{
    m_nSledCnt = 1;

    while (1)
    {
        if (m_ppPlayer == NULL || m_bEnd == true)
        {
            break;
        }

        char aRecvData[MAX_STRING] = {};	// ��M�p

        // ��M
        int nRecvByte = m_pClient->Recv(&aRecvData[0], MAX_STRING);

        // �}���`�X���b�h
        std::thread th(&CGame::ByteCheck, this, &aRecvData[0], nRecvByte);
        th.detach();
    }

    m_nSledCnt--;
}

//===================================================
// ��M�m�F
//===================================================
void CGame::ByteCheck(char *pRecvData, int nRecvByte)
{
    m_mutex.lock();

    m_nSledCnt++;
    D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    int nType = COMMAND_TYPE_NONE;	// �R�}���h�ԍ�
    int nId = -1;		// ID
    int nByte = 0;	// �o�C�g��
    int nDamage = 0;	// �_���[�W��
    CPlayer *pPlayer = NULL;		// �擪���擾
    CPlayer *pPlayerNext = NULL;	// ����ێ�

    if (nRecvByte <= 0)
    {
        m_nSledCnt--;
        m_mutex.unlock();
        return;
    }

    pPlayer = CPlayerManager::GetInstance()->GetTop();	// �擪���擾

    // �I�[�����܂Ŋm�F����
    while (nByte < nRecvByte)
    {
        bool bIn = false;	// ���ɂ��邩�ǂ���
        memcpy(&nId, &pRecvData[nByte], sizeof(int));
        nByte += sizeof(int);

        // �R�}���h�擾
        memcpy(&nType, &pRecvData[nByte], sizeof(int));
        nByte += sizeof(int);

        if (nId != -1)
        {
            // �R�}���h���Ƃɕ�����
            switch (nType)
            {

            case COMMAND_TYPE_SETPOSITION:

                memcpy(&pos, &pRecvData[nByte], sizeof(D3DXVECTOR3));
                nByte += sizeof(D3DXVECTOR3);
                break;

            case COMMAND_TYPE_SETROTATION:

                memcpy(&pos, &pRecvData[nByte], sizeof(D3DXVECTOR3));
                nByte += sizeof(D3DXVECTOR3);
                break;

            case COMMAND_TYPE_SETLIFE:

                memcpy(&nDamage, &pRecvData[nByte], sizeof(int));
                nByte += sizeof(int);
                break;

            case COMMAND_TYPE_START_OK:

                break;

            case COMMAND_TYPE_CREATE:
                break;

            case COMMAND_TYPE_DAMAGE:

                memcpy(&nDamage, &pRecvData[nByte], sizeof(int));
                nByte += sizeof(int);
                break;

            case COMMAND_TYPE_DELETE:
                break;

            case COMMAND_TYPE_GETID:
                break;
            }
        }

        if (m_ppPlayer == NULL)
        {
            m_nSledCnt--;
            m_mutex.unlock();
            return;
        }

        if (nId != -1 && (*m_ppPlayer)->GetId() != -1)
        {// ID����M�ł���

            pPlayer = CPlayerManager::GetInstance()->GetTop();	// �擪���擾

            while (pPlayer != NULL)
            {// �g�p����Ă���ԌJ��Ԃ�
                pPlayerNext = pPlayer->GetNext();	// ����ێ�

                if (nId == pPlayer->GetId() && (*m_ppPlayer)->GetId() != nId)
                {// �����ȊO�����삵�Ă��Ȃ�

                 // �R�}���h���Ƃɕ�����
                    switch (nType)
                    {
                    case COMMAND_TYPE_SETPOSITION:

                        pPlayer->SetDiffPosition(pos);
                        break;

                    case COMMAND_TYPE_SETROTATION:

                        pPlayer->SetRotation(pos);
                        break;

                    case COMMAND_TYPE_SETLIFE:

                        pPlayer->SetLife(nDamage);
                        break;

                    case COMMAND_TYPE_START_OK:

                        break;

                    case COMMAND_TYPE_CREATE:

                        break;

                    case COMMAND_TYPE_DAMAGE:

                        pPlayer->Damage(nDamage);
                        break;

                    case COMMAND_TYPE_GOAL:

                        break;

                    case COMMAND_TYPE_DELETE:

                        pPlayer->Uninit();
                        break;

                    case COMMAND_TYPE_GETID:

                        break;
                    }

                    bIn = true;	// �����Ԃɂ���
                    break;
                }
                else if (nId == pPlayer->GetId() && (*m_ppPlayer)->GetId() == nId)
                {// �����ȊO������L����

                    bIn = true;	// �����Ԃɂ���
                    break;
                }

                pPlayer = pPlayerNext;	// ���Ɉړ�
            }

            if (bIn == false && (*m_ppPlayer)->GetId() != -1 && nType > COMMAND_TYPE_NONE && nType < COMMAND_TYPE_MAX)
            {// �܂����݂��Ă��Ȃ��ꍇ
                pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL, NULL);
                pPlayer->BindId(nId);
                pPlayer->SetType(CPlayer::TYPE_NONE);
            }
        }
        else if (nId == -1 && (*m_ppPlayer)->GetId() == -1)
        {// ID����M�ł��Ă��Ȃ����������g��ID�����݂��Ă��Ȃ�
            nId = nType;

            // ������ID��ݒ�
            (*m_ppPlayer)->BindId(nId);

            break;
        }
    }

    m_nSledCnt--;
    m_mutex.unlock();
}

//===================================================
// ���W���M
//===================================================
void CGame::SendPosition(D3DXVECTOR3& pos)
{
    if (m_pClient != nullptr)
    {
        char aSendData[MAX_STRING] = {};	// ���M�p
        int nProt = COMMAND_TYPE_SETPOSITION;

        // protocol��}��
        memcpy(&aSendData[0], &nProt, sizeof(int));

        // ���W��}��
        memcpy(&aSendData[sizeof(int)], &pos, sizeof(D3DXVECTOR3));

        // ���M
        m_pClient->Send(&aSendData[0], sizeof(int) + sizeof(D3DXVECTOR3));
    }
}

//===================================================
// �������M
//===================================================
void CGame::SendRotation(D3DXVECTOR3& rot)
{
    if (m_pClient != nullptr)
    {
        char aSendData[MAX_STRING] = {};	// ���M�p
        int nProt = COMMAND_TYPE_SETROTATION;

        // protocol��}��
        memcpy(&aSendData[0], &nProt, sizeof(int));

        // ���W��}��
        memcpy(&aSendData[sizeof(int)], &rot, sizeof(D3DXVECTOR3));

        // ���M
        m_pClient->Send(&aSendData[0], sizeof(int) + sizeof(D3DXVECTOR3));
    }
}

//===================================================
// �_���[�W���M
//===================================================
void CGame::SendDamage(int nDamage)
{
    if (m_pClient != nullptr)
    {
        char aSendData[MAX_STRING] = {};	// ���M�p
        int nProt = COMMAND_TYPE_DAMAGE;

        // protocol��}��
        memcpy(&aSendData[0], &nProt, sizeof(int));

        // �_���[�W��}��
        memcpy(&aSendData[sizeof(int)], &nDamage, sizeof(int));

        // ���M
        m_pClient->Send(&aSendData[0], sizeof(int) + sizeof(int));
    }
}

//===================================================
// �̗͑��M
//===================================================
void CGame::SendLife(int nLife)
{
    if (m_pClient != nullptr)
    {
        char aSendData[MAX_STRING] = {};	// ���M�p
        int nProt = COMMAND_TYPE_SETLIFE;

        // protocol��}��
        memcpy(&aSendData[0], &nProt, sizeof(int));

        // �_���[�W��}��
        memcpy(&aSendData[sizeof(int)], &nLife, sizeof(int));

        // ���M
        m_pClient->Send(&aSendData[0], sizeof(int) + sizeof(int));
    }
}

//===================================================
// �I�����M
//===================================================
void CGame::OnlineEnd(void)
{
    if (m_pClient != nullptr)
    {
        char aSendData[MAX_STRING] = {};	// ���M�p
        int nProt = COMMAND_TYPE_DELETE;

        // protocol��}��
        memcpy(&aSendData[0], &nProt, sizeof(int));

        // ���M
        m_pClient->Send(&aSendData[0], sizeof(int));
    }
}

//===============================================
// �̗͐ݒ�
//===============================================
void CGame::SendSetUp(void)
{
    if (m_pClient != nullptr)
    {
        char aSendData[MAX_STRING] = {};	// ���M�p
        int nProt = COMMAND_TYPE_START_OK;

        // protocol��}��
        memcpy(&aSendData[0], &nProt, sizeof(int));

        // ���M
        m_pClient->Send(&aSendData[0], sizeof(int));
    }
}

//===============================================
// �S�[�����M
//===============================================
void CGame::SendGoal(void)
{
    if (m_pClient != nullptr)
    {
        char aSendData[MAX_STRING] = {};	// ���M�p
        int nProt = COMMAND_TYPE_GOAL;

        // protocol��}��
        memcpy(&aSendData[0], &nProt, sizeof(int));


        // ���M
        m_pClient->Send(&aSendData[0], sizeof(int));
    }
}

//===================================================
// �A�h���X�ǂݍ���
//===================================================
void CGame::AddressLoad(char *pAddrss)
{
    FILE *pFile;	// �t�@�C���ւ̃|�C���^

    pFile = fopen(ADDRESSFILE, "r");

    if (pFile != NULL)
    {//�t�@�C�����J�����ꍇ
        
        //�e�L�X�g�ǂݍ���
        fscanf(pFile, "%s", pAddrss);

        //�t�@�C�������
        fclose(pFile);
    }
    else
    {//�t�@�C�����J���Ȃ������ꍇ
        return;
    }
}

//===================================================
// �J�n���o
//===================================================
bool CGame::StartDirection(void)
{

	return false;
}