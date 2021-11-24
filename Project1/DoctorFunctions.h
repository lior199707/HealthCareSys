#pragma once
#ifndef DOCTORFUNCTIONS_H
#define DOCTORFUNCTIONS_H
void doctorRegister();//gets the doctor full name, id,password,gender,medical field,title and the work hours (start hour and finish hour) and adds the doctor to the db
void doctorLogIn();//gets the doctor id and password and checks if exists in the db
char* getMedicalField();//gets the medical filed of the doctor from an existing list of medical fields
char* getListOfTurns();//gets the start hour and finish hour of a doctors day of work min hour is 08:00 and max hour is 22:00,gets also the time between meetings in minutes(15 or 30 or 60)
int chooseDate();//return the int that represents the day in february the user chose
char* colNameInDocTableByDate(int day);//gets a day in february and returns the string that represents it in the doctors db columns
void printAllBlockedDates(const char* str);//prints all blocked dates of a doctor to the screen
void tryBlockingDate(char* id);//if the day is blockable(wasnt blocked already,has no appointments yet) changed the value of the field to "Blocked", otherwise prints a proper masssage to the srcreen
char* getBookedAppointmentsList(const char* str);//gets a string that contains the available times and taken times on a specific day and retunrs only the string of the booked appointments a doctor has

#endif // !DOCTORFUNCTIONS_H
