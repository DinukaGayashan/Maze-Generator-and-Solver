#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <fstream>
#include <string>

using namespace std;

int height;
int width;
char road = '0';
char path = '#';

vector <char> col;
vector <vector<char>> maze;

class Point
{
public:
	int y, x;
	Point(int h = 0, int w = 0) : y(h), x(w) {}
};

//fuction to detect the size of the maze
void size_detector()
{
	ifstream maze_file;
	maze_file.open("generated_maze.txt");
	string line;
	getline(maze_file, line);
	height = line.length() / 2;
	width = line.length() / 2;
	col.resize(width);
	maze.resize(height, col);
	maze_file.close();
}

//function to read the maze
void read_maze()
{
	size_detector();

	ifstream maze_file;
	maze_file.open("generated_maze.txt");

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
			maze_file >> maze[h][w];
	}
}

void print_maze()
{
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
			cout << maze[h][w] << " ";
		cout << endl;
	}
}

//function to replace visited points with walls
void correct_maze()
{
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
			if (maze[h][w] == ' ')
				maze[h][w] = road;
	}
}

//function to save tha maze to a text file
void save_maze()
{
	ofstream maze_file;
	maze_file.open("solved_maze.txt");
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
			maze_file << maze[h][w] << " ";
		maze_file << endl;
	}
	cout << "\nSolved maze saved to \"solved_maze.txt\".\n";
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
	maze[visited_points.top().y][visited_points.top().x] = ' ';
	visited_points.pop();
	if (visited_points.empty())
		return;
	
	p = visited_points.top();

	search(p);
}

//function to search through roads and find ending point
void search(Point p)
{
	if (maze[p.y][p.x + 1] == 'B' || maze[p.y][p.x - 1] == 'B' || maze[p.y + 1][p.x] == 'B' || maze[p.y - 1][p.x] == 'B')
		return;

	vector <Point> available_points;

	if (p.y + 2 < height && maze[p.y + 1][p.x] == road)
		available_points.push_back(Point(p.y + 1, p.x));
	if (p.y > 1 && maze[p.y - 1][p.x] == road)
		available_points.push_back(Point(p.y - 1, p.x));
	if (p.x + 2 < width && maze[p.y][p.x + 1] == road)
		available_points.push_back(Point(p.y, p.x + 1));
	if (p.x > 1 && maze[p.y][p.x - 1] == road)
		available_points.push_back(Point(p.y, p.x - 1));

	if (available_points.size() == 0)
	{
		back_track(p);
		return;
	}

	int r = rand() % (available_points.size());

	maze[available_points[r].y][available_points[r].x] = path;
	visited_points.push(Point(available_points[r].y, available_points[r].x));
		
	search(Point(available_points[r].y, available_points[r].x));
}

//function to search the starting point
Point find_start()
{
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
			if (maze[h][w] == 'A')
				return Point(h, w);
	}
}

int main()
{
	srand((unsigned)time(NULL));

	cout << "Maze Solver\n\n";

	read_maze();

	Point p = find_start();
	search(p);

	correct_maze();
	print_maze();
	save_maze();

	return 0;
}