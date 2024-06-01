import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class MyFrame extends JFrame implements ActionListener 
{
    
    JButton 
    myButton1,      myButton2,       myButton3,        myButton4,        myButton5,
    myButton6,      myButton7,       myButton8,        myButton9,        myButton10,      
    myButton11,     myButton12,      myButton13,       myButton14,       myButton15,      
    myButton16,     myButton17,      myButton18,       myButton19,       myButton20,       
    myButton21,     myButton22,      myButton23,       myButton24,       myButton25,       
    myButton26,     myButton27,      myButton28,       myButton29,       myButton30,      
    myButton31,     myButton32,      myButton33,       myButton34,       myButton35;

    JTextField
    tField1,    tField2,    tField3,    tField4, 
    tField5,    tField6,    tField7,    tField8, 
    tField9,    tField10,   tField11,   tField12,
    tField13,   tField14,   tField15,   tField16;

    JTextArea tArea;
    JScrollPane scroll;

    int threshold;

    private static final String COMMA_DELIMITER = ",";
    // private static final String NEW_LINE_SEPARATOR = "\n";

    public MyFrame() 
    {
        setTitle("4x4 Matrix Input Frame");
        setSize(1200, 480);
        setLayout(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Font font1 = new Font("SansSerif", Font.BOLD, 20);

        myButton1 = new JButton("-");       myButton2 = new JButton("+");       myButton3 = new JButton("-");       myButton4 = new JButton("+");       myButton5 = new JButton("-");       
        myButton6 = new JButton("+");       myButton7 = new JButton("-");       myButton8 = new JButton("+");       myButton9 = new JButton("-");       myButton10 = new JButton("+");      
        myButton11 = new JButton("-");      myButton12 = new JButton("+");      myButton13 = new JButton("-");      myButton14 = new JButton("+");      myButton15 = new JButton("-");      
        myButton16 = new JButton("+");      myButton17 = new JButton("-");      myButton18 = new JButton("+");      myButton19 = new JButton("-");      myButton20 = new JButton("+");       
        myButton21 = new JButton("-");      myButton22 = new JButton("+");      myButton23 = new JButton("-");      myButton24 = new JButton("+");      myButton25 = new JButton("-");       
        myButton26 = new JButton("+");      myButton27 = new JButton("-");      myButton28 = new JButton("+");      myButton29 = new JButton("-");      myButton30 = new JButton("+");      
        myButton31 = new JButton("-");      myButton32 = new JButton("+");      myButton33 = new JButton("Reset");  myButton34 = new JButton("Freeze");  myButton35 = new JButton("Process");

        tField1 = new JTextField("0");     tField5 = new JTextField("0");     tField9 = new JTextField("0");      tField13 = new JTextField("0");
        tField2 = new JTextField("0");     tField6 = new JTextField("0");     tField10 = new JTextField("0");     tField14 = new JTextField("0");
        tField3 = new JTextField("0");     tField7 = new JTextField("0");     tField11 = new JTextField("0");     tField15 = new JTextField("0");
        tField4 = new JTextField("0");     tField8 = new JTextField("0");     tField12 = new JTextField("0");     tField16 = new JTextField("0");
        
        tArea = new JTextArea();
        scroll = new JScrollPane(tArea);
        scroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        // tArea.setEditable(false);

        JTextField[] textFields = {tField1, tField2, tField3, tField4, tField5, tField6, tField7, tField8, tField9, tField10, tField11, tField12, tField13, tField14, tField15, tField16};
        for (JTextField textField : textFields) 
        {
            // textField.setEnabled(false);
            textField.setFont(font1);
            textField.setHorizontalAlignment(JTextField.CENTER);
            textField.setForeground(Color.RED);
            textField.setBackground(Color.WHITE);
            textField.setEditable(false);
        }

        tArea.setFont(font1);
        tArea.setForeground(Color.BLUE);

        JButton[] myButtons = {myButton1, myButton2, myButton3, myButton4, myButton5, myButton6, myButton7, myButton8, myButton9, myButton10, myButton11, myButton12, myButton13, myButton14, myButton15, myButton16, myButton17, myButton18, myButton19, myButton20, myButton21, myButton22, myButton23, myButton24, myButton25, myButton26, myButton27, myButton28, myButton29, myButton30, myButton31, myButton32, myButton33, myButton34, myButton35};
        for (JButton button : myButtons) 
        {
            button.setFont(font1);
            // button.setForeground(Color.RED);
        }
        myButton35.setEnabled(false);

        myButton1.addActionListener(this);      myButton2.addActionListener(this);      myButton3.addActionListener(this);      myButton4.addActionListener(this);      myButton5.addActionListener(this);       
        myButton6.addActionListener(this);      myButton7.addActionListener(this);      myButton8.addActionListener(this);      myButton9.addActionListener(this);      myButton10.addActionListener(this);      
        myButton11.addActionListener(this);     myButton12.addActionListener(this);     myButton13.addActionListener(this);     myButton14.addActionListener(this);     myButton15.addActionListener(this);      
        myButton16.addActionListener(this);     myButton17.addActionListener(this);     myButton18.addActionListener(this);     myButton19.addActionListener(this);     myButton20.addActionListener(this);
        myButton21.addActionListener(this);     myButton22.addActionListener(this);     myButton23.addActionListener(this);     myButton24.addActionListener(this);     myButton25.addActionListener(this);
        myButton26.addActionListener(this);     myButton27.addActionListener(this);     myButton28.addActionListener(this);     myButton29.addActionListener(this);     myButton30.addActionListener(this);
        myButton31.addActionListener(this);     myButton32.addActionListener(this);     myButton33.addActionListener(this);     myButton34.addActionListener(this);     myButton35.addActionListener(this);

        myButton1.setBounds(10, 10, 50, 50);       myButton11.setBounds(210, 90, 50, 50);    myButton21.setBounds(410, 170, 50, 50);   myButton31.setBounds(610, 250, 50, 50);
        tField1.setBounds  (60,10,50, 50);        tField6.setBounds   (260,90,50, 50);      tField11.setBounds  (460,170,50, 50);     tField16.setBounds  (660,250,50, 50);
        myButton2.setBounds(110, 10, 50, 50);     myButton12.setBounds(310, 90, 50, 50);    myButton22.setBounds(510, 170, 50, 50);   myButton32.setBounds(710, 250, 50, 50);

        myButton3.setBounds(210, 10, 50, 50);     myButton13.setBounds(410, 90, 50, 50);    myButton23.setBounds(610, 170, 50, 50);   myButton33.setBounds(10, 320, 200, 100);
        tField2.setBounds  (260,10,50, 50);       tField7.setBounds   (460,90,50, 50);      tField12.setBounds  (660,170,50, 50);    
        myButton4.setBounds(310, 10, 50, 50);     myButton14.setBounds(510, 90, 50, 50);    myButton24.setBounds(710, 170, 50, 50);   myButton34.setBounds(285, 320, 200, 100);
        
        myButton5.setBounds(410, 10, 50, 50);     myButton15.setBounds(610, 90, 50, 50);    myButton25.setBounds(10, 250, 50, 50);     myButton35.setBounds(560, 320, 200, 100);
        tField3.setBounds  (460,10,50, 50);       tField8.setBounds   (660,90,50, 50);      tField13.setBounds  (60,250,50, 50);       
        myButton6.setBounds(510, 10, 50, 50);     myButton16.setBounds(710, 90, 50, 50);    myButton26.setBounds(110, 250, 50, 50);
        
        myButton7.setBounds(610, 10, 50, 50);     myButton17.setBounds(10, 170, 50, 50);     myButton27.setBounds(210, 250, 50, 50);
        tField4.setBounds  (660, 10,50, 50);      tField9.setBounds   (60,170,50, 50);      tField14.setBounds  (260,250,50, 50);        
        myButton8.setBounds(710, 10, 50, 50);     myButton18.setBounds(110, 170, 50, 50);   myButton28.setBounds(310, 250, 50, 50);
        
        myButton9.setBounds(10, 90, 50, 50);      myButton19.setBounds(210, 170, 50, 50);   myButton29.setBounds(410, 250, 50, 50);
        tField5.setBounds  (60, 90,50, 50);      tField10.setBounds  (260, 170,50, 50);    tField15.setBounds  (460, 250,50, 50);
        myButton10.setBounds(110, 90, 50, 50);   myButton20.setBounds(310, 170, 50, 50);   myButton30.setBounds(510, 250, 50, 50);

        scroll.setBounds(780, 10,400, 410);

        add(myButton1);   add(myButton11);   add(myButton21);   add(myButton31);
        add(tField1);     add(tField6);      add(tField11);      add(tField16);
        add(myButton2);   add(myButton12);   add(myButton22);   add(myButton32);

        add(myButton3);   add(myButton13);   add(myButton23);   add(myButton33);
        add(tField2);     add(tField7);      add(tField12);      
        add(myButton4);   add(myButton14);   add(myButton24);   add(myButton34);

        add(myButton5);   add(myButton15);   add(myButton25);   add(myButton35);
        add(tField3);     add(tField8);      add(tField13);      
        add(myButton6);   add(myButton16);   add(myButton26);

        add(myButton7);   add(myButton17);   add(myButton27);
        add(tField4);     add(tField9);      add(tField14);      
        add(myButton8);   add(myButton18);   add(myButton28);

        add(myButton9);   add(myButton19);   add(myButton29);
        add(tField5);     add(tField10);      add(tField15); 
        add(myButton10);  add(myButton20);   add(myButton30);

        add(scroll);

        setVisible(true);

    }

    public void actionPerformed(ActionEvent e) 
    {

        if (e.getActionCommand().equals("Reset")) 
        {
            JTextField[] textFields = {tField1, tField2, tField3, tField4, tField5, tField6, tField7, tField8, tField9, tField10, tField11, tField12, tField13, tField14, tField15, tField16};
            for (JTextField textField : textFields) 
            {
                textField.setText("0");
            }
        }

        if (e.getActionCommand().equals("Freeze")) 
        {
            JButton[] myButtons = {myButton1, myButton2, myButton3, myButton4, myButton5, myButton6, myButton7, myButton8, myButton9, myButton10, myButton11, myButton12, myButton13, myButton14, myButton15, myButton16, myButton17, myButton18, myButton19, myButton20, myButton21, myButton22, myButton23, myButton24, myButton25, myButton26, myButton27, myButton28, myButton29, myButton30, myButton31, myButton32, myButton33};
            for (JButton button : myButtons) 
            {
                button.setEnabled(false);
            }
            myButton34.setText("Unfreeze");
            myButton35.setEnabled(true);
        }
        
        if (e.getActionCommand().equals("Unfreeze")) 
        {
            JButton[] myButtons = {myButton1, myButton2, myButton3, myButton4, myButton5, myButton6, myButton7, myButton8, myButton9, myButton10, myButton11, myButton12, myButton13, myButton14, myButton15, myButton16, myButton17, myButton18, myButton19, myButton20, myButton21, myButton22, myButton23, myButton24, myButton25, myButton26, myButton27, myButton28, myButton29, myButton30, myButton31, myButton32, myButton33};
            for (JButton button : myButtons) 
            {
                button.setEnabled(true);
            }
            myButton34.setText("Freeze");
            myButton35.setEnabled(false);
        }

        if (e.getActionCommand().equals("Process")) 
        {
            // Continue from here
            int result = JOptionPane.showConfirmDialog(this, "Do you want to flip the input matrix upside-down?", "Generating Output File", JOptionPane.YES_NO_OPTION);
            
            if (result == JOptionPane.YES_OPTION) 
            {
                // System.out.println("You clicked Yes");
                JTextField[] textFields = {tField13, tField14, tField15, tField16, tField9, tField10, tField11, tField12, tField5, tField6, tField7, tField8, tField1, tField2, tField3, tField4};
                try 
                {
                    FileWriter writer = new FileWriter("data.csv");
    
                    for (int i = 0; i < textFields.length; i++) 
                    {
                        writer.append(textFields[i].getText());
                        
                        if ((i + 1) % 4 == 0) 
                        {
                            writer.append("\n");
                        } 
                        else if (i != textFields.length - 1) 
                        {
                            writer.append(",");
                        }
                    }
    
                    writer.flush();
                    writer.close();
    
                    JOptionPane.showMessageDialog(this, "Data saved to file!");
    
                } catch (IOException ex) {
                    ex.printStackTrace();
                    JOptionPane.showMessageDialog(this, "Error saving data to file.");
                }
                // dispose();
            } 
            else 
            {
                // System.out.println("You clicked No");
                JTextField[] textFields = {tField1, tField2, tField3, tField4, tField5, tField6, tField7, tField8, tField9, tField10, tField11, tField12, tField13, tField14, tField15, tField16};
                try 
                {
                    FileWriter writer = new FileWriter("data.csv");
    
                    for (int i = 0; i < textFields.length; i++) 
                    {
                        writer.append(textFields[i].getText());
                        
                        if ((i + 1) % 4 == 0) 
                        {
                            writer.append("\n");
                        } 
                        else if (i != textFields.length - 1) 
                        {
                            writer.append(",");
                        }
                    }
    
                    writer.flush();
                    writer.close();
    
                    JOptionPane.showMessageDialog(this, "Data saved to file!");
    
                } catch (IOException ex) {
                    ex.printStackTrace();
                    JOptionPane.showMessageDialog(this, "Error saving data to file.");
                }
                // dispose();
            }

            // Another logic here
            String csvFile = "data.csv";
            BufferedReader br = null;
            String csvFile2 = "distance.csv";
            BufferedReader br2 = null;

            try 
            {
                br = new BufferedReader(new FileReader(csvFile));
                String line = "";
                int row = 0;
                int column = 0;

                while ((line = br.readLine()) != null) 
                {
                    String[] values = line.split(COMMA_DELIMITER);
                    if (row == 0) 
                    {
                        column = values.length;
                    }
                    row++;
                }
                br.close();

                int[][] matrix = new int[row][column];
                br = new BufferedReader(new FileReader(csvFile));
                row = 0;

                //Get the matrix from the data file
                while ((line = br.readLine()) != null) 
                {
                    String[] values = line.split(COMMA_DELIMITER);
                    for (int i = 0; i < column; i++) 
                    {
                        matrix[row][i] = Integer.parseInt(values[i]);
                    }
                    row++;
                }
                br.close();

                // Print matrix
                tArea.append("\nMatrix Imported from Data File\n\n");
                for (int i = 0; i < row; i++) 
                {
                    for (int j = 0; j < column; j++) 
                    {
                        tArea.append(matrix[i][j] + " ");
                    }
                    tArea.append("\n");
                }

                // Calculate distance of each node
                try 
                {
                    ProcessBuilder pb = new ProcessBuilder("python", "distance.py");
                    pb.redirectErrorStream(true);
                    Process p = pb.start();
                    BufferedReader in = new BufferedReader(new InputStreamReader(p.getInputStream()));
                    String line3;
                    while ((line3 = in.readLine()) != null) {
                        System.out.println(line3);
                    }
                    in.close();
                } 
                catch (IOException e4) 
                {
                    e4.printStackTrace();
                }

                br2 = new BufferedReader(new FileReader(csvFile2));
                line = "";
                row = 0;
                column = 0;

                while ((line = br2.readLine()) != null) 
                {
                    String[] values = line.split(COMMA_DELIMITER);
                    if (row == 0) 
                    {
                        column = values.length;
                    }
                    row++;
                }
                br.close();

                matrix = new int[row][column];
                br2 = new BufferedReader(new FileReader(csvFile2));
                row = 0;


                //Get the matrix from the data file
                while ((line = br2.readLine()) != null) 
                {
                    String[] values = line.split(COMMA_DELIMITER);
                    for (int i = 0; i < column; i++) 
                    {
                        matrix[row][i] = Integer.parseInt(values[i]);
                    }
                    row++;
                }
                br2.close();

                // Print matrix
                tArea.append("\nDistance Matrix \n\n");
                for (int i = 0; i < row; i++) 
                {
                    for (int j = 0; j < column; j++) 
                    {
                        tArea.append(matrix[i][j] + " ");
                    }
                    tArea.append("\n");
                }


                // Getting the threshold value
                while(true)
                {
                    String input = JOptionPane.showInputDialog(this, "Enter the Threshold Value:");
                    try 
                    {
                        // Parse the input string as an integer
                        threshold = Integer.parseInt(input);
                        tArea.append("\nThe Threshold Value is : " + threshold + "\n");
                        break;
                    } 
                    catch (NumberFormatException e3) 
                    {
                        tArea.append("\nInvalid input.\n");
                    }
                }

                while(true)
                {
                    String input1 = JOptionPane.showInputDialog(this, "Enter the Source Node:");
                    try 
                    {
                        // Parse the input string as an integer
                        int source = Integer.parseInt(input1);
                        tArea.append("\nThe Source Node is : " + source + "\n");
                        FileWriter myWriter = new FileWriter("numbers.txt",false);
                        myWriter.write(source + ",");
                        myWriter.close();
                        break;
                    } 
                    catch (NumberFormatException e3) 
                    {
                        tArea.append("\nInvalid input.\n");
                    }
                }

                while(true)
                {
                    String input2 = JOptionPane.showInputDialog(this, "Enter the Destination Node:");
                    try 
                    {
                        // Parse the input string as an integer
                        int destination = Integer.parseInt(input2);
                        tArea.append("\nThe Destination Node is : " + destination + "\n");
                        FileWriter myWriter = new FileWriter("numbers.txt",true);
                        myWriter.write(destination + ",");
                        myWriter.close();
                        break;
                    } 
                    catch (NumberFormatException e3) 
                    {
                        tArea.append("\nInvalid input.\n");
                    }
                }

                tArea.append("\nMatrix with Threshold Value\n\n");
                for (int i = 0; i < row; i++) {
                    for (int j = 0; j < column; j++) {
                        if (matrix[i][j] > threshold) {
                            matrix[i][j] = 0;
                        }
                        tArea.append(matrix[i][j] + " ");
                    }
                    tArea.append("\n");
                }

                try 
                {
                    // Create a FileWriter object to write to the "data.csv" file and overwrite it
                    FileWriter writer = new FileWriter("data.csv", false);
                    
                    // Create a BufferedWriter object to buffer the FileWriter
                    BufferedWriter buffer = new BufferedWriter(writer);
                    
                    // Write each element of the distance array to the CSV file
                    for (int i = 0; i < row; i++) {
                        for (int j = 0; j < column; j++) 
                        {
                            if (matrix[i][j] > threshold) 
                            {
                                matrix[i][j] = 0;
                            }
                            buffer.write(matrix[i][j] + "");
                            if (j < column - 1) 
                            {
                                buffer.write(",");
                            }
                        }                        
                        buffer.newLine();
                    }
                    
                    // Close the buffer and writer
                    buffer.close();
                    writer.close();
                    
                    System.out.println("Data written to data.csv");
                } 
                catch (IOException e4) 
                {
                    System.err.println("Error writing to data.csv: " + e4.getMessage());
                }

                try 
                {
                    ProcessBuilder pb = new ProcessBuilder("python", "animation.py");
                    pb.redirectErrorStream(true);
                    Process p = pb.start();
                    BufferedReader in = new BufferedReader(new InputStreamReader(p.getInputStream()));
                    String line3;
                    while ((line3 = in.readLine()) != null) {
                        System.out.println(line3);
                    }
                    in.close();
                } 
                catch (IOException e3) 
                {
                    e3.printStackTrace();
                }
        

            }
            catch (IOException e1) 
            {
                e1.printStackTrace();
            }
        }






        if (e.getSource() == myButton1) 
        {
            int i=Integer.parseInt(tField1.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField1.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton2) 
        {
            int i=Integer.parseInt(tField1.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField1.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton3) 
        {
            int i=Integer.parseInt(tField2.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField2.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton4) 
        {
            int i=Integer.parseInt(tField2.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField2.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton5) 
        {
            int i=Integer.parseInt(tField3.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField3.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton6) 
        {
            int i=Integer.parseInt(tField3.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField3.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton7) 
        {
            int i=Integer.parseInt(tField4.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField4.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton8) 
        {
            int i=Integer.parseInt(tField4.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField4.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton9) 
        {
            int i=Integer.parseInt(tField5.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField5.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton10) 
        {
            int i=Integer.parseInt(tField5.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField5.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton11) 
        {
            int i=Integer.parseInt(tField6.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField6.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton12) 
        {
            int i=Integer.parseInt(tField6.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField6.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton13) 
        {
            int i=Integer.parseInt(tField7.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField7.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton14) 
        {
            int i=Integer.parseInt(tField7.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField7.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton15) 
        {
            int i=Integer.parseInt(tField8.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField8.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton16) 
        {
            int i=Integer.parseInt(tField8.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField8.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton17) 
        {
            int i=Integer.parseInt(tField9.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField9.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton18) 
        {
            int i=Integer.parseInt(tField9.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField9.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton19) 
        {
            int i=Integer.parseInt(tField10.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField10.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton20) 
        {
            int i=Integer.parseInt(tField10.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField10.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton21) 
        {
            int i=Integer.parseInt(tField11.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField11.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton22) 
        {
            int i=Integer.parseInt(tField11.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField11.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton23) 
        {
            int i=Integer.parseInt(tField12.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField12.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton24) 
        {
            int i=Integer.parseInt(tField12.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField12.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton25) 
        {
            int i=Integer.parseInt(tField13.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField13.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton26) 
        {
            int i=Integer.parseInt(tField13.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField13.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton27) 
        {
            int i=Integer.parseInt(tField14.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField14.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton28) 
        {
            int i=Integer.parseInt(tField14.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField14.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton29) 
        {
            int i=Integer.parseInt(tField15.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField15.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton30) 
        {
            int i=Integer.parseInt(tField15.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField15.setText(Integer.toString(i));
            }
        }


        if (e.getSource() == myButton31) 
        {
            int i=Integer.parseInt(tField16.getText());
            if(i==1 || i==2 || i==3 || i==4 || i==5 || i==6)
            {
                i--;
                tField16.setText(Integer.toString(i));
            }
        }

        if (e.getSource() == myButton32) 
        {
            int i=Integer.parseInt(tField16.getText());
            if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)
            {
                i++;
                tField16.setText(Integer.toString(i));
            }
        }
    }

    public static void main(String[] args) 
    {
        new MyFrame();
    }
}
