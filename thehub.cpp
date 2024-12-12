#include "rooms.cpp"
#include <cstdio>
#include <ncurses.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>		//contains the code for errno
#include <cstring> 		//contains the code for strerror

#include <chrono>
#include <thread>

#include <string>
#include <cstdlib>

#include <vector>

const int REQUEST_SIZE = 100;

int startswith(const char* s1, const char* s2, int prefixSize){
	for(int i = 0;i<prefixSize;i++){
		if(s1[i] != s2[i]){
			return 0;
		}
	}
	return 1;
}

int main(){
	using namespace std::chrono_literals;
	using std::string;
	using std::vector;

	string path = getenv("HOME");
	path += "/hub/hubrequests";
	const char* fifoname = path.c_str();
	int failed = mkfifo(fifoname, 0666);

	if(failed){
		printf("Error code %d\n", failed);
		printf("Error on making fifo\n");
		printf("%s\n", std::strerror(errno));
		return failed;
	}

	initscr();
	nodelay(stdscr, true);

	int fd = open(fifoname, O_RDONLY | O_NONBLOCK);
	char data[REQUEST_SIZE+1];
	rooms r;
	
	int running = 1;
	while(running){
		clear();
		printw("file available on %s\n", fifoname);
		printw("===========================\n%s\n", r.to_string().c_str());
		printw("press q to exit");
		fflush(stdout);
		refresh();
		char c = getch();
		if(c == 'q'){
			running = false;
		}
		else{
			int bytes_read = read(fd, data, REQUEST_SIZE);
			if(bytes_read > 0){
				//TODO
				data[bytes_read] = '\0';
				string command = data;
				if(startswith(data, "createroom", 10)){
					string roomname = command.substr(11);
					r.makeRoom(roomname);
				}
			}
			std::this_thread::sleep_for(500ms);
		}
	}
			

	unlink(fifoname);
	endwin();
	return 0;
}

