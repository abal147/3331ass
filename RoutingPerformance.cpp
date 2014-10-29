/*
 *
 * COMP3331 Assignment 14s2
 * Aaron Balsara - z3414709
 * Nicholas Figueira - z3416333
 *
 */
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <utility>
#include <cmath>
#include <numeric>

using namespace std;

class pqNode {
	public:
		double dist;
		int to;
		int from;
		int delay;
		vector<int> path;

		pqNode(int _to = -1) {
			dist = 0;
			to = _to;
			from = -1;
			delay = 0;
		}

		pqNode& operator=(const pqNode& o) {
			dist = o.dist;
			from = o.from;
			to = o.to;
			path = o.path;
			delay = o.delay;

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
         } else if (time < o.time) {
            return false;
         } else if (isStart && !o.isStart) {
            return true;            
         } else {
            return false;
         }
      }
};

vector<int> start, to, succ, delays, edgeFrom;
map<pair<int, int>, int> edgeCap, edgeMax;
int packetRate;

pair<int, int> intToKey(int u, int v) {
   return make_pair(min(u, v),max(u, v));
}

void printPath(const vector<int>& v) {
   for (int i = 0; i < v.size(); ++i) {
      cout << char(v[i] + 'A') << " ";
   }
   cout << endl;
}

void addEdge(int u, int v, int delay, int cap) {
//   cout << "Adding: " << u << " " << v << " " << delay << " " << cap << endl;
	
	to.push_back(v);
	succ.push_back(start[u]);
	start[u] = to.size() - 1;
	
	delays.push_back(delay);
	edgeFrom.push_back(u);

   edgeCap[intToKey(u,v)] = cap;
   edgeMax[intToKey(u,v)] = cap;
   
}

vector<int> shortestPath(int from, int dest, int mode, int *length) {
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

		//printf("Expanding %c\n", node.to + 'A');

		prev[node.to] = node.from;
		seen[node.to] = true;

		if (node.to == dest) {
			//printf("Dest found, dist: %f\n", node.dist);
			node.path.push_back(dest);
			*length = node.delay;
			return node.path;
		}

		for (int e = start[node.to]; ~e; e = succ[e]) {
			if (seen[to[e]]) continue;

			//printf("Expanding edge from %c -> %c\n", edgeFrom[e] + 'A', to[e] + 'A');
			pqNode temp(to[e]);
			temp.from = node.to;
			temp.path = node.path;
			temp.path.push_back(node.to);
			temp.delay = node.delay + delays[e];

			if (mode == 1) {
				temp.dist = node.dist + 1;
			} else if (mode == 2) {
				temp.dist = node.dist + delays[e];
			} else {
				temp.dist = max(node.dist, 1 - ((double)edgeCap[intToKey(node.to, to[e])]
                                                   /edgeMax[intToKey(node.to, to[e])]));
			}

			pq.push(temp);
		}
	}
	vector<int> v;
	return v;
}

void simulateNetwork(char *fileName, bool isCircuit, int mode) {
   
   double startTime, duration;
   char from, dest;
   
   priority_queue<request> pq;

   int totalRequests = 0;
   
   int totalPackets = 0;
   int successfulPackets = 0;
   int failedPackets = 0;
   
   vector<int> numHops;
   vector<int> numDelays;
   
   FILE *file = fopen(fileName, "r");
   
   while (fscanf(file, "%lf %c %c %lf\n", &startTime, &from, &dest, &duration) == 4) {
   
      if (isCircuit) {
         request temp(true, startTime, duration, from - 'A', dest - 'A');
         pq.push(temp);
         //printf("%.6f %c %c %.6f\n", startTime, from, dest, duration);
         ++totalRequests;
         totalPackets += ceil(duration * (double)packetRate);
      } else {
         int numPackets = ceil(duration * (double)packetRate);
         
         for (int i = 0; i < numPackets; ++i) {
            request temp(true, startTime + (i * 1.0 / packetRate), 1.0 / packetRate, from - 'A', dest - 'A');
            pq.push(temp);
         }
         
         totalRequests += ceil(duration * (double)packetRate);
         totalPackets  += ceil(duration * (double)packetRate);
      }
   }
   
   fclose(file);
   
   request temp;
   
   int pathDelay;
   
   while (!pq.empty()) {
      temp = pq.top();
      pq.pop();

      //printf("CURRENT TIME: %.6f\n", temp.time);
      
      if (temp.isStart) {
         /* Node begins a new request */
         vector<int> path = shortestPath(temp.from, temp.dest, mode, &pathDelay);

         //printPath(v);

         bool validPath = true;
         for (int i = 0; i < path.size() - 1; ++i) {
            if (edgeCap[intToKey(path[i], path[i+1])] == 0) {
               validPath = false;
            }
         }

         if (validPath) {
            for (int i = 0; i < path.size() - 1; ++i) {
               edgeCap[intToKey(path[i], path[i+1])]--;
            }

            request end(false, temp.time + temp.duration, 0, temp.from, temp.dest, path);
            
            pq.push(end);

            successfulPackets += ceil(temp.duration * (double)packetRate);
            numHops.push_back(path.size() - 1);
            numDelays.push_back(pathDelay);

         } else {
            //cout << "PACKET BLOCKED" << endl;
            failedPackets += ceil(temp.duration * (double)packetRate);
         }
      } else {
         /* Node is an end node, finishing a request */
         for (int i = 0; i < temp.path.size() - 1; ++i) {
            edgeCap[intToKey(temp.path[i], temp.path[i+1])]++;
         }

         //printf("Released: %c -> %c\n", temp.from + 'A', temp.dest + 'A');
      }
      
      //printf("%d %.6f %c %c\n", temp.isStart, temp.time, temp.from + 'A', temp.dest + 'A');
   }

   int sum = accumulate(numHops.cbegin(), numHops.cend(), 0);
   
   double avgHops = sum / (double)numHops.size();
   
   sum = accumulate(numDelays.cbegin(), numDelays.cend(), 0);
   
   double avgDelay = sum / (double)numDelays.size();

   

//   printf("\n");
   printf("total number of virtual circuit requests: %d\n", totalRequests);
//   printf("\n");
   printf("total number of packets: %d\n", totalPackets);
//   printf("\n");
   printf("number of successfully routed packets: %d\n", successfulPackets);
//   printf("\n");
   printf("percentage of successfully routed packets: %.2f\n", 100.0 * (double)successfulPackets/totalPackets);
//   printf("\n");
   printf("number of blocked packets: %d\n", failedPackets);
//   printf("\n");
   printf("percentage of blocked packets: %.2f\n", 100.0 * (double)failedPackets/totalPackets);
//   printf("\n");
   printf("average number of hops per circuit: %.2f\n", avgHops);
//   printf("\n");
   printf("average cumulative propagation delay per circuit: %.2f\n", avgDelay);

}

int main(int argc, char *argv[]) {

   srand(10);

   bool isCircuit = true;

   if (string(argv[1]) == string("CIRCUIT")) {
      //cout << "Circuit Network" << endl;
      isCircuit = true;
   } else {
      //cout << "Packet Network" << endl;
      isCircuit = false;
   }

	for (int i = 0; i < 26; ++i) {
		start.push_back(-1);
	}

   int mode = 0;
   if (string(argv[2]) == string("SHP")) {
      mode = 1;
      //cout << "SHP Network\n";
   } else if (string(argv[2]) == string("SDP")) {
      mode = 2;
      //cout << "SDP Network\n";
   } else {
      mode = 3;
      //cout << "LLP Network\n";
   }

   packetRate = atoi(argv[5]);

   int delay, cap;
	char from, dest;
	
	FILE *file = fopen(argv[3], "r");

   while (fscanf(file, "%c %c %d %d\n", &from, &dest, &delay, &cap) == 4) {
      int convertFrom = from - 'A';
      int convertTo = dest - 'A';
		addEdge(convertFrom, convertTo, delay, cap);
   	addEdge(convertTo, convertFrom, delay, cap);

   	//cout << from << " " << dest << " " << delay << " " << cap << endl << flush;
   }

   fclose(file);

   simulateNetwork(argv[4], isCircuit, mode);

	return 0;
}
