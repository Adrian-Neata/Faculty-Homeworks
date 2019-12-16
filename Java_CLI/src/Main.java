/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import java.io.*;
import java.util.ArrayList;

/**
 *
 * @author adrian
 */
public class Main {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        ArrayList<ICommand> commands = new ArrayList<>();
        CommandFactory factory = CommandFactory.getInstance();
        String line;

        try {
            BufferedReader input = new BufferedReader(new FileReader(args[0]));
            BufferedWriter output = new BufferedWriter(new FileWriter(args[1]));
            BufferedWriter errors = new BufferedWriter(new FileWriter(args[2]));

            while ((line = input.readLine()) != null) {
                try {
                    commands.add(factory.createCommand(line, output, errors));
                } catch (NoSuchMethodException ex) {
                    System.out.println(ex.getMessage());
                }
            }
            for (ICommand command : commands) {
                command.execute();
            }
            input.close();
            output.close();
            errors.close();
        } catch (FileNotFoundException ex) {
            System.out.println(ex.getMessage());
        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }

    }

}
