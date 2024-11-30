using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// �R�A�̔h���N���X
public class LifeCore : EnemyCore
{
    // �ÓI�����o�ϐ�
    private static float STATETIMER = 0.75f;

    // ��ԗ񋓌^�̒�`
    private enum STATE
    {
        STATE_NONE = 0, // �ʏ�
        STATE_DAMAGE,   // �_���[�W
        STATE_MAX
    };

    // �����o�ϐ���`
    [SerializeField] private int m_Life;    // �̗�
    private int m_StartLife;    // �J�n���̗�
    private STATE m_State;      // ���
    private float m_StateTimer; // ��ԕω��Ǘ��^�C�}�[
    private float m_AddTimer;   // �^�C�}�[���Z��

    // Start is called before the first frame update
    void Start()
    {
        // �����ݒ�
        m_StateTimer = 0.0f;
        m_StartLife = m_Life;
        m_State = STATE.STATE_NONE;
        m_AddTimer = 1.0f / STATETIMER;
    }

    // Update is called once per frame
    void Update()
    {
        // ��]
        Rotation();

        if(m_State == STATE.STATE_DAMAGE)   // �_���[�W���
        {
            m_StateTimer += m_AddTimer * Time.deltaTime;

            if(m_StateTimer >= 1.0f) // ��Ԃ�߂�
            { 
                SetState(STATE.STATE_NONE);
            }
        }
    }

    // ��Ԑݒ�
    private void SetState(STATE state)
    {
        // �w�肳�ꂽ��Ԃɐݒ�
        m_State = state;
        m_StateTimer = 0.0f;

        // ��Ԃ��ƂɐF��ύX
        if (m_State == STATE.STATE_DAMAGE)
        {
            GetComponent<SpriteRenderer>().material.color = Color.red;
        }
        else
        {
            // �̗͂����邲�Ƃɍ�������
            Color col;
            float white = (float)m_Life / (float)m_StartLife;
            col = new Color(white, white, white, Color.white.a);
            GetComponent<SpriteRenderer>().material.color = col;
        }
    }

    // �Փ˂���
    private void OnCollisionEnter(Collision collider)
    {
        if(m_State != STATE.STATE_NONE) { return; } // �_���[�W���󂯂Ȃ���Ԃ̏ꍇ

        if (collider.gameObject.CompareTag("Bullet"))   // �e�ƏՓ�
        {
            // �_���[�W��^����
            m_Life--;
            SetState(STATE.STATE_DAMAGE);

            // ���S������
            if(m_Life <= 0)
            {
                // �����𐶐�
                Instantiate(Resources.Load("Explosion"), transform.position, Quaternion.identity);

                // �G�t�F�N�g�̐���
                Instantiate((GameObject)Resources.Load("HitEffect"), transform.position, Quaternion.identity);

                // ���S�T�E���h��炷
                AudioClip clip = Instantiate((AudioClip)Resources.Load("se_bossbomb"));
                AudioSource.PlayClipAtPoint(clip, transform.position);

                // �e���폜
                m_Parent.CoreDestroy(this);    // ���g��r��
                m_Parent = null;

                // �G��S�폜
                GameObject[] obj = GameObject.FindGameObjectsWithTag("Enemy");

                for (int i = 0; i < obj.Length; i++)
                {
                    Enemy enemy = obj[i].GetComponent<Enemy>();

                    if (enemy == null) { continue; } // �G���擾�ł��Ȃ�����

                    enemy.ItemDestroy();
                }
            }
        }
    }
}
