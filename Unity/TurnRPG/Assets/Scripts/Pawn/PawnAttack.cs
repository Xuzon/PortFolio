using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PawnAttack : MonoBehaviour
{
    public int Attack { get; set; }
    public int AttackRange { get; set; }
    [SerializeField]
    protected GameObject particleToSpawn;
    protected ParticleSystem instantiatedParticle;

    /// <summary>
    /// Spawn the particle at start to avoid GarbageAllocation on every attack
    /// </summary>
    private void Start()
    {
        if(particleToSpawn != null)
        {
            GameObject go = Instantiate(particleToSpawn, transform);
            instantiatedParticle = go.GetComponent<ParticleSystem>();
            if(instantiatedParticle == null)
            {
                DestroyImmediate(go);
            }
            else
            {
                go.SetActive(false);
            }
        }
    }

    /// <summary>
    /// Hurt other pawn and play the particle
    /// </summary>
    /// <param name="other"></param>
    internal void AttackTo(Pawn other)
    {
        if (instantiatedParticle != null)
        {
            instantiatedParticle.transform.position = other.transform.position;
            instantiatedParticle.gameObject.SetActive(true);
            instantiatedParticle.Play();
        }
        other.Hurt(Attack);
    }
}
