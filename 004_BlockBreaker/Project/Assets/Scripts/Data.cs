using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// 敵情報クラス
[System.Serializable]   // クラスのデータをインスペクタに表示させるために必要
public class EnemyData
{
    // 生成するプレハブ
    public GameObject m_Prefab;

    // 出現レベル
    public int m_Level;
}

// 生成レベルクラス
[System.Serializable]   // クラスのデータをインスペクタに表示させるために必要
public class SpawnLevelData
{
    // 各レベル配置数
    public int[] m_aCnt;
}