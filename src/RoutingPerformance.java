import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.Vector;


public class RoutingPerformance {
	
	private Vector<Integer> start, to, succ, delays, caps, edgeFrom, maxCaps;
	
	public RoutingPerformance(String source) {

		start = new Vector<Integer>();
		
		for (int i = 0; i < 26; ++i) {
			start.add(-1);
		}
		
		to = new Vector<Integer>();
		succ = new Vector<Integer>();
		delays = new Vector<Integer>();
		caps = new Vector<Integer>();
		edgeFrom = new Vector<Integer>();
		maxCaps = new Vector<Integer>();
		
		BufferedReader reader;
		try {
			reader = new BufferedReader(new FileReader(source));
			String line;
			while ((line = reader.readLine()) != null) {
				String[] words = line.split(" ");
				addEdge(words[0].charAt(0) - 'A', words[1].charAt(0) - 'A', Integer.parseInt(words[2]), Integer.parseInt(words[3]));
				addEdge(words[1].charAt(0) - 'A', words[0].charAt(0) - 'A', Integer.parseInt(words[2]), Integer.parseInt(words[3]));
			}
		} catch (FileNotFoundException e) {
			System.err.println("File not found!!!");
			e.printStackTrace();
		} catch (IOException e) {
			System.err.println("IOException");
			e.printStackTrace();
		}
		
		for (int i = 0; i < start.size(); ++i) {
			System.out.print(i + ": ");
			for (Integer e = start.get(i); e != -1; e = succ.get(e)) {
				System.out.print("(" + to.get(e) + ", " + delays.get(e) + ", " + caps.get(e) + "), ");
			}
			System.out.println();
		}
	}
	
	private void addEdge(Integer u, Integer v, Integer delay, Integer cap) {
		//System.out.println(u + " " + v + " " + delay + " " + cap);
		int nextIndex = to.size();
		to.add(v);
		succ.add(start.get(u));
		delays.add(delay);
		caps.add(cap);
		start.set(u, nextIndex);
		edgeFrom.add(u);
		maxCaps.add(cap);
		
//		for (int e = start[u];e != -1; e = succ[e])
	}
	
	public Vector<Integer> shortestPath(int from, int dest, int mode) {

		qEntry node = new qEntry();
		node.to = from;
		node.from = -1;
		
		PriorityQueue<qEntry> pq = new PriorityQueue<RoutingPerformance.qEntry>(100, new qEntryComparator());
		
		pq.add(node);
		boolean seen[] = new boolean[26];
		int prev[] = new int[26];
		for (int i = 0; i < 26; ++i) prev[i]=-1;
		
		while (!pq.isEmpty()) {
			node = pq.poll();
						
			if (seen[node.to]) continue;
			
			System.out.println("Expanding " + node.to);
			
			prev[node.to] = node.from;
			seen[node.to] = true;
			
			if (node.to == dest) {
				System.out.println("Found, dist: " + node.dist);
				node.path.add(dest);
				return node.path;
			}
			
			for (int e = start.get(node.to); e != -1; e = succ.get(e)) {
				System.out.println("Examining edge from " + edgeFrom.get(e) + " to " + to.get(e));
				qEntry temp = new qEntry();
				temp.from = node.to;
				temp.to = to.get(e);
				if (mode == 1) {
					temp.dist = node.dist + 1; //replace by func
				} else if (mode == 2) {
					temp.dist = node.dist + delays.get(e); //replace by func
				} else {
					temp.dist = Math.max(node.dist, 1 - ((double)caps.get(e)) / maxCaps.get(e));
				}
				
				for (int i = 0; i < node.path.size(); ++i) {
					temp.path.add(node.path.get(i));
				}
				
				temp.path.add(node.to);
				if (!seen[to.get(e)]){
					pq.add(temp);
					System.out.println("\tadded");
				}
			}
		}
		
		return null;
	}

	private class qEntry {
		public double dist;//Distance travelled, to be calculated by function
		public int to;
		public int from;
		public Vector<Integer> path;
		
		public qEntry() {
			dist = 0;
			to = -1;
			from = -1;
			path = new Vector<Integer>();
		}
		
		public qEntry(Integer to) {
			dist = 0;
			this.to = to;
			from = -1;
			path = new Vector<Integer>();
		}
		
		public qEntry clone() {
			qEntry newEntry = new qEntry(to);
			newEntry.dist = dist;
			newEntry.from = from;
			newEntry.path = path;
			return newEntry;
		}
	}
	
	private class qEntryComparator implements Comparator<qEntry> {

		@Override
		public int compare(qEntry o1, qEntry o2) {
			if (o1.dist < o2.dist){
				return -1;
			} else if (o1.dist == o2.dist) {
				return 0;
			} else {
				return 1;
			}
		}
		
	}
	
	public static void main(String[] args) {
		for (int i = 0; i < args.length; ++i) {
			System.out.println(i + ": " + args[i]);
		}
		
		RoutingPerformance rp = new RoutingPerformance(args[2]);
		Vector<Integer> path = rp.shortestPath(0, 4, 3);
		for (int i = 0; i < path.size(); ++i) {
			System.out.println("Step " + i + ": " + path.get(i));
		}
	}
}
