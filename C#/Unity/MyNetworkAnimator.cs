using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using UnityEngine;
using UnityEngine.Networking;

public class MyNetworkAnimator : NetworkBehaviour {

    //Time
    [Range(0,60)]
    public float sendRate = 30f; //HZ
    protected float lastTime = 0;
    protected float interval = 0;

    //Control parameters
    protected AnimatorControllerParameter[] animParameters;
    protected AnimData[] lastParameters;
    protected BinaryFormatter formatter;

    //SEND DATA
    protected List<AnimData> sendParameters;
    protected MemoryStream memStream;

    //RECEIVE DATA
    protected List<AnimData> receiveParameters;
    protected MemoryStream receiveMemStream;

    protected Animator animator;

    /// <summary>
    /// Initialize components
    /// </summary>
    protected void Start () {
        animator = this.GetComponent<Animator>();
        formatter = new BinaryFormatter();
        memStream = new MemoryStream();
        animParameters = animator.parameters;
        lastParameters = new AnimData[animParameters.Length];
        sendParameters = new List<AnimData>();
        interval =  1f / sendRate;
        InitializeDataList();
    }

    /// <summary>
    /// Initialize parameters list
    /// </summary>
    private void InitializeDataList() {
        for (int i = 0; i < animParameters.Length; i++) {
            lastParameters[i] = new AnimData();
            int hash = animParameters[i].nameHash;
            lastParameters[i].id = hash;
            lastParameters[i].type = animParameters[i].type;
        }
    }

    /// <summary>
    /// See if any parameter has changed since last update and count dirties
    /// </summary>
    /// <returns>Number of changed parameters</returns>
    public int UpdateDataList() {
        int dirties = 0;
        for(int i = 0; i < animParameters.Length; i++) {
            int hash = animParameters[i].nameHash;
            //Lets see if the value has changed
            switch (animParameters[i].type) {
                case AnimatorControllerParameterType.Bool:
                    bool bValue = animator.GetBool(hash);
                    lastParameters[i].dirty = lastParameters[i].boolValue != bValue;
                    lastParameters[i].boolValue = bValue;
                    break;
                case AnimatorControllerParameterType.Int:
                    int iValue = animator.GetInteger(hash);
                    lastParameters[i].dirty = lastParameters[i].intValue != iValue;
                    lastParameters[i].intValue = iValue;
                    break;
                case AnimatorControllerParameterType.Float:
                    float fValue = animator.GetFloat(hash);
                    lastParameters[i].dirty = lastParameters[i].floatValue != fValue;
                    lastParameters[i].floatValue = fValue;
                    break;
            }
            if (lastParameters[i].dirty) {
                dirties++;
            }
        }
        return dirties;
    }
    
    protected void Update () {
        //Only local player will update
        if (!isLocalPlayer) {
            return;
        }
        //Check the send rate
        if (SendRate()) {
            return;
        }

        int dirties = UpdateDataList();
        //TODO optimize
        //Get parameters to send
        sendParameters.Clear();
        for(int i = 0; i < lastParameters.Length; i++) {
            if (lastParameters[i].dirty) {
                sendParameters.Add(lastParameters[i].Clone());
                lastParameters[i].dirty = false;
            }
        }
        if(dirties == 0) {
            return;
        }
        //Serialize
        memStream.Flush();
        memStream.Close();
        memStream = new MemoryStream();
        formatter.Serialize(memStream, sendParameters);
        //Send parameters
        CmdChangeParameters(memStream.ToArray(), dirties);
    }

    /// <summary>
    /// Check the send rate
    /// </summary>
    /// <returns>true if I have to wait</returns>
    private bool SendRate() {
        bool toRet = true;
        if(Time.time > lastTime + interval) {
            lastTime = Time.time + interval;
            toRet = false;
        }
        return toRet;
    }

    /// <summary>
    /// Send the parameters to the server
    /// </summary>
    /// <param name="data">Serialized list of AnimData</param>
    /// <param name="size">Number of AnimDatas</param>
    [Command]
    void CmdChangeParameters(byte[] data, int size) {
        RpcChangeParameters(data, size);
    }

    /// <summary>
    /// Receive the parameters from the server
    /// </summary>
    /// <param name="data">Serialized list of AnimData</param>
    /// <param name="size">Number of AnimDatas</param>
    [ClientRpc]
    public void RpcChangeParameters(byte[] data, int size) {
        if (isLocalPlayer) {
            return;
        }
        UpdateAnimator(data, size);
    }

    /// <summary>
    /// Set the parameters to the animator
    /// </summary>
    /// <param name="data">Serialized list of AnimData</param>
    /// <param name="size">Number of AnimDatas</param>
    private void UpdateAnimator(byte[] data, int size) {
        //Deserialize
        receiveMemStream = new MemoryStream(data);
        receiveParameters = formatter.Deserialize(receiveMemStream) as List<AnimData>;
        //Set the data
        for(int i = 0; i < receiveParameters.Count; i++) {
            int hash = receiveParameters[i].id;
            //Check and set type
            switch (receiveParameters[i].type) {
                case AnimatorControllerParameterType.Bool:
                    animator.SetBool(hash, receiveParameters[i].boolValue);
                    break;
                case AnimatorControllerParameterType.Int:
                    animator.SetInteger(hash, receiveParameters[i].intValue);
                    break;
                case AnimatorControllerParameterType.Float:
                    animator.SetFloat(hash, receiveParameters[i].floatValue);
                    break;
            }
        }
    }


    /// <summary>
    /// Serializable class where I write animation data
    /// </summary>
    [System.Serializable()]
    public class AnimData{
        public int id;
        public AnimatorControllerParameterType type;
        public bool dirty;

        public float floatValue;
        public bool boolValue;
        public int intValue;

        /// <summary>
        /// Clone this object into a new object
        /// </summary>
        /// <returns>Copy of this object without dirty</returns>
        public AnimData Clone() {
            AnimData toRet = new AnimData();
            toRet.id = this.id;
            toRet.type = this.type;
            toRet.floatValue = this.floatValue;
            toRet.boolValue = this.boolValue;
            toRet.intValue = this.intValue;
            return toRet;
        }
    }
}
