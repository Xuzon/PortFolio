using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.UI;

public class HUD : MonoBehaviour
{
    protected readonly string SKIP_ACTION_PATTERN = "Skip <color=red>{0}</color>";

    [Header("EndGame")]
    public EndPanel endPanel;


    [Header("Buttons")]
    [SerializeField]
    protected Button skipTurnButton;
    [SerializeField]
    protected Button skipActionButton;
    [SerializeField]
    protected TextMeshProUGUI skipActionText;

    [Header("Info Panel")]
    [SerializeField]
    protected GameObject infoPanel;
    [SerializeField]
    protected Image pawnAvatar;
    [SerializeField]
    protected TextMeshProUGUI healthText;    
    [SerializeField]
    protected TextMeshProUGUI damageText;
    [SerializeField]
    protected TextMeshProUGUI rangeText;
    
    protected Player currentPlayer;

    /// <summary>
    /// Callback when the active pawn is selected
    /// </summary>
    /// <param name="pawn"></param>
    public void OnChangePawn(Pawn pawn)
    {
        bool toSet = pawn != null;
        skipActionButton.gameObject.SetActive(toSet);
        infoPanel.gameObject.SetActive(toSet);
        if(toSet)
        {
            //if a new pawn is selected show all its info
            healthText.text = pawn.Health.ToString();
            damageText.text = pawn.Attack.Attack.ToString();
            rangeText.text = pawn.Attack.AttackRange.ToString();
            pawnAvatar.sprite = pawn.pawnSprite;
            skipTurnButton.gameObject.SetActive(false);
            ChangeSkipActionText("Movement");
        }
        else
        {
            //if the current pawn is null, check if restore the skip turn button
            if(!(currentPlayer is AI))
            {
                skipTurnButton.gameObject.SetActive(true);
            }
        }
    }

    /// <summary>
    /// Callback when a new player get its turn
    /// </summary>
    /// <param name="player"></param>
    public void OnChangeTurn(Player player)
    {
        bool toSet = !(player is AI);
        currentPlayer = player;
        skipTurnButton.gameObject.SetActive(toSet);
    }

    /// <summary>
    /// Change the skip action button text
    /// </summary>
    /// <param name="text"></param>
    public void ChangeSkipActionText(string text)
    {
        skipActionText.text = string.Format(SKIP_ACTION_PATTERN, text);
    }
}
