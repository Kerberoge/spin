#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <termios.h>
#include <sys/time.h>

#define WIDTH 100
#define HEIGHT 100
#define SHAPE_WIDTH 20
#define SHAPE_HEIGHT 20
#define ANGLE 2
#define SLEEP_TIME 0.1

typedef struct {
	float x;
	float y;
} Point;

char bg_buffer[HEIGHT * WIDTH];
char fg_buffer[HEIGHT * WIDTH];
const char fg_c = '@';
const char bg_c = '.';
int done = 0;
Point point;

void fill_bg_buffer(const char c)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			bg_buffer[i * WIDTH + j] = c;
		}
	}
}

void print_bg_buffer(void)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			printf("%c ", bg_buffer[i * WIDTH + j]);
		}
		printf("\n");
	}
}

void print_shape_on_fg_buffer(void)
{
	int offset_x = WIDTH / 2 - SHAPE_WIDTH / 2;
	int offset_y = HEIGHT / 2 - SHAPE_HEIGHT / 2;

	for (int i = 0; i < SHAPE_WIDTH; i++)
	{
		for (int j = 0; j < SHAPE_HEIGHT; j++)
		{
			fg_buffer[ (offset_y + i * WIDTH) + offset_x + j] = fg_c;
		}
	}
}

Point rotate_point ( const Point p )
{
	// convert to radians
	float angle = ( (float) ANGLE / 180 ) * M_PI;
	
	Point new_p;
	new_p.x = p.x * cos(angle) - p.y * sin(angle);
	new_p.y = p.x * sin(angle) + p.y * cos(angle);

	return new_p;
}

void set_point ( const Point p )
{
	int col = WIDTH / 2 + round(p.x);
	int row = HEIGHT / 2 - round(p.y);
	old_shape_buffer[row][col] = fg_c;
}


void* exit_on_keypress(void* ptr)
{
	if ( getc(stdin) )
	{
		done = 1;
	}

	return NULL;
}

void* draw_buffer(void* ptr)
{
	while (!done)
	{
		system("clear");

		point = rotate_point(point);
		set_point(point);
	
		print_buffer();
		
		struct timespec ts;
		ts.tv_sec = (int) SLEEP_TIME;
		ts.tv_nsec = ( SLEEP_TIME - ts.tv_sec ) * 1e+9;
		nanosleep(&ts, NULL);
	}

	return NULL;
}

int main()
{
	// this code makes it possible to quit the program when any key is pressed
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
/*
	fill_buffer(bg_c);

	point.x = 40;
	point.y = 0;

	pthread_t terminate_thread, draw_thread;
	pthread_create(&terminate_thread, NULL, exit_on_keypress, NULL);
	pthread_create(&draw_thread, NULL, draw_buffer, NULL);
	pthread_join(draw_thread, NULL);
*/

	print_shape_on_buffer();


	// reset terminal settings
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return 0;
}
