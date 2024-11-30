using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Item : MonoBehaviour
{
    // 静的メンバ変数
    private static float MOVE = -2.0f;  // 移動量

    // 種類列挙型
    public enum TYPE
    {
        TYPE_ADDBULLET = 0, // 弾増加
        TYPE_ENEMYDEATH,      // 全ての敵死亡
        TYPE_MAX
    }

    // メンバ変数定義
    [SerializeField] private TYPE m_type;   // 種類

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        // 移動
        Vector3 pos = transform.position;
        pos.x += MOVE * Time.deltaTime;
        transform.position = pos;
    }

    // 衝突
    private void OnTriggerEnter(Collider other)
    {
        if(other.CompareTag("Player"))  // プレイヤーと衝突した
        {
            //効果発動
            Activation();

            // 自身の削除
            Destroy(gameObject);
        }
        else if (other.CompareTag("DeadLine"))  // プレイヤーと衝突した
        {
            // 自身の削除
            Destroy(gameObject);
        }
    }

    // 効果発動
    private void Activation()
    {
        // 種類ごとに発動
        switch (m_type)
        {
            case TYPE.TYPE_ADDBULLET:
                {
                    // UIManagerタグのオブジェを取得
                    GameObject obj = GameObject.FindWithTag("UIManager");
                    if (obj == null) { return; }    // 取得できなかった

                    // UIManagerを取得
                    UIManager ui = obj.GetComponent<UIManager>();
                    if (ui == null) { return; }    // 取得できなかった

                    // 弾を増加
                    ui.AddBullet(1);

                    // アイテム取得サウンドを鳴らす
                    AudioClip clip = Instantiate((AudioClip)Resources.Load("se_item"));
                    AudioSource.PlayClipAtPoint(clip, transform.position);
                }
                break;

            case TYPE.TYPE_ENEMYDEATH:
                {
                    GameObject obj = GameObject.FindGameObjectWithTag("Enemy");

                    if(obj != null) // オブジェクトを取得できた
                    {
                        Enemy enemy = obj.GetComponent<Enemy>();

                        if (enemy != null) // 敵を取得できた
                        {
                            enemy.ItemDestroy();
                        }
                    }

                    // 爆発サウンドを鳴らす
                    AudioClip clip = Instantiate((AudioClip)Resources.Load("se_explosion"));
                    AudioSource.PlayClipAtPoint(clip, transform.position);
                }
                break;
        }
    }
}
