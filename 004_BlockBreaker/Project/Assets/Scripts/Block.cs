using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Block : MonoBehaviour
{
    // �ÓI�����o�ϐ�
    private static int DEFAULTSCORE = 100;  // ��{�X�R�A
    private static int RAND = 7;            // �A�C�e���̏o��m��(n����1)

    // �����o�ϐ���`
    [SerializeField] GameObject[] m_aItemPrefab;    // �A�C�e��
    protected Enemy m_Parent; // �e
    protected int m_MultiPly; // �X�R�A�{��

    // Start is called before the first frame update
    void Start()
    {
        // �����l�ݒ�
        m_MultiPly = 1;
    }

    // Update is called once per frame
    void Update()
    {
        if(m_Parent == null)    // �e������
        {
            // ���g�̍폜
            Destroy(gameObject);
        }
    }

    // �Փ˂���
    private void OnCollisionEnter(Collision collider)
    {
        if (collider.gameObject.CompareTag("Bullet"))   // �Փ˂����̂��e
        {
            // �Փ˃T�E���h��炷
            AudioClip clip = Instantiate((AudioClip)Resources.Load("se_hit"));
            AudioSource.PlayClipAtPoint(clip, transform.position);

            // �A�C�e�����o��������
            {
                // ������������
                int spawnrand = Random.Range(0, RAND);

                if (spawnrand == 0) // ���������ꍇ
                {
                    // ���������A�C�e���������_���Ō��߂�
                    int rand = Random.Range(0, m_aItemPrefab.Length);
                    Instantiate(m_aItemPrefab[rand], transform.position, Quaternion.identity);
                }
            }

            if (m_Parent != null)    // �e���ݒ肳��Ă���
            {
                // �e�̊Ǘ����玩�g��r��
                m_Parent.BlockListOut(this);
                m_Parent = null;
                Dead(); // ���g�����ł�����
            }
        }
    }

    // ���S
    public void Dead()
    {
        // �G�t�F�N�g�̐���
        Instantiate((GameObject)Resources.Load("HitEffect"), transform.position, Quaternion.identity);

        // UI�}�l�[�W���[���擾����
        GameObject obj = GameObject.FindWithTag("UIManager");
        if (obj != null) // UI�}�l�[�W���[�����݂��Ă���
        {
            // �X�R�A�̑���
            UIManager ui = obj.GetComponent<UIManager>();
            ui.AddScore(DEFAULTSCORE * m_MultiPly);
        }

        // ���g�̍폜
        Destroy(gameObject);
    }

    // �e��ݒ�
    public void SetParent(Enemy enemy, int MultiPly)
    {
        m_Parent = enemy;       // �e���o����
        m_MultiPly = MultiPly;  // �X�R�A�{�����o����
    }
}
