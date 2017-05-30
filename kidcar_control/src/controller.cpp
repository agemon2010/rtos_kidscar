#include <stdio.h>
#include <termios.h>

#include "ros/ros.h"
#include "kidcar_control/keyop.h"


static struct termios old, new_;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new_ = old; /* make new settings same as old settings */
  new_.c_lflag &= ~ICANON; /* disable buffered i/o */
  new_.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new_); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}

/* Let's test it out */
/*
int main(void) {
  char c;
  printf("(getche example) please type a letter: ");
  c = getche();
  printf("\nYou typed: %c\n", c);
  printf("(getch example) please type a letter...");
  c = getch();
  printf("\nYou typed: %c\n", c);
  return 0;
}
*/



int main(int argc, char **argv) {
	int angle = 0;
	int velocity = 0;
	int input_char;
	kidcar_control::keyop key_cmd;

	ros::init(argc, argv, "key_controller");
	ros::NodeHandle node_handler;
	
	ros::Publisher keyop_pub = node_handler.advertise<kidcar_control::keyop>("kidcar_cmd", 1);

	while(1) {
		input_char = getch();
		//printf("input charactor: %d\n", input_char);

		switch(input_char) {
			case 'w':
				velocity+=1;		
				break;
			case 's':
				velocity-=1;
				break;
			case 'a':
				angle-=1;
				break;
			case 'd':
				angle+=1;
				break;
			case 3:
				printf("Quit the program\n");
				break;
			default:
				printf("Undefined input!\n");
				break;
		} 

		if(input_char == 'q') break;

		key_cmd.angle = angle;
		key_cmd.velocity = velocity;
		
		keyop_pub.publish(key_cmd);			

		//ROS_INFO();
		printf("[current state] angle:%d, velocity:%d\n", angle, velocity); 
		
		
	}

	return 0;
}

