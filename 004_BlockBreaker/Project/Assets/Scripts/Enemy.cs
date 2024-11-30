using System.Collections;
using System.Collections.Generic;
using UnityEngine;
 
public class Enemy : MonoBehaviour
{
    // �ÓI�����o�ϐ�
    private static float MOVE = -1.0f;  // ���̈ړ���

    // �����o�ϐ���`
    [SerializeField] private EnemyCore m_core;       // �R�A�I�u�W�F�N�g
    [SerializeField] private List<Block> m_blocks;    // �u���b�N���X�g
    [SerializeField] private int m_MultiPly = 1;      // �X�R�A�{��
    [SerializeField] protected int m_MoveTimer;       // �ړ��^�C�}�[
    private float m_NowTimer;                         // ���݂̈ړ��^�C�}�[
    private float m_AddTimer;                         // �^�C�}�[���Z��
    private Vector3 m_StartPos;                       // �J�n�n�_
    private Vector3 m_EndPos;                         // �ڕW�n�_

    // �ڕW�n�_�v���p�e�B�̐ݒ�
    public Vector3 EndPos
    {
        get { return m_EndPos; }
        set { m_EndPos = value; }
    }

    // ���݂̃^�C�}�[�v���p�e�B�̐ݒ�
    public float NowTimer
    {
        get { return m_NowTimer; }
        set { m_NowTimer = value; }
    }

    // Start is called before the first frame update
    void Start()
    {
        // �Q�Ɛݒ�
        SetParent();

        // �^�C�}�[�l�ݒ�
        SetTimer();

        // �����ڕW�n�_�ݒ�
        SetMovePosition();
    }

    // Update is called once per frame
    void Update()
    {
        // �^�C�}�[����
        AddTimer();

        // �^�C�}�[����
        NextTimer();

        Move();
    }

    // ���X�g����p��
    public void BlockListOut(Block block)
    {
        if(block != null)   // ���݂��Ă���
        {
            m_blocks.Remove(block); // ���X�g����폜
        }
    }

    // �R�A���j��
    public void CoreDestroy(EnemyCore core)
    {
        if (m_core != core) { return; }  // ��v���Ă��Ȃ�

        // �u���b�N��j�󂷂�
        for (int i = 0; i < m_blocks.Count; i++)
        {
            m_blocks[i].Dead();
        }

        // �R�A���j�󂷂�
        m_core.Dead();
        m_core = null;

        // ���g��p������
        Destroy(gameObject);
    }

    // ���S�n�_�ł̍폜
    public void DeadLineDestroy()
    {
        Destroy(gameObject);
    }

    // �A�C�e���Ŕj��
    public void ItemDestroy()
    {
        // �u���b�N��j�󂷂�
        for (int i = 0; i < m_blocks.Count; i++)
        {
            m_blocks[i].Dead();
        }

        // �R�A���j�󂷂�
        m_core.Dead();
        m_core = null;

        // ���g��p������
        Destroy(gameObject);
    }

    // �^�C�}�[����
    protected void AddTimer()
    {
        if(m_NowTimer >= 1.0f) { return; }  // �^�C�}�[�����I���ς�

        m_NowTimer += m_AddTimer * Time.deltaTime;

        if(m_NowTimer > 1.0f) { m_NowTimer = 1.0f; }    // �^�C�}�[���K��l�𒴂���
    }

    // �^�C�}�[�ړ�
    protected void NextTimer()
    {
        if (m_NowTimer >= 1.0f)  // �ړ��^�C�}�[�K��l
        {
            m_NowTimer = 0.0f;  // �^�C�}�[���Z�b�g
            m_StartPos = m_EndPos;  // �ړ��J�n�n�_��O��̃S�[���n�_��
            m_EndPos.x += MOVE;     // �S�[���n�_��O�֐i�߂�
        }
    }

    // �ړ�
    protected void Move()
    {
        Vector3 pos = m_StartPos;   // �J�n���W
        pos += (m_EndPos - m_StartPos) * m_NowTimer;    // ������⊮
        transform.position = pos;   // �l��ݒ�
    }

    // �ړ���̍��W��ݒ�
    protected void NextPosition(Vector3 pos)
    {
        // ���݂̈ʒu����w�肵���n�_���S�[���ɂ���
        m_NowTimer = 0.0f;  // �^�C�}�[���Z�b�g
        m_StartPos = transform.position;
        m_EndPos = pos;
    }

    // �^�C�}�[�l�ݒ�
    protected void SetTimer()
    {
        m_AddTimer = 1.0f / m_MoveTimer;
        m_NowTimer = 0.0f;
    }

    // �J�n�I���n�_���Z�b�g
    protected void SetMovePosition()
    {
        m_StartPos = transform.position;
        m_EndPos = m_StartPos;
    }

    // �u���b�N�ɎQ�Ɛݒ�
    protected void SetParent()
    {
        // �u���b�N�Ɏ��g��n��
        for (int i = 0; i < m_blocks.Count; i++)
        {
            m_blocks[i].SetParent(this, m_MultiPly);
        }

        // �R�A�Ɏ��g��n��
        if (m_core != null)
        {
            m_core.SetParent(this, m_MultiPly);
        }
    }
}
