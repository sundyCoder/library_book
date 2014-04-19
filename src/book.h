/*
 * book.h
 *
 *  Created on: 2012-8-3
 *      Author: root
 */

#ifndef BOOK_H_
#define BOOK_H_
#include <sqlite3.h>
sqlite3 *pdb;

typedef struct Book{
	int sno;
	char name[20];
	float price;
	char author[20];
	char publish[30];
}Book;



int insert(Book *pbook);   //增加
int update(Book *pbook,int id); //修改
int delete(int id);        //删除
int deleteAll();						  //删除所以
int findById(int id,Book *pbook);//通过书的ID查找该书的信息
int findAll(Book* pbooks,int n);

#endif /* BOOK_H_ */






