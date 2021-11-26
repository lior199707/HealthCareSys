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
void watchFutureAppointments(char* id);//prints the client future appointments to the screen
char* chooseDocIdByField(char* medicalField);//get the id of the doctor the client want to book an appointment with
char* createNewAllAppointmentsListByComa(char* AllAppointmentsList, int numOfComaToDel);//gets the future appoinmtnet list of the client and returns the list without the appointment to cancel by the index of the coma to delete
int numOfComasForAppointmentCancelation(char* tempAllAppointmentsList, char* tempAppointmentToCancel);//return the number of comas has passed untill found the appointment to cancel,if wasnt found return -1
char* chooseTimeForAppointment(char* docId, char* chosenDate, char* dateInDocDb);//get the time the client wants to schedule an appointment in
char* createTempAppointmentsList(char* allAppointmentsList);//gets the appointmnets list by doctor: id name gender date time and return a string :id*date*time
char* createTempAppointmentToCancle(char* docId, char* dateForAppointment, char* timeForAppointment);//gets as an input the doctor id,the date and the time of the appointment to cancel and returns it in this format:id*date*time  
#endif // !CLIENTFUNCTIONS_H
