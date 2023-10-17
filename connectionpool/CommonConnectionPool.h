#pragma once
#include <string>
#include <queue>
#include "Connection.h"
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <functional>
#include <condition_variable>
using namespace std;
class ConnectionPool
{
public:
	static ConnectionPool* getConnectionPool();
	//���ⲿ�ṩ�ӿڣ������ӳ��л�ȡһ�����õĿ�������
	shared_ptr<Connection*> getConnection();
private:
	ConnectionPool();
	bool lodaConfigFile();
	//�����ڶ������߳��У�ר�Ÿ�������������
	void produceConnectionTask();
	//�������м���������
	string _ip; //mysql ip 
	unsigned short _port;	//3306
	string _username; 
	string _password;
	string _dbname;
	int _initSize; //���ӳصĳ�ʼ������
	int _maxSize;//���ӳص����������
	int _maxIdleTime;//���ӳص�������ʱ��
	int _connectionTimeout;//���ӳصĳ�ʱʱ��

	queue<Connection*> _connectionQue;//�洢mysql���ӳ�
	mutex  _queueMutex;
	atomic_int _connectionCnt;//��¼������������connection���ӵ�������
	condition_variable cv;//�������������ߺ�������ͨ�ŵ���������
};

