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




//printf("shalom");
//jklkjlkjlkjlk
