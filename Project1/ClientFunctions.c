#define _CRT_SECURE_NO_WARNINGS
#include "ClientFunctions.h"
#include "MutualFunctions.h"
#include "DoctorFunctions.h"

//gets the client info and adds him to the database
void clientRegister()
{
	char* fullName = getName();//gets the client full name
	char* id = getId();//gets the client id
	char* password = getPass();//gets the client password
	addClientToDb(id, password, fullName);//adds him to the db
	free(fullName);
	free(id);
	free(password);
}

//add the information: id,password,full name to the client db
void addClientToDb(char* id, char* pass, char* fullName)
{
	char* errmsg = NULL;
	char query[MAXSIZE] = "";
	sqlite3* db;
	sqlite3_stmt* stmt;
	sqlite3_open("clientDb.db", &db);//opent the client db
	int rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS clientInfo(id TEXT UNIQUE,password TEXT, full_name Text, future_appointments TEXT DEFAULT 'NULL');", NULL, NULL, errmsg);//if its the first client to join the sb create the table
	if (rc != SQLITE_OK)//if there was a problem when the table was created
		printf("Error: %s\n", errmsg);//print the problem
	sprintf(query, "INSERT INTO clientInfo (id,password,full_name) VALUES ('%s', '%s', '%s');", id, pass, fullName);//prepeare the query with the information of the client
	rc = sqlite3_exec(db, query, NULL, NULL, &errmsg);//execute the query, adds the client to the db
	if (rc != SQLITE_OK)//if there was a problem while trying to add the client to the db
		printf("Error: %s\n", errmsg);//print it to the screen
	sqlite3_close(db);
}

//asks the client to choose a date for the appointment, prints to the screen a list of the doctors blocked dates so the client wont choose them
int chooseDateForAppointmnent(docId)
{
	char* blockedDatesList = GetDetailsFromDb("blocked_dates", "doctorDb.db", "doctorInfo", docId);//gets the blocked dates list of the doctor by his id
	puts("\nPlease choose the day for the appointment that doesnt appear in the doctor's blocked dates list\n");
	printAllBlockedDates(blockedDatesList);//prints the blocked dates list ot the screen
	int date = chooseDate();//asks the client to choose a date
	//check if this date appears in the blocked dates list
	while (isDateAppearInBlockDateList(blockedDatesList, date))//ass long it appears in the blockedl list
	{
		puts("The date you chose is blocked, please try again");
		date = chooseDate();//asks for amother date from the client
	}
	free(blockedDatesList);
	return date;
}
//gets the id and password from the client and checks if it exists in the data base, the client has maximum 
//3 tries to succeed providing his login details if he succeeds he will move to the client options menu otherwise he
//will return to the login/register menu
void clientLogIN()
{
	char password[MAXSIZE];
	char* fullName = "";
	char id[MAXSIZE];
	int remainingTries = 3, returnToClientMenuFlag = 0;
	getchar();
	puts("Please enter your Id:");
	gets(id);//get the id from the user
	puts("Please enter your password");
	gets(password);//get the password from the user
	fullName = detailsExistsInDb("clientDb.db", id, password);//return the full name of the client if the id and passwors exists in db  otherwise returns an empty string
	while (!strcmp(fullName, ""))//if the details as provided by the user doesnt exist in the db
	{
		remainingTries -= 1;
		if (remainingTries == 0)//if the client has already tried 3 times and failed
		{
			returnToClientMenuFlag = 1;//indicate he should return to the ligin/register menu
			break;
		}
		printf("There is a problem with the details you provided\nYou have %d tries left\n", remainingTries);
		puts("Please enter your Id again:");
		gets(id);//get the id again
		puts("Please enter your password again");
		gets(password);//get the password again
		fullName = detailsExistsInDb("clientDb.db", id, password);//check if exists in the db
	}
	if (!returnToClientMenuFlag)//means the password and id the client provided exist in the data base 
		clientOptionsMenu(fullName, id);
}
//asks the client to choose the medical field for his meeting than prints a list of all the doctors from the chosen medical field
//after he chooses a doctor a list of the doctor's blocked dates will be printed to the screen and the client will be asked
//to enter a date for the meeting, after he chooses the date a list of all the available hours on the chosen date will be
//printed to the screen and finally the client will choose the time he wants.
//the final step is to add the chosen meeting to the client's list of future appointments and to update the doctor list of booked appointment
//on this date.
void bookAppointment(char* id)
{
	char* dateBuffer[6];
	char* medicalField = getMedicalField();//gets the wanted medical field from the client
	char* docId = chooseDocIdByField(medicalField);//prints all the doctors from the chosen medical field in the db and returnd the ID of the chossen doctor
	char* docName = GetDetailsFromDb("full_name", "doctorDb.db", "doctorInfo", docId);//returns the doctor's name by his id
	char* docGender = GetDetailsFromDb("gender", "doctorDb.db", "doctorInfo", docId);//returns the doctor's gender by his id
	int date = chooseDateForAppointmnent(docId);//print a list of all the blocked dates and asks the client to choose a date that isnt blocked
	char* colNameInDocDb = colNameInDocTableByDate(date);//returns the date as appears in the doctor db columns
	snprintf(dateBuffer, 6, "%d/02", date);
	char* time = chooseTimeForAppointment(docId, dateBuffer, colNameInDocDb);//prints all the available times from the said date and asks the user to chose a time
	if (time == NULL)//if there are no available times on this date
	{
		free(medicalField);
		free(docId);
		free(docName);
		free(docGender);
		return;
	}
	editAvailableAndNotStrAfterBooking(colNameInDocDb, docId, time);//removes the time from the available times list and adds it to the boooked list of the doctor
	editFutureAppointmentsAfterBooking(id, docId, docName, docGender, dateBuffer, time);//adds the docID,full name,gender,date and time to the client's list of future appointments
	free(medicalField);
	free(docId);
	free(docName);
	free(docGender);
	free(time);
}

