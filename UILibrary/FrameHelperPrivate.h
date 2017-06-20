#pragma once
#include "AppGlobal.h"

#include <QObject>
#include <QPoint>
#include <QPointer>
#include <QMargins>
#include <QScopedPointer>
#include <QAbstractNativeEventFilter>

class QEvent;
class QTimer;
class QWidget;
class QMouseEvent;
class QAbstractButton;

//==================================================================================================
class FrameHelperPrivate : public QObject, QAbstractNativeEventFilter
{
    Q_OBJECT
public:
	QPointer<QWidget> parent;
	QPointer<QWidget> title;
	QPointer<QAbstractButton> btnClose;
	QPointer<QAbstractButton> btnMinimized;
	QPointer<QAbstractButton> btnMaxRestore;

    QList<QPointer<QWidget>> dragWidgets;
    QList<QPointer<QWidget>> edgeWidgets;
    QMargins margins;

    FrameHelperPrivate(QWidget *parent, QWidget * title);
	~FrameHelperPrivate();

    void addEdgeWidget(QWidget *widget);
    void addDragWidget(QWidget *widget);

public slots:
	void refreshMaximizedButton();

protected:
    enum MoveDirection {
        None = 0,
        Left,
        Right,
        LeftTop,
        RightTop,
        LeftBottom,
        RightBottom,
        Top,
        Bottom,
    };

    enum MoveOperation {
        MoveNone = 0,
        MoveResize,
        MoveDrag,
    };

protected:
	bool checkTitleButton(int x, int y);
    bool eventFilter(QObject *obj, QEvent *event);
	bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);

private slots:
    void checkPos();
    void resize(const QPoint &cursorPos);

private:
    bool isMaximized;
    bool isMousePressed;

	QScopedPointer<QTimer> timer;
    QPoint dragBasePoint;
	MoveDirection direction;
	MoveOperation operation;
};

//==================================================================================================
