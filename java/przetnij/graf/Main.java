package przetnij.graf;

import java.io.IOException;
import javax.swing.SwingUtilities;

public class Main {
    public static void main(String[] args) {
        if (args.length > 0 && (args[0].equals("--help") || args[0].equals("/?"))) {
            showHelp();
            return;
        }
        
        if (args.length == 0) {
            // Launch GUI mode
            SwingUtilities.invokeLater(() -> new GraphGUI());
        } else {
            // Handle command-line mode
            if (args.length < 3) {
                System.out.println("Invalid arguments. Use --help for usage information.");
                return;
            }
            
            String inputFile = args[0];
            int number = Integer.parseInt(args[1]);
            float margin = Float.parseFloat(args[2]);
            String outputFile = "output.txt";
            boolean terminalOutput = false;
            boolean binaryOutput = false;

            // Parse optional arguments
            for (int i = 3; i < args.length; i++) {
                switch (args[i]) {
                    case "-o" -> {
                        if (i + 1 < args.length) outputFile = args[++i];
                    }
                    case "-t" -> terminalOutput = true;
                    case "-b" -> binaryOutput = true;
                }
            }

            try {
                // Load graph
                Graph original = GraphIO.fromTextFile(inputFile);
                if (original == null) {
                    System.err.println("Error loading graph from " + inputFile);
                    System.exit(1);
                }

                // Split graph
                Graph split = GraphSplitter.splitGraph(original, number, margin);
                
                // Save result
                if (binaryOutput) {
                    // Binary output not implemented in this simplified version
                    System.out.println("Binary output not implemented");
                } else {
                    GraphIO.toTextFile(split, outputFile);
                    System.out.println("Graph written to " + outputFile);
                }

                // Terminal output
                if (terminalOutput) {
                    System.out.println("Split Graph:");
                    System.out.println(GraphIO.toString(split));
                }

            } catch (IOException e) {
                System.err.println("Error processing graph: " + e.getMessage());
                System.err.println("Error details: " + e.getClass().getSimpleName() + ": " + e.getMessage());
                System.exit(1);
            }
        }
    }
    
    private static void showHelp() {
        System.out.println("Usage:");
        System.out.println("  GUI Mode: java -jar przetnij-graf.jar");
        System.out.println("  CLI Mode: java -jar przetnij-graf.jar <input> <split_count> <split_margin> [options]");
        System.out.println("Options:");
        System.out.println("  -o <file>      Output file (default: output.txt)");
        System.out.println("  -t             Show result in terminal");
        System.out.println("  -b             Save in binary format");
        System.out.println("  --help, /?     Show this help message");
        System.out.println("Example: java -jar przetnij-graf.jar graph.csrrg 3 5 -o result.txt");
    }
}
