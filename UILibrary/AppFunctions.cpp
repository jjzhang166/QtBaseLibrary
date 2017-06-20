#include "stdafx.h"
#include "AppAssiant.h"
#include "MessageBox.h"
#include "AppFunctions.h"

#include <QStyle>
#include <QWidget>
#include <QMessageBox>

//==================================================================================================
QWidget * MainFrame(QWidget * frame /*= NULL*/)
{
	static QWidget * mainFramePtr = NULL;
	CHK_EXP_RET(frame == NULL, mainFramePtr);
	return (mainFramePtr = frame);
}

//==================================================================================================
void qMsgBoxError(const QString & Text, QWidget * Parent /*= NULL*/)
{
	CHK_EXP_RUN(Parent == NULL, Parent = MainFrame());
	MessageBox box(Parent);
	box.setContent(MT_ERROR, Text);
    box.exec();
}

void qMsgBoxAlert(const QString & Text, QWidget * Parent /*= NULL*/)
{
	CHK_EXP_RUN(Parent == NULL, Parent = MainFrame());
	MessageBox box(Parent);
	box.setContent(MT_WARNING, Text);
    box.exec();
}

void qMsgBoxInfo(const QString & Text, QWidget * Parent /*= NULL*/)
{
	CHK_EXP_RUN(Parent == NULL, Parent = MainFrame());
	MessageBox box(Parent);
	box.setContent(MT_INFO, Text);
    box.exec();
}

void qMsgBoxOk(const QString & Text, QWidget * Parent /*= NULL*/)
{
	CHK_EXP_RUN(Parent == NULL, Parent = MainFrame());
	MessageBox box(Parent);
	box.setContent(MT_OK, Text);
    box.exec();
}

int qMsgBoxQuestion(const QString & Text, QWidget * Parent /*= NULL*/)
{
	CHK_EXP_RUN(Parent == NULL, Parent = MainFrame());
	MessageBox box(Parent);
	box.setContent(MT_QUESTION, Text);
	return box.exec();
}

int qMsgBoxQuestionTwo(const QString & Text, QWidget * Parent /*= NULL*/)
{
	CHK_EXP_RUN(Parent == NULL, Parent = MainFrame());
	MessageBox box(Parent);
	box.setContent(MT_QUESTION2, Text);
	return box.exec();
}

//==================================================================================================
static QString AppTitle(const QString & title)
{
	static QString appTitle("");
	CHK_EXP_RET(title.isEmpty(), appTitle);

	appTitle = title;
	return appTitle;
}

void appTitle(const QString & title){ AppTitle(title); }
QString appTitle(){ return AppTitle(QString()); }

//==================================================================================================
static QIcon AppIcon(const QString & icon)
{
	static QString appIcon("");
    CHK_EXP_RET(icon.isEmpty(), QIcon(appIcon));

	appIcon = icon;
	return QIcon(appIcon);
}

void appIcon(const QString & icon){ AppIcon(icon); }
QIcon appIcon(){ return AppIcon(QString()); }

//==================================================================================================
void updateStyle(QWidget * widget, const char * propertyName, const QVariant & value)
{
	widget->setProperty(propertyName, value);
	updateStyle(widget);
}

void updateStyle(QWidget * widget, const QString & objectName)
{
	widget->setObjectName(objectName);
	updateStyle(widget);
}

void updateStyle(QWidget * widget)
{
	widget->style()->unpolish(widget);
	widget->style()->polish(widget);
	widget->update();
}

//==================================================================================================
void Hint( QFrame * frame , QLabel * label , E_HINT_TYPE hintType , const QString & text )
{
	Hint( label , hintType, text );

	frame->disconnect(SIGNAL(timeout()));
	frame->setVisible( true );

	//if ( autoHide ) QTimer::singleShot( 10000, frame, SLOT(hide()) );
}

void Hint( QLabel * label , E_HINT_TYPE hintType , const QString & text )
{
	CHK_EXP_END( label == NULL );

	static const QString hintNames[ ] = 
	{
		QString("hintNone")  , QString("hintInfo")   , QString("hintAlert") ,
		QString("hintError") , QString("hintAccept") , QString("hintReject")
	};

	//hintType = ace_range( HT_NONE , HT_REJECT , hintType );
	label->setVisible(true);
	label->setObjectName( hintNames[ hintType ] );
	label->setText( text );
	updateStyle( label );
}

//==================================================================================================
QColor getRGBColor(int color)
{
	int blue = color & 255;
	int green = color >> 8 & 255;
	int red = color >> 16 & 255;
	return QColor(blue, green, red);
}

int getCustomColor(const QColor & color)
{
	int blue = color.blue();
	int green = color.green();
	int red = color.red();
	return blue << 16 | green << 8 | red;
}

QString getRGBColor(const QColor & color)
{
	int blue = color.blue();
	int green = color.green();
	int red = color.red();
	return QString("rgb(%1, %2, %3)").arg(red).arg(green).arg(blue);
}

//==================================================================================================