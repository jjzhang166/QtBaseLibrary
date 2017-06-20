#pragma once

#include <QString>
#include <QStringList>

#include "AppGlobal.h"
#include "AppStruct.h"
#include "PushButton.h"

class QWidget;
class QAbstractButton;

//注册函数
//==================================================================================================
//注册字体
UILIBRARY_EXPORT void regCustomFont(const QString & path);

//注册按钮， 使按钮可使用自定义图标字体（定义按钮图标使用ButtonFontIcon类
UILIBRARY_EXPORT void regButtonIcon(QAbstractButton * button, QWidget * parent);

//注册窗口函数自绘标题栏
UILIBRARY_EXPORT void regWindowTitle(QWidget * widget, QAbstractButton * btnClose = NULL,
	QAbstractButton * btnMin = NULL, QAbstractButton * btnMaxRestore = NULL, QWidget * title = NULL);

//==================================================================================================


//==================================================================================================
//设置翻译文件
UILIBRARY_EXPORT void useChineseTrans(bool bUse = true);

//设置皮肤[通过外部加载资源]
UILIBRARY_EXPORT void setStyleSheet(const QString & Path, bool Append = false);
