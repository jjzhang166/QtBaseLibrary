#include "stdafx.h"
#include "PushButton.h"
#include "AppFontIcon.h"

#include <QMenu>
#include <QLabel>

//==================================================================================================
PushButton::PushButton(QWidget *parent)
	: QPushButton(parent)
{
	setFlat(true);
}

PushButton::~PushButton()
{

}

//==================================================================================================
void PushButton::setMenu(QMenu* menu)
{
#ifdef Q_OS_LINUX
    connect(menu, SIGNAL(aboutToHide()), this, SLOT(recover()));
#endif

	QPushButton::setMenu(menu);
}

QFont PushButton::iconFont()
{
	return fontIcon;
}

void PushButton::setIconFont(const QFont & font)
{
	fontIcon = font;
	paintButtonIcon();
}

QString PushButton::iconFontText()
{
	return fontIconText;
}

void PushButton::setIconFontText(const QString & text)
{
	fontIconText = text;
	paintButtonIcon();
}

QColor PushButton::iconFontColor()
{
	return fontIconColor;
}

void PushButton::setIconFontColor(const QColor & color)
{
	fontIconColor = color;
	paintButtonIcon();
}

void PushButton::paintButtonIcon()
{
	if (fontIconText.isEmpty())
		return;

	QPixmap img = paintFontIcon(fontIcon, fontIconText, fontIconColor, iconSize());
	setIcon(img);
}

//==================================================================================================
void PushButton::recover()
{
#ifdef Q_OS_LINUX
	bool flag = this->rect().contains(this->mapFromGlobal(QCursor::pos()));
	this->setAttribute(Qt::WA_UnderMouse, flag);
	this->update();
#endif
}

//==================================================================================================
