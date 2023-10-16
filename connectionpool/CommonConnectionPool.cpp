#include "CommonConnectionPool.h"
#define _CRT_SECURE_NO_WARNINGS
ConnectionPool* ConnectionPool::getConnectionPool() 
{
	static ConnectionPool pool;
	return &pool;
 }

bool ConnectionPool::lodaConfigFile()
{
	FILE* pf = fopen("mysql.ini","r");
	if (pf == nullptr) {
		LOG("mysql,ini file is not exist");
		return false;
	}
	while (!feof(pf))
	{
		char line[1024] = { 0 };
		fgets(line,1024,pf);
		string str = line;
		int idx = str.find('=', 0);
		if (idx = -1)
		{
			continue;
		}
		int endidx = str.find('\n',idx);
		string key = str.substr(0,idx);
		string value = str.substr(idx+1,endidx - idx -1);

		if (key == "ip") {
			_ip = value;
		}
		else if (key == "port") {
			_port = atoi(value.c_str());
		}
		else if (key == "username") {
			_username = value;
		}
		else if (key == "password") {
			_password = value;
		}
		else if (key == "initSize") {
			_initSize = _port = atoi(value.c_str());
		}
		else if (key == "maxSize") {
			_maxSize = atoi(value.c_str());
		}
		else if (key == "maxIdleTime") {
			_maxIdleTime = atoi(value.c_str());
		}
		else if (key == "connectionTimeOut") {
			_connectionTimeout = atoi(value.c_str());
		}
		else if (key == "dbname") {
			_dbname = value;
		}
	}
}

ConnectionPool::ConnectionPool()
{
	if (!lodaConfigFile())
		return;

	//创建初始数量的连接
	for (int i = 0; i < _initSize; ++i) {
		Connection* p = new Connection();
		p->connect(_ip,_port,_username,_password,_dbname);
		_connectionQue.push(p);
		_connectionCnt++;
	}
	//启动一个线程，作为连接的生产者
	thread produce(std::bind(&ConnectionPool::produceConnectionTask,this));
}

void ConnectionPool::produceConnectionTask() {
	for (;;)
	{
		unique_lock<mutex> lock(_queueMutex);
		while (!_connectionQue.empty())
		{
			cv.wait(lock);
			
		}
		if (_connectionCnt <_maxSize)
		{
			Connection* p = new Connection();
			p->connect(_ip, _port, _username, _password, _dbname);
			_connectionQue.push(p);
			_connectionCnt++;
		}
		cv.notify_all();
	}
}