using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using UnityEngine.Events;

public class TreeRoveAlgorithm : MonoBehaviour{
    public GameObject root;
    Stack<GameObject> stack = new Stack<GameObject>();

    public delegate void gameObjectEvent(GameObject g);


    #region Events
    event gameObjectEvent nodeEvent;
    event gameObjectEvent leafEvent;

    public void addLeafEvent(gameObjectEvent action) {
        leafEvent += action;
    }

    public void addNodeEvent(gameObjectEvent action) {
        nodeEvent += action;
    }

    public void removeLeafEvent(gameObjectEvent action) {
        leafEvent -= action;
    }

    public void removeNodeEvent(gameObjectEvent action) {
        nodeEvent -= action;
    }
    #endregion

    public void initRove() {
        if(root == null) {
            root = GameObject.FindObjectOfType<GameObject>() as GameObject;
        }
        roveTree();
    }

    protected void roveTree() {
        //push the initial node
        stack.Push(root);
        //while we have nodes rove the tree
        while(stack.Count > 0) {
            //pick up the node and invoke the node events
            GameObject node = stack.Pop();
            if(node == null) {
                continue;
            }
            if (nodeEvent != null) {
                nodeEvent.Invoke(node);
            }
            //if the node have childs it is not a leaf so add the childs to the stack
            if (node.transform.childCount != 0) {
                foreach (Transform t in node.transform) {
                    stack.Push(t.gameObject);
                }
            } 
            //if it hasn't childs it means that it is a leaf
            else {
                if (leafEvent != null) {
                    leafEvent.Invoke(node);
                }
            }
        }
    }
}
