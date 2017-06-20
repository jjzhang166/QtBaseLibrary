#include "stdafx.h"
#include "ToolButton.h"
#include "AppFontIcon.h"

//==================================================================================================
ToolButton::ToolButton(QWidget *parent)
	: QToolButton(parent)
{
}

ToolButton::~ToolButton()
{
}

//==================================================================================================
QFont ToolButton::iconFont()
{
	return fontIcon;
}

void ToolButton::setIconFont(const QFont & font)
{
	fontIcon = font;
	paintButtonIcon();
}

QString ToolButton::iconFontText()
{
	return fontIconText;
}

void ToolButton::setIconFontText(const QString & text)
{
	fontIconText = text;
	paintButtonIcon();
}

QColor ToolButton::iconFontColor()
{
	return fontIconColor;
}

void ToolButton::setIconFontColor(const QColor & color)
{
	fontIconColor = color;
	paintButtonIcon();
}

void ToolButton::paintButtonIcon()
{
	if (fontIconText.isEmpty())
		return;

	QPixmap img = paintFontIcon(fontIcon, fontIconText, fontIconColor, iconSize());
	setIcon(img);
}

//==================================================================================================