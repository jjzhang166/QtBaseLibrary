#pragma once

#include <QString>
#include <QVariant>

//标志位
enum COLUMN_FLAGS
{
	CF_NONE = 0x0000,
	CF_SHOW = 0x0001,
	CF_EDIT = 0x0002,
	CF_PRIMARY = 0x0004,
	CF_LEFT = 0x0010,
	CF_RIGHT = 0x0020,
	CF_CENTER = 0x0040,

	CF_COMBOBOX = 0x1000,					//编辑框类型COMBOBOX。
	CF_DATE = 0x2000,						//编辑时显示Date
	CF_SPINBOX = 0x4000,					//编辑时显示DOUBLESPINBOX
	CF_DOUBLESPINBOX = 0x8000,				//编辑时显示SPINBOX

	CF_EDIT_PRIMARY = 0x0006,
	CF_SHOW_EDIT = 0x0003,
	CF_SHOW_PRIMARY = 0x0005,
	CF_SHOW_EDIT_PRIMARY = 0x0007,

	CF_SHOW_LEFT = 0x0011,
	CF_SHOW_RIGHT = 0x0021,
	CF_SHOW_CENTER = 0x0041,

	CF_EDIT_COMBOBOX = 0x1003,				//编辑时显示Combox， 默认为Edit
	CF_EDIT_DATE = 0x2003,					//编辑时显示Date， 默认为Edit
	CF_EDIT_SPINBOX = 0x4003,				//编辑时显示SPINBOX， 默认为Edit
	CF_EDIT_DOUBLESPINBOX = 0x8003			//编辑时显示DoubleSPINBOX， 默认为Edit
};

struct ColumnInfo
{
	int Length;
	QVariant::Type Type;
	COLUMN_FLAGS Flag;
	QString Name;
	QString Alias;

	ColumnInfo()
	{
		Length = 0;
		Type = QVariant::Invalid;
		Flag = CF_NONE;
	}
};
