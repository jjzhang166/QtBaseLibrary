#pragma once

#include "AppGlobal.h"
#include <QObject>
#include <QMargins>

class QWidget;
class QAbstractButton;
class FrameHelperPrivate;
//==================================================================================================

class FrameHelper : public QObject
{
    Q_OBJECT
public:
    explicit FrameHelper(QWidget *parent, QWidget * title = 0);
	~FrameHelper();


public:
    void setResizeMargins(int margin);
    void setResizeMargins(const QMargins &margins);

	void setCloseButton(QAbstractButton *button, bool edgeResize = true);
	void setMinimizedButton(QAbstractButton *button, bool edgeResize = true);
	void setMaxRestoreButton(QAbstractButton *button, bool edgeResize = true);

    void addEdgeWidget(QWidget *widget);
    void addDragWidget(QWidget *widget);

	void convertMaxAndRestore();

private:
    FrameHelperPrivate *d;
};

//==================================================================================================
