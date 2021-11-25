#pragma once
#ifndef CLIENTFUNCTIONS_H
#define CLIENTFUNCTIONS_H


void clientMenu();//login/register menu presented to the client 
void clientOptionsMenu(char* fullName, char* id);//the menu presented with actions for the client after he logged in
void clientRegister();//gets full name, id and password of the client and adds it to the data base
void addClientToDb(char* id, char* pass, char* fullName);//adds the client information to the chart in the SQLite database
int chooseDateForAppointmnent(docId);//gets the date the client wants to schedule an appointment 
void clientLogIN();//gets the id and password from the user and checks if it exists in the data base
void bookAppointment(char* id);//books an appointment by a choice of: medical field,doctor name,date and time,updates both client db and doctor db 
void editFutureAppointmentsAfterBooking(char* clientId, char* docId, char* docName, char* docGender, char* date, char* time);//updates the client db and doctor db after a client has booked an appointment
void cancelAppointment(char* id);//gets from the client the doctor id,the date and the time of the meeting he wants to cancel and updates both data bases


#endif // !CLIENTFUNCTIONS_H
