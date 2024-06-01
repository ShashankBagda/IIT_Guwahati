import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
import java.io.File;
import java.io.IOException;

public class trial2 extends JFrame implements ActionListener {
    JButton myButton1;
    JTextField tField1;
    JTextArea tArea;
    JScrollPane scroll;
    JPanel contentPane; // Panel to hold components and the background image

    public trial2() {
        setTitle("Gem5 Simulator Home Window");
        setSize(1200, 480);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // Create the content pane with a custom panel that has a background image
        contentPane = new JPanel() {
            @Override
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);
                try {
                    BufferedImage image = ImageIO.read(new File("background.jpg")); // Replace "background.jpg" with the path to your image file
                    g.drawImage(image, 0, 0, getWidth(), getHeight(), this);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        };
        contentPane.setLayout(new GridBagLayout());
        setContentPane(contentPane);

        Font font1 = new Font("SansSerif", Font.BOLD, 20);

        myButton1 = new JButton("-");
        tField1 = new JTextField("0");
        tArea = new JTextArea();
        scroll = new JScrollPane(tArea);
        scroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        tArea.setFont(font1);
        tArea.setForeground(Color.BLUE);

        myButton1.addActionListener(this);

        GridBagConstraints constraints = new GridBagConstraints();
        constraints.insets = new Insets(10, 10, 10, 10); // Add padding

        // Add myButton1
        constraints.gridx = 0;
        constraints.gridy = 0;
        constraints.gridwidth = 1;
        constraints.fill = GridBagConstraints.NONE;
        contentPane.add(myButton1, constraints);

        // Add tField1
        constraints.gridx = 1;
        constraints.gridy = 0;
        constraints.gridwidth = 1;
        constraints.fill = GridBagConstraints.NONE;
        contentPane.add(tField1, constraints);

        // Add scroll
        constraints.gridx = 0;
        constraints.gridy = 1;
        constraints.gridwidth = 2;
        constraints.gridheight = 1;
        constraints.fill = GridBagConstraints.BOTH;
        constraints.weightx = 1.0;
        constraints.weighty = 1.0;
        contentPane.add(scroll, constraints);

        setVisible(true);
    }

    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == myButton1) {

        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            new trial2();
        });
    }
}
