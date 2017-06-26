#pragma once

#include <QtWidgets/QWidget>
#include "ui_TestLibs.h"

class TestLibs : public QWidget
{
	Q_OBJECT

public:
	TestLibs(QWidget *parent = Q_NULLPTR);

private:
	Ui::TestLibsClass ui;
};
