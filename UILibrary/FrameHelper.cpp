#include "stdafx.h"
#include "AppAssiant.h"
#include "FrameHelper.h" 
#include "FrameHelperPrivate.h"

#include <QWidget>
#include <QPushButton>
#include <QPropertyAnimation>

//==================================================================================================
FrameHelper::FrameHelper(QWidget *parent, QWidget * title) : QObject(parent)
{
    d = new FrameHelperPrivate(parent, title);
}

FrameHelper::~FrameHelper()
{
	CHK_POINT_DESTORY(d);
}

void FrameHelper::setResizeMargins(int margin)
{
    setResizeMargins(QMargins(margin, margin, margin, margin));
}

void FrameHelper::setResizeMargins(const QMargins &margins)
{
    d->margins = margins;
}

void FrameHelper::setCloseButton(QAbstractButton *button, bool edgeResize)
{
	CHK_EXP_END(button == NULL);
    d->btnClose = button;
	connect(d->btnClose, SIGNAL(clicked(bool)), parent(), SLOT(close()));

    if(edgeResize) {
        d->addEdgeWidget(button);
    }
}

void FrameHelper::setMaxRestoreButton(QAbstractButton *button, bool edgeResize)
{
	CHK_EXP_END(button == NULL);
    d->btnMaxRestore = button;
	connect(d->btnMaxRestore, SIGNAL(clicked(bool)), d, SLOT(refreshMaximizedButton()));

    if(edgeResize) {
        d->addEdgeWidget(button);
    }
}

void FrameHelper::setMinimizedButton(QAbstractButton *button, bool edgeResize)
{
	CHK_EXP_END(button == NULL);
    d->btnMinimized = button;
	connect(d->btnMinimized, SIGNAL(clicked(bool)), parent(), SLOT(showMinimized()));

    if(edgeResize) {
        d->addEdgeWidget(button);
    }
}

void FrameHelper::addEdgeWidget(QWidget *widget)
{
	CHK_EXP_END(widget == NULL);
    d->addEdgeWidget(widget);
}

void FrameHelper::addDragWidget(QWidget *widget)
{
	CHK_EXP_END(widget == NULL);
    d->addDragWidget(widget);
}

void FrameHelper::convertMaxAndRestore()
{
	d->refreshMaximizedButton();
}

//==================================================================================================
