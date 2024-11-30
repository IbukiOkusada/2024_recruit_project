using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bullet : MonoBehaviour
{
    // �����o�ϐ���`
    [SerializeField] private float m_speed; // �����x
    private Rigidbody m_rigitBody;          // ���W�b�g�{�f�B
    private float m_StartSpeed;             // �J�n���x

    // Start is called before the first frame update
    void Start()
    {
        // �������x��ݒ�
        m_rigitBody = GetComponent<Rigidbody>();    // �擾
        m_rigitBody.velocity = new Vector3(m_speed, m_speed, 0);    // �ݒ�

        // �J�n���x��ۑ�
        m_StartSpeed = m_speed;
    }

    // Update is called once per frame
    void Update()
    {
        // ��ʓ�����
        ScreenCollision();

        // ���x�m�F
        SpeedCheck();
    }

    // ��ʔ͈͓������m�F
    void ScreenCollision()
    {
        Vector3 pos = transform.position;
        Vector3 move = m_rigitBody.velocity;

        // ��ʂ̔�����W���擾
        Vector3 leftDown = Camera.main.ScreenToWorldPoint(new Vector2(0.0f, 0.0f)); // �����̍��W
        Vector3 rightUp = Camera.main.ScreenToWorldPoint(new Vector2(Screen.width, Screen.height)); // �E��̍��W

        // �����蔻��
        if(pos.x >= rightUp.x)  // �E�[
        {
            move.x *= -1.0f;
        }

        // �ړ��ʂ�ݒ�
        m_rigitBody.velocity = move;
    }

    // �Փ�
    private void OnCollisionEnter(Collision collider)
    {
        if (collider.gameObject.CompareTag("Player"))   // �Փ˂����̂��v���C���[
        {
            Vector3 PlayerPos = collider.transform.position;    // �v���C���[�̍��W
            Vector3 MyPos = transform.position;                 // ���g�̍��W

            // ���������p�x���擾
            Vector3 vec = MyPos - PlayerPos;
            vec = vec.normalized;   // ���K��

            // ���x�����߂Đݒ�
            float speed = m_rigitBody.velocity.magnitude;
            speed += 0.1f;  // ���x������
            m_rigitBody.velocity = vec * speed;
        }
        else if (collider.gameObject.CompareTag("DeadLine"))   // �Փ˂����̂����S�n�_
        {
            // ���S�T�E���h��炷
            AudioClip clip = Instantiate((AudioClip)Resources.Load("se_bulletdead"));
            AudioSource.PlayClipAtPoint(clip, transform.position);

            // �G�t�F�N�g
            Instantiate((GameObject)Resources.Load("BulletLineEffect"), transform.position, Quaternion.identity);

            // ���g�̍폜
            Destroy(gameObject);
            return;
        }

        // �Փ˃T�E���h��炷
        {
            AudioClip clip = Instantiate((AudioClip)Resources.Load("se_playerhit"));
            AudioSource.PlayClipAtPoint(clip, transform.position);
        }

        // �G�t�F�N�g�̐���
        if (!collider.gameObject.CompareTag("Bullet"))   // �e�Ƃ̏ՓˈȊO
        {
            Instantiate((GameObject)Resources.Load("BulletHitEffect"), transform.position, Quaternion.identity);
        }
    }

    // ���W�ݒ�
    public void SetPosition(Vector3 pos)
    {
        transform.position = pos;
    }

    // ���x�m�F
    private void SpeedCheck()
    {
        float nowspeed = m_rigitBody.velocity.magnitude;    // ���݂̑��x

        if(nowspeed >= m_StartSpeed) { return; }    // ������葬��

        // ���݂̈ړ��������擾
        Vector3 vec = m_rigitBody.velocity;
        vec = vec.normalized;   // ���K��

        // �ړ��ʂ������l�ɍĐݒ�
        m_rigitBody.velocity = vec * m_StartSpeed;
    }
}
