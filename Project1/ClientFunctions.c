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


