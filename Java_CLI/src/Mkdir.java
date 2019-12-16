/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import java.io.BufferedWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;

/**
 *
 * @author adrian
 */
public class Mkdir implements ICommand {

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
    public Mkdir(BufferedWriter output, BufferedWriter errors, int nrCrtCommand) {
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
     * Creates a new directory with the name and path specified.
     */
    @Override
    public void execute() {
        int index;
        String name;
        Directory destDir;
        ArrayList<Document> list;
        boolean noDirectory = true;
        try {
            output.write(nrCrtCommand + "\n");
            errors.write(nrCrtCommand + "\n");
            name = path.substring(path.lastIndexOf('/') + 1, path.length());
            list = Document.findDocuments(path.substring(0, path.lastIndexOf("/") + 1));
            for (Document doc : list) {
                if (doc instanceof Directory) {
                    noDirectory = false;
                    destDir = (Directory) doc;
                    index = destDir.getDocumentIndex(name);
                    if (index != -1) {
                        errors.write("mkdir: cannot create directory " + destDir.documents.get(index).getAbsolutePath() + ": Node exists\n");
                    } else {
                        Directory dir = new Directory(name);
                        dir.containingDirectory = destDir;
                        destDir.documents.add(dir);
                        Collections.sort(destDir.documents);
                    }
                }
            }
            if (list.isEmpty() || noDirectory) {
                errors.write("mkdir: " + path.substring(0, path.lastIndexOf('/')) + ": No such directory\n");
            }
        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

}
