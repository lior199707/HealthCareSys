#define _CRT_SECURE_NO_WARNINGS
#include "ClientFunctions.h"
#include "MutualFunctions.h"
#include "DoctorFunctions.h"


//login/register menu presented to the client 
void clientMenu()
{
	int exitFlag = 1;//indicates if the user wants to leave the program
	int userChoice;//the choice from the menu
	while (exitFlag)
	{
		puts("----------------------------------------------------");
		puts("User Menu:\n");
		puts("1. Login");
		puts("2. Register");
		puts("3. Back to main menu");
		puts("----------------------------------------------------");
		puts("If you already have an account please login otherwise register");
		puts("Please choose your prefered option");
		scanf_s("%d", &userChoice);
		while ((userChoice < 1) || (userChoice > 3)) //if user choice is not a valid option let him choose again
		{
			puts("The option you choose isnt listed above, please try again");
			puts("Please choose your prefered option");
			scanf_s("%d", &userChoice);
		}
		switch (userChoice)
		{
		case 1: //login menu for user
		{
			clientLogIN();
			break;
		}
		case 2: //user register
		{
			clientRegister();
			break;
		}
		case 3: //return to main menu
		{
			puts("Back to main menu");
			exitFlag = 0;
			break;
		}
		default:
		{break; }
		}
	}
}

//the menu presented with actions for the client after he logged in
void clientOptionsMenu(char* fullName, char* id)
{
	int exitFlag = 1;//indicate if the client wants to exit
	int userChoice;//the client choice from the menu
	printf("Hello %s\n", fullName);
	while (exitFlag)
	{
		puts("----------------------------------------------------");
		puts("1. Book an appointment with a doctor");
		puts("2. Cancel an appointment");
		puts("3. Watch all my future appointments");
		puts("4. Log out");
		puts("----------------------------------------------------");
		scanf_s("%d", &userChoice);
		while (userChoice < 1 || userChoice > 4)//if the client choice isnt valid
		{
			puts("The option you choose isnt listed above, please try again");
			puts("Please choose your prefered option");
			scanf_s("%d", &userChoice);//ask him to choose again
		}
		switch (userChoice)
		{
		case 1://Book an appointment with a doctor
		{
			bookAppointment(id);
			break;
		}
		case 2://Cancel an appointment
		{
			cancelAppointment(id);
			break;
		}
		case 3://Watch all my future appointments
		{
			watchFutureAppointments(id);
			break;
		}
		case 4://Log out
		{
			exitFlag = 0;
			puts("Log out");
			break;
		}
		default:
			break;
		}
	}
}

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

//after a client booked an appointment adds all the detail of the appointment to his future appointments list
void editFutureAppointmentsAfterBooking(char* clientId, char* docId, char* docName, char* docGender, char* date, char* time)
{
	char buffer[MAXSIZE] = "";
	char* currFutureAppointmentsList = GetDetailsFromDb("future_appointments", "clientDb.db", "clientInfo", clientId);//gets the future appointments list of the client
	if (strcmp(currFutureAppointmentsList, "NULL"))//if there are future appointments already
		strcpy(buffer, currFutureAppointmentsList);//copy them to the buffer
	strcat(buffer, docId);
	strcat(buffer, " ");
	strcat(buffer, docName);
	strcat(buffer, " ");
	strcat(buffer, docGender);
	strcat(buffer, " ");
	strcat(buffer, date);
	strcat(buffer, " ");
	strcat(buffer, time);
	strcat(buffer, ",");
	//the buffer now looks like this: "docId:.... DocNmae:.... docGender:... date:... time:..., DocId:........."
	char* newFutureAppointmetnsStr = toString(buffer);
	EditDetailsInDb("future_appointments", newFutureAppointmetnsStr, "clientDb.db", "clientInfo", clientId);
	free(newFutureAppointmetnsStr);
	free(currFutureAppointmentsList);
}

//gives the client the option to cancel a future meeting, if the client has no future meetings, prints it and returns to the options menu
//oterwise prints the list of future meetings and asks for the id of the doctor the date and time of the meeting to cancel
//if the detials provided dont much any of the future appointments prints a proper message to the screen and returns
//to the options menu ,otherwise, deletes the meeting from the future appointments list and marks the time slot as 
//available in the doctors available and not times list
void cancelAppointment(char* id)
{
	char chosenDate[6];
	char chosenTime[MAXSIZE];
	char* allAppointmentsList = GetDetailsFromDb("future_appointments", "clientDb.db", "clientInfo", id);//gets the future appointments string of the client
	if (!strcmp(allAppointmentsList, "NULL"))//if the client has no future appointments
	{
		puts("You have no future appointments yet");
		return;
	}
	puts("\nYou are about to enter the doctor ID, the date and the hour of the appointment you want to cancel\n");
	puts("Here is your future appointments list:");
	printByComa(0, allAppointmentsList, 1);//print all the client's future appointments to the screen
	//creater a temporary list from the future appointments list in this format: DocId1*Date1*Time1,DocId2*Date2*Time2
	char* tempAllAppointmentsList = createTempAppointmentsList(allAppointmentsList);
	puts(tempAllAppointmentsList);
	getchar();
	puts("Please enter the doctor ID");
	char* docId = getId();//gets the doctor id of the appointment to cancel from the client
	puts("\nPlease enter the day of the date");
	int date = chooseDate();//gets the date of the appointment to cancel
	snprintf(chosenDate, 6, "%d/02", date);
	puts("Please enter the time");
	getchar();
	gets(chosenTime);//gets the time of the appointmnet to cancel
	while (!isChosenTimeValid(chosenTime, chosenTime))//checks if the time is valid by his pattern(xw:qw)
	{
		gets(chosenTime);
	}
	//creates a temporary string from the client input in this format: docId*date*time
	char* tempAppointmentToCancle = createTempAppointmentToCancle(docId, chosenDate, chosenTime);
	int numOfComasPassedUntillFoundMatch = numOfComasForAppointmentCancelation(tempAllAppointmentsList, tempAppointmentToCancle);//finds the place of the appointment to cancel in the future appointments string by the amount of comas has passed untill saw it
	if (numOfComasPassedUntillFoundMatch == -1)//if there was no such appointment
	{
		puts("The details you entered are not legal so the program returned you to the options menu\n");
		return;//retunrn to the client options menu
	}
	char* newAllAppointmentsList = createNewAllAppointmentsListByComa(allAppointmentsList, numOfComasPassedUntillFoundMatch);//remove the appointment to cancel from the future appointments list
	//update newAllAppointmentsList to the client future_appointments column
	char* colNameInDocDb = colNameInDocTableByDate(date);
	editAvailableAndNotStrAfterCancelation(colNameInDocDb, docId, chosenTime);//adds the time to the doctor available time list and removes it from the not available times list
	if (!strcmp(newAllAppointmentsList, ""))//if after the deletion there are no future appointments	
		EditDetailsInDb("future_appointments", "NULL", "clientDb.db", "clientInfo", id);//update the string to NULL in the client db
	else//if there are still future appointments
		EditDetailsInDb("future_appointments", newAllAppointmentsList, "clientDb.db", "clientInfo", id);//update the future appointments list to the new one after the deletion
	free(allAppointmentsList);
	free(tempAllAppointmentsList);
	free(docId);
	free(tempAppointmentToCancle);
	free(newAllAppointmentsList);
}

//prints to the screen all the client future appointments
void watchFutureAppointments(char* id)
{
	char* allAppointmentsList = GetDetailsFromDb("future_appointments", "clientDb.db", "clientInfo", id);//gets the future appointmentms list by the id of the client
	puts("Future appointments:");
	if (!strcmp(allAppointmentsList, "NULL"))//if there are no future appointments
		puts("You have no future meetings");
	else//if there are future appointments
		printByComa(0, allAppointmentsList, 1);//print them
}

//gets the medical field chosen by the user, prints to the screen all the doctors from the chosen medical field
//and asks the user to choose a doctor by entering his id
char* chooseDocIdByField(char* medicalField)
{
	char* docIdList = getListOfDoctorsByMedicalField(medicalField);//prints to the screen the list of doctor from the chosen medical feild and returns a list of all their ID's
	puts("\nYou are about to enter the ID of the doctor you want to book an appointment with");
	getchar();
	char* id = getId();//gets the id of the doctor the client wants to schedule an appointment with
	char* ret = strstr(docIdList, id);//check if the id provided by the client exists in the doctors id list
	while (ret == NULL) //if it doesnt exists
	{
		puts("The id you choose doesnt exist in the list above");
		id = getId();//get the id again
		ret = strstr(docIdList, id);//and check if it exists in the ID's list again
	}
	free(docIdList);
	return toString(id);
}

//returns the AllAppointmentsList of the client without the appointment he wants to cancel
//numOfComaToDel: this parameter tells after how many comas ,from the tart of the AllAppointmentsList,
//the appointment to cancel show, and when the number of commas passed equals to the number of coma to delete
//we dont copy from the  all appointments list to teh buffer
char* createNewAllAppointmentsListByComa(char* AllAppointmentsList, int numOfComaToDel)//////////////////////////////////
{
	int size = strlen(AllAppointmentsList);
	char buffer[MAXSIZE];
	int bufferIndex = 0;//buffer writing index
	int numOfComaPassed = 0;//how many comas passed from the start of the word
	for (int i = 0; i < size; i++)//run on the allAppoinmentsList 
	{
		if (AllAppointmentsList[i] == ',') //if we see a coma
		{
			numOfComaPassed++;
		}
		if (numOfComaPassed == numOfComaToDel)//if we reached to the appointment to cancel
			continue;//dont opy to buffer
		else//if its another appointment
		{
			if (bufferIndex == 0 && AllAppointmentsList[i] == ',')//prevents the aapearence of ',' in the start of the new allAppoinmentsList 
				continue;
			buffer[bufferIndex] = AllAppointmentsList[i];//copy it to the buffer
			bufferIndex++;
		}

	}
	buffer[bufferIndex] = '\0';
	if (!strcmp(buffer, ""))
		return toString("");
	return toString(buffer);
}

//gets the tempAllAppointmentsList and tempAppointmentToCancel, returns the number of comas we passed untill reached to
//the tempAppointmentToCancel in the tempAllAppointmentsList.(it will be the same number of comas we should pass
//in order to find the appointment to cancel in the real allAppointmentsList)
//if didnt found a match returns -1 indicates the client didnt provide us with accurate information
int numOfComasForAppointmentCancelation(char* tempAllAppointmentsList, char* tempAppointmentToCancel)
{
	char buffer[MAXSIZE] = "";
	int numOfComasPassed = 0;
	int size = strlen(tempAllAppointmentsList);
	int bufferIndex = 0;//the next index in buffer to write to
	for (int i = 0; i < size; i++) //run on all the tempAllAppointmentsList
	{
		if (tempAllAppointmentsList[i] == ',') //if found a coma
		{
			//means that buffer now contains the information about a certain appointment
			if (!strcmp(buffer, tempAppointmentToCancel))//check if the appointment in the buffer matches the appointment to cancel
				return numOfComasPassed;//if true return the number of comas we saw before
			numOfComasPassed++;//if the buufer differs from the appointment to cancel
			memset(buffer, '\0', MAXSIZE);//empty the buffer
			bufferIndex = 0;//set the place to write in buffer to 0
		}
		else //id it isnt a coma
		{
			buffer[bufferIndex] = tempAllAppointmentsList[i];//add it to the buffer
			bufferIndex++;
		}
	}
	return -1;//if didnt found an appointments that matches the client appointment to cancel
}

