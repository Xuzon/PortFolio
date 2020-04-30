using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public interface ISelectable
{
    bool OnSelect(Player currentPlayer);
    void OnDeselect();
}
