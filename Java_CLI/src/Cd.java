/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import java.io.BufferedWriter;
import java.io.IOException;
import java.util.ArrayList;

/**
 *
 * @author adrian
 */
public class Cd implements ICommand {

    private String path;
    private final int nrCrtCommand;
    private final BufferedWriter output;
    private final BufferedWriter errors;

    /**
     *
     * @param output the file in which the output goes
     * @param errors the file in which the errors go
     * @param nrCrtCommand the current number of the command
     */
    public Cd(BufferedWriter output, BufferedWriter errors, int nrCrtCommand) {
        this.output = output;
        this.errors = errors;
        this.nrCrtCommand = nrCrtCommand;
    }

    /**
     *
     * @param path the relative or absolute path of the directory
     */
    public void setPath(String path) {
        this.path = path;
    }

    /**
     * Changes the current directory to the one found at the path.
     */
    @Override
    public void execute() {
        Document doc;
        ArrayList<Document> list;
        try {
            output.write(nrCrtCommand + "\n");
            errors.write(nrCrtCommand + "\n");
            list = Document.findDocuments(path);
            if (list.isEmpty() || (doc = list.get(0)) instanceof File) {
                errors.write("cd: " + path + ": No such directory\n");
                return;
            }
            Directory.currentDirectory = (Directory) doc;
        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

}
