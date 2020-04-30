using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AI : Player
{

    protected Player targetPlayer;

    public void Start()
    {
        foreach(Player player in GameManager.singleton.players)
        {
            if(!(player is AI))
            {
                targetPlayer = player;
                return;
            }
        }
    }
    public override void SetTurn()
    {
        base.SetTurn();
        //TODO will need to think
        StartCoroutine(TurnThink());
    }

    private IEnumerator TurnThink()
    {
        foreach(Pawn pawn in MyPawns)
        {
            CurrentPawn = pawn;
            CurrentPawn.OnSelect(this);
            ManagePawn(pawn);
            while(pawn.CurrentState != Pawn.State.End)
            {
                yield return new WaitForSeconds(0.1f);
            }
        }
    }

    /// <summary>
    /// Iterate over the actions of the pawn
    /// </summary>
    /// <param name="pawn"></param>
    private void ManagePawn(Pawn pawn)
    {
        //maybe I killed all player's pawns
        if (targetPlayer.MyPawns.Count == 0)
        {
            pawn.SkipAction();
            pawn.SkipAction();
            return;
        }
        Pawn target = ChoseTarget(pawn);
        StartCoroutine(ManagePawnRoutine(pawn, target));
    }

    /// <summary>
    /// Manage the actions of the pawn to the target
    /// </summary>
    /// <param name="pawnToManage"></param>
    /// <param name="target"></param>
    /// <returns></returns>
    IEnumerator ManagePawnRoutine(Pawn pawnToManage, Pawn target)
    {
        while(pawnToManage.CurrentState != Pawn.State.End)
        {
            switch (pawnToManage.CurrentState)
            {
                case Pawn.State.Idle:
                    if(waiting)
                    {
                        break;
                    }
                    Vector3 nearestPos = GameManager.singleton.map.pathfinding.NearestPointTo(pawnToManage.transform.position, target.transform.position);
                    //maybe I'm at the better placement
                    if(nearestPos == pawnToManage.transform.position)
                    {
                        pawnToManage.SkipAction();
                        break;
                    }
                    MoveCurrentPawnTo(nearestPos);
                    break;
                case Pawn.State.Moved:
                    if(!GameManager.singleton.map.pathfinding.CanMoveTo(target.transform.position))
                    {
                        pawnToManage.SkipAction();
                    }
                    else
                    {
                        pawnToManage.AttackTo(target);
                    }
                    break;
            }
            yield return null;
        }
    }

    /// <summary>
    /// Choose nearest enemy pawn
    /// </summary>
    /// <param name="pawn"></param>
    /// <returns></returns>
    private Pawn ChoseTarget(Pawn pawn)
    {
        Pawn targetPawn = targetPlayer.MyPawns[0];
        float sqrDis = (targetPawn.transform.position - pawn.transform.position).sqrMagnitude;
        for (int i = 1; i < targetPlayer.MyPawns.Count; ++i)
        {
            Pawn buff = targetPlayer.MyPawns[1];
            float buffSqrDis = (buff.transform.position - pawn.transform.position).sqrMagnitude;
            if (buffSqrDis < sqrDis)
            {
                sqrDis = buffSqrDis;
                targetPawn = buff;
            }
        }
        return targetPawn;
    }
}
