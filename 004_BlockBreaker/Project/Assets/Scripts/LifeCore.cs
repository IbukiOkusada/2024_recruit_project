using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// コアの派生クラス
public class LifeCore : EnemyCore
{
    // 静的メンバ変数
    private static float STATETIMER = 0.75f;

    // 状態列挙型の定義
    private enum STATE
    {
        STATE_NONE = 0, // 通常
        STATE_DAMAGE,   // ダメージ
        STATE_MAX
    };

    // メンバ変数定義
    [SerializeField] private int m_Life;    // 体力
    private int m_StartLife;    // 開始時体力
    private STATE m_State;      // 状態
    private float m_StateTimer; // 状態変化管理タイマー
    private float m_AddTimer;   // タイマー加算量

    // Start is called before the first frame update
    void Start()
    {
        // 初期設定
        m_StateTimer = 0.0f;
        m_StartLife = m_Life;
        m_State = STATE.STATE_NONE;
        m_AddTimer = 1.0f / STATETIMER;
    }

    // Update is called once per frame
    void Update()
    {
        // 回転
        Rotation();

        if(m_State == STATE.STATE_DAMAGE)   // ダメージ状態
        {
            m_StateTimer += m_AddTimer * Time.deltaTime;

            if(m_StateTimer >= 1.0f) // 状態を戻す
            { 
                SetState(STATE.STATE_NONE);
            }
        }
    }

    // 状態設定
    private void SetState(STATE state)
    {
        // 指定された状態に設定
        m_State = state;
        m_StateTimer = 0.0f;

        // 状態ごとに色を変更
        if (m_State == STATE.STATE_DAMAGE)
        {
            GetComponent<SpriteRenderer>().material.color = Color.red;
        }
        else
        {
            // 体力が減るごとに黒くする
            Color col;
            float white = (float)m_Life / (float)m_StartLife;
            col = new Color(white, white, white, Color.white.a);
            GetComponent<SpriteRenderer>().material.color = col;
        }
    }

    // 衝突した
    private void OnCollisionEnter(Collision collider)
    {
        if(m_State != STATE.STATE_NONE) { return; } // ダメージを受けない状態の場合

        if (collider.gameObject.CompareTag("Bullet"))   // 弾と衝突
        {
            // ダメージを与える
            m_Life--;
            SetState(STATE.STATE_DAMAGE);

            // 死亡させる
            if(m_Life <= 0)
            {
                // 爆発を生成
                Instantiate(Resources.Load("Explosion"), transform.position, Quaternion.identity);

                // エフェクトの生成
                Instantiate((GameObject)Resources.Load("HitEffect"), transform.position, Quaternion.identity);

                // 死亡サウンドを鳴らす
                AudioClip clip = Instantiate((AudioClip)Resources.Load("se_bossbomb"));
                AudioSource.PlayClipAtPoint(clip, transform.position);

                // 親を削除
                m_Parent.CoreDestroy(this);    // 自身を排除
                m_Parent = null;

                // 敵を全削除
                GameObject[] obj = GameObject.FindGameObjectsWithTag("Enemy");

                for (int i = 0; i < obj.Length; i++)
                {
                    Enemy enemy = obj[i].GetComponent<Enemy>();

                    if (enemy == null) { continue; } // 敵を取得できなかった

                    enemy.ItemDestroy();
                }
            }
        }
    }
}
