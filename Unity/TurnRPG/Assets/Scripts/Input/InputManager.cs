using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InputManager : MonoBehaviour
{

    /// <summary>
    /// Will process mouse clicks and touch
    /// </summary>
    /// <param name="inputPos"></param>
    /// <returns></returns>
   public static bool HasInput(out Vector3 inputPos)
    {
        inputPos = Input.mousePosition;
        bool toRet = false;
        //we want to select only when we had one finger pointing and not now
        if (Input.touchCount > 0)
        {
            Touch touch = Input.GetTouch(0);
            toRet = touch.phase == TouchPhase.Ended;
            inputPos = touch.position;
        }
        toRet = toRet || Input.GetMouseButtonUp(0);
        return toRet;
    }
}
