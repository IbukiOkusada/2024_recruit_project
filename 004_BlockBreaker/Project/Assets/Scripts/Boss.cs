using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Boss : Enemy
{
    // �ÓI�����o�ϐ�
    private static float MOVE = 1.2f;   // ��x�̈ړ���

    // �����o�ϐ���`
    [SerializeField] private EnemySpawn m_Spawn;        // �X�|�[�����
    [SerializeField] private int m_StartSetEnemy = 1;   // �����z�u�G��

    // Start is called before the first frame update
    void Start()
    {
        // �����ݒ�
        SetParent();        // �Q�Ɛݒ�
        SetTimer();         // �^�C�}�[�ݒ�
        SetMovePosition();  // �ړ��J�n�n�_���Z�b�g
        m_Spawn.SetTimer(); // �^�C�}�[�ݒ�
        m_Spawn.Spawn(0, m_StartSetEnemy, transform.position); // ��̔z�u
    }

    // Update is called once per frame
    void Update()
    {
        // �X�|�[���J�E���g����
        m_Spawn.SpawnAddCnt(transform.position);

        // �^�C�}�[����
        AddTimer();

        // �ړ�
        Move();
    }

    // �O�i����
    public void Forward()
    {
        // ���W���ړ�
        Vector3 pos = EndPos;
        pos.x -= MOVE;
        NextPosition(pos);
        Debug.Log("NextPositon");

        // �ړ��T�E���h��炷
        AudioClip clip = Instantiate((AudioClip)Resources.Load("se_bossmove"));
        AudioSource.PlayClipAtPoint(clip, transform.position);
    }

    // �����G������
    public void AddDeadEnemy()
    {
        m_Spawn.AddDeadEnemy();
    }
}