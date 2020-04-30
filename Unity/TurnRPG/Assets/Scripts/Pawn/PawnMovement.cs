using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PawnMovement : MonoBehaviour
{

    public int MovementRange { get; set; }
    [SerializeField]
    protected float speed = 10;    
    [SerializeField]
    protected float rotationSpeed = 10;
    [SerializeField]
    protected float distanceToChangeWayPoint = 0.1f;
    protected float sqrToChange = 0.01f;

    private void Start()
    {
        sqrToChange = distanceToChangeWayPoint * distanceToChangeWayPoint;
    }

    /// <summary>
    /// Move through a list of vector3
    /// </summary>
    /// <param name="path"></param>
    /// <returns></returns>
    public IEnumerator Move(List<Vector3> path)
    {
        while(path.Count > 0)
        {
            //get direction
            var dir = path[0] - transform.position;
            dir.Normalize();
            dir *= speed * Time.fixedDeltaTime;
            //rotate
            Quaternion toRot = Quaternion.LookRotation(dir, Vector3.up);
            transform.rotation = Quaternion.Lerp(transform.rotation, toRot, rotationSpeed);
            //move
            transform.position = transform.position + dir;
            //check reached path node
            if ((path[0] - transform.position).sqrMagnitude < sqrToChange)
            {
                path.RemoveAt(0);
            }
            yield return null;
        }
    }
}
