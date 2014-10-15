import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Vector;


public class RoutingPerformance {
	
	private Vector<Integer> start, to, succ, delays, caps;
	
	public RoutingPerformance(String source) {

		start = new Vector<Integer>();
		
		for (int i = 0; i < 26; ++i) {
			start.add(-1);
		}
		
		to = new Vector<Integer>();
		succ = new Vector<Integer>();
		delays = new Vector<Integer>();
		caps = new Vector<Integer>();
		
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
		
//		for (int e = start[u];e != -1; e = succ[e])
	}
	
	public static void main(String[] args) {
		for (int i = 0; i < args.length; ++i) {
			System.out.println(i + ": " + args[i]);
		}
		
		RoutingPerformance rp = new RoutingPerformance(args[2]);
	}
}
