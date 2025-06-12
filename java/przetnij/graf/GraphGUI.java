package przetnij.graf;

import java.awt.*;
import java.io.File;
import java.io.IOException;
import javax.swing.*;

public class GraphGUI extends JFrame {
    private Graph graph;
    private GraphPanel graphPanel;
    private JToolBar toolBar;
    private JSpinner splitCountSpinner;
    private JSpinner splitMarginSpinner;

    public GraphGUI() {
        setTitle("Graph Splitter");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(800, 600);
        setLayout(new BorderLayout());

        createMenuBar();
        createToolBar();
        createGraphPanel();

        setVisible(true);
    }

    private void createMenuBar() {
        JMenuBar menuBar = new JMenuBar();

        // File menu
        JMenu fileMenu = new JMenu("File");
        JMenuItem loadItem = new JMenuItem("Load Graph");
        loadItem.addActionListener(e -> loadGraph());
        fileMenu.add(loadItem);

        JMenuItem saveItem = new JMenuItem("Save Graph");
        saveItem.addActionListener(e -> saveGraph());
        fileMenu.add(saveItem);

        fileMenu.addSeparator();
        
        JMenuItem exitItem = new JMenuItem("Exit");
        exitItem.addActionListener(e -> System.exit(0));
        fileMenu.add(exitItem);
        
        menuBar.add(fileMenu);

        // View menu
        JMenu viewMenu = new JMenu("View");
        JCheckBoxMenuItem toolPanelItem = new JCheckBoxMenuItem("Show Tool Panel", true);
        toolPanelItem.addActionListener(e -> toolBar.setVisible(toolPanelItem.isSelected()));
        viewMenu.add(toolPanelItem);
        menuBar.add(viewMenu);

        setJMenuBar(menuBar);
    }
    
    private void createToolBar() {
        toolBar = new JToolBar();
        toolBar.setFloatable(false);

        // Split count control
        toolBar.add(new JLabel("Split Count:"));
        splitCountSpinner = new JSpinner(new SpinnerNumberModel(2, 2, 10, 1));
        toolBar.add(splitCountSpinner);

        toolBar.addSeparator();

        // Split margin control
        toolBar.add(new JLabel("Split Margin:"));
        // Use float values for margin
        splitMarginSpinner = new JSpinner(new SpinnerNumberModel(0.0f, 0.0f, 100.0f, 0.1f));
        toolBar.add(splitMarginSpinner);

        toolBar.addSeparator();

        // Split button
        JButton splitButton = new JButton("Split Graph");
        splitButton.addActionListener(e -> splitGraph());
        toolBar.add(splitButton);

        add(toolBar, BorderLayout.NORTH);
    }

    private void createGraphPanel() {
        graphPanel = new GraphPanel();
        add(graphPanel, BorderLayout.CENTER);
    }

    private void loadGraph() {
        JFileChooser fileChooser = new JFileChooser();
        if (fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            try {
                graph = GraphIO.fromTextFile(file.getAbsolutePath());
                graphPanel.setGraph(graph);
                graphPanel.repaint();
            } catch (IOException e) {
                JOptionPane.showMessageDialog(this, "Error loading graph: " + e.getMessage(),
                        "Load Error", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void saveGraph() {
        if (graph == null) {
            JOptionPane.showMessageDialog(this, "No graph loaded!", "Save Error", JOptionPane.ERROR_MESSAGE);
            return;
        }

        JFileChooser fileChooser = new JFileChooser();
        if (fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            try {
                // GraphIO.toTextFile(graph, file.getAbsolutePath());
            } catch (Exception e) {
                JOptionPane.showMessageDialog(this, "Error saving graph: " + e.getMessage(),
                        "Save Error", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void splitGraph() {
        if (graph == null) {
            JOptionPane.showMessageDialog(this, "Load a graph first!", "Split Error", JOptionPane.ERROR_MESSAGE);
            return;
        }

        int splitCount = (Integer) splitCountSpinner.getValue();
        float splitMargin = (Float) splitMarginSpinner.getValue();

        graph = GraphSplitter.splitGraph(graph, splitCount, splitMargin);
        graphPanel.setGraph(graph); // Update the graph in the panel
        graphPanel.repaint();
    }
    
    class GraphPanel extends JPanel {
        private Graph graph;
        private float[] nodeX;
        private float[] nodeY;

        public void setGraph(Graph graph) {
            this.graph = graph;
            if (graph != null) {
                generateLayout();
            }
        }
        
        private void generateLayout() {
            int nodeCount = graph.getNodeCount();
            nodeX = new float[nodeCount];
            nodeY = new float[nodeCount];

            // Circular layout
            int centerX = getWidth() / 2;
            int centerY = getHeight() / 2;
            int radius = Math.min(centerX, centerY) - 20;
            
            double angleStep = 2 * Math.PI / nodeCount;
            for (int i = 0; i < nodeCount; i++) {
                double angle = i * angleStep;
                nodeX[i] = (float) (centerX + radius * Math.cos(angle));
                nodeY[i] = (float) (centerY + radius * Math.sin(angle));
            }
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            
            if (graph == null) {
                g.drawString("Load a graph to begin", getWidth()/2 - 50, getHeight()/2);
                return;
            }

            // Set up rendering hints for better quality
            Graphics2D g2d = (Graphics2D) g;
            g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            
            // Create color palette for segments
            Color[] segmentColors = {
                Color.RED, Color.BLUE, Color.GREEN, Color.ORANGE, Color.MAGENTA,
                Color.CYAN, Color.PINK, Color.YELLOW, new Color(128, 0, 128), new Color(255, 165, 0)
            };
            
            // Draw edges
            g2d.setColor(Color.LIGHT_GRAY);
            for (int i = 0; i < graph.getEdgeCount(); i++) {
                Edge edge = graph.getEdge(i);
                int from = edge.src;
                int to = edge.dest;

                int x1 = (int) nodeX[from];
                int y1 = (int) nodeY[from];
                int x2 = (int) nodeX[to];
                int y2 = (int) nodeY[to];
                
                g2d.drawLine(x1, y1, x2, y2);
            }
            
            // Draw nodes with segment colors
            int nodeSize = 8;
            for (int i = 0; i < graph.getNodeCount(); i++) {
                int segment = graph.getSegment(i);
                Color color = (segment >= 0 && segment < segmentColors.length) ? segmentColors[segment] : Color.GRAY;
                g2d.setColor(color);
                
                int x = (int) nodeX[i] - nodeSize/2;
                int y = (int) nodeY[i] - nodeSize/2;
                
                g2d.fillOval(x, y, nodeSize, nodeSize);
                
                // Draw node ID
                g2d.setColor(Color.BLACK);
                g2d.drawString(String.valueOf(i), x + nodeSize, y + nodeSize);
            }
        }
    }
}
