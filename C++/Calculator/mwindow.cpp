#include "mwindow.h"
#include "ui_mwindow.h"

MWindow::MWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MWindow){
    ui->setupUi(this);
    this->firstOperand = 0;
    this->secondOperand = 0;
    this->firstOperandChoosing = true;
    this->dot = false;
}

MWindow::~MWindow(){
    delete ui;
}

#pragma region Numbers
void MWindow::BOnePressed() {
    StoreNumber(1);
}

void MWindow::BTwoPressed() {
    StoreNumber(2);
}

void MWindow::BThreePressed() {
    StoreNumber(3);
}

void MWindow::BFourPressed() {
    StoreNumber(4);
}

void MWindow::BFivePressed() {
    StoreNumber(5);
}

void MWindow::BSixPressed() {
    StoreNumber(6);
}

void MWindow::BSevenPressed() {
    StoreNumber(7);
}

void MWindow::BEightPressed() {
    StoreNumber(8);
}

void MWindow::BNinePressed() {
    StoreNumber(9);
}

void MWindow::BZeroPressed() {
    StoreNumber(0);
}

#pragma endregion

void MWindow::BDotPressed() {
    if (!dot) {
        if (firstOperandChoosing) {
            sFirstOperand += ".";
        } else {
            sSecondOperand += ".";
        }
        divider = 10;
    }
    dot = true;
}

#pragma region Operations
void MWindow::BAddPressed() {
    this->ui->LState->setText("+");
    ChangeOperation(Operation::Add);
}

void MWindow::BSubstractPressed() {
    this->ui->LState->setText("-");
    ChangeOperation(Operation::Substract);
}

void MWindow::BMultiplyPressed() {
    this->ui->LState->setText("X");
    ChangeOperation(Operation::Muliply);
}

void MWindow::BDividePressed() {
    this->ui->LState->setText("/");
    ChangeOperation(Operation::Divide);
}

void MWindow::ChangeOperation(Operation op) {
    currentOperation = op;
    this->dot = false;
    if (firstOperand == 0) {
        firstOperand = result;
        sFirstOperand = QString::number(result);
    }
    firstOperandChoosing = false;
}

#pragma endregion

#pragma region Memory
void MWindow::BSaveMemoryPressed() {
    this->ui->LMemoryState->setText("M");
    memory += displayNumber;
}

void MWindow::BSubstractMemoryPressed() {
    this->ui->LMemoryState->setText("M");
    memory -= displayNumber;
}


void MWindow::BUseMemoryPressed() {
    if (firstOperandChoosing) {
        firstOperand = memory;
    }else{
        secondOperand = memory;
    }
    ShowLCD(&QString::number(memory));
}

void MWindow::BDeleteMemoryPressed() {
    this->ui->LMemoryState->setText("");
    memory = 0;
}

#pragma endregion

void MWindow::BResetPressed() {
    this->dot = false;
    this->ui->LState->setText("");
    sFirstOperand = "";
    firstOperand = 0;
    sSecondOperand = "";
    secondOperand = 0;
    firstOperandChoosing = true;
    ShowLCD(&QString::number(0));
}

void MWindow::BEqualsPressed() {
    //Do real operations
    switch (currentOperation) {
    case Operation::Add:
        result = firstOperand + secondOperand;
        break;
    case Operation::Divide:
        result = firstOperand / secondOperand;
        break;
    case Operation::Muliply:
        result = firstOperand * secondOperand;
        break;
    case Operation::Substract:
        result = firstOperand - secondOperand;
        break;
    }
    this->firstOperandChoosing = true;
    this->firstOperand = 0;
    sFirstOperand = "";
    this->secondOperand = 0;
    sSecondOperand = "";
    this->dot = false;
    displayNumber = result;
    this->ui->LState->setText("");
    ShowLCD(&QString::number(result));
}
#pragma region Aux
void MWindow::StoreNumber(float number) {
    QString toAdd = QString::number(number);
    if (number == 0 && !this->dot) {
        toAdd = "";
    }
    if (this->firstOperandChoosing) {
        sFirstOperand += toAdd;
        if (this->dot) {
            //keep adding in the decimal part
            number = number / divider;
            divider *= 10;
            this->firstOperand += number;
        }else {
            //normal adding
            this->firstOperand = this->firstOperand * 10;
            this->firstOperand += number;
        }
        displayNumber = firstOperand;
    }else {
        sSecondOperand += toAdd;
        if (this->dot) {
            //keep adding in the decimal part
            number = number / divider;
            divider *= 10;
            this->secondOperand += number;
        }else {
            //keep adding
            this->secondOperand = this->secondOperand * 10;
            this->secondOperand += number;
        }
        displayNumber = firstOperand;
    }
    QString showNumber = firstOperandChoosing ? sFirstOperand : sSecondOperand;
    ShowLCD(&showNumber);
}



void MWindow::ShowLCD(QString* value) {
    this->ui->lcd->setText(*value);
}

///Get the number of decimals of a float (format 0.xxxxxxx)
float MWindow::GetNumberOfDecimals(float f) {
    //TODO
    float toRet = 0;
    int counter = 0;
    int iTemp = 0;
    while (counter < decimalThreshold) {
        f = f * 10;
        iTemp = (int)f;
        if (iTemp == 0) {
            counter++;
        }else {
            f = f - iTemp;
            counter = 0;
            toRet++;
        }
    }
    return toRet;
}

///This method will change two buttons positions
void MWindow::ButtonPressed() {
    int rNumber = rand() % 100;
    /*if (rNumber < 10) {
        QObjectList* childList = this->children;
        int size = childList->count;
        int first = rand() % size;
        int second = first;
        QObject* firstObject;
        QObject* secondObject;
        while (true) {
            second = rand() % size;
            if (first != second) {
                secondObject = childList->at(second);
                if(QWidget* q = dynamic_cast<QWidget*>(secondObject)){
                    break;
                }
            }
        }

        QWidget* firstWidget = dynamic_cast<QWidget*>(firstObject);
        QWidget* secondWidget = dynamic_cast<QWidget*>(secondObject);
        //int* x = firstWidget->x;
        //int* y = firstWidget->y;

    }*/
}
#pragma endregion