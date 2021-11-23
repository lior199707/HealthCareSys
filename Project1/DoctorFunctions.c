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





//printf("shalom");
//jklkjlkjlkjlk
