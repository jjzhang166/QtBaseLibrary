#pragma once
#include "AppGlobal.h"

#include <QFrame>
#include <QLabel>

//==================================================================================================
UILIBRARY_EXPORT void qMsgBoxError(const QString & Text, QWidget * Parent = NULL);
UILIBRARY_EXPORT void qMsgBoxAlert(const QString & Text, QWidget * Parent = NULL);
UILIBRARY_EXPORT void qMsgBoxInfo(const QString & Text, QWidget * Parent = NULL);
UILIBRARY_EXPORT void qMsgBoxOk(const QString & Text, QWidget * Parent = NULL);
UILIBRARY_EXPORT int qMsgBoxQuestion(const QString & Text, QWidget * Parent = NULL);
UILIBRARY_EXPORT int qMsgBoxQuestionTwo(const QString & Text, QWidget * Parent = NULL);

//==================================================================================================
UILIBRARY_EXPORT void updateStyle(QWidget * widget, const char * propertyName, const QVariant & value);
UILIBRARY_EXPORT void updateStyle(QWidget * widget, const QString & objectName);
UILIBRARY_EXPORT void updateStyle(QWidget * widget);

//==================================================================================================
UILIBRARY_EXPORT QWidget * MainFrame(QWidget * mainFrame = NULL);

//==================================================================================================
UILIBRARY_EXPORT void appTitle(const QString & title);
UILIBRARY_EXPORT void appIcon(const QString & icon);
UILIBRARY_EXPORT QString appTitle(void);
UILIBRARY_EXPORT QIcon appIcon(void);

//==================================================================================================
enum E_HINT_TYPE { HT_NONE , HT_INFO , HT_ALERT , HT_ERROR , HT_ACCEPT , HT_REJECT };
UILIBRARY_EXPORT void Hint(QFrame * frame, QLabel * label, E_HINT_TYPE hintType, const QString & text);
UILIBRARY_EXPORT void Hint(QLabel * label, E_HINT_TYPE hintType, const QString & text);


//==================================================================================================
UILIBRARY_EXPORT QColor getRGBColor(int color);
UILIBRARY_EXPORT int getCustomColor(const QColor & color);
UILIBRARY_EXPORT QString getRGBColor(const QColor & color);


//==================================================================================================
//将QList， QVector 转为 通过分隔符分割的QString
UILIBRARY_EXPORT QString listToString(const QVariantList & list, const QString & spilt);
