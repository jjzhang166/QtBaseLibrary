#pragma once
#include "AppGlobal.h"

#include <QObject>
#include <QPixmap>
#include <QPointer>
#include <QAbstractButton>

//==================================================================================================
QPixmap paintFontIcon(const QFont &font, const QString & text, const QColor &color, const QSize &iconSize);

 //==================================================================================================
class ButtonFontIcon : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QFont fontIcon READ iconFont WRITE setIconFont)
	Q_PROPERTY(QSize fontIconSize READ iconSize WRITE setIconSize)
	Q_PROPERTY(QString fontIconText READ iconFontText WRITE setIconFontText)
	Q_PROPERTY(QColor fontIconColor READ iconFontColor WRITE setIconFontColor)

public:
	ButtonFontIcon(QAbstractButton * btnbutton, QWidget * parent);
	~ButtonFontIcon();

public:
	QFont iconFont();
	void setIconFont(const QFont & font);

	QSize iconSize();
	void setIconSize(const QSize & size);

	QString iconFontText();
	void setIconFontText(const QString & text);

	QColor iconFontColor();
	void setIconFontColor(const QColor & color);

protected:
	void paintButtonIcon();

private:
	QFont fontIcon;
	QSize fontIconSize;
	QColor fontIconColor;
	QString fontIconText;
	QPointer<QAbstractButton> btnIconFont;
};

//==================================================================================================