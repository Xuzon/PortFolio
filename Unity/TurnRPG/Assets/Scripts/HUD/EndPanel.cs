using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class EndPanel : MonoBehaviour
{
    [SerializeField]
    protected TextMeshProUGUI endText;
    [SerializeField]
    protected string winText = "You <color=green>WIN!!!</color>";    
    [SerializeField]
    protected string loseText = "You <color=red>LOSE!!!</color>";

    public void Enable(bool win)
    {
        endText.text = win ? winText : loseText;
        gameObject?.SetActive(true);
    }


    public void Restart()
    {
        int buildIndex = UnityEngine.SceneManagement.SceneManager.GetActiveScene().buildIndex;
        UnityEngine.SceneManagement.SceneManager.LoadScene(buildIndex);
    }
}
