#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <array>
#include <vector.h>
#include <map.h>
#include <set.h>
#include <strlib.h>
#include <queue.h>

using namespace std;

const string infile = "spiderMaze.txt";
Map<string, Vector< array <string, 2 > > > points;
Set<string> visited;
Stack <string> dfsorder;
Stack<string> dfsoutput;
Stack<string> DFS;
Queue<string> BFS;

// used to link points for backwards traversalls
string reversedirection(string x) {
	if (x == "E")
		return "W";
	if (x == "S")
		return "N";
	if (x == "SW")
		return "NE";
	if (x == "SE")
		return "NW";
}

int main() {

	//read in file

	ifstream fin;
	fin.open(infile);
	if (fin.fail()) {
		cerr << "Error opening input file '";
		cerr << infile << "', exiting!" << endl;
		return -1;
	}
	string tempread = "";
	fin >> tempread;
	int numPoints = stoi(tempread.substr(3, tempread.size()));

	// fill my map (points) to create a graph representation
	while (!fin.eof()) {
		string direction = "", start = "", end = "";
		fin >> start >> end >> direction;
		Vector< array <string, 2> > tempvec;
		array <string, 2> temparray;
		temparray[0] = end;
		temparray[1] = direction;
		//forward link
		//if point goes to multiple points, need to copy the vector of values, and add the new point that can be visited
		if (points.containsKey(start)) {
			tempvec = points.get(start);
			tempvec.add(temparray);
			points.put(start, tempvec);
		}
		else {
			tempvec.add(temparray);
			points.put(start, tempvec);
		}
		//backwards link
		tempvec.clear();
		temparray[0] = start;
		temparray[1] = reversedirection(direction);
		if (points.containsKey(end)) {
			tempvec = points.get(end);
			tempvec.add(temparray);
			points.put(end, tempvec);
		}
		else {
			tempvec.add(temparray);
			points.put(end, tempvec);
		}
	}
	fin.close();
	// output file so one can see each interation as a walk through
	ofstream fout;
	fout.open("iterations.txt");
	if (fout.fail()) {
		cerr << "Error opening input file '";
		cerr << " exiting!" << endl;
		return -1;
	}
	// DFS search of tree
	clock_t t1, t2;
	double timeDFS, timeBFS;
	t1 = clock();
	string currentpoint = "0";
	visited.add("0");
	int count = 0;
	//while loop to run until the ladyspider is found
	while (currentpoint != integerToString(numPoints + 1)) {
		fout << " current " << currentpoint << endl;
		int countpush = 0;
		int countpop = 0;
		//for all the points branching from the current
		for (int i = 0; i < points.get(currentpoint).size(); i++) {
			string temppoint1 = points.get(currentpoint)[i][0];
			string currentdirection = points.get(currentpoint)[i][1];
			//all the points branching after first point
			for (int j = 0; j < points.get(temppoint1).size(); j++) {
				string temppoint2 = points.get(temppoint1)[j][0];
				string goingdirection = points.get(temppoint1)[j][1];
				// to assure a straigh line traversal is used
				if (currentdirection == goingdirection) {
					//for all the points that branch from the second point
					for (int k = 0; k < points.get(temppoint2).size(); k++) {
						string temppoint3 = points.get(temppoint2)[k][0];
						//add point to the stack if we can visit it(reachable and appropriate direction), and it hasnt been visited
						if (!visited.contains(temppoint3) && currentdirection == points.get(temppoint2)[k][1] && goingdirection == points.get(temppoint2)[k][1]) {
							DFS.push(temppoint3);
							countpush++;
						}
					}
				}
			}
		}
		// if DFS is empty, then no more points can be visited, and thus no solution possible
		if (DFS.isEmpty()) {
			cerr << "No possible solution" << endl;
			return -1;
		}
		// show iterations on output file
		fout << " visited " << visited << endl;
		fout << " stack " << DFS << endl;
		fout << " ------------------ " << endl;
		//update current point and visited
		currentpoint = DFS.pop();
		visited.add(currentpoint);
		count++;
		// if nothing added, we reached a dead end and want to delete points from the output stack until the last made decision point
		// as those points wont lead to the solution
		if (countpush == 0) {
			while (count > 0) {
				dfsorder.pop();
				count--;
			}
		}
		//if we push multiple points, this is a decision point
		if (countpush > 1)
			count = 0;
		dfsorder.push(currentpoint);
	}
	// output the final iteration to the iteration representation file
	fout << " current " << currentpoint << endl;
	fout << " visited " << visited << endl;
	fout << " stack " << DFS << endl;

	//clock for time comparison
	t2 = clock();
	timeDFS = (t2 - t1) / double(CLOCKS_PER_SEC);
	int size = dfsorder.size();
	// reverse stacks order so we can output points in order of how we want to visit them
	for(int i = 0; i < size; i++)
		dfsoutput.push(dfsorder.pop());
	dfsoutput.push("0");
	cout << "Depth first search results: " << endl;
	while (!dfsoutput.isEmpty()) 
		cout << dfsoutput.pop() << " ";
	cout << endl;

	// END DFS search through tree, now for time compare for EC

	cout << endl << "END OF PROJECT" << endl << "Now time compare with BFS for Extra Credit" << endl;
	currentpoint = "0";
	visited.clear();

	//start BFS search through tree, same as DFS, except uses a Queue and dequeue

	visited.add("0");
	t1 = clock();
	while (currentpoint != integerToString(numPoints + 1)) {
		for (int i = 0; i < points.get(currentpoint).size(); i++) {
			string temppoint1 = points.get(currentpoint)[i][0];
			string currentdirection = points.get(currentpoint)[i][1];
			for (int j = 0; j < points.get(temppoint1).size(); j++) {
				string temppoint2 = points.get(temppoint1)[j][0];
				string goingdirection = points.get(temppoint1)[j][1];
				if (currentdirection == goingdirection) {
					for (int k = 0; k < points.get(temppoint2).size(); k++) {
						string temppoint3 = points.get(temppoint2)[k][0];
						if (!visited.contains(temppoint3) && currentdirection == points.get(temppoint2)[k][1] && goingdirection == points.get(temppoint2)[k][1]) {
							BFS.enqueue((temppoint3));
						}
					}
				}
			}
		}
		if (BFS.isEmpty()) {
			cerr << "No possible solution" << endl;
			return -1;
		}
		currentpoint = BFS.dequeue();
		visited.add(currentpoint);
	}
	t2 = clock();
	timeBFS = (t2 - t1) / double(CLOCKS_PER_SEC);
	cout << "Time compare: " << endl;
	cout << "DFS: " << timeDFS << endl;
	cout << "BFS: " << timeBFS << endl;
	fout.close();
	return 0;
}