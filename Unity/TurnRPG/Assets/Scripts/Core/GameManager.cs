using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : Singleton<GameManager>
{
    public HUD hud;
    public MapManager map;
    public List<Player> players = new List<Player>();
    protected int currentPlayer = 0;

    private void Start()
    {
        players[currentPlayer].SetTurn();
    }

    private void Update()
    {
        players[currentPlayer].TurnManagement();
    }

    /// <summary>
    /// Switch the turn to the next in the list on next frame
    /// </summary>
    public void SwitchTurn()
    {
        IEnumerator SwitchTurnAtEndOfFrame()
        {
            yield return null;
            currentPlayer++;
            if (currentPlayer >= players.Count)
            {
                currentPlayer = 0;
            }
            players[currentPlayer].SetTurn();
        }
        StartCoroutine(SwitchTurnAtEndOfFrame());
    }

    /// <summary>
    /// Callback when a pawn of a player is destroyed
    /// </summary>
    /// <param name="owner"></param>
    public void OnDestroyedPawn(Player owner)
    {
        if(owner.MyPawns.Count > 0)
        {
            return;
        }

        GameEnd(owner is AI);
    }

    /// <summary>
    /// Show feedback of ended game
    /// </summary>
    private void GameEnd(bool win)
    {
        //can happen when a pawn is destroyed after the game manager is destroyed (unloading)
        if(this == null)
        {
            return;
        }
        //disable game manager update (disable player input)
        this.enabled = false;
        GameManager.singleton.hud.endPanel.Enable(win);
    }
}
