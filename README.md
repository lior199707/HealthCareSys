# Health Care System

Doctor's appointments system where patients can schedule appointments independently and according to a doctor’s schedule on February.

## Description

* The project was made on the first semester of the degree's second year.

Uses the terminal for inputs and sqlite as the database.
Presents the user with a choice whether he is a client(patient) or a doctor, clients menu is accessible freely and the doctors menu is accessible with the password: "DOCTORPASSWORD"
The user than can sing up or log in to the system, if the the user gets the log in process wrong 3 times in a row he is returned to the previous menu.

### Client registrarion details:

full name, id and password.

### Doctor registrarion details:

full name, id ,password, gender, title, medical field(choice from a defined list of fields), daily work hours and the time required for a single meeting.
The system will create the doctor's work schedule for Febuary, each day will start and end at the times provided by the doctor and the difference between each available meeting time will be as the doctor defined.

### Client available actions:
* Book an appointment to a doctor- choose medical field, choose the doctor, choose a date, choose a time (only availables dates and times will appear, days that are fully scheduled, doctor's blocked dates and times that are already booked will not appear)
* Cancel an appointment- presents future appointmens and cancels an appointment by date and time input(makes the canceled appointment available again for the doctor)
* Watch future appointments
* Log out

### Doctor available actions:
* Watch future appointments by date - choose by input the day on February, presents all future appointments on that day. 
* Block a date for appointmets (Can't be performed on a date that has a scheduled appointment)
* Edit profile details
* Reopen a blocked date (can be performed only on blocked dates)
* Watch all blocked dates
* Log out

## Predependencies
Visual Studio 2019 - the project is not compitable with futue versions.
