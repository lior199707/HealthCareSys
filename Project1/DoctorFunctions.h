#pragma once
#ifndef DOCTORFUNCTIONS_H
#define DOCTORFUNCTIONS_H
void doctorRegister();//gets the doctor full name, id,password,gender,medical field,title and the work hours (start hour and finish hour) and adds the doctor to the db
void doctorLogIn();//gets the doctor id and password and checks if exists in the db
char* getMedicalField();//gets the medical filed of the doctor from an existing list of medical fields


#endif // !DOCTORFUNCTIONS_H
