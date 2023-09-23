#include<string>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<stdlib.h>
using namespace std;

int const MAXDEPS = 3;
string const EXITCODE = "EXIT";


	/*
	/=========================\
			 Structures
	\=========================/
	*/
	
struct History;

struct Human {
	string id, name, dateOfBirth, email, password, address, state;
	int age;
	char gender;
	Human *nextHuman, *prevHuman;
	History* topHistory, *botHistory;
};

struct Dependant {
	string id, name, relation;
	int age;
	char gender;
};

struct Location {
	string code, name, address, postCode, state;
	Location* nextLocation, *prevLocation;
};

struct History {
	Human* person;
	Dependant deps[MAXDEPS];
	Location* loc;
	History* hNext, * hPrev;
};

	/*
	/=========================\
	 Quality Of Life Functions
	\=========================/
	*/

void clear() {
	system("CLS");
}

void uppercase(string &input) {
	string upperOut = "";
	for (int i = 0; i < input.length(); i++)
	{
		if (input[i] >= 97 && input[i] <= 122)
		{
			input[i] -= 32;
			upperOut += input[i];
		}
		else
			upperOut += input[i];
	}
	input = upperOut;
}

void uppercase(char &input) {
	char upperOut = ' ';
	if (input >= 97 && input <= 122)
	{
		input -= 32;
		upperOut = input;
	}
	else
		upperOut = input;
}

	/*
	/=========================\
		 Human Functions
	\=========================/
	*/

void RegisterHuman() {
	Human humData;
	string pass = "1", confPass = "2";

	cout << "Enter Your ID/IC: ";
	getline(cin, humData.id);
	uppercase(humData.id);

	cout << "Enter Your Name: ";
	getline(cin, humData.name);
	uppercase(humData.name);

	cout << "Enter Your Email: ";
	getline(cin, humData.email);

	while (pass != confPass){
		cout << "Enter Your Password: ";
		getline(cin, pass);

		cout << "Confirm Password: ";
		getline(cin, confPass);

		if (pass != confPass){
			cout << "Passwords Do Not Match, Please Try Again." << endl;
		}

	}
	humData.password = pass;

	cout << "Enter Your Date Of Birth (DD/MM/YYYY): ";
	getline(cin, humData.dateOfBirth);

	cout << "Enter The State You Live In: ";
	getline(cin, humData.state);
	uppercase(humData.state);

	cout << "Enter Your Address: ";
	getline(cin, humData.address);
	uppercase(humData.address);

	cout << "Enter Your Age: ";
	cin >> humData.age;
	humData.gender = ' ';

	while (humData.gender != 'M' && humData.gender != 'F')
	{
		cout << "Enter Your Gender (M/F): ";
		cin >> humData.gender;
		cin.ignore();
		uppercase(humData.gender);
		if (humData.gender != 'M' && humData.gender != 'F')
		{
			cout << "Error: Invalid Input, Please Input M or F." << endl;
		}
	}
	cin.ignore();

	ofstream addHumanData(".\\UserData.txt", fstream::app);
	addHumanData << humData.id << "^" << humData.name << "^" << humData.email << "^" << humData.password << "^" << humData.dateOfBirth << "^" << humData.state << "^" << humData.address << "^" << humData.age << "^" << humData.gender << endl;
	addHumanData.close();
}

Human *getHumansFromFile(Human* &bot) {
	Human* top = NULL;
	ifstream readHumans(".\\UserData.txt");
	string userLine = "";
	bool fr = true;
	getline(readHumans, userLine);

	while (!userLine.empty()){
		Human *person = new Human;
		int dataCount = 0;
		string ageBE = "";

		for (int i = 0; i < userLine.length(); i++){
			if (userLine[i] != '^'){
				switch (dataCount) {
				case 0:
					person->id += userLine[i];
					break;
				case 1:
					person->name += userLine[i];
					break;
				case 2:
					person->email += userLine[i];
					break;
				case 3:
					person->password += userLine[i];
					break;
				case 4:
					person->dateOfBirth += userLine[i];
					break;
				case 5:
					person->state += userLine[i];
					break;
				case 6:
					person->address += userLine[i];
					break;
				case 7:
					ageBE += userLine[i];
					break;
				case 8:
					person->age = stoi(ageBE);
					person->gender = userLine[i];
					dataCount++;
					break;
				default:
					break;
				}
			} else {
				dataCount++;
			}
		}

		person->nextHuman = NULL;
		person->prevHuman = NULL;
		if (fr){
			top = person;
			bot = person;
			fr = false;
		} else {
			top->prevHuman = person;
			person->nextHuman = top;
			top = person;
		}
		getline(readHumans, userLine);
	}

	readHumans.close();


	return top;
}

Human *LoginHuman(Human *top) {
	bool notLogged = true;
	while (notLogged){
		string username = "", password = "";
		cout << "Enter Your Email or ID: ";
		getline(cin, username);
		cout << "Enter Your Passowrd: ";
		getline(cin, password);

		Human* navi = top;
		while (navi != NULL)
		{
			if (username == navi->email || username == navi->id) {
				if (password == navi->password) {
					return navi;
				}
			}
			navi = navi->nextHuman;
		}

		clear();
		cout << "Error: Wrong Email Or Password. Please Try Again" << endl;
	}
}

void rewriteHumans(Human* bot) {
	Human* navi = bot;

	ofstream writeFirstHumanData(".\\UserData.txt");
	writeFirstHumanData << navi->id << "^" << navi->name << "^" << navi->email << "^" << navi->password << "^" << navi->dateOfBirth << "^" << navi->state << "^" << navi->address << "^" << navi->age << "^" << navi->gender << endl;
	writeFirstHumanData.close();

	ofstream writeNextHumanData(".\\UserData.txt", fstream::app);
	navi = navi->prevHuman;

	while (navi != NULL){
		writeNextHumanData << navi->id << "^" << navi->name << "^" << navi->email << "^" << navi->password << "^" << navi->dateOfBirth << "^" << navi->state << "^" << navi->address << "^" << navi->age << "^" << navi->gender << endl;
		navi = navi->prevHuman;
	}

	writeNextHumanData.close();
}

Human* editHuman(Human* currentHuman) {
	bool exitNotRequested = true;
	int choice = 0;
	while (exitNotRequested) {
		clear();
		cout << "Current Data:\n1   Name:\t" << currentHuman->name << "\n2   Email:\t" << currentHuman->email << "\n3   Password:\t" << currentHuman->password 
			              << "\n4   Address:\t" << currentHuman->address << "\n5   State:\t" << currentHuman->state << "\n6   Age:\t" << currentHuman->age << endl;
		cout << "\nEnter The Number Of The Data You Would Like To Edit Or 0 To Stop Editing" << endl;
		cin >> choice;
		cin.ignore();

		switch (--choice) {
		case 0:
			clear();
			cout << "Current Name:\t" << currentHuman->name << "\nEnter The New Name: ";
			getline(cin, currentHuman->name);
			uppercase(currentHuman->name);
			break;
		case 1:
			clear();
			cout << "Current Email:\t" << currentHuman->email << "\nEnter The New Email: ";
			getline(cin, currentHuman->email);
			break;
		case 2:
			clear();
			cout << "Current Password:\t" << currentHuman->password << "\nEnter The New Password: ";
			getline(cin, currentHuman->password);
			break;
		case 3:
			clear();
			cout << "Current Address:\t" << currentHuman->address << "\nEnter The New Address: ";
			getline(cin, currentHuman->address);
			uppercase(currentHuman->address);
			break;
		case 4:
			clear();
			cout << "Current State:\t" << currentHuman->name << "\nEnter The New State: ";
			getline(cin, currentHuman->state);
			uppercase(currentHuman->state);
			break;
		case 5:
			clear();
			cout << "Current Age:\t" << currentHuman->name << "\nEnter The New Age: ";
			cin >> currentHuman->age;
			cin.ignore();
			break;
		case -1:
			clear();
			exitNotRequested = false;
			break;
		default:
			clear();
			cout << "Error: Invalid Input, Please Try Again." << endl;
			break;
		}
	}



	return currentHuman;
}

	/*
	/=========================\
		 Location Functions
	\=========================/
	*/	

void RegisterLocation() {
	Location locData;

	cout << "Enter The Location's Code: ";
	getline(cin, locData.code);
	uppercase(locData.code);

	cout << "Enter Location Name: ";
	getline(cin, locData.name);
	uppercase(locData.name);

	cout << "Enter Location Address: ";
	getline(cin, locData.address);
	uppercase(locData.address);

	cout << "Enter Location Post Code: ";
	getline(cin, locData.postCode);

	cout << "Enter The State Of The Location: ";
	getline(cin, locData.state);
	uppercase(locData.state);

	ofstream addLocationData(".\\LocationData.txt", fstream::app);
	addLocationData << locData.code << "^" << locData.name << "^" << locData.address << "^" << locData.postCode << "^" << locData.state << endl;
	addLocationData.close();
}

Location* getLocationsFromFile(Location* &bot) {
	Location* top = NULL;
	ifstream readLocation(".\\LocationData.txt");
	string locationLine = "";
	bool fr = true;
	getline(readLocation, locationLine);

	while (!locationLine.empty()) {
		Location* place = new Location;
		int dataCount = 0;
		string ageBE = "";

		for (int i = 0; i < locationLine.length(); i++) {
			if (locationLine[i] != '^') {
				switch (dataCount) {
				case 0:
					place->code += locationLine[i];
					break;
				case 1:
					place->name += locationLine[i];
					break;
				case 2:
					place->address += locationLine[i];
					break;
				case 3:
					place->postCode += locationLine[i];
					break;
				case 4:
					place->state += locationLine[i];
					break;
				default:
					break;
				}
			}
			else {
				dataCount++;
			}
		}

		place->nextLocation = NULL;
		place->prevLocation = NULL;
		if (fr) {
			top = place;
			bot = place;
			fr = false;
		}
		else {
			top->prevLocation = place;
			place->nextLocation = top;
			top = place;
		}
		getline(readLocation, locationLine);
	}

	readLocation.close();


	return top;
}

void rewriteLocations(Location* bot) {
	Location* navi = bot;

	ofstream writeFirstLocationData(".\\LocationData.txt");
	writeFirstLocationData << navi->code << "^" << navi->name << "^" << navi->postCode << "^" << navi->address << "^" << navi->state << endl;
	writeFirstLocationData.close();

	ofstream writeNextLocationData(".\\LocationData.txt", fstream::app);
	navi = navi->prevLocation;

	while (navi != NULL) {
		writeNextLocationData << navi->code << "^" << navi->name << "^" << navi->postCode << "^" << navi->address << "^" << navi->state << endl; 
		navi = navi->prevLocation;
	}

	writeNextLocationData.close();
}

Location* editLocation(Location* top) {
	Location* currentLoc = NULL;
	string locCode = "";
	bool locNotFound = true;
	clear();

	while (locNotFound) {
		cout << "Enter The Code Of The Location You Would Like To Edit: ";
		getline(cin, locCode);
		uppercase(locCode);
		Location *navi = top;
		while (navi != NULL){
			if (navi->code == locCode){
				locNotFound = false;
				currentLoc = navi;
				break;
			}
			navi = navi->nextLocation;
		}
		clear();
		if (locNotFound){
			cout << "Error: No Location With Such Code. Please Try Again." << endl;
		}
	}

	bool exitNotRequested = true;
	int choice = 0;
	while (exitNotRequested){
		clear();
		cout << "Current Data:\n1   Name:\t" << currentLoc->name << "\n2   Address:\t" << currentLoc->address << "\n3   Post Code:\t" << currentLoc->postCode << "\n4   State:\t" << currentLoc->state << endl;
		cout << "\nEnter The Number Of The Data You Would Like To Edit Or 0 To Stop Editing" << endl;
		cin >> choice;
		cin.ignore();

		switch (--choice){
		case 0:
			clear();
			cout << "Current Name:\t" << currentLoc->name << "\nEnter The New Name: ";
			getline(cin, currentLoc->name);
			uppercase(currentLoc->name);
			break;
		case 1:
			clear();
			cout << "Current Address:\t" << currentLoc->address << "\nEnter The New Address: ";
			getline(cin, currentLoc->address);
			uppercase(currentLoc->address);
			break;
		case 2:
			clear();
			cout << "Current Post Code:\t" << currentLoc->postCode << "\nEnter The New Post Code: ";
			getline(cin, currentLoc->postCode);
			break;
		case 3:
			clear();
			cout << "Current State:\t" << currentLoc->state << "\nEnter The New State: ";
			getline(cin, currentLoc->state);
			uppercase(currentLoc->state);
			break;
		case -1:
			clear();
			exitNotRequested = false;
			break;
		default:
			clear();
			cout << "Error: Invalid Input, Please Try Again." << endl;
			break;
		}
	}

	return top;
}

	/*
	/=========================\
		Dependant Functions
	\=========================/
	*/

Dependant addDependant() {
	Dependant depData;

	cout << "Enter The Dependant's ID/IC: " << endl;
	getline(cin, depData.id);
	uppercase(depData.id);

	cout << "Enter The Dependant's Name: " << endl;
	getline(cin, depData.name);
	uppercase(depData.name);

	cout << "Enter The Relation Of The Dependant." << endl;
	getline(cin, depData.relation);
	uppercase(depData.relation);

	cout << "Enter Dependant's Age: " << endl;
	cin >> depData.age;
	cin.ignore();

	while (depData.gender != 'M' && depData.gender != 'F')
	{
		cout << "Enter Dependant's Gender (M/F): ";
		cin >> depData.gender;
		cin.ignore();
		uppercase(depData.gender);
		if (depData.gender != 'M' && depData.gender != 'F')
		{
			cout << "Error: Invalid Input, Please Input M or F." << endl;
		}
	}

	return depData;
}

Dependant removeDependant() {
	Dependant depData = {"","","",0,'\0'};
	return depData;
}

	/*
	/=========================\
		History Functions
	\=========================/
	*/

void addVisit(Location* lTop, Human* currentLog, Dependant deps[MAXDEPS]) {
	Location* locVisited = NULL;
	string locCode = "";
	bool locNotFound = true;
	clear();

	while (locNotFound){
		cout << "Enter The visited Location's Code: " << endl;
		getline(cin, locCode);

		Location* navi = lTop;
		while (navi != NULL) {
			if (navi->code == locCode) {
				locVisited = navi;
				locNotFound = false;
				break;
			}
			navi = navi->nextLocation;
		}
		if (navi == NULL) {
			clear();
			cout << "Error: No Such Code Found, Please Try Again." << endl;
		}
	}

	ofstream addHistoryData(".\\History.txt", fstream::app);
	addHistoryData << currentLog->id << "^";

	for (int i = 0; i < MAXDEPS; i++) {
		if (deps[i].id != ""){
			addHistoryData << deps[i].id << "{" << deps[i].name << "{" << deps[i].relation << "{" << deps[i].age << "{" << deps[i].gender << "^";
		}
		else
		{
			addHistoryData << "^";
		}
	}
	addHistoryData << locVisited->code << endl;
	clear();
	cout << "Visitation To " << locVisited->name << " Has Added To The System" << endl;
	addHistoryData.close();
}

History* getVisitsFromFile(History* &bot, Human* currentLog, Location* lTop) {
	History* top = NULL;
	ifstream readHistory(".\\History.txt");
	string historyLine = "";
	bool fr = true;
	getline(readHistory, historyLine);

	while (!historyLine.empty()) {
		History* past = new History;
		int dataCount = 0;
		int depDataCount1 = 0, depDataCount2 = 0, depDataCount3 = 0;
		string simId = "";
		string age1 = "", age2 = "", age3 = "";
		string locId = "";

		for (int i = 0; i < historyLine.length(); i++) {
			if (historyLine[i] != '^') {
				simId += historyLine[i];
			}
			else {
				break;
			}
		}
		
		if (simId == currentLog->id)
		{
			past->person = currentLog;
			for (int i = 0; i < historyLine.length(); i++) {
				if (historyLine[i] != '^') {
					switch (dataCount) {
					case 0:
						break;
					case 1:
						if (historyLine[i] != '{') {
							switch (depDataCount1)
							{
							case 0:
								past->deps[0].id += historyLine[i];
								break;
							case 1:
								past->deps[0].name += historyLine[i];
								break;
							case 2:
								past->deps[0].relation += historyLine[i];
								break;
							case 3:
								age1 += historyLine[i];
								break;
							case 4:
								past->deps[0].age = stoi(age1);
								past->deps[0].gender = historyLine[i];
								break;
							default:
								break;
							}
						}
						else {
							depDataCount1++;
						}
						break;
					case 2:
						if (historyLine[i] != '{') {
							switch (depDataCount2)
							{
							case 0:
								past->deps[1].id += historyLine[i];
								break;
							case 1:
								past->deps[1].name += historyLine[i];
								break;
							case 2:
								past->deps[1].relation += historyLine[i];
								break;
							case 3:
								age2 += historyLine[i];
								break;
							case 4:
								past->deps[1].age = stoi(age2);
								past->deps[1].gender = historyLine[i];
								break;
							default:
								break;
							}
						}
						else {
							depDataCount2++;
						}
						break;
					case 3:
						if (historyLine[i] != '{') {
							switch (depDataCount3)
							{
							case 0:
								past->deps[2].id += historyLine[i];
								break;
							case 1:
								past->deps[2].name += historyLine[i];
								break;
							case 2:
								past->deps[2].relation += historyLine[i];
								break;
							case 3:
								age3 += historyLine[i];
								break;
							case 4:
								past->deps[2].age = stoi(age3);
								past->deps[2].gender = historyLine[i];
								break;
							default:
								break;
							}
						}
						else {
							depDataCount3++;
						}
						break;
					case 4:
						locId += historyLine[i];
						break;
					default:
						break;
					}
				}
				else {
					dataCount++;
				}
			}
			Location* navi = lTop;
			while (navi != NULL) {
				if (navi->code == locId){
					past->loc = navi;
					break;
				}
				navi = navi->nextLocation;
			}

			past->hNext = NULL;
			past->hPrev = NULL;
			if (fr) {
				top = past;
				bot = past;
				fr = false;
			}
			else {
				top->hPrev = past;
				past->hNext = top;
				top = past;
			}
		}

		getline(readHistory, historyLine);
	}

	readHistory.close();

	return top;
}

void displayHistory(History* histTop) {
	History* navi = histTop;
	clear();

	while (navi != NULL) {
		cout << "User ID & Name: \t" << navi->person->id << ", " << navi->person->name << endl;
		if (navi->deps[0].id != ""){
			cout << "Dependant Details: \t(ID, Name, Relation, Age, Gender)" << endl;
			cout << "\t\t\t" << navi->deps[0].id << ", " << navi->deps[0].name << ", " << navi->deps[0].relation << ", " << navi->deps[0].age << ", " << navi->deps[0].gender << endl;
		}
		if (navi->deps[1].id != "")
			cout << "\t\t\t" << navi->deps[1].id << ", " << navi->deps[1].name << ", " << navi->deps[1].relation << ", " << navi->deps[1].age << ", " << navi->deps[1].gender << endl;
		if (navi->deps[2].id != "")
			cout << "\t\t\t" << navi->deps[2].id << ", " << navi->deps[2].name << ", " << navi->deps[2].relation << ", " << navi->deps[2].age << ", " << navi->deps[2].gender << endl;
		cout << "Location Code & Name:\t" << navi->loc->code << ", " << navi->loc->name << endl;
		cout << endl;
		cout << endl;

		navi = navi->hNext;
	}

	cout << "Press Enter When Done Viewing" << endl;
	cin.ignore();
	clear();
}

int main() {
	/*
	/==========================\
	 Initialization & Variables
	\==========================/
	*/
	Human* hTop, *hBot;
	hTop = getHumansFromFile(hBot);

	Location* lTop, *lBot;
	lTop = getLocationsFromFile(lBot);

	Dependant currentDeps[MAXDEPS];
	int depsUsed = 0;

	string uIn = "randval";
	Human *currentLogin = NULL;
	while (uIn != EXITCODE){

		/*
		/=========================\
				Login System
		\=========================/
		*/
		while (uIn != EXITCODE && currentLogin == NULL){
			cout << "Enter 1 to login, 2 to register, or type 0 to exit the program" << endl;
			getline(cin, uIn, '\n');
			if (uIn == "2"){
				clear();
				RegisterHuman();
				hTop = getHumansFromFile(hBot);
			} else if (uIn == "1") {
				currentLogin = LoginHuman(hTop);
			} else if (uIn == "0") {
				uIn = EXITCODE;
			} else if (uIn == "935") {
				RegisterLocation();
				lTop = getLocationsFromFile(lBot);
			} else if (uIn == "936") {
				lTop = editLocation(lTop);
				rewriteLocations(lBot);
			} else {
				cout << "Error: Invalid Input Please Try again." << endl;
			}
		}
		clear();

		/*
		/=========================\
				After Login
		\=========================/
		*/

		while (uIn != EXITCODE && currentLogin != NULL) {
			int pick = 2;
			cout << "Logged In As: " << currentLogin->name << endl;
			cout << "Dependants: ";
			for (int i = 0; i < MAXDEPS; i++){
				if (currentDeps[i].id != ""){
					cout << currentDeps[i].name;
					if (i != MAXDEPS - 1){
						cout << ", ";
					}
				}
			}
			cout << "\nPlease Select One Of The Following Services. \n0   Logout. \n1   Visit Location. \n2   Add or Remove Dependants. \n3   Edit Personal Information. \n4   View History" << endl;
			cin >> pick;
			cin.ignore();
			switch (pick)
			{
			case 0: //Logout
				clear();
				cout << "Logging Out..." << endl;
				currentLogin = NULL;
				break;
			case 1: //Visit
				addVisit(lTop,currentLogin,currentDeps);
				break;
			case 2: //Dependant Control
			{
				bool notOutYet = true;
				int uPick = 0;
				clear();
				while (notOutYet) {
					cout << "Pick a function: \n0   Go Back.\n1   Add Dependant.\n2   Remove Latest Dependant." << endl;
					cin >> uPick;
					cin.ignore();

					switch (uPick) {
					case 0:
						clear();
						notOutYet = false;
						break;
					case 1:
					{
						for (int i = 0; i < MAXDEPS; i++) {
							if (currentDeps[i].id != "") {
								if (i == MAXDEPS - 1) {
									clear();
									cout << "No More Dependants Can Be Added." << endl;
								}
								continue;
							}
							currentDeps[i] = addDependant();
							depsUsed++;
							break;
						}
					}
						clear();
						break;
					case 2: 
					{
						clear();
						cout << "Removed Dependant: " << currentDeps[--depsUsed].name << endl;
						currentDeps[depsUsed] = removeDependant();
					}	
						break;
					default:
						clear();
						cout << "Error: Invalid Input, Please Try Again." << endl;
						break;
					}
				}
			}
				break;
			case 3: //Edit Personal Info
				editHuman(currentLogin);
				rewriteHumans(hTop);
				break;
			case 4: //History
				currentLogin->topHistory = getVisitsFromFile(currentLogin->botHistory,currentLogin,lTop);
				displayHistory(currentLogin->topHistory);
				break;
			default:
				clear();
				cout << "Error: Invalid Input Please Try Again." << endl;
				break;
			}

		}
	}

	rewriteLocations(lBot);
	rewriteHumans(hBot);
}