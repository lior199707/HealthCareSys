#define _CRT_SECURE_NO_WARNINGS
#include "ClientFunctions.h"
#include "MutualFunctions.h"
#include "DoctorFunctions.h"

//gets the client info and adds him to the database
void clientRegister()
{
	char* fullName = getName();//gets the client full name
	char* id = getId();//gets the client id
	char* password = getPass();//gets the client password
	addClientToDb(id, password, fullName);//adds him to the db
	free(fullName);
	free(id);
	free(password);
}

