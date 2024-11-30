using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bg : MonoBehaviour
{
    // メンバ変数定義
    [SerializeField]
    private float _scrollX;         // 移動量
    private Material m_Material;    // 自身のマテリアル

    // Start is called before the first frame update
    void Start()
    {
        // マテリアルを取得
        m_Material = GetComponent<MeshRenderer>().material;
    }

    // Update is called once per frame
    void Update()
    {
        // テクスチャスクロール
        Vector2 offset = m_Material.mainTextureOffset;
        offset.x += _scrollX * Time.deltaTime;
        m_Material.mainTextureOffset = offset;
    }
}
