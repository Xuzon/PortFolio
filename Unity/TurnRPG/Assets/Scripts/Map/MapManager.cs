using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MapManager : MonoBehaviour
{
    public MapPathfinding pathfinding = new MapPathfinding();

    [Header("Grid")]
    public Vector2Int mapSize;
    [SerializeField]
    protected float cellSize = 1;
    [SerializeField]
    protected GameObject gridObject;

    [Header("Pawns")]
    [SerializeField]
    protected List<PawnSetup> pawnsToSpawn = new List<PawnSetup>();
    
    [Header("Colors")]
    [SerializeField]
    protected Color mapColor = Color.gray;
    public Color MapColor => mapColor;

    public Renderer[,] cellsRenderers;
    protected bool[,] mapCells;

    protected void Awake()
    {
        GenerateMap();
        SpawnPawns();
    }

    internal bool IsCellFree(Vector2Int pos) => !mapCells[pos.x, pos.y];

    /// <summary>
    /// Check if a vector is outside of the grid
    /// </summary>
    /// <param name="pos"></param>
    /// <returns></returns>
    internal bool OutOfBounds(Vector2Int pos) =>  pos.x < 0 || pos.x >= mapSize.x || pos.y < 0 || pos.y >= mapSize.y;

    /// <summary>
    /// Spawn the pawns that are in setup
    /// </summary>
    private void SpawnPawns()
    {
        foreach(PawnSetup setup in pawnsToSpawn)
        {
            Pawn pawn = setup.pawnObject == null ? null : setup.pawnObject.GetComponent<Pawn>();
            if (!SpawnPawnAssurance(setup, pawn, true))
            {
                continue;
            }
            Vector3 worldPos = GridToWorldCoords(setup.gridPos);
            SetCell(setup.gridPos, true);
            GameObject instantiated = Instantiate(setup.pawnObject, worldPos, Quaternion.identity);
            pawn = instantiated.GetComponent<Pawn>();
            pawn.GetComponent<Renderer>().material.color = setup.controlPlayer.myColor;
            pawn.Setup(setup.controlPlayer, setup.health, setup.attack, setup.movementRange, setup.attackRange);
            pawn.OnMovement += OnPawnMoved;
            pawn.OnDestroyed += OnPawnDestroyed;
        }
    }

    /// <summary>
    /// Callback to free the cell on that a pawn was destroyed
    /// </summary>
    /// <param name="obj"></param>
    private void OnPawnDestroyed(Vector3 obj)
    {
        SetCell(GameManager.singleton.map.WorldToGridCoords(obj), false);
    }

    /// <summary>
    /// Callback to change what cells are free because a movement
    /// </summary>
    /// <param name="from"></param>
    /// <param name="to"></param>
    private void OnPawnMoved(Vector3 from, Vector3 to)
    {
        SetCell(GameManager.singleton.map.WorldToGridCoords(from), false);
        SetCell(GameManager.singleton.map.WorldToGridCoords(to), true);
    }

    /// <summary>
    /// Shortcut to mapCells[vector.x, vector.y] = bool
    /// </summary>
    /// <param name="pos"></param>
    /// <param name="toSet"></param>
    protected void SetCell(Vector2Int pos, bool toSet) => mapCells[pos.x, pos.y] = toSet;

    /// <summary>
    /// Assure tha a pawn setup can spawn
    /// </summary>
    /// <param name="setup"></param>
    /// <param name="pawn"></param>
    /// <returns></returns>
    private bool SpawnPawnAssurance(PawnSetup setup, Pawn pawn, bool checkIsCellFree)
    {
        if (pawn == null)
        {
            Debug.LogWarning("Pawn Object invalid setup!!");
            return false;
        }
        if(setup.controlPlayer == null)
        {
            Debug.LogWarning("Pawn without player, please, assign one");
            return false;
        }
        if (OutOfBounds(setup.gridPos))
        {
            Debug.LogWarning("Pawn setup out of the map");
            return false;
        }
        if (checkIsCellFree && mapCells[setup.gridPos.x, setup.gridPos.y])
        {
            Debug.LogWarning("Tried to spawn a pawn in a position that has been placed other pawn");
            return false;
        }

        return true;
    }


    /// <summary>
    /// Generate the map grid
    /// </summary>
    void GenerateMap()
    {
        cellsRenderers = new Renderer[mapSize.x, mapSize.y];
        mapCells = new bool[mapSize.x, mapSize.y];
        Vector2Int buff = Vector2Int.zero;
        for (int x = 0; x < mapSize.x; ++x)
        {
            buff.x = x;
            for(int y = 0; y < mapSize.y; ++y)
            {
                buff.y = y;
                Vector3 worldPos = GridToWorldCoords(buff);
                GameObject go = Instantiate(gridObject, worldPos, Quaternion.identity);
                go.transform.parent = transform;
                Renderer rend = go.GetComponent<Renderer>();
                if(rend == null)
                {
                    Debug.LogWarning("Warning, the cell object has no renderer");
                }
                rend.material.color = mapColor;
                cellsRenderers[x, y] = rend;
            }
        }
    }

    /// <summary>
    /// Giving a world space point return a vector2int with the grid coords
    /// </summary>
    /// <param name="point"></param>
    /// <returns></returns>
    public Vector2Int WorldToGridCoords(Vector3 point)
    {
        point.x = Mathf.Round(point.x);
        point.z = Mathf.Round(point.z);
        int xVoxel = (int)(point.x / cellSize);
        int zVoxel = (int)(point.z / cellSize);
        xVoxel = Mathf.Clamp(xVoxel, 0, mapSize.x - 1);
        zVoxel = Mathf.Clamp(zVoxel, 0, mapSize.y - 1);
        return new Vector2Int(xVoxel, zVoxel);
    }

    /// <summary>
    /// Giving a grid space coord return a world space coord
    /// </summary>
    /// <param name="grid"></param>
    /// <returns></returns>
    public Vector3 GridToWorldCoords(Vector2Int grid)
    {
        float x = grid.x * cellSize;
        float z = grid.y * cellSize;
        float y = 0;
        return new Vector3(x, y, z);
    }

    /// <summary>
    /// Draw the grid and spawns to help designers visualize
    /// </summary>
    private void OnDrawGizmos()
    {
        DrawGridGizmos();
        DrawPawnGizmos();
    }

    /// <summary>
    /// Draw the grid using box gizmos
    /// </summary>
    private void DrawGridGizmos()
    {
        Vector3 size = new Vector3(cellSize, 0.1f, cellSize);
        Vector2Int buff = Vector2Int.zero;
        Gizmos.color = mapColor;
        for (int x = 0; x < mapSize.x; ++x)
        {
            buff.x = x;
            for (int y = 0; y < mapSize.y; ++y)
            {
                buff.y = y;
                Vector3 worldPos = GridToWorldCoords(buff);
                Gizmos.DrawWireCube(worldPos, size);
            }
        }
    }

    /// <summary>
    /// Draw the pawn gizmos (if it is marked red the setup will fail)
    /// </summary>
    private void DrawPawnGizmos()
    {
        foreach (PawnSetup setup in pawnsToSpawn)
        {
            Pawn pawn = setup.pawnObject == null ? null : setup.pawnObject.GetComponent<Pawn>();
            Color colorToDraw = Color.red;
            if (setup.controlPlayer != null)
            {
                colorToDraw = setup.controlPlayer.myColor;
            }
            colorToDraw = SpawnPawnAssurance(setup, pawn, false) ? colorToDraw : Color.red;
            Gizmos.color = colorToDraw;
            Gizmos.DrawSphere(GridToWorldCoords(setup.gridPos), cellSize / 2f);
        }
    }
}
