#pragma once
#ifndef DOCTORFUNCTIONS_H
#define DOCTORFUNCTIONS_H

void doctorMenu();//login/register menu of a doctor
void doctorRegister();//gets the doctor full name, id,password,gender,medical field,title and the work hours (start hour and finish hour) and adds the doctor to the db
void doctorLogIn();//gets the doctor id and password and checks if exists in the db
void watchMeetingsByDate(char* id);//gets the number of the day in february from the doctor and prints him all his appointments on said day 
char getGender();//gets the gender of the doctor, M or F
char* getTitle();//gets the title of the doctor from an existing list of titles
char* getMedicalField();//gets the medical filed of the doctor from an existing list of medical fields
char* getListOfTurns();//gets the start hour and finish hour of a doctors day of work min hour is 08:00 and max hour is 22:00,gets also the time between meetings in minutes(15 or 30 or 60)
void addDoctorToDb(char* id, char* pass, char* fullName, char* medicalField, char* title, char* gender, char* listOfHours);//adds a doctor with all his detaild to the db
int chooseDate();//return the int that represents the day in february the user chose
char* colNameInDocTableByDate(int day);//gets a day in february and returns the string that represents it in the doctors db columns
void printAllBlockedDates(const char* str);//prints all blocked dates of a doctor to the screen
void openBlockedDate(char* id);//gets a day the doctor wants to reopen, if the chosen day wasnt blocked prints a proper massage to the screen otherwise changes the day value to the original list of hours the doctor had before he blocked the date and removes said date fron the blocked dates list
char* getListOfDoctorsByMedicalField(char* medicalField);//returns a string of the ID's of the doctor from the wanted medical field separeted by comas and prints their IDs names and gender to the screen
void tryBlockingDate(char* id);//if the day is blockable(wasnt blocked already,has no appointments yet) changed the value of the field to "Blocked", otherwise prints a proper masssage to the srcreen
void addDateToBlockedList(int day, char* id);//after a date was blocked adds the number of the date to the blocked dates column in the doctor db 
char* getBookedAppointmentsList(const char* str);//gets a string that contains the available times and taken times on a specific day and retunrs only the string of the booked appointments a doctor has
char* getOnlyAvailableTimesList(char* availableAndNot);//gets the string of available times and not available times a doctor has in a certain date and returns only the available ones
int isDateAppearInBlockDateList(char* blockDateList, int date);//gets an int that represents a day in february and checks if he exists in the blocked dates list in the doctor db
char* createListOfHours(int startHour, int endHour, int timeBetweenMeetings);//get the start hour, end hour and time between meetings and creates the list of appointments for the doctor
int isTitleValid(const char* title);//checks if a title of a doctor is valid, a valid title is a title that exists in the list of titles the system holds
int isHourValid(const char* startHour, const char* endHour);//gets the start hour and end hour, checks that their formation matches to an hour formation and checks the start hour is previous to the end hour
void editAvailableAndNotStrAfterBooking(char* colNameInDb, char* docId, char* time);//after an appointment was booked changes the string that represents the  available times and taken times, returns a new string regards to the new appointment time, removes the new time from the available times and adds it to the taken times 
void editAvailableAndNotStrAfterCancelation(char* colNameInDb, char* docId, char* time);//after an appointment was canceled changes the string that represents the  available times and taken times, returns a new string regards to the canceled appointment time, removes the canceled time from the taken times and adds it to the available times
void doctorOptionsMenu(char* fullName, char* id);//the doctor options menu after he logged in
void EditProfileMenu(char* id);//the options menu to edit a doctor profile
#endif // !DOCTORFUNCTIONS_H
