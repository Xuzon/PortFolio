#include "mwindow.h"
#include "ui_mwindow.h"

MWindow::MWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MWindow)
{
	ui->setupUi(this);
}

MWindow::~MWindow()
{
	delete ui;
}

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

void MWindow::BAddPressed() {
	currentOperation = Operation::Add;
}

void MWindow::BSubstractPressed() {
	currentOperation = Operation::Substract;
}

void MWindow::BMultiplyPressed() {
	currentOperation = Operation::Muliply;
}

void MWindow::BDividePressed() {
	currentOperation = Operation::Divide;
}

void MWindow::BEqualsPressed() {
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
	firstOperandChoosing = true;
	firstOperand = 0;
	secondOperand = 0;
	ShowLCD(QString::number(result));
}

void MWindow::StoreNumber(float number) {
	if (this->firstOperandChoosing) {
		this->firstOperand = number;
	}
	else {
		this->secondOperand = number;
	}

	firstOperandChoosing = !firstOperandChoosing;
	ShowLCD(QString::number(number));
}


void MWindow::ShowLCD(QString value) {
	this->ui->lcd->setText(value);
}