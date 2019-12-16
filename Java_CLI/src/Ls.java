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
public class Ls implements ICommand {

    private boolean R;
    private String path;
    private Grep grep = null;
    private final int nrCrtCommand;
    private final BufferedWriter output;
    private final BufferedWriter errors;

    /**
     *
     * @param output the file in which the output goes
     * @param errors the file in which the errors go
     * @param nrCrtCommand the current number of the command
     */
    public Ls(BufferedWriter output, BufferedWriter errors, int nrCrtCommand) {
        this.output = output;
        this.errors = errors;
        this.nrCrtCommand = nrCrtCommand;
    }

    /**
     *
     * @param R true if the ls command that goes with grep has this extension or
     * false else
     */
    public void setR(boolean R) {
        this.R = R;
    }

    /**
     *
     * @param path the relative or absolute path of the directory
     */
    public void setPath(String path) {
        this.path = path;
    }

    /**
     *
     * @param grep the grep command that goes with ls. May be null if it doesn't
     * exist.
     */
    public void setGrep(Grep grep) {
        this.grep = grep;
    }

    /**
     * Prints the documents or the tree found in the directory. It may do that
     * by executing the grep command instead.
     */
    @Override
    public void execute() {
        Directory dir;
        ArrayList<Document> list;
        boolean noDirectory = true;
        try {
            output.write(nrCrtCommand + "\n");
            errors.write(nrCrtCommand + "\n");
            list = Document.findDocuments(path);
            for (Document doc : list) {
                if (doc instanceof Directory) {
                    noDirectory = false;
                    dir = (Directory) doc;
                    if (grep != null) {
                        grep.setDir(dir);
                        if (R) {
                            grep.setR(true);
                        }
                        grep.execute();
                        return;
                    }
                    if (R) {
                        output.write(dir.showTree());
                        return;
                    }
                    output.write(dir.toString());
                }
            }
            if (list.isEmpty() || noDirectory) {
                errors.write("ls: " + path + ": No such directory\n");
            }
        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

}
