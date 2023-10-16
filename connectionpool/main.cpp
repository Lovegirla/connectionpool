#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include "Connection.h"
int main() {
	Connection conn;
	char sql[1024] = {0};
	sprintf(sql, "insert into user(name,age,sex) values('%s','%d','%s')",
		"zhang san", 20, "male");
	conn.connect("127.0.0.1",3306,"root","root","chat");
	conn.update(sql);
	return 0;
}