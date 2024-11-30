using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyCore : Block
{
    // �����o�ϐ���`
    [SerializeField, Range(0.0f, 100.0f)] 
    private float m_RotSpeed = 8.0f;  // ��]���x

    // �R���X�g���N�^
    public EnemyCore()
    {
        
    }

    // �f�X�g���N�^
    ~EnemyCore()
    {

    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        // ��]
        Rotation();
    }

    // �Փ˂���
    private void OnCollisionEnter(Collision collider)
    {
        bool Death = false;
        if (collider.gameObject.CompareTag("DeadLine"))   // �I���n�_�ƏՓ�
        {
            // �{�X�O�i
            Debug.Log("DeadLine!");
            BossForward();
            PlayerDamage();
        }
        else if (!collider.gameObject.CompareTag("Bullet"))   // �e�ȊO�ƏՓ�
        {
            return;
        }
        else // �e�ƏՓ�
        {
            // ���S�t���O�𗧂ĂăJ�E���g����
            AddDeadEnemy();
            Death = true;
        }

        if (m_Parent != null)    // �e���ݒ肳��Ă���
        {
            if (Death)  // �󂳂ꂽ
            {
                // �����𐶐�
                Instantiate((GameObject)Resources.Load("Explosion"), transform.position, Quaternion.identity);

                // �����T�E���h��炷
                AudioClip clip = Instantiate((AudioClip)Resources.Load("se_explosion"));
                AudioSource.PlayClipAtPoint(clip, transform.position);

                // �e���폜
                m_Parent.CoreDestroy(this);    // ���g��r��
                m_Parent = null;
            }
            else
            {
                // ���S�T�E���h��炷
                AudioClip clip = Instantiate((AudioClip)Resources.Load("se_coredead"));
                AudioSource.PlayClipAtPoint(clip, transform.position);

                // �G�t�F�N�g�̐���
                Instantiate((GameObject)Resources.Load("BulletHitEffect"), transform.position, Quaternion.identity);

                // ���S���C���Ȃ̂ō폜
                m_Parent.DeadLineDestroy();
            }
        }

    }

    // ����������
    private void AddDeadEnemy()
    {
        // �{�X���擾����
        GameObject obj = GameObject.FindWithTag("Boss");

        if (obj != null && m_Parent != obj) // �{�X�����݂��{�X�ȊO�̃R�A
        {
            Boss boss = obj.GetComponent<Boss>();   // �{�X���擾

            if (boss != null)    // �{�X���擾�ł���
            {
                boss.AddDeadEnemy();    // ����������
            }
        }
    }

    // �{�X�O�i
    private void BossForward()
    {
        // �{�X���擾����
        GameObject obj = GameObject.FindWithTag("Boss");

        if (obj != null && m_Parent != obj) // �{�X�����݂��{�X�ȊO�̃R�A
        {
            Boss boss = obj.GetComponent<Boss>();   // �{�X���擾

            // �{�X��O�i������
            if (boss != null)
            {
                boss.Forward();
            }
            else
            {
#if UNITY_EDITOR    // �G�f�B�^�[
                Debug.Log("BossNull!");
#endif
            }
        }
        else
        {
#if UNITY_EDITOR    // �G�f�B�^�[
            Debug.Log("objNull!");
#endif
        }
    }

    // �v���C���[�ւ̃_���[�W
    private void PlayerDamage()
    {
        // �{�X���擾����
        GameObject obj = GameObject.FindWithTag("Player");

        if (obj != null) // �{�X�����݂��{�X�ȊO�̃R�A
        {
            Player player = obj.GetComponent<Player>();   // �{�X���擾

            // �v���C���[�փ_���[�W��^����
            if (player != null)
            {
                player.Damage();
            }
            else
            {
#if UNITY_EDITOR    // �G�f�B�^�[
                Debug.Log("Playernull!");
#endif
            }
        }
    }

    // ��]
    protected void Rotation()
    {
        // ��]
        Vector3 rot = transform.eulerAngles;
        rot.z += Mathf.PI * m_RotSpeed * Time.deltaTime;
        transform.eulerAngles = rot;
    }
}
