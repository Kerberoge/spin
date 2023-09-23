#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define WIDTH 50
#define HEIGHT 50
#define SIZE 20
#define ANGLE 45
#define ANGLE_SPACING 2.5
#define SLEEP 0.1

char fg_c = '@';
char bg_c = '.';
char buffer[HEIGHT * WIDTH];

float angle_deg = ANGLE;
float radius;

float deg_to_rad(float deg)
{
	return deg / 180 * M_PI;
}

void calculate_radius(void)
{
	radius = (float) 0.5 * ( sqrt(pow(SIZE, 2) + pow(SIZE, 2)) );
}

void fill_buffer(void)
{
	for (int i = 0; i < HEIGHT * WIDTH; i++)
	{
		buffer[i] = bg_c;
	}
}

void print_buffer(void)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			printf("%c ", buffer[i * WIDTH + j]);
		}
		printf("\n");
	}
}

char get_char_at(int x, int y)
{
	int row = HEIGHT / 2 - y;
	int col = WIDTH / 2 + x - 1;

	if ( row >= HEIGHT ) { printf("error: y value too small\n"); return 0; }
	if ( row < 0 ) { printf("error: y value too big\n"); return 0; }
	if ( col >= WIDTH ) { printf("error: x value too big\n"); return 0; }
	if ( col < 0 ) { printf("error: x value too small\n"); return 0; }

	return buffer[row * WIDTH + col];
}

void set_char_at(int x, int y, char c)
{
	int row = HEIGHT / 2 - y;
	int col = WIDTH / 2 + x - 1;

	if ( row >= HEIGHT ) { printf("error: y value too small\n"); return; }
	if ( row < 0 ) { printf("error: y value too big\n"); return; }
	if ( col >= WIDTH ) { printf("error: x value too big\n"); return; }
	if ( col < 0 ) { printf("error: x value too small\n"); return; }

	buffer[row * WIDTH + col] = c;
}



float f(int x, int y)
{
	float a = deg_to_rad(angle_deg);
	return fabs( x * cos(a) - y * sin(a) ) + fabs( x * sin(a) + y * cos(a) );
}

void draw_in_buffer(void)
{
	for (int x = -24; x <= 25; x++)
	{
		for (int y = -24; y <= 25; y++)
		{
			if ( round( f(x,y) ) == round(radius) )
			{
				set_char_at(x, y, fg_c);
			}
		}
	}
}



int main()
{
	calculate_radius();

	struct timespec t;
	t.tv_sec = (int) SLEEP;
	t.tv_nsec = ( SLEEP - t.tv_sec ) * 1e9;

	for (;;)
	{
		system("clear");
		fill_buffer();
		draw_in_buffer();
		angle_deg += ANGLE_SPACING;

		print_buffer();
		nanosleep(&t, NULL);
	}

	return 0;
}
