using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bg : MonoBehaviour
{
    // �����o�ϐ���`
    [SerializeField]
    private float _scrollX;         // �ړ���
    private Material m_Material;    // ���g�̃}�e���A��

    // Start is called before the first frame update
    void Start()
    {
        // �}�e���A�����擾
        m_Material = GetComponent<MeshRenderer>().material;
    }

    // Update is called once per frame
    void Update()
    {
        // �e�N�X�`���X�N���[��
        Vector2 offset = m_Material.mainTextureOffset;
        offset.x += _scrollX * Time.deltaTime;
        m_Material.mainTextureOffset = offset;
    }
}
