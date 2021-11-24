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

//gets the name of the col to edit in the db('what'),what to edit it to(newWhat),db name, table name in the db,the id of the user we want to perform the changes on
void EditDetailsInDb(const char* what, const char* newWhat, const char* dbNmae, const char* tableName, const char* id)
{
	char* errmsg = NULL;
	char query[500] = "";
	sqlite3* db;
	sqlite3_stmt* stmt;
	sprintf(query, "UPDATE %s SET %s = '%s' WHERE id = %s", tableName, what, newWhat, id);//prepeare the query with variables
	sqlite3_open(dbNmae, &db);//opens the db
	int rc = sqlite3_exec(db, query, NULL, NULL, &errmsg);//excute the query
	if (rc != SQLITE_OK)//if there was an error
		printf("Error: %s\n", errmsg);//print it
	sqlite3_close(db);
}

//gets the name of the col to get the info from in the db('what'),db name, table name in the db,the id of the user we want to take the info from
//return the information wanted
char* GetDetailsFromDb(const char* what, const char* dbName, const char* tableName, const char* id)
{
	char* errmsg = NULL;
	char query[500] = "";
	sqlite3* db;
	sqlite3_stmt* stmt;
	sprintf(query, "SELECT %s FROM %s where id = %s", what, tableName, id);//prepeare the query with variables
	sqlite3_open(dbName, &db);//open the db
	sqlite3_prepare_v2(db, query, -1, &stmt, 0);//execute the query
	const unsigned char* str = NULL;
	sqlite3_step(stmt);
	str = sqlite3_column_text(stmt, 0);//str is now the piece of info we looked for
	char* result = toString(str);
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return result;
}

//prints str from the index startIndex with a certain max num of comas in a row
//str will look like this: 12:00,12:30,13:00,13:30,14:00,14:30,......
void printByComa(int startIndex, const char* str, int maxNumOfComaInARow)
{
	int index = startIndex;//the index we start printing from
	int size = strlen(str);
	int numOfComas = 0;
	for (index; index < size; index++)
	{
		if (str[index] == ',') //if found a coma
		{
			numOfComas += 1;//indicate we found it
			if (numOfComas == maxNumOfComaInARow) //if we reached to the maximum number of comas in a row
			{
				printf("\n");//go down a row
				numOfComas = 0;//and start counting the num of comas in the row from 0
			}
			else//if still havent reached to the max num of comas in a row
				printf("%c", str[index]);//print the coma
		}

		else//if it isnt a coma
			printf("%c", str[index]);//print it
	}
	printf("\n");
}

//gets a string that contains values separated by comas and the starting place of the substring we want to delete from the string 'str'
//returns the string 'str' without the substring
//str will look like this: 12:00,12:30,13:00,13:30,14:00,14:30,......
char* deleteSubString(char* str, char* placeOfSubStr)
{
	int size = strlen(str);
	int strIndex = 0;//the index we run on in the 'str' string
	int resultIndex = 0;//the index we run on in the result string
	char result[MAXSIZE];//'str' without the appearance of the substring to delete
	for (strIndex; strIndex < size; strIndex++)//run on all str
	{
		if (placeOfSubStr == &str[strIndex])//if reached to the start of the subString to delete dont copy it
		{
			strIndex++;
			while (str[strIndex] != ',')//keep going untill the next coma
			{
				strIndex++;
			}
		}
		else//if it ist the subString to delete
		{
			result[resultIndex] = str[strIndex];//copy it to the result string
			resultIndex++;
		}
	}
	result[resultIndex] = '\0';
	return toString(result);
}