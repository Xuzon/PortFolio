using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    public List<Pawn> MyPawns { get; set; } = new List<Pawn>();
    protected Pawn currentPawn;
    protected Pawn CurrentPawn
    {
        get => currentPawn;
        set
        {
            currentPawn = value;
            GameManager.singleton.hud.OnChangePawn(value);
        }
    }

    public Color myColor;
    protected Camera cam;

    /// <summary>
    /// Flag that I'm waiting to a pawn action to complete
    /// </summary>
    protected bool waiting;

    private void Start()
    {
        cam = Camera.main;
    }

    /// <summary>
    /// Restore the pawns turns
    /// </summary>
    public virtual void SetTurn()
    {
        foreach (Pawn pawn in MyPawns)
        {
            pawn.ResetTurn();
        }
        GameManager.singleton.hud.OnChangeTurn(this);
        CurrentPawn = null;
    }

    /// <summary>
    /// Turn management called from GameManager
    /// </summary>
    public virtual void TurnManagement()
    {
        if (waiting)
        {
            return;
        }
        Vector3 pos;
        if (!InputManager.HasInput(out pos))
        {
            return;
        }
        Ray ray = cam.ScreenPointToRay(pos);
        if (Physics.Raycast(ray, out RaycastHit hit, Mathf.Infinity))
        {
            //we don't want the same click to select the pawn and manage the action
            if (SelectPawn(hit))
            {
                return;
            }
            PawnManagement(hit);
        }
    }

    /// <summary>
    /// Manage the active pawn
    /// </summary>
    protected void PawnManagement(RaycastHit hit)
    {
        if (CurrentPawn == null)
        {
            return;
        }
        switch (CurrentPawn.CurrentState)
        {
            case Pawn.State.Idle:
                //manage movement
                ManageMovement(hit);
                break;
            case Pawn.State.Moved:
                //manage attack
                ManageCombat(hit);
                break;
        }
    }

    /// <summary>
    /// Manage the combat of the active pan
    /// </summary>
    /// <param name="hit"></param>
    private void ManageCombat(RaycastHit hit)
    {
        Pawn hitPawn = hit.collider.GetComponent<Pawn>();
        //cannot attack if its not a pawn
        if (hitPawn == null)
        {
            return;
        }
        //nor if it is not an enemy
        bool cantAttack = !hitPawn.IsEnemy(this);
        //nor I don't have enough range (Im using CanMoveTo because the pathfinding has all cells I can attack)
        cantAttack = cantAttack || !GameManager.singleton.map.pathfinding.CanMoveTo(hitPawn.transform.position);
        if (cantAttack)
        {
            return;
        }
        currentPawn.AttackTo(hitPawn);
    }

    /// <summary>
    /// Manage movement of the pawn
    /// </summary>
    /// <param name="hit"></param>
    private void ManageMovement(RaycastHit hit)
    {
        Vector3 pos = hit.collider.transform.position;
        MoveCurrentPawnTo(pos);
    }

    /// <summary>
    /// Move the current pawn to a position and wait until complete
    /// </summary>
    /// <param name="pos"></param>
    protected void MoveCurrentPawnTo(Vector3 pos)
    {
        if (!GameManager.singleton.map.pathfinding.CanMoveTo(pos))
        {
            return;
        }
        IEnumerator WaitToMove()
        {
            waiting = true;
            yield return StartCoroutine(CurrentPawn.MoveTo(pos));
            waiting = false;
        }
        StartCoroutine(WaitToMove());
    }

    /// <summary>
    /// Callback to receive from pawns where its turn is over
    /// </summary>
    public void OnEndedPawnTurn(Pawn pawn)
    {
        if (CurrentPawn == pawn)
        {
            CurrentPawn.OnDeselect();
            CurrentPawn = null;
        }
        else
        {
            Debug.LogWarning("A pawn that is not the selected pawn is calling on ended pawn!! " + pawn.gameObject.name);
        }
        CheckEndTurn();
    }

    /// <summary>
    /// Iterate over my pawns, if all has ended, switch turn
    /// </summary>
    private void CheckEndTurn()
    {
        foreach (Pawn pawn in MyPawns)
        {
            if (pawn.CurrentState != Pawn.State.End)
            {
                return;
            }
        }
        GameManager.singleton.SwitchTurn();
    }

    /// <summary>
    /// Try to get a selectable pawn from the hit
    /// </summary>
    /// <param name="hit"></param>
    private bool SelectPawn(RaycastHit hit)
    {
        if (CurrentPawn != null)
        {
            return false;
        }
        ISelectable selectable = hit.collider.GetComponent<ISelectable>();
        if (selectable == null)
        {
            return false;
        }
        if (selectable.OnSelect(this))
        {
            if (selectable is Pawn)
            {
                CurrentPawn = selectable as Pawn;
            }
        }
        return true;
    }

    public void SkipCurrentPawnAction()
    {
        CurrentPawn.SkipAction();
    }
}
