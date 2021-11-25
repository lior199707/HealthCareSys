#include "DoctorFunctions.h"
#include "MutualFunctions.h"
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
	int hoursValid = 0;//indicate if start and end hours are valid
	int timeBetweenMeetings;
	puts("You are about to enter the hour you want to start work at and the hour you want to finish work at, a valid start/end hour should be later than 08:00 and less than 22:00");
	puts("the hour you will enter should be in this formation: xy:00, x,y represent numbers between 0 and 9");
	puts("Keep in mind, the start hour should be before the finish hour");
	puts("Please enter your start hour:");
	getchar();
	gets(startHour);//get teh start hour
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
	//prepears the query with the variables
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
