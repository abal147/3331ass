import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;


public class RoutingPerformance {
	
	public RoutingPerformance(String source) {
		BufferedReader reader;
		try {
			reader = new BufferedReader(new FileReader(source));
			String line;
			while ((line = reader.readLine()) != null) {
				System.out.println(line);
			}
		} catch (FileNotFoundException e) {
			System.err.println("File not found!!!");
			e.printStackTrace();
		} catch (IOException e) {
			System.err.println("IOException");
			e.printStackTrace();
		}

	}
	
	public static void main(String[] args) {
		for (int i = 0; i < args.length; ++i) {
			System.out.println(i + ": " + args[i]);
		}
		
		RoutingPerformance rp = new RoutingPerformance(args[2]);
	}
}
