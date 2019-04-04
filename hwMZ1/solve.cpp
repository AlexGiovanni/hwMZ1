//Alexis Santiago  November 10, 2018


#include <vector>
#include <string>
#include <unordered_set>
#include <queue>
#include <unordered_map>
#include <iostream>
#include "vertex.h"

using namespace std;
//hash table under the hood. list of vertices(pointers to Vertex objects)
unordered_map<string, Vertex*> vertexSet;
//Algorithm tools
unordered_map<Vertex*, Vertex*> breadCrumbs;
int rc[2];//stores the number of rows and columns in array

//finds the number of columns and rows and returns an array containing those values row,col
void findRowCol(string maze) {
	rc[0] = 0; rc[1] = 0;
	for (int i = 0; i < maze.size() - 1; i++) {//find #of row and col
		if (maze[i] == '\n') {
			rc[1] = i+1;//find # of col
			break;
		}
	}
	//cout << "colum: " << rc[1] << endl;
	//rc[0] = (maze.size() / rc[1] + 1) - 2;//#of rows
	rc[0] = (maze.size() / rc[1] ) -1;//#of rows
	rc[1] = rc[1] - 2;//# of columns
	//cout <<"row: "<< rc[0]<<" column: " << rc[1] <<" maze.size(): "<<maze.size()<< endl;
}


//Add a basic (bidirectional) edge connecting a and b
//each string is a coordinate x,y
void addEdge(string a, string b)
{
	Vertex * aVert = vertexSet[a];//find object to a vertex create a pointer to it
	Vertex * bVert = vertexSet[b];
	//avert point to vertex a //bvert points to vertex b
	aVert->neighs.push_back(bVert);//add b to the list of a's neighbors
	bVert->neighs.push_back(aVert);//add a to the list of b's neighbors
}



//checks if im on the border of the maze
bool inBorder(string maze, int r, int c) {
	if (r == 0 || c == 0 || r == rc[0] || c == rc[1]) {
		return true;
	}
	return false;
}

//check if there is a vertex on top
//if the vertex i'm serching for exists in vertexset
bool checkTop(int r, int c) {
	string key = to_string(r-1) + "," + to_string(c);
	if (vertexSet.find(key) == vertexSet.end()) {//not a vertex
		return false;
	}
	return true;
}

//check if there is a vertex on its left
bool checkLeft(int r, int c) {
	string key = to_string(r) + "," + to_string(c-1);
	if (vertexSet.find(key) == vertexSet.end()) {//not a vertex
		return false;
	}
	return true;
}

//Bread First Search!
void breadthFirstSearch(string source)
{
	//cout << "in BFS" << endl;
	//Variables, tools, etc.
	queue<Vertex*> Q;
	unordered_set<Vertex*> marked;

	//step 0:sets s to the starting vertex
	Vertex * s = vertexSet[source];//set starting vertex

	//step 1: Mark s, put it in Q
	marked.insert(s);//inserts into marked list
	Q.push(s);

	//step 2: The BFS loop!
	while (!Q.empty())
	{
		//2.1 Remove top item from Q
		Vertex * x = Q.front();
		Q.pop();

		//2.2 For each unvisited neighbor y of x,
		//mark y, put in Q
		for (int i = 0; i < x->neighs.size(); i++)//traversing the list of neighbours
		{
			Vertex * y = x->neighs[i];
			if (marked.find(y) == marked.end())//if not marked
			{
				marked.insert(y);//add to list of marked vertecies
				Q.push(y);//put it in the que

				//augmentations:
				breadCrumbs[y] = x;
			}
		}
	}

}//end of BFS

//computes the index of the maze string given its coordinates
int findIndex(string maze, int r, int c) {
	//cout << "in find index" << endl;
	//int * rowCol = findRowCol(maze);
	return (r * (rc[1] + 2) )+ c;
}

//computes the shortest pathe from start to end
string shortestPath(string * startEnd, string maze)
{
	//cout << "Shortest Path" << endl;
	//cout << "start: " << startEnd[0] << " end: " << startEnd[1] << endl;
	Vertex * s = vertexSet[startEnd[0]];//creates a pointer to a vertex that's the start
	Vertex * t = vertexSet[startEnd[1]];//creates a pointer to the end vertex
	//cout << "Found the start and end vertecies " << endl;
	//step 1:
	breadthFirstSearch(startEnd[0]);
	
	
	//step 2: Use the breadcrumbs computed in bfs
	//to trace back a path from s to t:
	
	Vertex * current = t;//pointer to the end (follow the breadcrumns back to start)
	while (current != s)//while not at the beggining
	{
		//output = current->data + " " + output;//concatonate the string into output
		//use coordinates to find index in maze string
		maze[findIndex(maze, current->row, current->col)] = 'o';
		current = breadCrumbs[current];//move down the list of breadcrumbs
	}
	
	maze[findIndex(maze, s->row, s->col)] = 'o';
	cout << maze;
	
	return maze;
}


//creates the graph from the maze string
//creates vertecies and stores them in vertexset
string * createGraph(string maze) {
	//Step 1: create the graph
	static string startEnd[2]; startEnd[0] = ""; startEnd[1] = "";
	cout << maze << endl;
	int borderCounter = 0;
	int r = 0; int c = 0;
	for (int i = 0; i < maze.size() - 1; i++) {//traverse the string
		//cout << x << "," << y<<" ";
		//cout << maze[i];
		if (maze[i] == ' ') {
			if (inBorder(maze, r, c) && borderCounter==0) {//if it's first vertex on the border
				//cout << "S";
				Vertex * babyvertex = new Vertex(r, c);//create a pointer to vertex object
				string coor = to_string(r) + "," + to_string(c);//key is string of coordinates
				vertexSet[coor] = babyvertex;//add vertex to list of vertecies
				//save it as a start vertex;
				startEnd[0] = coor;//save coordinates to start vertex
				//check if there should be an edge to top or left
				if (checkTop(r, c)) {//if there is a top vertex add it
					string coorTop = to_string(r-1)+","+to_string(c);
					addEdge( coor,  coorTop);
				}
				if (checkLeft(r,c)) {//if there is a left vertex add it
					string coorLeft = to_string(r) + "," + to_string(c-1);
					addEdge(coor, coorLeft);
				}
				borderCounter++;
			}
			else if (inBorder(maze, r, c) && borderCounter == 1) {//if it's the second vertex on the border
				//cout << "E";
				Vertex * babyvertex = new Vertex(r, c);//create a pointer to vertex object
				string coor = to_string(r) + "," + to_string(c);//key is string of coordinates
				vertexSet[coor] = babyvertex;//add vertex to list of vertecies
				//save it as a start vertex;
				startEnd[1] = coor;//save coordinates to start vertex
				//check if there should be an edge to top or left
				if (checkTop(r, c)) {//if there is a top vertex add it
					string coorTop = to_string(r - 1) + "," + to_string(c);
					addEdge(coor, coorTop);
				}
				if (checkLeft(r, c)) {//if there is a left vertex add it
					string coorLeft = to_string(r) + "," + to_string(c - 1);
					addEdge(coor, coorLeft);
				}
			}
			else {//add to list of vertices 
			  Vertex * babyvertex = new Vertex(r, c);//create a pointer to vertex object
			  string coor = to_string(r) + "," + to_string(c);//key is string of coordinates
			  vertexSet[coor] = babyvertex;//add vertex to list of vertecies
			  //check if there should be an edge to top or left
			  if (checkTop(r, c)) {//if there is a top vertex add it
				  string coorTop = to_string(r - 1) + "," + to_string(c);
				  addEdge(coor, coorTop);
			  }
			  if (checkLeft(r, c)) {//if there is a left vertex add it
				  string coorLeft = to_string(r) + "," + to_string(c - 1);
				  addEdge(coor, coorLeft);
			  }
			}

		}
		if (maze[i] == '\n') { r++; c = 0; }
		else { c++; }
	}
//	cout << "\nFinished creating the graph" << endl;
//	cout << "start: " << startEnd[0] << " end: " << startEnd[1] << endl;
	return startEnd;

}//end of create graph




// For the mandatory running time, assume that the time for
// operations of queue, unordered_set, and map are O(1). 
// (They are for average-case, but not worst-case).
//
// For the mandatory running time below, s is the length of 
// the input string representing the maze.
// 
// For a complete description of the maze string 
// and maze solution formats, see the assignment pdf.


// Returns a string representing a shortest solution to the maze.
// Has undefined behavior if the maze is not valid or has no solution.
//
// Must run in O(s) time.
string solve(string maze) {
	vertexSet.clear();
	breadCrumbs.clear();
	findRowCol(maze);
	return shortestPath(createGraph(maze), maze);
}

