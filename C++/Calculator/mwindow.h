#ifndef MWINDOW_H
#define MWINDOW_H

#include <QMainWindow>

namespace Ui {
class MWindow;
}

class MWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MWindow(QWidget *parent = 0);
    ~MWindow();
    enum Operation{Add = 1,Substract = 2,Divide = 3,Muliply = 4};
    Operation currentOperation;

private slots:

    void BOnePressed();

    void BTwoPressed();

    void BThreePressed();

    void BFourPressed();

    void BFivePressed();

    void BSixPressed();

    void BSevenPressed();

    void BEightPressed();

    void BNinePressed();

    void BAddPressed();

    void BSubstractPressed();

    void BMultiplyPressed();

    void BDividePressed();

    void BEqualsPressed();

private:
    Ui::MWindow *ui;
    bool firstOperandChoosing;
    float firstOperand;
    float secondOperand;
    float result;
    void StoreNumber(float number);
    void ShowLCD(QString value);
};

#endif // MWINDOW_H
