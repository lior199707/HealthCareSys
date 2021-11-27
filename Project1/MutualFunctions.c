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

//gets the full name from the user, and checks if the name is valid, a valid name consists of only letters
char* getName()
{
	char fullName[MAXSIZE];
	int nameValid = 0;//indicates if the name is valid
	char* name = NULL;
	puts("Please enter your full Name, a valid full name consists of only letters and spaces");
	getchar();
	gets(fullName);//gets the full name from the user
	nameValid = isNameValid(fullName);//checks if the name is valid
	while (!nameValid) //as long as the name isnt valid
	{
		puts("The name you entered is not valid");
		puts("Please enter your name again");
		//getchar();
		gets(fullName);//take it again from the user
		nameValid = isNameValid(fullName);
	}
	name = toString(fullName);
	return name;
}

// gets the id of the use, checks if the id is valid, a valid id consists of exactly 9 digits and the first degit cnat be 0
char* getId()
{
	char Id[MAXSIZE];
	int IdValid;//indicates if the id is valid
	char* id = NULL;
	puts("Please enter the ID, a valid id consists of 9 numbers and the first digit cant be 0");
	//getchar();
	gets(Id);//get the id from the user
	IdValid = isIdValid(Id);//check if id is valid
	while (!IdValid)//as long as not valid
	{
		puts("The ID you entered is not valid");
		puts("Please enter the ID again");
		//getchar();
		gets(Id);//get the id again from the user
		IdValid = isIdValid(Id);//andcheck if its valid
	}
	id = toString(Id);
	return id;
}

//gets the password of the user,and checks if its valid, a valid pass contains at least: 1 upper, 1 lowwer and 1 digit and has at least 6 chars
char* getPass()
{
	char password[MAXSIZE];
	int passValid;//indicates if the password is valid
	char* pass = NULL;
	puts("Please enter your password, a legal password contains:");
	puts("* At least 6 digits");
	puts("* At least one capital letter");
	puts("* At least one lowwer letter");
	puts("* At least one number");
	gets(password);//get teh password from the user
	passValid = isPassValid(password);
	while (!passValid)//as long as the password is not valid
	{
		puts("The password you entered is not valid");
		puts("Please enter your password again, according to the rules above");
		gets(password);//get the password again from the user
		passValid = isPassValid(password);//check if its valid
	}
	pass = toString(password);
	return pass;
}

//checks if a name is valid, a valid name contains only letters and spaces 
int isNameValid(char* name)
{
	int size = strlen(name);
	if (size == 0)//a name cant be empty
		return 0;
	for (int i = 0; i < size; i++) //run on all the name
		if (((name[i] < 'A') && (name[i] != ' ')) || (name[i] > 'z') || ((name[i] > 'Z') && (name[i] < 'a')))//if found a char that isnt a letter or a space
			return 0;//indicates its illegal
	return 1;//leggal
}


//checks id the id is valid, a valid id contains exactly 9 digits and the first digit cant be 0
int isIdValid(char* id)
{
	int size = strlen(id);
	if ((id[0] == '0') || (size != 9))//if the first letter is 0 or the number of digits isnt 9
		return 0;//indicate its illegal
	for (int i = 0; i < size; i++)//run on all the id
		if ((id[i] < '0') || (id[i] > '9'))//if there is a char that isnt a number
			return 0;//indicate its illegal
	return 1;//leggal
}

//checks if the pass is valid, a valid pass contains at least: 1 upper, 1 lowwer and 1 digit and has at least 6 chars
int isPassValid(char* pass)
{
	int numOfUpper = 0;//num of upper letters
	int numOfLowwer = 0;//num of lowwer letters
	int numOfDigits = 0;
	int minimumSize = 6;
	int size = strlen(pass);
	if (size < minimumSize)//if has less than 6 chars
		return 0;
	for (int i = 0; i < size; i++)//run on all the string and count the loower,upper and digits
	{
		if ((pass[i] >= 'a') && (pass[i] <= 'z'))//if its a lowwer letter
			numOfLowwer += 1;
		else if ((pass[i] >= 'A') && (pass[i] <= 'Z'))//if its an upper letter
			numOfUpper += 1;
		else if ((pass[i] >= '0') && (pass[i] <= '9'))//if its a digit
			numOfDigits += 1;
	}
	if ((numOfDigits >= 1) && (numOfLowwer >= 1) && (numOfUpper >= 1))//if the password is valid
		return 1;
	return 0;
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
	int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);//execute the query
	if (rc != SQLITE_OK)//if there are no registered users from type doc/client 
		return toString("tableIsEmpty");
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