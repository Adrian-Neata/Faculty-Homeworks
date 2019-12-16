/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import java.io.BufferedWriter;
import java.io.IOException;

/**
 *
 * @author adrian
 */
public class Pwd implements ICommand {

    private final int nrCrtCommand;
    private final BufferedWriter output;
    private final BufferedWriter errors;

    /**
     *
     * @param output the file in which the output goes
     * @param errors the file in which the errors go
     * @param nrCrtCommand the current number of the command
     */
    public Pwd(BufferedWriter output, BufferedWriter errors, int nrCrtCommand) {
        this.errors = errors;
        this.output = output;
        this.nrCrtCommand = nrCrtCommand;

    }

    /**
     * Shows the absolute path of the current directory.
     */
    @Override
    public void execute() {
        try {
            output.write(nrCrtCommand + "\n");
            errors.write(nrCrtCommand + "\n");
            output.write(Document.currentDirectory.getAbsolutePath() + "\n");
        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

}
