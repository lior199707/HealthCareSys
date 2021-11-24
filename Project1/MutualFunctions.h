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
#endif // !MUTUALFUNCTIONS_H
