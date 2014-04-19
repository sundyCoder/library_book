/*
 * manage.h
 *
 *  Created on: 2012-8-4
 *      Author: root
 */

#ifndef MANAGE_H_
#define MANAGE_H_
#include "book.h"

typedef struct Person{
	char name[20];
	int passwd;
}Person;

int system_start();
int managerInsert();
int managerDeleteAll();
void manager_login();
void user_login();
int user_checkin();

int T_findById();
int T_findAll();

int update_passwd();           //修改用户密码
int delete_user();             //删除某一个用户
int find_all_user();           //查看所有用户的信息
int delete_all_user();				   //删除所有用户

int login(Person *per,int t);  //用户或者管理员登录
int user_insert(Person *per);  //普通用户注册
int user_count();

int delete_all_table_msg();
void system_exit();
#endif /* MANAGE_H_ */
