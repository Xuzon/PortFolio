using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Pawn : MonoBehaviour, ISelectable
{

    public Sprite pawnSprite;

    #region Health
    protected int maxHealth = 1;
    public int MaxHealth 
    {
        get => maxHealth; 
        set
        {
            maxHealth = value;
            health = value;
        }
    }
    protected int health;
    public int Health
    {
        get => health;
        protected set
        {
            health = value;
            OnChangedHealth?.Invoke(health);
            if(health <= 0)
            {
                Destroy(gameObject);
            }
        }
    }
    internal void Hurt(int attack)
    {
        this.Health -= attack;
    }
    public event System.Action<int> OnChangedHealth;
    #endregion

    public PawnAttack Attack { get; protected set; }
    public PawnMovement Movement { get; protected set; }
    protected Renderer rend;
    protected Player myPlayer;

    protected State currentState = State.Idle;
    /// <summary>
    /// The state of the pawn is the one who manages what cells to show
    /// </summary>
    public State CurrentState 
    { 
        get => currentState; 
        protected set
        {
            currentState = value;
            switch (value)
            {
                case State.Idle:
                    rend.material.color = myPlayer.myColor;
                    break;
                case State.Moved:
                    GameManager.singleton.map.pathfinding.EndShow();
                    GameManager.singleton.map.pathfinding.ShowMovement(this.transform.position, Attack.AttackRange, Color.red, true);
                    GameManager.singleton.hud.ChangeSkipActionText("Combat");
                    break;
                case State.End:
                    GameManager.singleton.map.pathfinding.EndShow();
                    myPlayer.OnEndedPawnTurn(this);
                    rend.material.color *= 0.3f;
                    break;
            }
        }
    }
    /// <summary>
    /// Event that will be triggered when a movement is set
    /// </summary>
    public event System.Action<Vector3, Vector3> OnMovement;
    /// <summary>
    /// Event that will be triggered on destroy this
    /// </summary>
    public event System.Action<Vector3> OnDestroyed;
    private void OnDestroy()
    {
        myPlayer?.MyPawns.Remove(this);
        GameManager.singleton?.OnDestroyedPawn(myPlayer);
        OnDestroyed?.Invoke(transform.position);
    }
    public void Setup(Player player, int maxHealth, int attack, int movementRange, int attackRange)
    {
        Attack = GetComponent<PawnAttack>();
        Movement = GetComponent<PawnMovement>();
        rend = GetComponent<Renderer>();
        this.MaxHealth = maxHealth;
        Attack.Attack = attack;
        Attack.AttackRange = attackRange;
        Movement.MovementRange = movementRange;
        myPlayer = player;
        myPlayer.MyPawns.Add(this);
    }

    /// <summary>
    /// Move to 
    /// </summary>
    /// <param name="pos"></param>
    public IEnumerator MoveTo(Vector3 pos)
    {
        if(CurrentState != State.Idle)
        {
            yield break;
        }
        List<Vector3> path = GameManager.singleton.map.pathfinding.Path(transform.position,pos);
        OnMovement?.Invoke(transform.position, pos);
        GameManager.singleton.map.pathfinding.EndShow();
        yield return StartCoroutine(Movement.Move(path));
        GameManager.singleton.map.pathfinding.ShowMovement(transform.position, Attack.AttackRange, Color.red);
        CurrentState = State.Moved;
    }

    /// <summary>
    /// Check if it is a enemy pawn
    /// </summary>
    /// <param name="player"></param>
    /// <returns></returns>
    internal bool IsEnemy(Player player) => player != myPlayer;

    /// <summary>
    /// Attack to other pawn whose owner its not my player
    /// </summary>
    /// <param name="other"></param>
    public void AttackTo(Pawn other)
    {
        if(CurrentState != State.Moved || other.myPlayer == myPlayer)
        {
            return;
        }
        Attack.AttackTo(other);
        CurrentState = State.End;
    }

    /// <summary>
    /// Skip action (move, or attack)
    /// </summary>
    public void SkipAction()
    {
        switch (CurrentState)
        {
            case State.Idle:
                CurrentState = State.Moved;
                break;
            case State.Moved:
                CurrentState = State.End;
                break;
        }
    }

    /// <summary>
    /// Can I be selected by the current player? if so, change my material to have a visual feedback
    /// </summary>
    /// <param name="currentPlayer"></param>
    /// <returns></returns>
    public bool OnSelect(Player currentPlayer)
    {
        if(CurrentState != State.Idle || currentPlayer != myPlayer)
        {
            return false;
        }
        rend.material.color = Color.yellow;
        GameManager.singleton.map.pathfinding.ShowMovement(transform.position, Movement.MovementRange, Color.cyan);
        return true;
    }

    /// <summary>
    /// Change my visual feedback
    /// </summary>
    public void OnDeselect()
    {
        rend.material.color = myPlayer.myColor;
    }

    /// <summary>
    /// Reset state of the turn
    /// </summary>
    internal void ResetTurn()
    {
        CurrentState = State.Idle;
    }
    public enum State { Idle, Moved, End}
}
