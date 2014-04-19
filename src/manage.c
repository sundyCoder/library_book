/*
 * manage.c
 *
 *  Created on: 2012-8-4
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "book.h"
#include "manage.h"

int system_start() {
	printf("\n============欢迎光临JChen的图书管理系统============\n");
	while (1) {
		printf("===============================================\n");
		printf("1 管理员登录请按		 1\n");
		printf("2 普通用户登录请按	 2\n");
		printf("3 普通用户注册请按	 3\n");
		printf("4 获取当前用户数请按4\n");
		printf("5 退出系统请按			 5\n");

		printf("===============================================\n");
		printf("清输入你的选项>:");
		int choice;
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			manager_login();
			break;
		case 2:
			user_login();
			break;
		case 3:
			user_checkin();
			break;
		case 4:
			user_count();
			break;
		case 5:
			system_exit();
			break;
		default:
			break;
		}
	}
	return 0;
}

//管理员登录验证
void manager_login() {
	printf("==========尊敬的管理员请登录============\n");

	Person per = { { '\0' }, 0 };
	setbuf(stdin, NULL);
	printf("1 清输入你的管理帐号>");
	//scanf("%s", per.name);   //遇到空格也会结束
	//read(STDIN_FILENO,per.name,sizeof(per.name)-2);//将\r\n也读入了，出错
	gets(per.name); //可以,但是说很危险，用什么比较好了?
	//fgets(per.name,sizeof(per.name),stdin);  //将\r\n也读入了，出错
	printf("2 清输入你的管理密码>");
	scanf("%d", &per.passwd);
	int ret = login(&per, 2); //在login()中1代表普通用户登录,2代表管理员登录
	if (ret) {
		printf("恭喜你！%s成功登录!\n", per.name);
		while (1) {
			printf("请选择以下信息:\n");
			printf("===============================================\n");
			printf("1 具体查询书籍请按			1.\n");
			printf("2 查询全部书籍请按  			2.\n");
			printf("3 插入书籍信息请按			3.\n");
			printf("4 删除全部书籍请按			4.\n");
			printf("5 踢出不合法用户请按		5.\n");
			printf("6 查看所有普通用户请按	6.\n");
			printf("7 删除所有普通用户请按	7.\n");
			printf("8 清理整个数据库表请按	8.\n");
			printf("9 退出系统请按					9.\n");
			printf("10 返回请按						0.\n");
			printf("===============================================\n");
			printf("请输入你的选项>:");
			int choice;
			scanf("%d", &choice);
			switch (choice) {
			case 1:
				T_findById();
				break;
			case 2:
				T_findAll();
				break;
			case 3:
				managerInsert();
				break;
			case 4:
				managerDeleteAll();
				break;
			case 5:
				delete_user();
				break;
			case 6:
				find_all_user();
				break;
			case 7:
				delete_all_user();
				break;
			case 8:
				delete_all_table_msg();
				break;
			case 9:
				system_exit();
				break;
			default:
				break;
			}
			if (choice == 0)
				break; //为返回上一级而设定的
		}
	} else {
		printf("很抱歉!用户名或者密码输入错误!请重新输入!\n");
		//goto l1;
	}
	return;
}

void user_login() {
	printf("==========欢迎普通用户登录============\n");

	Person per;
	//setbuf(stdin, NULL);
	printf("1 请输入你的用户名>");
	//scanf("%s", per.name);
	setbuf(stdin, NULL);
	gets(per.name);
	printf("2 清输入你的密码>");
	scanf("%d", &per.passwd);
	int ret = login(&per, 1);
	if (ret) {
		printf("恭喜你！%s成功登录!\n", per.name);
		while (1) {
			printf("请选择以下信息:\n");
			printf("===============================================\n");
			printf("1 具体书籍信息查询请按		1.\n");
			printf("2 查询全部书籍信息请按		2.\n");
			printf("3 修改密码请按						3.\n");
			printf("4 退出系统请按						4.\n");
			printf("5 返回上一级请按					0.\n");
			printf("===============================================\n");
			printf("请输入你的选项>:");
			int choice;
			scanf("%d", &choice);
			switch (choice) {
			case 1:
				T_findById();
				break;
			case 2:
				T_findAll();
				break;
			case 3:
				update_passwd();
				break;
			case 4:
				system_exit();
				break;
			default:
				break;
			}
			if (0 == choice)
				return;
		}
	} else {
		printf("很抱歉!用户名或者密码输入错误!请重新输入!\n");
		//goto l1;
	}
	return;
}

int T_findById() {
	Book book;
	printf("\t清输入所查书籍的ID号>");
	scanf("%d", &book.sno);
	int ret = findById(book.sno, &book);
	if (ret) {
		printf("\n查询结果是: ID号:%d\t书名:%s\t价格:%lf\t作者:%s\t出版社:%s\n", book.sno,
				book.name, book.price, book.author, book.publish);
	}
	return 0;
}

int T_findAll() {
	printf("清输入一共查多少本书>");
	int N = 0;
	scanf("%d", &N);
	Book books[N];
	int ret = findAll(books, N);
	//printf("ret=%d\n", ret);
	if (ret != -1) {
		int i = 0;
		for (i = 0; i < ret; ++i) {
			printf("\n查询结果是: ID号:%d\t书名:%s\t价格:%lf\t作者:%s\t出版社:%s\n",
					books[i].sno, books[i].name, books[i].price,
					books[i].author, books[i].publish);
		}
	}
	return 0;
}

int managerInsert() {
	Book book;// = { 0, "C++ program", 50, "JChen", "sun" };
	printf("请输入书籍ID号>");
	scanf("%d", &book.sno);
	printf("请输入书籍名字>");
	//scanf("%s", book.name);
	setbuf(stdin, NULL);
	gets(book.name);
	printf("请输入书籍价格>");
	scanf("%f", &book.price);
	printf("请输入书籍作者>");
	//scanf("%s", book.author);
	setbuf(stdin, NULL);
	gets(book.author);
	printf("请输入书籍出版社>");
	//scanf("%s", book.publish);
	setbuf(stdin, NULL);
	gets(book.publish);
	printf("你插入的书籍为(%d,%s,%f,%s,%s)", book.sno, book.name, book.price,
			book.author, book.publish);
	int ret = insert(&book);
	if (ret) {
		printf("insert book successful!\n");
		return 1;
	} else {
		printf("insert book failured!\n");
		return 0;
	}
}

int managerDeleteAll() {
	int ret = deleteAll();
	if (ret) {
		printf("delete all the table is successful!\n");
		return 1;
	} else {
		printf("delete all the table is failured!\n");
	}
	return 0;
}

int user_checkin() {
	printf("==========欢迎注册本图书管理系统============\n");
	Person per = { { '\0' }, 0 };
	printf("清输入用户名>");
	//scanf("%s", per.name);
	setbuf(stdin, NULL);
	gets(per.name);
	printf("清输入密码>");
	scanf("%d", &per.passwd);
	int ret = user_insert(&per);
	if (ret && per.name != NULL) {
		printf("尊敬的%s用户,恭喜你成功注册!\n", per.name);
		return 1;
	} else {
		printf("用户名和密码不能为空!\n");
		return 0;
	}
}

int delete_user() {
	printf("清输入你要踢出的用户名>");
	Person per = { { '\0' }, 0 };
	//scanf("%s", per.name);
	gets(per.name);

	int ret = sqlite3_open("book.db3", &pdb);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "create or open book.db3 was failured!%s\n",
				sqlite3_errmsg(pdb));
		return 0;
	}

	char sql[128] = { '\0' };
	sprintf(sql, "delete from User where name='%s'", per.name);
	ret = sqlite3_exec(pdb, sql, NULL, NULL, NULL);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "delete one user was failured!%s\n",
				sqlite3_errmsg(pdb));
		return 0;
	}
	sqlite3_close(pdb);
	printf("成功删除普通用户%s的所有信息\n", per.name);
	return 1;
}

int delete_all_user() {
	int ret = sqlite3_open("book.db3", &pdb);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "create or open book.db3 was failured!%s\n",
				sqlite3_errmsg(pdb));
		return 0;
	}

	char sql[128] = { '\0' };
	sprintf(sql, "delete from user");
	ret = sqlite3_exec(pdb, sql, NULL, NULL, NULL);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "delete all user was failured!%s\n",
				sqlite3_errmsg(pdb));
		return 0;
	}

	sqlite3_close(pdb);
	printf("成功删除所有用户的信息!\n");
	return 1;
}
int find_all_user() {
	int ret = sqlite3_open("book.db3", &pdb);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "create or open book.db3 was failured!%s\n",
				sqlite3_errmsg(pdb));
		return 0;
	}

	char sql[128] = { '\0' };
	sprintf(sql, "select * from user");
	sqlite3_stmt *stmt = NULL;
	ret = sqlite3_prepare(pdb, sql, strlen(sql), &stmt, NULL);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "In find_all_user: prepare was failured!%s\n",
				sqlite3_errmsg(pdb));
		return 0;
	}

	//sqlite3_column_count(stmt);
	printf("请输入你要查询的人数>");
	int N = 0;
	scanf("%d", &N);
	//Person per[N];

	int index = 0;
	while (SQLITE_ROW == sqlite3_step(stmt) && index < N) {
		int i = -1;
		printf("用户名:%s\t", sqlite3_column_text(stmt, ++i));
		printf("密码:%d", sqlite3_column_int(stmt, ++i));
		++index;
		printf("\n");
	}

	sqlite3_finalize(stmt);
	sqlite3_close(pdb);
	return 1;
}

int delete_all_table_msg() {
	int ret = sqlite3_open("book.db3", &pdb);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "create or open book.db3 was failured!%s\n",
				sqlite3_errmsg(pdb));
		return 0;
	}

	char sql[128] = { '\0' };
	sprintf(sql, "delete from user;delete from book");
	ret = sqlite3_exec(pdb, sql, NULL, NULL, NULL);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "delete all user was failured!%s\n",
				sqlite3_errmsg(pdb));
		return 0;
	}
	sqlite3_close(pdb);
	printf("成功删除所有表的信息!\n");
	return 1;
}

int update_passwd() {
	Person per;
	printf("请输入用户名>");
	//scanf("%s",per.name);
	setbuf(stdin, NULL);
	gets(per.name);
	printf("清输入更改的密码>");
	scanf("%d", &per.passwd);

	int ret = sqlite3_open("book.db3", &pdb);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "create or open book.db3 was failured!%s\n",
				sqlite3_errmsg(pdb));
		return 0;
	}

	char sql[128] = { '\0' };
	sprintf(sql, "update user set passwd=%d where name = '%s'", per.passwd,
			per.name);
	ret = sqlite3_exec(pdb, sql, NULL, NULL, NULL);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "In update_passwd:exec was failured@!%s\n",
				sqlite3_errmsg(pdb));
		return 0;
	}
	sqlite3_close(pdb);
	printf("修改成功!\n");
	return 1;
}

int user_count(){
	int ret = sqlite3_open("book.db3",&pdb);
	if(SQLITE_OK!=ret){
		fprintf(stderr,"In user_count:create or open book.db3 was failured!%s\n",sqlite3_errmsg(pdb));
		return 0;
	}

	sqlite3_stmt *stmt = NULL;
	char sql[128] = {'\0'};
	sprintf(sql,"select * from user");
	ret = sqlite3_prepare(pdb, sql, strlen(sql), &stmt, NULL);
		if (SQLITE_OK != ret) {
			fprintf(stderr, "In user_count: prepare was failured!%s\n",
					sqlite3_errmsg(pdb));
			return 0;
		}

	int count = 0;
	while(SQLITE_ROW == sqlite3_step(stmt)){
		count++;
	}
	printf("当前系统的用户数量为%d\n",count);
	sqlite3_close(pdb);
	return 1;
}

void system_exit() {
	printf("欢迎下次再来!\n");
	sleep(1);
	system("clear");
	//break;
	exit(0);
}

