using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Explosion : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        // �A�j���[�^�[�̃R���|�[�l���g���擾����
        Animator animator = GetComponent<Animator>();

        // �Đ����Ԃ������������`�F�b�N(0�ԃ��C���[�̃A�j���[�V������1(�Đ�����)��)
        if (animator.GetCurrentAnimatorStateInfo(0).normalizedTime >= 1)
        {
            GameObject.Destroy(gameObject);
        }
    }
}
