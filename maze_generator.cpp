#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <fstream>

using namespace std;

char wall = '1';
char road = '0';
int height;
int width;
int dimension;

vector <char> col;
vector <vector<char>> maze;

class Point
{
public:
	int y, x;
	Point(int h = 0, int w = 0) : y(h), x(w) {}
};

//function to take maze size as input
void input_dimension()
{
	//the size of a maze should be an odd value larger than 4
	do
	{
		cout << "Input dimension : ";
		cin >> dimension;
		if (dimension % 2 == 0)
			cout << "\t*Dimention of a square maze should be odd values\n";
		if (dimension < 5)
			cout << "\t*Dimention of a square maze should be higher than value 4\n";
	} while (dimension % 2 == 0 || dimension < 5);
	col.resize (dimension);
	maze.resize(dimension, col);
	height = dimension;
	width = dimension;
}

void print_maze()
{
	cout << endl;
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
			cout << maze[h][w] << " ";
		cout << endl;
	}
}

//function to save maze to a text file
void save_maze()
{
	ofstream maze_file;
	maze_file.open("generated_maze.txt");
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
			maze_file << maze[h][w] << " ";
		maze_file << endl;
	}
	cout << "\nGenerated maze saved to \"generated_maze.txt\".\n";
}

int random_number(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

stack <Point> visited_points;

void search(Point p);

//function to back track
void back_track(Point p)
{
	visited_points.pop();
	if (visited_points.empty())
		return;

	visited_points.pop();
	if (visited_points.empty())
		return;

	p = visited_points.top();

	search(p);
}

//function for searching pair of points and making the road
void search(Point p)
{
	vector <Point> available_points;

	if (p.y + 2 < height && maze[p.y + 1][p.x] == wall && maze[p.y + 2][p.x] == wall)
	{
		available_points.push_back(Point(p.y + 1, p.x));
		available_points.push_back(Point(p.y + 2, p.x));
	}
	if (p.y > 1 && maze[p.y - 1][p.x] == wall && maze[p.y - 2][p.x] == wall)
	{
		available_points.push_back(Point(p.y - 1, p.x));
		available_points.push_back(Point(p.y - 2, p.x));
	}
	if (p.x + 2 < width && maze[p.y][p.x + 1] == wall && maze[p.y][p.x + 2] == wall)
	{
		available_points.push_back(Point(p.y, p.x + 1));
		available_points.push_back(Point(p.y, p.x + 2));
	}
	if (p.x > 1 && maze[p.y][p.x - 1] == wall && maze[p.y][p.x - 2] == wall)
	{
		available_points.push_back(Point(p.y, p.x - 1));
		available_points.push_back(Point(p.y, p.x - 2));
	}

	if (available_points.size() == 0) 
	{
		back_track(p);
		return;
	}

	int r = random_number(0, (available_points.size() / 2) - 1);
	
	maze[available_points[2 * r].y][available_points[2 * r].x] = road;
	visited_points.push(Point(available_points[2 * r].y, available_points[2 * r].x));
	maze[available_points[2 * r + 1].y][available_points[2 * r + 1].x] = road;
	visited_points.push(Point(available_points[2 * r + 1].y, available_points[2 * r + 1].x));

	search(Point(available_points[2 * r + 1].y, available_points[2 * r + 1].x));
}

void maze_generator()
{
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
			maze[h][w] = wall;
	}

	Point a;
	Point b;

	//assigning start point to a random odd location
	do
	{
		a.x = random_number(1, width / (width / 4 + 1));
	} while (a.x % 2 == 0);
	do
	{
		a.y = random_number(1, height / (height / 4 + 1));
	} while (a.y % 2 == 0);
	
	maze[a.y][a.x] = 'A';

	search(a);

	//assigning end point to a random odd location
	do
	{
		b.x = random_number((width / 4) * width / (width / 4 + 1), width - 1);
	} while (b.x % 2 == 0);
	do
	{
		b.y = random_number((height / 4) * height / (height / 4 + 1), height - 1);
	} while (b.y % 2 == 0);
	
	maze[b.y][b.x] = 'B';
}

int main()
{
	srand((unsigned)time(NULL));

	cout << "Maze Generator\n\n";

	input_dimension();
	
	maze_generator();
	
	print_maze();
	save_maze();

	return 0;
}