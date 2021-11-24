#define _CRT_SECURE_NO_WARNINGS
#include "MutualFunctions.h"
#include "ClientFunctions.h"


//this menu lets the user choose wether he is a client or a doctor
void startMenu()
{
	char docPass[] = DOCTORPASSWORD;
	int exitFlag = 1;//indicates if the user wants to leave the program
	int userChoice;//the choice from the menu
	while (exitFlag)
	{
		puts("----------------------------------------------------");
		puts("1. User menu");
		puts("2. Doctors menu");
		puts("3. exit");
		puts("----------------------------------------------------");
		puts("Please choose your prefered option");
		scanf_s("%d", &userChoice);
		while ((userChoice < 1) || (userChoice > 3)) //if user choice is not a valide option let him choose again
		{
			puts("The option you choose isnt listed above, please try again");
			puts("Please choose your prefered option");
			scanf_s("%d", &userChoice);
		}
		switch (userChoice)
		{
		case 1:  //users menu
		{
			clientMenu();
			break;
		}
		case 2: //doctors menu
		{
			char pass[MAXSIZE];//the password entered by the user
			int numOfTries = 1;//current num of tries
			int maxNumOfTries = 3;
			getchar();
			puts("Please enter the doctors security code");
			gets(pass);//gets the password from the user
			int isEqual = strcmp(pass, docPass);
			//as long as the user gets the password wrong it lets him try another time up to maximum of 3 times
			while (isEqual)
			{
				if (numOfTries >= maxNumOfTries)//if the user has passed the tries limit
				{
					numOfTries += 1;
					break;//go back to the main menu  
				}
				//if he didnt pass the tries limit yet
				puts("The security code isnt correct,please try again");
				printf("Remaining: %d tries\n", maxNumOfTries - numOfTries);
				gets(pass);//get the password again
				isEqual = strcmp(pass, docPass);//check if it matches the doctor password in the system
				numOfTries += 1;
			}
			if (numOfTries <= maxNumOfTries)//if the user didnt pass the tries limit 
				doctorMenu();//open the doctors login/register menu
			break;
		}
		case 3://exit the program
		{
			puts("You chose 3, Goodebye");
			exitFlag = 0;
			break;
		}
		default:
		{break; }
		}
	}
}




//gets a char[] and return a dynamic allocation of the char[] it gets
char* toString(const char* str)
{
	char* result = NULL;//the final string to return
	result = (char*)malloc(strlen(str) + 1);
	if (!result)
	{
		puts("error converting to string");
		return "";
	}
	strcpy(result, str);
	return result;
}

//used for user login, checks if the password and id provided by the user exists in the db, 
//if exists return his full name, otherwise returns an empty string
char* detailsExistsInDb(const char* dbName, const char* id, const char* password)
{
	char* errmsg = NULL;
	char query[500] = "";
	char* tableName = "";
	sqlite3* db;
	sqlite3_stmt* stmt;
	if (!strcmp(dbName, "doctorDb.db"))//if the data base is the doctor db
		tableName = "doctorInfo";//the name of the table is doctorInfo
	else//if its the cleint db
		tableName = "clientInfo";//the name of the table is clientInfo
	sprintf(query, "SELECT id,password,full_name FROM %s", tableName);//prepare the query with variables
	sqlite3_open(dbName, &db);//open the wanted db
	sqlite3_prepare_v2(db, query, -1, &stmt, 0);//execute the query
	const unsigned char* currId = NULL;
	const unsigned char* currPassword = NULL;
	const unsigned char* currFullName = NULL;
	while (sqlite3_step(stmt) != SQLITE_DONE)//run on all the database line by line
	{
		currId = sqlite3_column_text(stmt, 0);
		currPassword = sqlite3_column_text(stmt, 1);
		currFullName = sqlite3_column_text(stmt, 2);
		if (!strcmp(currId, id) && !strcmp(currPassword, password))//check if the pass and id match to the ones that the user provided
		{
			//if found a match 
			char* fullName = toString(currFullName);
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return fullName;//return his full name
		}
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return toString("");//otherwise return empty string
}

