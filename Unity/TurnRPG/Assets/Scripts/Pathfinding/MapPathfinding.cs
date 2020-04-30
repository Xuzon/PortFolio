using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MapPathfinding
{
    protected Dictionary<Vector2Int, Node> froms = new Dictionary<Vector2Int, Node>();
    protected List<Vector2Int> changedTiles = new List<Vector2Int>();
    
    /// <summary>
    /// Show the cells I can move to, also calculate the paths at the same time (recursion)
    /// </summary>
    /// <param name="pos"></param>
    /// <param name="movementRange"></param>
    public void ShowMovement(Vector3 pos, int movementRange, Color movementColor, bool ignoreObstacles = false)
    {
        froms.Clear();
        changedTiles.Clear();
        Vector2Int gridPos = GameManager.singleton.map.WorldToGridCoords(pos);
        RecursiveExpand(gridPos + new Vector2Int(1, 0), gridPos, movementRange, movementColor, ignoreObstacles);
        RecursiveExpand(gridPos + new Vector2Int(-1, 0), gridPos, movementRange, movementColor, ignoreObstacles);
        RecursiveExpand(gridPos + new Vector2Int(0, 1), gridPos, movementRange, movementColor, ignoreObstacles);
        RecursiveExpand(gridPos + new Vector2Int(0, -1), gridPos, movementRange, movementColor, ignoreObstacles);
    }

    /// <summary>
    /// Recursive add tile to walkable
    /// </summary>
    /// <param name="pos"></param>
    /// <param name="from"></param>
    /// <param name="range"></param>
    private void RecursiveExpand(Vector2Int pos, Vector2Int from, int range, Color colorToSet, bool ignoreObstacles)
    {
        if (GameManager.singleton.map.OutOfBounds(pos) || (!ignoreObstacles && !GameManager.singleton.map.IsCellFree(pos)))
        {
            return;
        }
        //check if another I've already have a better path to this poisition
        if (froms.ContainsKey(pos))
        {
            if (froms[pos].remainingRange < range)
            {
                //if this new path is better, remove previous and continue the expansion
                froms.Remove(pos);
                froms.Add(pos, new Node(from, range));
            }
            else
            {
                //the path before is better, so, stop expanding
                return;
            }
        }
        else
        {

            froms.Add(pos, new Node(from, range));
            changedTiles.Add(pos);
        }
        VisualFeedback(pos, colorToSet);
        if (--range <= 0)
        {
            return;
        }
        RecursiveExpand(pos + new Vector2Int(1, 0), pos, range, colorToSet, ignoreObstacles);
        RecursiveExpand(pos + new Vector2Int(-1, 0), pos, range, colorToSet, ignoreObstacles);
        RecursiveExpand(pos + new Vector2Int(0, 1), pos, range, colorToSet, ignoreObstacles);
        RecursiveExpand(pos + new Vector2Int(0, -1), pos, range, colorToSet, ignoreObstacles);
    }


    /// <summary>
    /// Change the grid cell color
    /// </summary>
    /// <param name="pos"></param>
    private void VisualFeedback(Vector2Int pos, Color color) => GameManager.singleton.map.cellsRenderers[pos.x, pos.y].material.color = color;

    /// <summary>
    /// Restore cells colors
    /// </summary>
    public void EndShow()
    {
        foreach(Vector2Int vector in changedTiles)
        {
            VisualFeedback(vector, GameManager.singleton.map.MapColor);
        }
    }

    /// <summary>
    /// Get the  path from pos to pos
    /// </summary>
    /// <param name="from"></param>
    /// <param name="to"></param>
    /// <returns></returns>
    public List<Vector3> Path(Vector3 from, Vector3 to)
    {
        List<Vector3> path = new List<Vector3>();
        Vector2Int fromGrid = GameManager.singleton.map.WorldToGridCoords(from);
        Vector2Int toGrid = GameManager.singleton.map.WorldToGridCoords(to);
        //I know that it would be the same as to, but if the to wasn't exactly at the center now it is
        path.Add(GameManager.singleton.map.GridToWorldCoords(toGrid));
        while(froms[toGrid].from != fromGrid)
        {
            toGrid = froms[toGrid].from;
            path.Add(GameManager.singleton.map.GridToWorldCoords(toGrid));
        }
        path.Reverse();
        return path;
    }

    /// <summary>
    /// Check in the paths if has a path to the position
    /// </summary>
    /// <param name="to"></param>
    /// <returns></returns>
    public bool CanMoveTo(Vector3 to) => froms.ContainsKey(GameManager.singleton.map.WorldToGridCoords(to));

    /// <summary>
    /// Get the nearest point of the current cells I can go to TO
    /// Return the from vector if surrounded or best position I can move
    /// </summary>
    /// <param name="to"></param>
    /// <returns></returns>
    public Vector3 NearestPointTo(Vector3 from, Vector3 to)
    {
        //maybe I'm surrounded and I can't move
        Vector3 toRet = from;
        //avoid square roots
        float sqrDis = (toRet - to).sqrMagnitude;
        for(int i = 0; i < changedTiles.Count; ++i)
        {
            Vector3 buff = GameManager.singleton.map.GridToWorldCoords(changedTiles[i]);
            float sqrDisBuff = (buff - to).sqrMagnitude;
            if(sqrDisBuff < sqrDis)
            {
                toRet = buff;
                sqrDis = sqrDisBuff;
            }
        }
        return toRet;
    }

    public struct Node
    {
        public Vector2Int from;
        public int remainingRange;

        public Node(Vector2Int pos, int remaining)
        {
            from = pos;
            remainingRange = remaining;
        }
    }
}
