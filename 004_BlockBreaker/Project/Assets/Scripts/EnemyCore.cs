using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyCore : Block
{
    // メンバ変数定義
    [SerializeField, Range(0.0f, 100.0f)] 
    private float m_RotSpeed = 8.0f;  // 回転速度

    // コンストラクタ
    public EnemyCore()
    {
        
    }

    // デストラクタ
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
        // 回転
        Rotation();
    }

    // 衝突した
    private void OnCollisionEnter(Collision collider)
    {
        bool Death = false;
        if (collider.gameObject.CompareTag("DeadLine"))   // 終了地点と衝突
        {
            // ボス前進
            Debug.Log("DeadLine!");
            BossForward();
            PlayerDamage();
        }
        else if (!collider.gameObject.CompareTag("Bullet"))   // 弾以外と衝突
        {
            return;
        }
        else // 弾と衝突
        {
            // 死亡フラグを立ててカウント増加
            AddDeadEnemy();
            Death = true;
        }

        if (m_Parent != null)    // 親が設定されている
        {
            if (Death)  // 壊された
            {
                // 爆発を生成
                Instantiate((GameObject)Resources.Load("Explosion"), transform.position, Quaternion.identity);

                // 爆発サウンドを鳴らす
                AudioClip clip = Instantiate((AudioClip)Resources.Load("se_explosion"));
                AudioSource.PlayClipAtPoint(clip, transform.position);

                // 親を削除
                m_Parent.CoreDestroy(this);    // 自身を排除
                m_Parent = null;
            }
            else
            {
                // 死亡サウンドを鳴らす
                AudioClip clip = Instantiate((AudioClip)Resources.Load("se_coredead"));
                AudioSource.PlayClipAtPoint(clip, transform.position);

                // エフェクトの生成
                Instantiate((GameObject)Resources.Load("BulletHitEffect"), transform.position, Quaternion.identity);

                // 死亡ラインなので削除
                m_Parent.DeadLineDestroy();
            }
        }

    }

    // 討伐数増加
    private void AddDeadEnemy()
    {
        // ボスを取得する
        GameObject obj = GameObject.FindWithTag("Boss");

        if (obj != null && m_Parent != obj) // ボスが存在かつボス以外のコア
        {
            Boss boss = obj.GetComponent<Boss>();   // ボスを取得

            if (boss != null)    // ボスを取得できた
            {
                boss.AddDeadEnemy();    // 討伐数増加
            }
        }
    }

    // ボス前進
    private void BossForward()
    {
        // ボスを取得する
        GameObject obj = GameObject.FindWithTag("Boss");

        if (obj != null && m_Parent != obj) // ボスが存在かつボス以外のコア
        {
            Boss boss = obj.GetComponent<Boss>();   // ボスを取得

            // ボスを前進させる
            if (boss != null)
            {
                boss.Forward();
            }
            else
            {
#if UNITY_EDITOR    // エディター
                Debug.Log("BossNull!");
#endif
            }
        }
        else
        {
#if UNITY_EDITOR    // エディター
            Debug.Log("objNull!");
#endif
        }
    }

    // プレイヤーへのダメージ
    private void PlayerDamage()
    {
        // ボスを取得する
        GameObject obj = GameObject.FindWithTag("Player");

        if (obj != null) // ボスが存在かつボス以外のコア
        {
            Player player = obj.GetComponent<Player>();   // ボスを取得

            // プレイヤーへダメージを与える
            if (player != null)
            {
                player.Damage();
            }
            else
            {
#if UNITY_EDITOR    // エディター
                Debug.Log("Playernull!");
#endif
            }
        }
    }

    // 回転
    protected void Rotation()
    {
        // 回転
        Vector3 rot = transform.eulerAngles;
        rot.z += Mathf.PI * m_RotSpeed * Time.deltaTime;
        transform.eulerAngles = rot;
    }
}
