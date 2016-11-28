#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

 
#define IN  0
#define OUT 1
 
#define LOW  0
#define HIGH 1
 
#define PIN  192 
#define POUT 202
#define PIN2  192 
#define POUT2 201
//#define PIN2 195
//#define POUT2 194
//#define PIN3 196
//#define POUT3 195
//#define PIN4 198
//#define POUT4 197
//#define PIN5 199
//#define POUT5 193

static int
GPIOExport(int pin)
{
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if ( 1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(1);
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}
 
static int
GPIOUnexport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (2 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(2);
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}
 
static int
GPIODirection(int pin, int dir)
{
	static const char s_directions_str[]  = "in\0out";
 
#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;
 
	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}
 
	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}
 
	close(fd);
	return(0);
}
 
static int
GPIORead(int pin)
{
#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}
 
	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}
 
	close(fd);
 
	return(atoi(value_str));
}
 
static int
GPIOWrite(int pin, int value)
{
	static const char s_values_str[] = "01";
 
	char path[VALUE_MAX];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}
 
	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}
 
	close(fd);
	return(0);
}
 

		  
int main(int argc, char *argv[])
{
    int user_command;
	int repeat = 100; 
	int motor_speed_a = 1500; // 1.5ms for the motors to work  
	int motor_speed_b = 1500; 
	while (1) {
	    while (1) {
		    printf("Enter a correct value \n:");
            user_command = getchar( );
            putchar( user_command );
            if (user_command == 'w') {
                printf("Go\n" + user_command);
                repeat = 100;
                motor_speed_a = 2000;
                motor_speed_b = 2000;
                break;
                
            }
            if (user_command == 'a') {
                repeat = 100;
                motor_speed_a = 1000;
                motor_speed_b = 1000;
                break;
            }
            if (user_command == 's') {
                repeat = 50;
                motor_speed_a = 1000;
                motor_speed_b = 2000;
                break;
            }
            if (user_command == 'd') {
                repeat = 100;
                motor_speed_a = 1000;
                motor_speed_b = 2000;
                
                break;
            }
            //this will exit the program and it's repeat one time only  
            if (user_command == 'x') {
                repeat = 1;
                break;
            }
            //if the value enterd not the same it will print wrong value
           printf( "Wrong value\n");
	   }

     	while (repeat--) {
     	    //forward
     	    if ( user_command == 'w') {
     	        GPIOWrite(POUT, 1);
     	        GPIOWrite(POUT2, 1);
     	        usleep(motor_speed_b);
     	        GPIOWrite(POUT2, 0);
     	        usleep(motor_speed_a - motor_speed_b);
     	        GPIOWrite(POUT, 0);
     	        usleep(20000-motor_speed_a);
     	    }
     	    //back
     	    if ( user_command == 's') {
                GPIOWrite(POUT, 1);
     	        GPIOWrite(POUT2, 1);     	        
     	        usleep(motor_speed_a);  
     	        GPIOWrite(POUT, 0);
     	        GPIOWrite(POUT2, 0);
     	        usleep(20000-motor_speed_a);
     	    }
     	    //right
     	     if (user_command == 'd') {
     	         GPIOWrite(POUT, 1);
     	        GPIOWrite(POUT2, 1);
     	        usleep(motor_speed_a);
     	        GPIOWrite(POUT, 0);
     	        usleep(motor_speed_b - motor_speed_a);
     	        GPIOWrite(POUT2, 0);
     	        usleep(20000-motor_speed_a);
     	    }
     	    //left
     	     if (user_command == 'a') {
     	      GPIOWrite(POUT2, 1);
     	       GPIOWrite(POUT, 1);
     	        usleep(motor_speed_b);
     	        GPIOWrite(POUT2, 1);
     	        GPIOWrite(POUT, 1);
     	        usleep(20000-motor_speed_b);
     	    }
     	    // exit the program
     	    if ( user_command == 'x') { 
     	        return 0;
     	    }
		    //reading the value in pins
		   // printf("I'm reading %d in GPIO %d\n", GPIORead(PIN), PIN);
		   // printf("I'm reading %d in GPIO %d\n", GPIORead(PIN2), PIN2);
		    
             
	    }
	}
}
	
    

 






