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
public class Rm implements ICommand {

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
    public Rm(BufferedWriter output, BufferedWriter errors, int nrCrtCommand) {
        this.output = output;
        this.errors = errors;
        this.nrCrtCommand = nrCrtCommand;
    }

    /**
     *
     * @param path the relative or absolute path of the document/-s to be
     * removed
     */
    public void setPath(String path) {
        this.path = path;
    }

    /**
     * Removes the file/-s found at the path given.
     */
    @Override
    public void execute() {
        String name;
        ArrayList<Document> list;
        boolean noDirectory = true;

        try {
            output.write(nrCrtCommand + "\n");
            errors.write(nrCrtCommand + "\n");
            if (path.equals(".")) {
                return;
            } else {
                list = Document.findDocuments(path);
            }
            if (list.isEmpty()) {
                errors.write("rm: cannot remove " + path + ": No such file or directory\n");
                return;
            }
            for (Document doc : list) {
                String s1 = Document.currentDirectory.getAbsolutePath();
                String s2 = doc.getAbsolutePath();
                if (s1.startsWith(s2) == false) {
                    doc.removeDoc();
                }
            }
        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

}
