import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
// import java.io.*;
// import java.io.BufferedReader;
// import java.io.FileReader;
// import java.io.IOException;

public class MyFrame extends JFrame implements ActionListener 
{
    
    JButton myButton1;

    JTextField tField1;

    JTextArea tArea;
    JScrollPane scroll;

    public MyFrame() 
    {
        setTitle("4x4 Matrix Input Frame");
        setSize(1200, 480);
        setLayout(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Font font1 = new Font("SansSerif", Font.BOLD, 20);

        myButton1 = new JButton("-"); 

        tField1 = new JTextField("0");

        tArea = new JTextArea();
        scroll = new JScrollPane(tArea);
        scroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        // tArea.setEditable(false);
        tArea.setFont(font1);
        tArea.setForeground(Color.BLUE);

        myButton1.addActionListener(this);
        
        myButton1.setBounds(10, 10, 50, 50);
        tField1.setBounds  (60,10,50, 50);
        
        scroll.setBounds(780, 10,400, 410);

        add(myButton1);
        add(tField1);

        add(scroll);

        setVisible(true);

    }

    public void actionPerformed(ActionEvent e) 
    {
        if (e.getSource() == myButton1) 
        {
            
        }
    }

    public static void main(String[] args) 
    {
        new MyFrame();
    }
}
