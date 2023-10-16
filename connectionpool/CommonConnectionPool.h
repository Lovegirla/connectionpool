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
	//给外部提供接口，从连接池中获取一个可用的空闲连接
	shared_ptr<Connection*> getConnection();
private:
	ConnectionPool();
	bool lodaConfigFile();
	//运行在独立的线程中，专门负责生产新连接
	void produceConnectionTask();
	//从配置中加载配置项
	string _ip; //mysql ip 
	unsigned short _port;	//3306
	string _username; 
	string _password;
	string _dbname;
	int _initSize; //连接池的初始连接量
	int _maxSize;//连接池的最大连接量
	int _maxIdleTime;//连接池的最大空闲时间
	int _connectionTimeout;//连接池的超时时间

	queue<Connection*> _connectionQue;//存储mysql连接池
	mutex  _queueMutex;
	atomic_int _connectionCnt;//记录连接所创建的connection连接的总数量
	condition_variable cv;//用于连接生产者和消费者通信的条件变量
};

