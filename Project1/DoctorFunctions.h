#pragma once
#ifndef DOCTORFUNCTIONS_H
#define DOCTORFUNCTIONS_H
void doctorRegister();//gets the doctor full name, id,password,gender,medical field,title and the work hours (start hour and finish hour) and adds the doctor to the db
void doctorLogIn();//gets the doctor id and password and checks if exists in the db
char* getMedicalField();//gets the medical filed of the doctor from an existing list of medical fields
int chooseDate();//return the int that represents the day in february the user chose
char* colNameInDocTableByDate(int day);//gets a day in february and returns the string that represents it in the doctors db columns
void printAllBlockedDates(const char* str);//prints all blocked dates of a doctor to the screen
#endif // !DOCTORFUNCTIONS_H
