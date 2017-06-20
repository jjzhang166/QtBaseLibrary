#include "stdafx.h"
#include "TitleBar.h"
#include "ui_TitleBar.h"

#include "PushButton.h"
#include "AppHelper.h"


//==================================================================================================
TitleBar::TitleBar(QWidget *parent)
: QWidget(parent), ui(new Ui::TitleBar)
{
	ui->setupUi(this);

	regWindowTitle(parent, ui->btnClose, ui->btnMin, ui->btnMax, this);
}

TitleBar::~TitleBar()
{
	delete ui;
}

//==================================================================================================
void TitleBar::setPixmap(const QPixmap & pximap)
{
	ui->labIcon->setPixmap(pximap);
}

void TitleBar::setTitleText(const QString & title)
{
	ui->labTitle->setText(title);
}

void TitleBar::setBtnVisible(ButtonType type, bool visible)
{
	switch (type)
	{
	case BUTTON_MIN:
		ui->btnMin->setVisible(visible);
		break;
	case BUTTON_MAX:
		ui->btnMax->setVisible(visible);
		break;
	case BUTTON_CLOSE:
		ui->btnClose->setVisible(visible);
		break;
	case BUTTON_SKIN:
		break;
	case BUTTON_MORE:
		break;
	default:
		break;
	}
}

//==================================================================================================