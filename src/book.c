/*
 * book.c
 *
 *  Created on: 2012-8-3
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manage.h"

int insert(Book *pbook) { //增加
	int ret = sqlite3_open("book.db3", &pdb);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "In insert: create or open db file failured!%s\n",
				sqlite3_errmsg(pdb));
		return 0;
	}

	char sql[128] = { '\0' };
	sprintf(sql, "insert into book values(null,'%s',%f,'%s','%s')",
			pbook->name, pbook->price, pbook->author, pbook->publish);
	ret = sqlite3_exec(pdb, sql, NULL, NULL, NULL);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "in insert:insert data failured!%s\n", sqlite3_errmsg(
				pdb));
		return 0;
	}
	sqlite3_close(pdb);
	return 1;
}

int update(Book *pbook, int id) { //修改
	int ret = sqlite3_open("book.db3", &pdb);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "create or open db failured!%s", sqlite3_errmsg(pdb));
		return 0;
	}

	char sql[128] = { '\0' };
	sprintf(
			sql,
			"update book set sno = %d name = '%s' price=%f author= '%s' publish = '%s' where sno = id",
			pbook->sno, pbook->name, pbook->price, pbook->author,
			pbook->publish);

	ret = sqlite3_exec(pdb, sql, NULL, NULL, NULL);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "update data is failured!%s\n", sqlite3_errmsg(pdb));
		return 0;
	}
	sqlite3_close(pdb);
	return 1;
}

int deleteById(int id) { //删除
	int ret = sqlite3_open("book.db3", &pdb);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "create or open file is failured!%s\n", sqlite3_errmsg(
				pdb));
		return 0;
	}

	char sql[128] = { '\0' };
	sprintf(sql, "delete * form book where sno = %d", id);
	ret = sqlite3_exec(pdb, sql, NULL, NULL, NULL);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "delete data was failured!%s\n", sqlite3_errmsg(pdb));
		return 0;
	}
	sqlite3_close(pdb);
	return 1;
}

int deleteAll() {
	int ret = sqlite3_open("book.db3", &pdb);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "create or open file was failured!%s\n",
				sqlite3_errmsg(pdb));
		return 0;
	}

	char sql[128] = { '\0' };
	sprintf(sql, "delete from book");
	ret = sqlite3_exec(pdb, sql, NULL, NULL, NULL);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "delete all the book.db3 is err!%s\n", sqlite3_errmsg(
				pdb));
		return 0;
	}
	sqlite3_close(pdb);
	return 1;
}

int findById(int id, Book *pbook) { //通过书的ID查找该书的信息
	int ret = sqlite3_open("book.db3", &pdb);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "create or open db file failured!%s\n", sqlite3_errmsg(
				pdb));
		return 0;
	}

	char sql[128] = { '\0' };
	sprintf(sql, "select * from book where sno = %d", id);
	sqlite3_stmt *stmt = NULL; //stmt为数据库中表的指针，指向表第一行的上一行
	ret = sqlite3_prepare(pdb, sql, -1, &stmt, NULL);//-1 VS strlen(sql)
	if (SQLITE_OK == ret) {
		printf("sqlite3_prepare data successful!\n");
	} else {
		fprintf(stderr, "prepare failured!%s\n", sqlite3_errmsg(pdb));
		return 0;
	}
	sqlite3_column_count(stmt); //统计表的列数
	int flag = sqlite3_step(stmt);
	if (SQLITE_ROW == flag) { //SQLITE_ROW代表那一行还有数据
		pbook->sno = id;
		int i = 0;
		strcpy(pbook->name, (char *) sqlite3_column_text(stmt, ++i)); //name在第一列所以填1
		pbook->price = sqlite3_column_double(stmt, ++i);
		strcpy(pbook->author, (char *) sqlite3_column_text(stmt, ++i));
		strcpy(pbook->publish, (char *) sqlite3_column_text(stmt, ++i));
	}
	sqlite3_finalize(stmt);
	sqlite3_close(pdb);
	return 1;
}

int findAll(Book* pbooks, int n) {
	int ret = sqlite3_open("book.db3", &pdb);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "create or open db file is failured!%s\n",
				sqlite3_errmsg(pdb));
		return -1;
	}

	char sql[128] = { '\0' };
	sprintf(sql, "select * from book");
	sqlite3_stmt *stmt = NULL;
	ret = sqlite3_prepare(pdb, sql, strlen(sql), &stmt, NULL);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "prepare data is failured!%s\n", sqlite3_errmsg(pdb));
		return -1;
	}

	sqlite3_column_count(stmt);
	int flag = sqlite3_step(stmt);
	int index = 0;
	while (SQLITE_ROW == flag && index < n) {
		int i = -1;
		pbooks[index].sno = sqlite3_column_int(stmt, ++i);
		strcpy(pbooks[index].name, (char *) sqlite3_column_text(stmt, ++i));
		pbooks[index].price = sqlite3_column_double(stmt, ++i);
		strcpy(pbooks[index].author, (char *) sqlite3_column_text(stmt, ++i));
		strcpy(pbooks[index].publish, (char *) sqlite3_column_text(stmt, ++i));
		++index;
		flag = sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(pdb);
	return index;
}

int login(Person *per, int t) {
	//printf("per->name=%s,per->passwd=%d\n", per->name, per->passwd);// just for testing....
	int ret = sqlite3_open("book.db3", &pdb);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "In userLogin create or open file was failured!%s\n",
				sqlite3_errmsg(pdb));
		return 0;
	}

	char sql[128] = { '\0' };
	if (1 == t) {
		sprintf(sql, "select * from User");
	} else if (2 == t) {
		sprintf(sql, "select * from Manager");
	}

	sqlite3_stmt *stmt = NULL;
	ret = sqlite3_prepare(pdb, sql, -1, &stmt, NULL);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "In userLogin prepare data is failured!%s\n",
				sqlite3_errmsg(pdb));
		return 0;
	}

	//sqlite3_column_count(stmt);
	while (SQLITE_ROW == sqlite3_step(stmt)) {
		int i = -1;
		char *sname = malloc(20);
		strcpy(sname, (char *) sqlite3_column_text(stmt, ++i));
		int spwd = sqlite3_column_int(stmt, ++i);
		if ((strncmp(per->name, sname, 20) == 0) && (spwd == per->passwd)) {//
		//	printf("sname=%s,spwd=%d\n", sname, spwd);  // just for testing.....
			sqlite3_finalize(stmt);    //very important!!!!!
			sqlite3_close(pdb);
			free(sname);
			return 1;
		} else {
			continue;
		}
	}
	sqlite3_finalize(stmt);
	sqlite3_close(pdb);
	return 0;
}


int user_insert(Person *per){
	int ret = sqlite3_open("book.db3", &pdb);
		if (SQLITE_OK != ret) {
			fprintf(stderr, "In user_insert: create or open db file failured!%s\n",
					sqlite3_errmsg(pdb));
			return 0;
		}

		char sql[128] = { '\0' };
		sprintf(sql, "insert into User values('%s',%d)",per->name,per->passwd);
		ret = sqlite3_exec(pdb, sql, NULL, NULL, NULL);
		if (SQLITE_OK != ret) {
			fprintf(stderr, "in insert:insert user failured!%s\n", sqlite3_errmsg(
					pdb));
			return 0;
		}
		sqlite3_close(pdb);
		return 1;
}
