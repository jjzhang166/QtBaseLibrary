#pragma once

#include <QString>
#include <QStringList>

#include "AppGlobal.h"
#include "AppStruct.h"
#include "PushButton.h"

class QWidget;
class QAbstractButton;

//ע�ắ��
//==================================================================================================
//ע������
UILIBRARY_EXPORT void regCustomFont(const QString & path);

//ע�ᰴť�� ʹ��ť��ʹ���Զ���ͼ�����壨���尴ťͼ��ʹ��ButtonFontIcon��
UILIBRARY_EXPORT void regButtonIcon(QAbstractButton * button, QWidget * parent);

//ע�ᴰ�ں����Ի������
UILIBRARY_EXPORT void regWindowTitle(QWidget * widget, QAbstractButton * btnClose = NULL,
	QAbstractButton * btnMin = NULL, QAbstractButton * btnMaxRestore = NULL, QWidget * title = NULL);

//==================================================================================================


//==================================================================================================
//���÷����ļ�
UILIBRARY_EXPORT void useChineseTrans(bool bUse = true);

//����Ƥ��[ͨ���ⲿ������Դ]
UILIBRARY_EXPORT void setStyleSheet(const QString & Path, bool Append = false);
