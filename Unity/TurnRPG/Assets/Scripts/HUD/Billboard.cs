using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Billboard : MonoBehaviour
{

    Camera cam;

    IEnumerator Start()
    {
        if (Camera.main == null)
        {
            yield return new WaitUntil(() => Camera.main != null);
        }
        cam = Camera.main;
    }

    void Update()
    {
        RefreshRotation();
    }

    private void RefreshRotation()
    {
        if (cam != null)
        {
            transform.LookAt(transform.position + cam.transform.rotation * Vector3.forward,
             cam.transform.rotation * Vector3.up);
        }
    }
}
