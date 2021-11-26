#include "DoctorFunctions.h"
#include "MutualFunctions.h"



//doctor register/login menu
void doctorMenu()
{
	int exitFlag = 1;//indicates if the user wants to leave the program
	int userChoice;//the choice from the menu
	while (exitFlag)
	{
		puts("----------------------------------------------------");
		puts("Doctors Menu:\n");
		puts("1. Login");
		puts("2. Register");
		puts("3. Back to main menu");
		puts("----------------------------------------------------");
		puts("If you already have an account please login otherwise register");
		puts("Please choose your prefered option");
		scanf_s("%d", &userChoice);
		while ((userChoice < 1) || (userChoice > 3)) //if user choice is not a valid option let him choose again
		{
			puts("The option you chose isnt listed above, please try again");
			puts("Please choose your prefered option");
			scanf_s("%d", &userChoice);
		}
		switch (userChoice)
		{
		case 1: //login menu for doctor
		{
			doctorLogIn();
			break;
		}
		case 2: //doctor register
		{
			doctorRegister();
			break;
		}
		case 3: //return to main menu
		{
			exitFlag = 0;
			break;
		}
		default:
		{break; }
		}
	}
}

//the menu presented to the doctor after he logged in
void doctorOptionsMenu(char* fullName, char* id)
{
	int exitFlag = 1;//indicate the doc wants to exit the menu
	int userChoice;//the doc choice
	printf("Hello doctor %s\n", fullName);
	free(fullName);
	while (exitFlag)
	{
		puts("----------------------------------------------------");
		puts("1. Watch all meeting by date");
		puts("2. Block a date for appointments");
		puts("3. Edit profile");
		puts("4. open blocked date for appointments");
		puts("5. Watch all blocked appointments");
		puts("6. Log out");
		puts("----------------------------------------------------");
		scanf_s("%d", &userChoice);
		while (userChoice < 1 || userChoice > 6)//if the doc choses an invalid option
		{
			puts("The option you choose isnt listed above, please try again");
			puts("Please choose your prefered option");
			scanf_s("%d", &userChoice);
		}
		switch (userChoice)
		{
		case 1://Watch all meeting by date
		{
			watchMeetingsByDate(id);
			break;
		}
		case 2://Block a date for appointments
		{
			tryBlockingDate(id);
			break;
		}
		case 3://Edit profile
		{
			EditProfileMenu(id);
			break;
		}
		case 4://open blocked date for appointments
		{
			openBlockedDate(id);
			break;
		}
		case 5://Watch all blocked appointments
		{
			char* blockedDatesStr = GetDetailsFromDb("blocked_dates", "doctorDb.db", "doctorInfo", id);
			printAllBlockedDates(blockedDatesStr);
			free(blockedDatesStr);
			break;
		}
		case 6://Log out
		{
			exitFlag = 0;
			puts("Loged out");
			break;
		}
		default:
			break;
		}
	}
}


//this menu presents the doctor options for editing his profile, a doctor can edit his: full name, title,medical field,gender
void EditProfileMenu(char* id)
{
	int exitFlag = 1;
	int userChoice;
	char* newThing = NULL;
	while (exitFlag)
	{
		puts("----------------------------------------------------");
		puts("1. Edit full name");
		puts("2. Edit title");
		puts("3. Edit medical field");
		puts("4. Edit gender");
		puts("5. Return to doctor options menu");
		puts("----------------------------------------------------");
		scanf_s("%d", &userChoice);//gets the doctor choice
		while (userChoice < 1 || userChoice > 5)//if the choice isnt valid
		{
			puts("The option you choose isnt listed above, please try again");
			puts("Please choose your prefered option");
			scanf_s("%d", &userChoice);//get it again
		}
		switch (userChoice)
		{
		case 1://Edit full name
		{
			newThing = getName();//get the ned name
			EditDetailsInDb("full_name", newThing, "doctorDb.db", "doctorInfo", id);
			break;
		}
		case 2://Edit title
		{
			newThing = getTitle();//get the new title
			EditDetailsInDb("title", newThing, "doctorDb.db", "doctorInfo", id);
			break;
		}
		case 3://Edit medical field
		{
			newThing = getMedicalField();//get the new medical field
			EditDetailsInDb("medical_field", newThing, "doctorDb.db", "doctorInfo", id);
			break;
		}
		case 4://Edit gender
		{
			getchar();
			char c = getGender();//get the new gender
			if (c == 'M')
				EditDetailsInDb("gender", "Male", "doctorDb.db", "doctorInfo", id);
			else
				EditDetailsInDb("gender", "Female", "doctorDb.db", "doctorInfo", id);
			break;
		}
		case 5://exit
		{
			exitFlag = 0;
			break;
		}
		default:
			break;
		}
	}
	free(newThing);
}


//gets all the information of a doctor and adds it to the data base
void doctorRegister()
{
	char* fullName = getName();//gets the name
	char* id = getId();//gets the id
	char* password = getPass();//gets the password 
	char gender = getGender();//gets the gender
	char* title = getTitle();//gets the title
	char* medicalField = getMedicalField();//gets the medical field
	char* listOfHours = getListOfTurns();//gets the list of appointment hours
	addDoctorToDb(id, password, fullName, medicalField, title, gender, listOfHours);//adds all the information to the db
	free(fullName);
	free(id);
	free(password);
	free(title);
	free(medicalField);
	free(listOfHours);
}

//gets the doctor id and password and checks if exists in the db
void doctorLogIn()
{
	char password[MAXSIZE];
	char* fullName = "";
	char id[MAXSIZE];
	int remainingTries = 3, returnToDocMenuFlag = 0;
	getchar();
	puts("Please enter your Id:");
	gets(id);//gets the doctor id
	puts("Please enter your password");
	gets(password);//gets the doctor password
	fullName = detailsExistsInDb("doctorDb.db", id, password);//if id and pass exist in db return the name of the doctor else return ""
	while (!strcmp(fullName, ""))//if the details doesnt exist in the db, let the doctor to try inserting his id and pass again untill maximum 3 times
	{
		remainingTries -= 1;
		if (remainingTries == 0)//if there are no more tries left
		{
			returnToDocMenuFlag = 1;//indicate we should return to the login/regiter menu
			break;
		}
		printf("There is a problem with the details you provided\nYou have %d tries left\n", remainingTries);
		puts("Please enter your Id again:");
		gets(id);
		puts("Please enter your password again");
		gets(password);
		fullName = detailsExistsInDb("doctorDb.db", id, password);//if exists return the name of the doctor
	}
	if (!returnToDocMenuFlag)//means the password and id the doctor provided exist in the data base 
		doctorOptionsMenu(fullName, id);
	else //else return to the doc register/login menu
		free(fullName);

}

//gets the id of the doctor who is logged in to the system and ask him to choose a date, than prints to the screen the 
//list of appointments the doctor have on the desired date
void watchMeetingsByDate(char* id)
{
	int dayChoice = chooseDate();//get the day from the doctor
	char* columnName = colNameInDocTableByDate(dayChoice);//returns the column name in the chart of doctors that matches the desired date
	char* errmsg = NULL;
	char query[500] = "";
	sqlite3* db;
	sqlite3_stmt* stmt;
	sprintf(query, "SELECT %s FROM doctorInfo where id = %s", columnName, id);//prepare query with wanted variables
	sqlite3_open("doctorDb.db", &db);//open the doctor db for reading
	sqlite3_prepare_v2(db, query, -1, &stmt, 0);//execute the query
	const unsigned char* availableAndNot = NULL;
	sqlite3_step(stmt);
	availableAndNot = sqlite3_column_text(stmt, 0);//gets the string of the available and not available time on the desired date from the doctors chart
	printf("The list of appointments on %d/02:\n", dayChoice);
	if (!strcmp(availableAndNot, "Blocked"))//if day is blocked
		puts("You blocked said day to appointments");
	else //if day is not blocked
	{
		char* bookedStr = getBookedAppointmentsList(availableAndNot);//free in the end free(booked)
		if (!strcmp(bookedStr, "taken:"))//if nothing appears after 'taken:', means there are no appointments on this day
			puts("there are no appointments");
		else//if there are appointments booked by clients
			printByComa(6, bookedStr, 3);//print them
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

//returns the gender, M for male and F for female
char getGender()
{
	char gender;
	puts("Plese enter your gender, use only upper letters, enter: 'M' for male or 'F' for female");
	scanf_s("%c", &gender);
	while ((gender != 'M') && (gender != 'F'))
	{
		puts("The gender you entered is not valid, please try again");
		getchar();
		scanf_s("%c", &gender);
	}
	getchar();
	return gender;
}

//returns the chosen title by the doctor, a valid title must be one of the following:Mr.,Ms.,Doc.,Doctor
char* getTitle()
{
	char title[MAXSIZE];
	int titleValid = 0;
	char* resultTitle = NULL;
	puts("Please enter your title, a legal title should be one of follows(dont forget to add a period in the end):");
	puts("* Mr.");
	puts("* Ms.");
	puts("* Doc.");
	puts("* Doctor");
	gets(title);//gets the title from the user
	titleValid = isTitleValid(title);//check if the title is valid
	while (!titleValid)//as long as the title is not valid
	{
		puts("The title you entered is not valid");
		puts("Please enter your title again, according to the rules above");
		gets(title);//get the title again from the user
		titleValid = isTitleValid(title);//and check if it valid again
	}
	resultTitle = toString(title);
	return resultTitle;
}

//returns the chosen medical field by the doctor, a valid medical field must be one of follows:
//"Allergy and immunology", "Dermatology", "Family medicine", "Neurology","Pathology", "Psychiatry", "Surgery", "Urology";
char* getMedicalField()
{
	const char alergy[] = "Allergy and immunology", dermatology[] = "Dermatology", family[] = "Family medicine", neurology[] = "Neurology";
	const char pathology[] = "Pathology", psychiatry[] = "Psychiatry", surgery[] = "Surgery", urology[] = "Urology";
	int doctorChoice;
	puts("Please choose the number that represents your medical field");
	printf("1.%s        2.%s\n", alergy, dermatology);
	printf("3.%s               4.%s\n", family, neurology);
	printf("5.%s                     6.%s\n", pathology, psychiatry);
	printf("7.%s                       8.%s\n", surgery, urology);
	scanf_s("%d", &doctorChoice);//get the  choice from the doctor
	while ((doctorChoice < 1) || (doctorChoice > 8))//as long as the doctor didnt choose a valid option
	{
		puts("The number you chose doesnt exist, please try again");
		scanf_s("%d", &doctorChoice);
	}
	if (doctorChoice == 1)
		return toString(alergy);
	else if (doctorChoice == 2)
		return toString(dermatology);
	else if (doctorChoice == 3)
		return toString(family);
	else if (doctorChoice == 4)
		return toString(neurology);
	else if (doctorChoice == 5)
		return toString(pathology);
	else if (doctorChoice == 6)
		return toString(psychiatry);
	else if (doctorChoice == 7)
		return toString(surgery);
	else
		return toString(urology);
}
//returns a string of the doctor's list of hours by: start hour,end hour and difference of time between meetings
char* getListOfTurns()
{
	char startHour[MAXSIZE];
	char endHour[MAXSIZE];
	int hoursValid = 0;//indicates if start and end hours are valid
	int timeBetweenMeetings;
	puts("You are about to enter the hour you want to start work at and the hour you want to finish work at, a valid start/end hour should be later than 08:00 and less than 22:00");
	puts("the hour you will enter should be in this formation: xy:00, x,y represent numbers between 0 and 9");
	puts("Keep in mind, the start hour should be before the finish hour");
	puts("Please enter your start hour:");
	getchar();
	gets(startHour);//get the start hour
	puts("Please enter your end hour:");
	gets(endHour);//get the end hour
	hoursValid = isHourValid(startHour, endHour);//check if the hours are valid
	while (!hoursValid)//as long as the hours are not valid geet it again from the doctor
	{
		puts("Something went wrong with the hours you entered, please follow the rules above and try again");
		puts("Please enter your start hour:");
		gets(startHour);
		puts(startHour);
		puts("Please enter your end hour:");
		gets(endHour);
		hoursValid = isHourValid(startHour, endHour);
	}
	puts("Please enter the time required for a single meeting in minutes, valid times are: 15 , 30, 60 only");
	scanf_s("%d", &timeBetweenMeetings);//get the difference of time between meetings
	while ((timeBetweenMeetings != 15) && (timeBetweenMeetings != 30) && (timeBetweenMeetings != 60))//if its not vaild
	{
		puts("the time gap you entered is not valid, please try again");
		scanf_s("%d", &timeBetweenMeetings);//get it again
	}
	getchar();
	return createListOfHours(atoi(startHour), atoi(endHour), timeBetweenMeetings);//returns the list of appointment hours by the start, end times and the difference between meetings
}

//adds a doctor with all his details to the doctor db
void addDoctorToDb(char* id, char* pass, char* fullName, char* medicalField, char* title, char* gender, char* listOfHours)
{
	char* errmsg = NULL;
	char query[20000] = "";
	char* finalGender = NULL;
	sqlite3* db;
	sqlite3_stmt* stmt;
	sqlite3_open("doctorDb.db", &db);
	int rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS doctorInfo(id TEXT UNIQUE,password TEXT, full_name Text, medical_field TEXT,"
		"title TEXT, gender TEXT, original_list_of_hours TEXT, one TEXT,"
		"two TEXT, three TEXT, four TEXT, five Text, six Text, seven Text,"
		"eight Text , nine Text, ten Text, eleven Text, twelve Text, thirteen Text,"
		"fourteen Text,fifteen Text,sixteen Text,seventeen Text,eighteen Text,nineteen Text,"
		"twenty Text,twenty_one Text,twenty_two Text,twenty_three Text,twenty_four Text,twenty_five Text,"
		" twenty_six Text,twenty_seven Text,twenty_eight Text,blocked_dates TEXT);", NULL, NULL, errmsg);
	if (rc != SQLITE_OK)
		printf("Error: %s\n", errmsg);
	if (gender == 'M')
		finalGender = "Male";
	else
		finalGender = "Female";
	//prepars the query with the variables
	sprintf(query, "INSERT INTO doctorInfo VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',"
		"'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',"
		"'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',"
		"'%s','%s','%s','%s','%s','NULL');", id, pass, fullName, medicalField, title, finalGender, listOfHours,
		listOfHours, listOfHours, listOfHours, listOfHours, listOfHours, listOfHours, listOfHours,
		listOfHours, listOfHours, listOfHours, listOfHours, listOfHours, listOfHours, listOfHours,
		listOfHours, listOfHours, listOfHours, listOfHours, listOfHours, listOfHours, listOfHours,
		listOfHours, listOfHours, listOfHours, listOfHours, listOfHours, listOfHours, listOfHours);

	rc = sqlite3_exec(db, query, NULL, NULL, &errmsg);//make insertion query
	if (rc != SQLITE_OK)//if an error occured
		printf("Error: %s\n", errmsg);
	else
		puts("You are successfuly registered to the system");
	sqlite3_close(db);
}


//returns the date in February the user chose as an int
int chooseDate()
{
	int dayChoice;
	puts("the minimum date you can choose is 01/02/22 and the maximum date is 28/02/22");
	puts("Please enter the day in february that you want to perform the action on");
	puts("for example: enter 1 for 01/02, enter 28 for 28/02");
	scanf_s("%d", &dayChoice);//get the date 
	while (dayChoice < 1 || dayChoice > 28)//while the date isnt valid
	{
		puts("the day you entered is not valid, please enter a number between 1 and 28");
		scanf_s("%d", &dayChoice);//get it again
	}
	return dayChoice;
}

//gets a day in February (for example 17 represents the 17/02), returns the column name in the doctors db that represents the day number in Fubruary
char* colNameInDocTableByDate(int day)
{
	char* a[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten",
				  "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen",
				  "eighteen", "nineteen", "twenty", "twenty_one", "twenty_two", "twenty_three",
				  "twenty_four", "twenty_five", "twenty_six", "twenty_seven", "twenty_eight" };
	return a[day - 1];
}

//gets the string of all the blocked dates and prints it to the screen
void printAllBlockedDates(const char* str)
{
	puts("Blocked dates list:");
	if (!strcmp(str, "NULL"))//ifthere are no blocked dates
	{
		puts("There are no blocked dates\n");
		return;
	}
	//if there are blocked dates
	int size = strlen(str);
	for (int i = 0; i < size; i++)//run on the list and print every date in a new line
	{
		if (str[i] == ',')
			printf("/02\n");
		else
			printf("%c", str[i]);
	}
	puts("");
}

//opens a day that was blocked before for mettings, replaces the string in the column from "Blocked" to the original list of hours of the doctor 
void openBlockedDate(char* id)
{
	char buffer[2] = "";
	char* blockedDatesList = GetDetailsFromDb("blocked_dates", "doctorDb.db", "doctorInfo", id);//returns the blocked date list of the doctors db
	printAllBlockedDates(blockedDatesList);//prints the blocked dates list to the screen
	if (!strcmp(blockedDatesList, "NULL"))//if there are no blocked days
		return;
	int dateToReopen = chooseDate();//lets the doctor choose a date
	char* colName = colNameInDocTableByDate(dateToReopen);//return the chosen date as written in the column of chart in the db
	char* colField = GetDetailsFromDb(colName, "doctorDb.db", "doctorInfo", id);//returns string in the field of the chosen column in the chart
	if (!strcmp(colField, "Blocked")) //if the day is blocked
	{
		//set the filled to be the original_list_of_hours
		char* originalListOfHours = GetDetailsFromDb("original_list_of_hours", "doctorDb.db", "doctorInfo", id);
		EditDetailsInDb(colName, originalListOfHours, "doctorDb.db", "doctorInfo", id);
		//erase it from the blocked_dates
		snprintf(buffer, 2, "%d", dateToReopen);//stores the date to reopen in buffer
		char* placeOfSubString = strstr(blockedDatesList, buffer);//return a pointer to the first appearance of the day to reopen in the blocked dates list
		char* newBlockedDatedList = deleteSubString(blockedDatesList, placeOfSubString);//returns the blocked dates list without the date to reopen
		if (!strcmp(newBlockedDatedList, ""))//if after the deletion there are no blocked dates update the field in the chart to NULL
			EditDetailsInDb("blocked_dates", "NULL", "doctorDb.db", "doctorInfo", id);
		else//otherwise update it to the new blocked dates list
			EditDetailsInDb("blocked_dates", newBlockedDatedList, "doctorDb.db", "doctorInfo", id);
		free(newBlockedDatedList);
	}
	else//if the date is not blocked
	{
		puts("Oops, the date you chose isnt blocked");
	}
	free(blockedDatesList);
	free(colField);
}

//prints all the doctors from the wanted medical field to the screen by this format: "ID: Name: Gneder:", 
//returns a list of all the id's of the doctors from the wanted medical field : id1,id2,id3...
char* getListOfDoctorsByMedicalField(char* medicalField)
{
	char doctorIdList[MAXSIZE] = "";//result to return
	char buffer[200] = "";
	char* errmsg = NULL;
	char query[500] = "";
	sqlite3* db;
	sqlite3_stmt* stmt;
	sprintf(query, "SELECT id,full_name,gender FROM doctorInfo WHERE medical_field = '%s'", medicalField);//prepare the query with variables
	sqlite3_open("doctorDb.db", &db);//open doctors db
	sqlite3_prepare_v2(db, query, -1, &stmt, 0);//execute the query
	const unsigned char* currId = NULL;
	const unsigned char* currFullName = NULL;
	const unsigned char* currGender = NULL;
	printf("\nDoctors list from the field %s:\n", medicalField);
	while (sqlite3_step(stmt) != SQLITE_DONE)//for each doctor in list of the doctors from the wanted medical field
	{
		currId = sqlite3_column_text(stmt, 0);
		currFullName = sqlite3_column_text(stmt, 1);
		currGender = sqlite3_column_text(stmt, 2);
		printf("ID: %s Name: %s Gender: %s\n", currId, currFullName, currGender);//print his id,name,gender
		sprintf(buffer, "%s,", currId);//setto the buffer to the current doctor id
		strcat(doctorIdList, buffer);//add the id to the result to return
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return toString(doctorIdList);
}

//gets the id of the logged in doctor, gets the day the doctor want to block for appointments, if the day is already blocked 
//or has at least one scheduled appointment prints a proper message to the screen, otherwise block the date by changing the 
//column field in the chart to 'Blocked'
void tryBlockingDate(char* id)
{
	int dayChoice = chooseDate();//gets the day to block
	char* columnName = colNameInDocTableByDate(dayChoice);//gets the name of the column representing the number of the day to block
	char* fieldInCol = GetDetailsFromDb(columnName, "doctorDb.db", "doctorInfo", id);//gets the filed in the column representing the day to block
	if (!strcmp(fieldInCol, "Blocked"))//if date is already blocked
	{
		puts("The day is already blocked");
		return;
	}
	char* bookedStr = getBookedAppointmentsList(fieldInCol);//gets only the book appointments string
	if (strcmp(bookedStr, "taken:"))//if they differ, means there is at least one appointment booked
	{
		puts("Sorry, but you cant block this day, looks like a patient has already booked an appointment on this day");
		return;
	}
	//set the string in the wanted column to "Blocked", indeicae the day is blocked
	EditDetailsInDb(columnName, "Blocked", "doctorDb.db", "doctorInfo", id);
	addDateToBlockedList(dayChoice, id);//add the number of the day to the blocked days list
	puts("The day was blocked succesfuly");
	free(fieldInCol);
	return;
}

//after a date was blocked adds it to the blocked dates list
void addDateToBlockedList(int day, char* id)
{
	char buffer[MAXSIZE] = "";
	char buffer2[MAXSIZE] = "";
	char* blockedDatesStr = GetDetailsFromDb("blocked_dates", "doctorDb.db", "doctorInfo", id);//returns the blocked dates list of the doctor from the db
	if (!strcmp(blockedDatesStr, "NULL"))//if there are no blocked dates
	{
		snprintf(buffer, MAXSIZE, "%d", day);//copy to the buffer the new blocked day
		strcat(buffer, ",");
		EditDetailsInDb("blocked_dates", buffer, "doctorDb.db", "doctorInfo", id);//update the blocked dates list to be with the date that was blocked
	}
	else //if there are blocked dates
	{
		sprintf(buffer, "%s", blockedDatesStr);//set buffer to have the previous list off blocked days
		snprintf(buffer2, MAXSIZE, "%d", day);//set buffer 2 with the new day that was blocked
		strcat(buffer, buffer2);//add the new blocked date to the previous list of blocked dates
		strcat(buffer, ",");
		EditDetailsInDb("blocked_dates", buffer, "doctorDb.db", "doctorInfo", id);//update the blocked dates list to be with the date that was blocked day
	}
	free(blockedDatesStr);
}

//gets the doctor's string of the available and not available time on a desired date and return only the not available string
char* getBookedAppointmentsList(const char* str)
{
	char bookedAppointments[700] = "";
	int size = strlen(str);
	int index = 0;
	int hasSeenAsterisk = 0;//the booked appointments apear after an the '*'
	for (int i = 0; i < size; i++)
	{
		if (str[i] == '*')//if the char we run on is asteriks
			hasSeenAsterisk = 1;//indicate that we found the asteriks
		else //if the char we run on isnt an asteriks
		{
			if (!hasSeenAsterisk)//if we havent seen the asteriks yet
				continue;//continue to the next char
			else //if we already saw the asteriks
			{
				bookedAppointments[index] = str[i];//copy the char
				index++;
			}
		}
	}
	return toString(bookedAppointments);
}
//gets the doctors's string of the available and not available times for appointment, returnsonlt the available times list 
char* getOnlyAvailableTimesList(char* availableAndNot)
{
	char availableTimes[MAXSIZE] = "";
	int  size = strlen(availableAndNot);
	int index = 0;
	while (availableAndNot[index] != '*')//as long as we didnt reach to the '*' keep coping fron the available and not string
	{
		availableTimes[index] = availableAndNot[index];
		index++;
	}
	availableTimes[index] = '\0';
	return toString(availableTimes);
}
//checks if a date appears in the blocked dates list of a doctor, if appears return 1 otherwise returns 0
//list of blocked dates looks like: 1,3,21,28,....
int isDateAppearInBlockDateList(char* blockDateList, int date)
{
	if (!strcmp(blockDateList, "NULL"))//if there are no blocked dates
		return 0;
	char buffer[3] = "";
	int bufferIndex = 0;//he index to write to in buffer
	int currDate = 0;
	int size = strlen(blockDateList);
	for (int i = 0; i < size; i++)
	{
		if (blockDateList[i] == ',')//if we got to the end of the date, means buffer contains the final date
		{
			currDate = atoi(buffer);//store the current date as an integer
			//strcpy(buffer, "");//empty the buffer//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			memset(buffer, '\0', 3);//empty the buffer
			bufferIndex = 0;//set the index to write to, to 0
			if (currDate == date)//if found a match
				return 1;
		}
		else//if the date isnt finished yet(we didnt saw the coma yet) keep coping it to the buffer
		{
			buffer[bufferIndex] = blockDateList[i];
			bufferIndex++;
		}
	}//if didnt found a match at all
	return 0;
}

//creates the string of the doctor's list of hours by: start hour,end hour and difference of time between meetings
char* createListOfHours(int startHour, int endHour, int timeBetweenMeetings)
{
	char result[MAXSIZE] = "";//the string to return
	char currHour[3];//the current hour we are runnig on
	int minutes = 0;//the current minute
	while (startHour < endHour)//as long as we didnt reach to the end hours
	{
		if (minutes == 60)//if an hour as passed
		{
			startHour += 1;//add 1 to the hour
			minutes = 0;
			if (startHour == endHour)
				break;
		}
		if (minutes == 0)//if the minutes are 0
		{
			if (startHour < 10) //if the curr hour is a single digit number add a '0' to the string
				strcat(result, "0");
			snprintf(currHour, 3, "%d", startHour);
			strcat(result, currHour);//than add the current hour to the string
			strcat(result, ":00,");//than add ':00' to the string
			minutes += timeBetweenMeetings;
		}
		else //if minutes == 15,30,
		{
			if (startHour < 10) //if the curr hour is a single digit number add a '0' to the string
				strcat(result, "0");
			snprintf(currHour, 3, "%d", startHour);
			strcat(result, currHour);//add the current hour to the string
			strcat(result, ":");//add ':' to the string
			snprintf(currHour, 3, "%d", minutes);
			strcat(result, currHour);//add the current minutes to the string
			strcat(result, ",");//add a coma
			minutes += timeBetweenMeetings;
		}
	}
	strcat(result, "*taken:");//add '*taken:' to the string 
	return toString(result);
}
//checks if a title is valid, a valid title can be one of follows: Mr. , Ms. , Doc. , Doctor
//if the title is valid returns 1 otherwise returns 0
int isTitleValid(const char* title)
{
	char mr[] = "Mr.";
	char ms[] = "Ms.";
	char doc[] = "Doc.";
	char doctor[] = "Doctor";
	if ((!(strcmp(title, mr))) || (!(strcmp(title, ms))) || (!(strcmp(title, doc))) || (!(strcmp(title, doctor))))
		return 1;
	return 0;
}
//check if the hours are valid hours, a valid hour has 5 chars, the hours must end in ':00' and the first two chars must be digits
//the first digit cant be hiher than 2 (no hour 30:00) ,the start hour must be previous to the end hour and the working hours must
//be between 08:00 to 22:00
int isHourValid(const char* startHour, const char* endHour)
{
	if ((strlen(startHour) != 5) || (strlen(endHour) != 5))//if the hour isnt in the right pattern  'xy:qz'
		return 0;
	//if the minutes are not '00' or the ':' isnt in the right place or the first 2 digits are not numbers
	if ((startHour[3] != '0') || (startHour[4] != '0') || (startHour[2] != ':') || (startHour[0] > '9') || (startHour[0] < '0') || (startHour[1] > '9') || (startHour[1] < '0'))
		return 0;
	if ((endHour[3] != '0') || (endHour[4] != '0') || (endHour[2] != ':') || (endHour[0] > '9') || (endHour[0] < '0') || (endHour[1] > '9') || (endHour[1] < '0'))
		return 0;
	if ((startHour[0] > '2') || (endHour[0] > '2'))//the first digit in an hour can be 0,1 or 2
		return 0;
	int maxHour = 22;//max hour 22:00
	int minHour = 8;//min hour 08:00
	int start = atoi(startHour);
	int end = atoi(endHour);
	//if the the start hour and the end hour are in the valid range and start hour is before end hour
	if ((start >= minHour) && (start <= maxHour) && (end >= minHour) && (end <= maxHour) && (start < end))
		return 1;
	return 0;
}
//after an appointment was booked changes the string that represents the  available times and taken times, returns a new 
//string regards to the new appointment time, removes the new time from the available times and adds it to the taken times 
//:param time: the time to remove from the available times list and add to the not available list
//:param colNameInDb: the date the action is performed on
void editAvailableAndNotStrAfterBooking(char* colNameInDb, char* docId, char* time)
{
	char buffer[MAXSIZE] = "";
	char* availableAndNot = GetDetailsFromDb(colNameInDb, "doctorDb.db", "doctorInfo", docId);//returns the available and not available times of a doctor in the date 'colNameInDb'
	char* availableStr = getOnlyAvailableTimesList(availableAndNot);//returns only the available times list
	char* notAvailableStr = getBookedAppointmentsList(availableAndNot);//returns only the not available times list
	char* placeOfTimeInAvailableStr = strstr(availableStr, time);//returns a pointer to the first appearance of the time to delete in the available times string
	char* newAvailableStr = deleteSubString(availableStr, placeOfTimeInAvailableStr);//deletes the time from the available string
	//recreate the available and not times string in the buffer
	strcat(buffer, newAvailableStr);
	strcat(buffer, "*");
	strcat(buffer, notAvailableStr);
	strcat(buffer, time);
	strcat(buffer, ",");
	char* newAvailableAndNot = toString(buffer);
	EditDetailsInDb(colNameInDb, newAvailableAndNot, "doctorDb.db", "doctorInfo", docId);//update the available and not string to the new one after booking
	free(availableAndNot);
	free(availableStr);
	free(notAvailableStr);
	free(newAvailableStr);
	free(newAvailableAndNot);
}

//after an appointment was canceled changes the string that represents the  available times and taken times, returns a new 
//string regards to the canceled appointment time, removes the new time from the not available times and adds it to the available times 
//:param time: the time to remove from the not available times list and add to the available list
//:param colNameInDb: the date the action is performed on
void editAvailableAndNotStrAfterCancelation(char* colNameInDb, char* docId, char* time)
{
	char availableStr[MAXSIZE] = "";
	char* availableAndNot = GetDetailsFromDb(colNameInDb, "doctorDb.db", "doctorInfo", docId);//returns the available and not available times of a doctor in the date 'colNameInDb'
	char* availableStrP = getOnlyAvailableTimesList(availableAndNot);//returns only the available times list
	strcpy(availableStr, availableStrP);
	free(availableStrP);
	char* notAvailableStr = getBookedAppointmentsList(availableAndNot);//returns only the not available times list
	char* placeOfTimeInNotAvailableStr = strstr(notAvailableStr, time);//returns a pointer to the first appearance of the time to delete in the  not available times string
	char* newNotAvailableStr = deleteSubString(notAvailableStr, placeOfTimeInNotAvailableStr);//deletes the time from the not available string
	strcat(availableStr, time);//adds the time of the cnaceled appointment to the available times list
	strcat(availableStr, ",");
	//recreate the available and not times string
	strcat(availableStr, "*");
	strcat(availableStr, newNotAvailableStr);
	char* newAvailableAndNot = toString(availableStr);
	EditDetailsInDb(colNameInDb, newAvailableAndNot, "doctorDb.db", "doctorInfo", docId);//update the available and not string to the new one after cancelation
	free(notAvailableStr);
	free(newNotAvailableStr);
	free(availableAndNot);
	free(newAvailableAndNot);
}
