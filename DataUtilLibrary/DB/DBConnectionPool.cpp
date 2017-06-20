#include "stdafx.h"
#include "DBConnectionPool.h"

#include <QStack>
#include <QSemaphore>


//==================================================================================================
class DBConnectionPoolPrivate
{
public:
	DBConnectionPoolPrivate();
	~DBConnectionPoolPrivate();

	//���ݿ���Ϣ
	QString strHostName;
	QString strDBName;
	QString strUserName;
	QString strPassword;
	QString strDBType;
	int iPort;

	bool bTestVaild;					//ȡ������ʱ��֤�����Ƿ���Ч�������������ܣ�
	int iMaxWaitTime;					//��ȡ�������ȴ�ʱ��
	int iMaxConnectionCount;			//���������
	QString strTestSql;					//���Է������ݿ�ʹ�õ�SQL

	QSemaphore * pSemaphore;					//�����ź���
	QStack<QString> usedConnectionNames;		//��ʹ�õ�����	
	QStack<QString> unUsedConnectionNames;		//δʹ�õ�����

	QMutex mutex;
	int iLastKey;						//����������ţ� ��֤�������ֲ��ظ�
};


//==================================================================================================
DBConnectionPoolPrivate::DBConnectionPoolPrivate()
{
	iPort = 0;
	iLastKey = 0;
	iMaxConnectionCount = 0;

	pSemaphore = new QSemaphore(iMaxConnectionCount);
}

DBConnectionPoolPrivate::~DBConnectionPoolPrivate()
{

}















//==================================================================================================




















//==================================================================================================
DBConnectionPool::DBConnectionPool()
{
}


DBConnectionPool::~DBConnectionPool()
{
}
