package przetnij.graf;

import java.io.IOException;

public class Main {
    public static void main(String[] args) {
        if (args.length < 3) {
            System.out.println("Usage: java przetnij.graf.Main <input> <N> <M> [-o output] [-t] [-b]");
            System.exit(1);
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
