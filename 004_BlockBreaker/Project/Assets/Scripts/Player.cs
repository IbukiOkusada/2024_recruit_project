using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    // �ÓI�����o�ϐ�
    private static int MAX_BULLET = 4;  // �ő�܂ŉ�ʏ�ɔ��˂ł���e��

    // �����o�ϐ���`
    [SerializeField] private float m_Speed;    // �ړ����x
    [SerializeField] private float m_Iner;     // �ړ�����
    [SerializeField] private float m_MaxSize;  // �ő卂��
    [SerializeField] private GameObject m_bulletprefab; // �e�̃v���n�u
    private Vector3 m_move;         // �ړ���
    private Rigidbody m_rigitBody;  // ���g�̃��W�b�g�{�f�B

    // Start is called before the first frame update
    void Start()
    {
        // �擾
        m_rigitBody = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        // ���Z�b�g
        if(m_rigitBody != null){ m_rigitBody.velocity = new Vector3(0.0f, 0.0f, 0.0f); }

        // �ړ�
        Move();

        // ����
        Shot();
    }

    // �ړ�
    void Move()
    {
        if (Input.GetKey(KeyCode.W))
        {// W�L�[�������ꂽ�Ƃ�
            m_move.y += Mathf.Cos(Mathf.PI * 0.0f) * (m_Speed * Time.deltaTime);
        }
        else if (Input.GetKey(KeyCode.S))
        {// S�L�[�������ꂽ�Ƃ�
            m_move.y += Mathf.Cos(Mathf.PI * 1.0f) * (m_Speed * Time.deltaTime);
        }

        // �ړ��ʌ���
        m_move.x -= m_move.x * m_Iner;  //x���W
        m_move.y -= m_move.y * m_Iner;	//y���W

        // ���W�X�V
        transform.Translate(m_move.x, m_move.y, m_move.z, Space.World); // �ړ��ʂ����Z
    }

    // �e����
    void Shot()
    {
        // Space�L�[�������ꂽ�Ƃ��̂ݐi��
        if (!Input.GetKeyDown(KeyCode.Space)) { return; }

        // ��ʏ�̒e���m�F
        {
            GameObject[] bullet = GameObject.FindGameObjectsWithTag("Bullet");

            if(bullet.Length > MAX_BULLET) { return; }   // ��ʏ�Ɏw�萔�ȏ�
        }

        // �G���[�`�F�b�N
        if (m_bulletprefab == null) { return; }  // �v���n�u�����݂��Ȃ�

        // UI�}�l�[�W���[���擾����
        GameObject obj = GameObject.FindWithTag("UIManager");
        if (obj == null) { return; } // UI�}�l�[�W���[�^�O���擾�ł��Ȃ�����
        UIManager ui = obj.GetComponent<UIManager>();
        if (ui == null) { return; }  // UI�}�l�[�W���[���擾�ł��Ȃ�����

        // �c�i�����m�F����
        if (ui.Bullet <= 0) // �c�e���Ȃ�
        {
            // ���ˎ��s�T�E���h��炷
            AudioClip clip = Instantiate((AudioClip)Resources.Load("se_noshot"));
            AudioSource.PlayClipAtPoint(clip, transform.position);

            return; 
        }

        // �e�𔭎�
        Vector3 SetPos = transform.position;
        SetPos.x += 0.5f;
        Instantiate(m_bulletprefab, SetPos, Quaternion.identity);
        ui.AddBullet(-1);   // �c�e�����炷

        // ���˃T�E���h��炷
        {
            AudioClip clip = Instantiate((AudioClip)Resources.Load("se_shot"));
            AudioSource.PlayClipAtPoint(clip, transform.position);
        }
    }

    // �_���[�W
    public void Damage()
    {
        // UIManager�^�O�̃I�u�W�F���擾
        GameObject obj = GameObject.FindWithTag("UIManager");
        if (obj == null) { return; }    // �擾�ł��Ȃ�����

        // UIManager���擾
        UIManager ui = obj.GetComponent<UIManager>();
        if (ui == null) { return; }    // �擾�ł��Ȃ�����
        ui.AddLife(-1); // �̗͂����炷
    }
}
