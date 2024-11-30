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
        // アニメーターのコンポーネントを取得する
        Animator animator = GetComponent<Animator>();

        // 再生時間が完了したかチェック(0番レイヤーのアニメーションが1(再生完了)か)
        if (animator.GetCurrentAnimatorStateInfo(0).normalizedTime >= 1)
        {
            GameObject.Destroy(gameObject);
        }
    }
}
