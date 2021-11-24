#pragma once
#ifndef CLIENTFUNCTIONS_H
#define CLIENTFUNCTIONS_H

void clientRegister();//gets full name, id and password of the client and adds it to the data base
void addClientToDb(char* id, char* pass, char* fullName);//adds the client information to the chart in the SQLite database
int chooseDateForAppointmnent(docId);//gets the date the client wants to schedule an appointment 
void clientLogIN();//gets the id and password from the user and checks if it exists in the data base
void bookAppointment(char* id);//books an appointment by a choice of: medical field,doctor name,date and time,updates both client db and doctor db 


#endif // !CLIENTFUNCTIONS_H
