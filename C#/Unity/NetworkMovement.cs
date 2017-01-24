using UnityEngine;
using System.Collections;
using UnityEngine.Networking;

public class NetworkMovement : NetworkBehaviour {
    #region Properties
    [SerializeField]
    protected Transform target;

    #region Setup
    [Header("Setup")]
    [Range(0,10)] public int SendRate = 2;
    [Range(0,2)] public float movementThreshold = 0.2f;
    [Range(0,30)] public float angleThreshold = 5;
    [Range(0, 10)] public float distanceBeforeSnap = 4;
    [Range(0,90)] public float angleBeforeSnap = 40;
    #endregion

    #region Interpolation
    [Header("Interpolation")]
    [Range(0,1)] public float movementInterpolation = 0.1f;
    [Range(0,1)] public float rotationInterpolation = 0.1f;
    #endregion

    #region Prediction
    public float thresholdMovementPrediction = 0.7f;
    public float thresholdRotationPrediction = 15;
    #endregion

    #region ProtectedProperties
    protected Vector3 lastDirectionPerFrame = Vector3.zero;
    protected Vector3 lastPositionSent = Vector3.zero;
    protected Quaternion lastRotationSent = Quaternion.identity;
    protected Quaternion lastRotationDirectionPerFrame = Quaternion.identity;
    protected bool send = false;
    protected bool sending = false;
    protected int count = 0;
    #endregion

    #endregion

    #region Logic
    void FixedUpdate () {
        if (isLocalPlayer) {
            sendInfo();
        } else {
            recontiliation();
        }
    }

    protected void sendInfo() {
        if (send) {
            if (count == SendRate) {
                count = 0;
                send = false;
                Vector3 v = target.position;
                Quaternion q = target.rotation;
                CmdSendPosition(v,q);
            } else {
                count++;
            }
        } else {
            checkIfSend();
        }
    }
    protected void checkIfSend() {
        if (sending) {
            send = true;
            sending = false;
            return;
        }
        Vector3 v = target.position;
        Quaternion q = target.rotation;
        float distance = Vector3.Distance(lastPositionSent, v);
        float angle = Quaternion.Angle(lastRotationSent, q); ;
        if (distance > movementThreshold || angle > angleThreshold) {
            send = true;
            sending = true;
        }
    }
    protected void recontiliation() {
        Vector3 v = target.position;
        Quaternion q = target.rotation;
        float distance = Vector3.Distance(lastPositionSent, v);
        float angle = Vector3.Angle(lastRotationSent.eulerAngles, q.eulerAngles);
        if (distance > distanceBeforeSnap) {
            target.position = lastPositionSent;
        }
        if (angle > angleBeforeSnap) {
            target.rotation = lastRotationSent;
        }
        //prediction
        v += lastDirectionPerFrame;
        q *= lastRotationDirectionPerFrame;
        //interpolation
        Vector3 vLerp = Vector3.Lerp(v, lastPositionSent, movementInterpolation);
        Quaternion qLerp = Quaternion.Lerp(q, lastRotationSent, rotationInterpolation);
            target.position = vLerp;
            target.rotation = qLerp;
    }
    #endregion

    #region OverNetwork
    [Command (channel = 1)]
    protected void CmdSendPosition(Vector3 newPos,Quaternion newRot) {
        RpcReceivePosition(newPos, newRot);
    }

    [ClientRpc(channel = 1)]
    protected void RpcReceivePosition(Vector3 newPos,Quaternion newRot) {
        int frames = (SendRate + 1);
        lastDirectionPerFrame = newPos - lastPositionSent;
        //right now prediction is made with the new direction and amount of frames
        lastDirectionPerFrame /= frames;
        if (lastDirectionPerFrame.magnitude > thresholdMovementPrediction) {
            lastDirectionPerFrame = Vector3.zero;
        }
        Vector3 lastEuler = lastRotationSent.eulerAngles;
        Vector3 newEuler = newRot.eulerAngles;
        if (Quaternion.Angle(lastRotationDirectionPerFrame, newRot) < thresholdRotationPrediction) {
            lastRotationDirectionPerFrame = Quaternion.Euler(( newEuler - lastEuler ) / frames);
        }else {
            lastRotationDirectionPerFrame = Quaternion.identity;
        }
        lastPositionSent = newPos;
        lastRotationSent = newRot;
    }
    #endregion
}
