#include "stdafx.h"
#include "AppAssiant.h"
#include "MessageBox.h"
#include "ui_MessageBox.h"

#include "FrameHelper.h"
#include "AppStruct.h"
#include "AppHelper.h"
#include "AppFontIcon.h"


//==================================================================================================
MessageBox::MessageBox(QWidget *parent)
	: QDialog(parent), ui(new Ui::MessageBox)
{
	ui->setupUi(this);
	pFrameHelper = NULL;
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	
	initDialog();
	initIcon();
}

MessageBox::~MessageBox()
{
	CHK_POINT_DESTORY(pFrameHelper);
	delete ui;
}

//==================================================================================================
void MessageBox::setContent(MessageType type, const QString & text)
{
	Type = type;
	ui->text->setText(text);
	initIcon();
}

//==================================================================================================
void MessageBox::initIcon()
{
	ui->btnNo->setVisible(false);
	ui->btnCancel->setVisible(false);
	switch (Type)
	{
	case MT_OK:
		ui->btnYes->setText(tr("Ok"));
		ui->labTitle->setText(tr("Info Message"));
		ui->icon->setProperty("msgtype", "ok");
		break;

	case MT_ERROR:
		ui->btnYes->setText(tr("Ok"));
		ui->labTitle->setText(tr("Error Message"));
		ui->icon->setProperty("msgtype", "error");
		break;

	case MT_WARNING:
		ui->btnYes->setText(tr("Ok"));
		ui->labTitle->setText(tr("Warn Message"));
		ui->icon->setProperty("msgtype", "warning");
		break;

	case MT_QUESTION:
		ui->btnYes->setText(tr("Yes"));
		ui->btnYes->setVisible(true);
		ui->btnNo->setVisible(true);
		ui->btnCancel->setVisible(true);
		ui->labTitle->setText(tr("Question Message"));
		ui->icon->setProperty("msgtype", "question");
		break;

	case MT_QUESTION2:
		ui->btnYes->setText(tr("Yes"));
		ui->btnYes->setVisible(true);
		ui->btnNo->setVisible(true);
		ui->labTitle->setText(tr("Question Message"));
		ui->icon->setProperty("msgtype", "question");
		break;

	case MT_INFO:
	default:
		ui->btnYes->setText(tr("Ok"));
		ui->labTitle->setText(tr("Kindly Reminder"));
		ui->icon->setProperty("msgtype", "info");
		break;
	}

}

void MessageBox::initDialog()
{
	Type = MT_INFO;
	resize(QSize(200, 130));
	ui->text->setWordWrap(true);

	pFrameHelper = new FrameHelper(this);
	pFrameHelper->addEdgeWidget(this);
	pFrameHelper->addDragWidget(this);
	pFrameHelper->setCloseButton(ui->btnClose);
}

//==================================================================================================
void MessageBox::on_btnYes_clicked()
{
	done(RET_YES);
}

void MessageBox::on_btnNo_clicked()
{
	done(RET_NO);
}

void MessageBox::on_btnCancel_clicked()
{
	done(RET_CANCEL);
}
