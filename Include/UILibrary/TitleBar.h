#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QAbstractButton>

#include "AppGlobal.h"
#include "AppStruct.h"


namespace Ui{ class TitleBar; }

//==================================================================================================
class UILIBRARY_EXPORT TitleBar : public QWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget *parent = 0);
	~TitleBar();

public:
	void setPixmap(const QPixmap & pximap);
	void setTitleText(const QString & title);
	void setBtnVisible(ButtonType Type, bool visible);

private:
	Ui::TitleBar * ui;
};

//==================================================================================================