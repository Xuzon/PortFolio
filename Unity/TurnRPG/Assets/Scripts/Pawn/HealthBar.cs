using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HealthBar : MonoBehaviour
{
    [SerializeField]
    protected Pawn pawn;
    [SerializeField]
    protected UnityEngine.UI.Image healthBar;
    [SerializeField]
    protected TMPro.TextMeshProUGUI healthText;

    void Start()
    {
        pawn.OnChangedHealth += UpdateBar;
        UpdateBar(pawn.MaxHealth);
    }

    /// <summary>
    /// Update the text and fillamount of this script
    /// </summary>
    /// <param name="value"></param>
    private void UpdateBar(int value)
    {
        healthText.text = value.ToString();
        healthBar.fillAmount = value / (float)pawn.MaxHealth;
    }
}
