#include <iostream>
#include <cerrno>
#include <vector>
#include <string>
#include <cstring>
#include <array>
#include <cassert>
#include <sstream>
#include <cstdlib>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

const int MAX_ROOMS = 100;

string getHubPath(const string& name);

class rooms{

private:
	vector<int> subscriberInRoom;
	vector<string> nameOfRoom;
	array<int, MAX_ROOMS> isNotAvailable{0};

public:
	~rooms(){
		//if any rooms(fifos) are made, delete them
		int foundSome = 0;
		for(int i = 0;i<MAX_ROOMS;i++){
			if(isNotAvailable[i]){
				//left for final output
				if(!foundSome){
					cout << "Left some rooms opened:" << endl;
					foundSome = 1;
				}
				cout << "\t" << nameOfRoom[i] << endl;
				int succeeded = destroyRoom(nameOfRoom[i]);	
				if(!succeeded){
					cout << "Was unable to delete FIFO" << endl;
					cout << strerror(errno) << endl;
				}
			}
		}

	}
	
	int getAvailableRoom(){
		for(int i = 0;i<MAX_ROOMS;i++){
			if(!isNotAvailable[i]){
				return i;
			}
		}
		return -1;
	}

	int getRoomIndex(const string& name){
		for(int i = 0;i<MAX_ROOMS;i++){
			if(isNotAvailable[i] && nameOfRoom[i] == name){
				return i;
			}
		}
		return -1;
	}

	int makeRoom(string& name){
		int index = getAvailableRoom();
		assert(index >= 0 && "All rooms are full, could not create room");

		int notsuccess;
		string fifopath = getHubPath(name);
		notsuccess = mkfifo(fifopath.c_str(), 0666);
		if(notsuccess){
			return -1;
		}

		while(index > nameOfRoom.size()){
			nameOfRoom.push_back("");
			subscriberInRoom.push_back(0);
		}
		nameOfRoom.push_back(name);
		subscriberInRoom.push_back(0);
		isNotAvailable[index] = 1;	

		return 1;
	}

	int destroyRoom(string& name){
		int roomIndex = getRoomIndex(name);
		assert(roomIndex >= 0 && "Room is not found");

		//cout << "We found j" << endl;
		//cout << "the name is " << name << endl;


		//clear the fifo before we clear the string
		//cout << "The hub path is : " << getHubPath(name) << endl;
		int failCode = unlink(getHubPath(name).c_str());

		//ensure our data is reset
		isNotAvailable[roomIndex] = 0;
		nameOfRoom[roomIndex].clear();
		subscriberInRoom[roomIndex] = 0;

		return !failCode;
	}

	int connectSubscriber(string& roomName){
		int index = getRoomIndex(roomName);
		assert(index >= 0 && "Room is not found");

		subscriberInRoom[index] = 1;
		return 1;
	}

	int disconnectSubscriber(string& roomName){
		int index = getRoomIndex(roomName);
		assert(index >= 0 && "Room is not found");

		subscriberInRoom[index] = 1;
		return 1;
	}
	
	int subscriberIsListening(string& roomName){
		int index = getRoomIndex(roomName);
		assert(index >= 0 && "Room is not found");

		
		return subscriberInRoom[index];
	}


	string to_string(){
		ostringstream os;
		os << "Rooms available:\n";
		for(int i = 0;i<MAX_ROOMS;i++){
			//cout << isNotAvailable[i] << endl;
			if(!isNotAvailable[i]){ continue; }
			os << "\t" << nameOfRoom[i] << "    ";
			if(subscriberInRoom[i]) { os << "0"; }
			else { os << "X"; }
			os << "\n";
		}
		return os.str();
	}

};
