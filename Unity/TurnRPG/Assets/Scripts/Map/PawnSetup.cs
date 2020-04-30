using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public struct PawnSetup
{
    public GameObject pawnObject;
    public Vector2Int gridPos;
    public Player controlPlayer;

    public int health;
    public int attack;
    public int movementRange;
    public int attackRange;
}
