#ifndef MWINDOW_H
#define MWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MWindow;
}

class MWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MWindow(QWidget *parent = 0);
    ~MWindow();
    enum Operation{Add = 1,Substract = 2,Divide = 3,Muliply = 4};
    Operation currentOperation;

private slots:

    #pragma region  Numbers
    void BOnePressed();

    void BTwoPressed();

    void BThreePressed();

    void BFourPressed();

    void BFivePressed();

    void BSixPressed();

    void BSevenPressed();

    void BEightPressed();

    void BNinePressed();

    void BZeroPressed();
    #pragma endregion

    void BDotPressed();

    #pragma region Operations
    void BAddPressed();

    void BSubstractPressed();

    void BMultiplyPressed();

    void BDividePressed();

    void BEqualsPressed();

    void BResetPressed();
    #pragma endregion

    #pragma region Memory
    void BSaveMemoryPressed();

    void BSubstractMemoryPressed();

    void BUseMemoryPressed();

    void BDeleteMemoryPressed();
    #pragma endregion

   
    #pragma region Aux
    void ChangeOperation(Operation op);

    float GetNumberOfDecimals(float f);

    void ButtonPressed();
    #pragma endregion




private:
    Ui::MWindow *ui;
    //states
    bool firstOperandChoosing;
    bool dot;
    int divider = 0;
    //operands
    float firstOperand;
    float secondOperand;
    QString sFirstOperand;
    QString sSecondOperand;
    float result;
    //Memory
    float memory;
    float displayNumber;
    //aux
    int decimalThreshold = 4;

    //Aux methods
    void StoreNumber(float number);
    void ShowLCD(QString* value);
};

#endif // MWINDOW_H
