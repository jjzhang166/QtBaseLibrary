#include "stdafx.h"
#include "TestLibs.h"

#include <QTime>
#include <QTableView>
#include <QPushButton>
#include <QtWidgets/QApplication>

#include "RecordTableModel.h"
#include "SysConfig.h"
#include "ImportDataUtilLibrary.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTime time1;
	
	DataBaseUtil::initDBConfig("config.json");

	/*
	DBUtilArguments dbArg;
	dbArg.strSQL = "select MAXPOST, ROUND(MAXMINOR,0) MAXMINOR, DEFECTTYPE, \
		MAXVAL1, LENGTH, DEFECTCLASS, TBCE, SPEEDATMAXVAL, POSTEDSPD from T_DEFECTRAW A where(A.DEFECTFAMILY IN(0, 1, 2, 3, 4, 6, 8, 9, 10, 12, 13)) AND(A.DEFECTCLASS = 1) AND VALID != 'N' \
		ORDER BY A.MAXPOST, A.MAXMINOR";
	QRecord rd = DataBaseUtil::selectRecord(dbArg);

	QRecord trans;
	ColumnInfo info;
	info.Name = "colname";
	trans.addCol(info);
	info.Name = "name_en";
	trans.addCol(info);
	info.Name = "name_cn";
	trans.addCol(info);

	QVariantList r1, r2, r3, r4, r5;
	r1.append("DefectType"); r1.append("CROSSLEVEL"); r1.append(QStringLiteral("ˮƽ"));
	r2.append("DefectType"); r2.append("CURVATURE RATE"); r2.append("AABBCC");
	r3.append("DefectType"); r3.append("GAUGE RATE"); r3.append("EEFFGG");
	r4.append("DefectType"); r4.append("L SURFACE"); r4.append("123214");
	r5.append("DefectType"); r5.append("LCEM"); r5.append("HHIIKK");
	trans.addRow(r1);
	trans.addRow(r2);
	trans.addRow(r3);
	trans.addRow(r4);
	trans.addRow(r5);


	QTableView view;

	QRecordTableModel model;
	model.setRecord(rd);
	model.setTransRecord(trans);
	
	for (int col = 0; col < 9; ++col)
	{
		model.setCol(col, CF_SHOW);
	}
	
	view.setModel(&model);

	view.show();
	*/

	DBUtilArguments dbArg;
	dbArg.strSQL = "select top 40000 * FROM defects";

	QTime time;
	time.start();
	QRecord rd = DataBaseUtil::selectRecord(dbArg);
	qDebug() << rd.getRows();

	qDebug() << endl << time.elapsed() / 1000.0 << "s";

	time1.start();
	qDebug() << DataBaseUtil::selectMaps(dbArg);
	qDebug() << endl << time1.elapsed() / 1000.0 << "s" << endl;
	TestLibs w;
	w.show();
	

	return a.exec();
}
