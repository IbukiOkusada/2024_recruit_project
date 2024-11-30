using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Block : MonoBehaviour
{
    // 静的メンバ変数
    private static int DEFAULTSCORE = 100;  // 基本スコア
    private static int RAND = 7;            // アイテムの出る確率(n分の1)

    // メンバ変数定義
    [SerializeField] GameObject[] m_aItemPrefab;    // アイテム
    protected Enemy m_Parent; // 親
    protected int m_MultiPly; // スコア倍率

    // Start is called before the first frame update
    void Start()
    {
        // 初期値設定
        m_MultiPly = 1;
    }

    // Update is called once per frame
    void Update()
    {
        if(m_Parent == null)    // 親が死んだ
        {
            // 自身の削除
            Destroy(gameObject);
        }
    }

    // 衝突した
    private void OnCollisionEnter(Collision collider)
    {
        if (collider.gameObject.CompareTag("Bullet"))   // 衝突したのが弾
        {
            // 衝突サウンドを鳴らす
            AudioClip clip = Instantiate((AudioClip)Resources.Load("se_hit"));
            AudioSource.PlayClipAtPoint(clip, transform.position);

            // アイテムを出現させる
            {
                // 生成乱数を回す
                int spawnrand = Random.Range(0, RAND);

                if (spawnrand == 0) // 生成される場合
                {
                    // 生成されるアイテムをランダムで決める
                    int rand = Random.Range(0, m_aItemPrefab.Length);
                    Instantiate(m_aItemPrefab[rand], transform.position, Quaternion.identity);
                }
            }

            if (m_Parent != null)    // 親が設定されている
            {
                // 親の管理から自身を排除
                m_Parent.BlockListOut(this);
                m_Parent = null;
                Dead(); // 自身を消滅させる
            }
        }
    }

    // 死亡
    public void Dead()
    {
        // エフェクトの生成
        Instantiate((GameObject)Resources.Load("HitEffect"), transform.position, Quaternion.identity);

        // UIマネージャーを取得する
        GameObject obj = GameObject.FindWithTag("UIManager");
        if (obj != null) // UIマネージャーが存在している
        {
            // スコアの増加
            UIManager ui = obj.GetComponent<UIManager>();
            ui.AddScore(DEFAULTSCORE * m_MultiPly);
        }

        // 自身の削除
        Destroy(gameObject);
    }

    // 親を設定
    public void SetParent(Enemy enemy, int MultiPly)
    {
        m_Parent = enemy;       // 親を覚える
        m_MultiPly = MultiPly;  // スコア倍率を覚える
    }
}
