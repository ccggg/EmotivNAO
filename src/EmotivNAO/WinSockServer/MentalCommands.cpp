#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

#if __linux__ || __APPLE__
#include <unistd.h>
int _kbhit(void);
#endif

#include <map>
#include <cassert>

#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"

#include "MentalCommands.h"

#ifdef __APPLE__
int _kbhit(void)
{
	struct timeval tv;
	fd_set rdfs;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&rdfs);
	FD_SET(STDIN_FILENO, &rdfs);

	select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &rdfs);
}
#endif

#ifdef __linux__
int _kbhit(void)
{
	struct timeval tv;
	fd_set read_fd;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&read_fd);
	FD_SET(0, &read_fd);

	if (select(1, &read_fd, NULL, NULL, &tv) == -1)
		return 0;

	if (FD_ISSET(0, &read_fd))
		return 1;

	return 0;
}
#endif


using namespace std;

typedef unsigned long ulong;

void handleMentalCommandEvent(ostream& os, EmoEngineEventHandle MentalCommandEvent);
void loadProfile(int userID);
void showTrainedActions(int userID);
void showCurrentActionPower(EmoStateHandle);
void trainMentalCommandActions(int headsetID);
void setActiveActions(int userID);
void setMentalCommandActions(int, IEE_MentalCommandAction_t);
int actionTraining;

bool mentalCommandsOn = true;

/* Saving and Loading directories for the Mental Data */
string profileNameForLoading = "G:/MentalData/1/161.emu";
string profileNameForSaving = "G:/MentalData/1/161.emu";

char action = 'n';
float actionPower = 0;

void startMental(int selectedOption) {
	/*
	Start the EmoEngine and the EmoState
	*/
	EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
	EmoStateHandle eState = IEE_EmoStateCreate();

	// Unique ID for the current user, should be changed if different people are training the headset.
	unsigned int userID = 5;
	const unsigned short composerPort = 1726;
	int option = selectedOption;
	int state = 0;
	bool isUserAdded = false;
	string input;
	actionTraining = 0;

	/*cout << "1. Display current Mental Actions" << "\n";
	cout << "2. Train Mental Actions" << "\n";
	cout << ">> ";


	getline(cin, input, '\n');
	option = atoi(input.c_str());*/

	if (IEE_EngineConnect() != EDK_OK) {
		throw runtime_error("Emotiv Engine start up failed.");
	}

	while (!_kbhit()) {
		state = IEE_EngineGetNextEvent(eEvent);

		// New event needs to be handled
		if (state == EDK_OK) {

			IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
			IEE_EmoEngineEventGetUserId(eEvent, &userID);

			switch (eventType) {
			case IEE_UserAdded: {
				cout << endl << "New user " << userID << " added" << endl;

				if (option == 1) {
					loadProfile(userID);
					showTrainedActions(userID);
				}

				if (option == 2) {
					setActiveActions(userID);
					setMentalCommandActions(userID, MC_NEUTRAL);
				}

				break;
			}

			case IEE_UserRemoved: {
				cout << endl << "User " << userID << " has been removed." << endl;
				break;
			}

			case IEE_EmoStateUpdated: {
				IEE_EmoEngineEventGetEmoState(eEvent, eState);
				if (option == 1)
					showCurrentActionPower(eState);

				break;
			}

									  // Handle MentalCommand training related event
			case IEE_MentalCommandEvent: {
				handleMentalCommandEvent(cout, eEvent);
				//trainMentalCommandActions(userID);
				break;
			}
			default:
				break;
			}
		}
	}

	/*
	Program is now closing so close the engine and any running events.
	*/
	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);
}

const char *byte_to_binary(long x) {
	static char b[9];
	b[0] = '\0';

	int z;
	for (z = 8192; z > 0; z >>= 1) {
		strcat(b, ((x & z) == z) ? "1" : "0");
	}

	return b;
}

/*
Load profile from the local directory
*/
void loadProfile(int userID) {
	if (IEE_LoadUserProfile(userID, profileNameForLoading.c_str()) == EDK_OK)
		cout << "Load Profile : done" << endl;
	else
		throw runtime_error("Can't load profile.");
}

void showTrainedActions(int userID) {
	unsigned long pTrainedActionsOut = 0;
	IEE_MentalCommandGetTrainedSignatureActions(userID, &pTrainedActionsOut);

	cout << "Trained Actions" << " : " << byte_to_binary(pTrainedActionsOut) << "\n";
}

void showCurrentActionPower(EmoStateHandle eState) {
	IEE_MentalCommandAction_t eeAction = IS_MentalCommandGetCurrentAction(eState);
	float currActionPower = IS_MentalCommandGetCurrentActionPower(eState);
	actionPower = currActionPower;

	switch (eeAction) {
		case MC_NEUTRAL: { cout << "Neutral" << " : " << currActionPower << "; \n"; action = 'n'; break; }
		case MC_PUSH: { cout << "Push" << " : " << currActionPower << "; \n"; action = 'p';  break; }
		case MC_PULL: { cout << "Pull" << " : " << currActionPower << "; \n"; action = 'l'; break; }
	}
}

void setActiveActions(int userID) {
	ulong action1 = (ulong)IEE_MentalCommandAction_t::MC_PUSH;
	ulong action2 = (ulong)IEE_MentalCommandAction_t::MC_PULL;
	ulong listAction = action1 | action2;

	int errorCode = EDK_OK;
	errorCode = IEE_MentalCommandSetActiveActions(userID, listAction);

	if (errorCode == EDK_OK) {
		cout << "Setting MentalCommand active actions Neutral, Push and Pull for user " << userID << endl;
	}
	else cout << "Setting MentalCommand active actions error: " << errorCode;
}

void setMentalCommandActions(int headsetID, IEE_MentalCommandAction_t action) {
	int errorCode = IEE_MentalCommandSetTrainingAction(headsetID, action);
	errorCode = IEE_MentalCommandSetTrainingControl(headsetID, MC_START);

	switch (action) {
	case MC_NEUTRAL: { cout << endl << "TRAINING NEUTRAL" << endl; break; }
	case MC_PUSH: { cout << endl << "TRAINING PUSH" << endl; break; }
	case MC_PULL: { cout << endl << "TRAINING PULL" << endl; break; }
	}
}

void trainMentalCommandActions(int headsetID) {
	if (actionTraining == 1) {
		setMentalCommandActions(headsetID, MC_PUSH);
		//setMentalCommandActions(headsetID, MC_PULL);
		//actionTraining++;
		return;
	}
	else if (actionTraining == 2) {
		setMentalCommandActions(headsetID, MC_PULL);
		return;
	}

	if (IEE_SaveUserProfile(headsetID, profileNameForSaving.c_str()) == EDK_OK) {
		cout << endl << "PROFILE SAVED";
	}
	else cout << endl << "ERROR SAVING PROFILE";
}

void handleMentalCommandEvent(ostream& os, EmoEngineEventHandle MentalCommandEvent) {

	unsigned int userID = 0;
	IEE_EmoEngineEventGetUserId(MentalCommandEvent, &userID);
	IEE_MentalCommandEvent_t eventType = IEE_MentalCommandEventGetType(MentalCommandEvent);

	switch (eventType) {

	case IEE_MentalCommandTrainingStarted: {
		os << endl << "MentalCommand training for user " << userID << " STARTED!" << endl;
		break;
	}

	case IEE_MentalCommandTrainingSucceeded: {
		os << endl << "MentalCommand training for user " << userID << " SUCCEEDED!" << endl;
		IEE_MentalCommandSetTrainingControl(userID, MC_ACCEPT);
#ifdef _WIN32
		Sleep(3000);
#endif
#if linux || __APPLE__
		sleep(3);
#endif
		break;
	}

	case IEE_MentalCommandTrainingFailed: {
		os << endl << "MentalCommand training for user " << userID << " FAILED!" << endl;
		break;
	}

	case IEE_MentalCommandTrainingCompleted: {
		os << endl << "MentalCommand training for user " << userID << " COMPLETED!" << endl;
		actionTraining++;
		cout << actionTraining << endl;
		trainMentalCommandActions(userID);
		break;
	}

	case IEE_MentalCommandTrainingRejected: {
		os << endl << "MentalCommand training for user " << userID << " REJECTED!" << endl;
		break;
	}

	case IEE_MentalCommandTrainingReset: {
		os << endl << "MentalCommand training for user " << userID << " RESET!" << endl;
		break;
	}

	case IEE_MentalCommandAutoSamplingNeutralCompleted: {
		os << endl << "MentalCommand auto sampling neutral for user " << userID << " COMPLETED!" << endl;
		break;
	}

	case IEE_MentalCommandSignatureUpdated: {
		os << endl << "MentalCommand signature for user " << userID << " UPDATED!" << endl;
		break;
	}

	case IEE_MentalCommandNoEvent:
		break;

	default: {
		assert(0);
		break;
	}
	}
}