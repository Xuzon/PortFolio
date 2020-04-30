using UnityEngine;

public abstract class Singleton<T> : MonoBehaviour where T : Singleton<T>
{
    public static T singleton { get; private set; }
    public bool forceQuit = false;

    protected virtual void Awake()
    {
        if (singleton == null)
        {
            singleton = (T)this;
        }
        else
        {
            if (forceQuit)
            {
                Debug.Log("Overriding instance of " + this.GetType());
                Destroy(singleton.gameObject);
                singleton = (T)this;
            }
            else
            {
                Debug.LogError("Got a second instance of the class " + this.GetType());
                Destroy(gameObject);
            }
        }
        AddAwake();
    }

    protected virtual void AddAwake()
    {

    }
}