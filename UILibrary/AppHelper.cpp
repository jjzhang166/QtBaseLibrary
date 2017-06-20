#include "stdafx.h"
#include "AppHelper.h"
#include "AppAssiant.h"
#include "FrameHelper.h"
#include "AppFontIcon.h"

#include <QFile>
#include <QPointer>
#include <QResource>
#include <QTranslator>
#include <QApplication>
#include <QFontDatabase>
#include <QScopedPointer>

//==================================================================================================
void regCustomFont(const QString & path)
{
	QFile res(path);
	if (!res.open(QIODevice::ReadOnly))
		return;

	QByteArray fontData(res.readAll());
	res.close();

	QFontDatabase::addApplicationFontFromData(fontData);
}

void regButtonIcon(QAbstractButton * button, QWidget * parent)
{
	QPointer<ButtonFontIcon> fontBtn = new ButtonFontIcon(button, parent);
}

void regWindowTitle(QWidget * widget, QAbstractButton * btnClose, QAbstractButton * btnMin, QAbstractButton * btnMaxRestore, QWidget * title)
{
	CHK_EXP_END(widget == NULL);

	while (widget->parent())
	{
		if (widget->parent()->inherits("QWidget"))
		{
			widget = qobject_cast<QWidget*>(widget->parent());
		}
		else
		{
			break;
		}
	}

	QPointer<FrameHelper> frameHelper = new FrameHelper(widget, title);

	frameHelper->addEdgeWidget(widget);
	frameHelper->addDragWidget(widget);
	frameHelper->setCloseButton(btnClose);
	frameHelper->setMinimizedButton(btnMin);
	frameHelper->setMaxRestoreButton(btnMaxRestore);

}


//==================================================================================================
void useChineseTrans(bool bUse)
{
	static QPointer<QTranslator> translator = 0;

	if (translator && !bUse)
	{
		qApp->removeTranslator(translator);
		translator = 0;
		return;
	}
	else if (!translator && bUse)
	{
		translator = new QTranslator();
		translator->load(":/lang/UILibrary_zh.qm");
		qApp->installTranslator(translator);
		return;
	}
}


void setStyleSheet(const QString & Path, bool Append)
{
	QFile skinFile(Path);
	if (skinFile.open(QFile::ReadOnly | QFile::Unbuffered))
	{
		if (Append)
		{
			QString styles = qApp->styleSheet() + QString(skinFile.readAll());
			qApp->setStyleSheet(styles);
		}
		else
		{
			qApp->setStyleSheet(skinFile.readAll());
		}
	}
	skinFile.close();
}



//==================================================================================================
