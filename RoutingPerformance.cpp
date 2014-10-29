#include <cstdio>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <utility>

using namespace std;

class pqNode {
	public:
		double dist;
		int to;
		int from;
		vector<int> path;

		pqNode(int _to = -1) {
			dist = 0;
			to = _to;
			from = -1;
		}

		pqNode& operator=(const pqNode& o) {
			dist = o.dist;
			from = o.from;
			to = o.to;
			path = o.path;

			return *this;
		}

		bool operator<(const pqNode& o) const {
			if (dist > o.dist) {
				return true;
			} else if (dist < o.dist) {
   			return false;
   		} else {
   		   if (rand() % 2) {
   		      return true;
   		   } else {
   		      return false;
   		   }
   		}
		}
};

class request {
   public:
      bool isStart;
      double time;
      double duration;
      int from, dest;
      vector<int> path;
      
      request(bool _isStart = 1, double _time = 0, double _duration = 0, int _from = -1
            , int _dest = -1, vector<int> _path = vector<int>()) 
      : isStart(_isStart), time(_time), duration(_duration), from(_from), dest(_dest), path(_path) {
      
      }
      
      bool operator<(const request& o) const {
         if (time > o.time) {
            return true;
         } else {
            return false;
         }
      }
};

vector<int> start, to, succ, delays, caps, edgeFrom, maxCap;
map<pair<int, int>, int> edgeMap;

void addEdge(int u, int v, int delay, int cap) {
   //cout << "Adding: " << u << " " << v << " " << delay << " " << cap << endl;
	
	to.push_back(v);
	succ.push_back(start[u]);
	start[u] = to.size() - 1;
	
	delays.push_back(delay);
	caps.push_back(cap);
	maxCap.push_back(cap);
	edgeFrom.push_back(u);
	
	edgeMap[make_pair(u, v)] = cap;
//	maxmap[make_pair(u, v)] = cap;
}

vector<int> shortestPath(int from, int dest, int mode = 1) {
	pqNode node(from);

	priority_queue<pqNode> pq;

	pq.push(node);

	bool seen[26];
	int prev[26];

	fill(seen, seen + 26, false);
	fill(prev, prev + 26, -1);

	while (!pq.empty()) {
		node = pq.top();
		pq.pop();

		if (seen[node.to]) continue;

		printf("Expanding %c\n", node.to + 'A');

		prev[node.to] = node.from;
		seen[node.to] = true;

		if (node.to == dest) {
			printf("Dest found, dist: %f\n", node.dist);
			node.path.push_back(dest);
			return node.path;
		}

		for (int e = start[node.to]; ~e; e = succ[e]) {
			if (seen[to[e]]) continue;

			printf("Expanding edge from %c -> %c\n", edgeFrom[e] + 'A', to[e] + 'A');
			pqNode temp(to[e]);
			temp.from = node.to;
			temp.path = node.path;
			temp.path.push_back(node.to);

			if (mode == 1) {
				temp.dist = node.dist + 1;
			} else if (mode == 2) {
				temp.dist = node.dist + delays[e];
			} else {
				temp.dist = max(node.dist, 1 - ((double)caps[e]/maxCap[e]));
			}

			pq.push(temp);
		}
	}
	vector<int> v;
	return v;
}

void simulateNetwork(char *fileName) {
   ifstream file(fileName);
   
   double startTime, duration;
   char from, dest;
   
   file >> startTime >> from >> dest >> duration;
   
   priority_queue<request> pq;
   
   while (file.good()) {
      
      request temp(true, startTime, from - 'A', dest - 'A');
      pq.push(temp);
      printf("%.6f %c %c %.6f\n", startTime, from, dest, duration);
      
      file >> startTime >> from >> dest >> duration;
   }
   
   
   request temp;
   
   double currTime = 0;
   
   while (!pq.empty()) {
      temp = pq.top();
      pq.pop();
      
      if (temp.isStart) {
         
      }
      
      printf("%d %.6f %c %c\n", temp.isStart, temp.time, temp.from + 'A', temp.dest + 'A');
   }
}

int main(int argc, char *argv[]) {

   srand(100);

   if (string(argv[1]) == string("CIRCUIT")) {
      cout << "Circuit Network" << endl;
   } else {
      cout << "Packet Network" << endl;
   }

	for (int i = 0; i < 26; ++i) {
		start.push_back(-1);
	}

   int mode = 0;
   if (string(argv[2]) == string("SHP")) {
      mode = 1;
      cout << "SHP Network\n";
   } else if (string(argv[2]) == string("SDP")) {
      mode = 2;
      cout << "SDP Network\n";
   } else {
      mode = 3;
      cout << "LLP Network\n";
   }

   ifstream file(argv[3]);

   int delay, cap;
	char from, dest;

   file >> from >> dest >> delay >> cap;
   
   while (file.good()) {
      int convertFrom = from - 'A';
      int convertTo = dest - 'A';
		addEdge(convertFrom, convertTo, delay, cap);
   	addEdge(convertTo, convertFrom, delay, cap);
   	
   	cout << from << " " << dest << " " << delay << " " << cap << endl<< flush;
      file >> from >> dest >> delay >> cap;

      
   }

   simulateNetwork(argv[4]);

/*
	vector<int> path = shortestPath(0, 1, mode);
	
	for (int i = 0; i < path.size(); ++i) {
	   char c = path[i] + 'A';
	   cout << c << " ";
	}
	cout << endl;
*/


	return 0;
}
