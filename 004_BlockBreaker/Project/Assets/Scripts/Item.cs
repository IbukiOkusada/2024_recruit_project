using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Item : MonoBehaviour
{
    // �ÓI�����o�ϐ�
    private static float MOVE = -2.0f;  // �ړ���

    // ��ޗ񋓌^
    public enum TYPE
    {
        TYPE_ADDBULLET = 0, // �e����
        TYPE_ENEMYDEATH,      // �S�Ă̓G���S
        TYPE_MAX
    }

    // �����o�ϐ���`
    [SerializeField] private TYPE m_type;   // ���

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        // �ړ�
        Vector3 pos = transform.position;
        pos.x += MOVE * Time.deltaTime;
        transform.position = pos;
    }

    // �Փ�
    private void OnTriggerEnter(Collider other)
    {
        if(other.CompareTag("Player"))  // �v���C���[�ƏՓ˂���
        {
            //���ʔ���
            Activation();

            // ���g�̍폜
            Destroy(gameObject);
        }
        else if (other.CompareTag("DeadLine"))  // �v���C���[�ƏՓ˂���
        {
            // ���g�̍폜
            Destroy(gameObject);
        }
    }

    // ���ʔ���
    private void Activation()
    {
        // ��ނ��Ƃɔ���
        switch (m_type)
        {
            case TYPE.TYPE_ADDBULLET:
                {
                    // UIManager�^�O�̃I�u�W�F���擾
                    GameObject obj = GameObject.FindWithTag("UIManager");
                    if (obj == null) { return; }    // �擾�ł��Ȃ�����

                    // UIManager���擾
                    UIManager ui = obj.GetComponent<UIManager>();
                    if (ui == null) { return; }    // �擾�ł��Ȃ�����

                    // �e�𑝉�
                    ui.AddBullet(1);

                    // �A�C�e���擾�T�E���h��炷
                    AudioClip clip = Instantiate((AudioClip)Resources.Load("se_item"));
                    AudioSource.PlayClipAtPoint(clip, transform.position);
                }
                break;

            case TYPE.TYPE_ENEMYDEATH:
                {
                    GameObject obj = GameObject.FindGameObjectWithTag("Enemy");

                    if(obj != null) // �I�u�W�F�N�g���擾�ł���
                    {
                        Enemy enemy = obj.GetComponent<Enemy>();

                        if (enemy != null) // �G���擾�ł���
                        {
                            enemy.ItemDestroy();
                        }
                    }

                    // �����T�E���h��炷
                    AudioClip clip = Instantiate((AudioClip)Resources.Load("se_explosion"));
                    AudioSource.PlayClipAtPoint(clip, transform.position);
                }
                break;
        }
    }
}
