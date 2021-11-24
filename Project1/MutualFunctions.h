#pragma once
#ifndef MUTUALFUNCTIONS_H
#define MUTUALFUNCTIONS_H
#define _CRT_SECURE_NO_WARNINGS
#define DOCTORPASSWORD "0123456789"
#define MAXSIZE 1000
#include "sqlite3.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 

void startMenu();//this menu lets the user choose wether he is a client or a doctor

char* toString(const char* str);//returns a dynamical allocation of a char[] 

char* detailsExistsInDb(const char* dbName, const char* id, const char* password);//checks if the id and password provided by the user exists in the db, used for log in
void EditDetailsInDb(const char* what, const char* newWhat, const char* dbNmae, const char* tableName, const char* id);//gets what to edit, to what to edit it and where to edit it and edits it in the db
char* GetDetailsFromDb(const char* what, const char* dbName, const char* tableName, const char* id);//gets what to get from the db and where to get it from and return a string of it
void printByComa(int startIndex, const char* str, int maxNumOfComaInARow);//prints 'str' from 'startIndex' in regars to how many comas should a line contain
char* deleteSubString(char* str, char* placeOfSubStr);//gets the place of the subString to delete in 'str' and returns str without said subString
#endif // !MUTUALFUNCTIONS_H
