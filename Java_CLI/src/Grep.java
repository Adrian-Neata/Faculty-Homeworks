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
public class Grep implements ICommand {

    private Directory dir;
    private boolean R = false;
    private final String regex;
    private final BufferedWriter output;
    private final BufferedWriter errors;

    /**
     *
     * @param output the file in which the output goes
     * @param errors the file in which the errors go
     * @param regex the regular expression to which the names of the documents
     * found in the directory are to be matched
     */
    public Grep(BufferedWriter output, BufferedWriter errors, String regex) {
        this.regex = regex;
        this.output = output;
        this.errors = errors;
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
     * @param dir the directory that is going to have its contents shown
     */
    public void setDir(Directory dir) {
        this.dir = dir;
    }

    /**
     * Prints the documents or the tree found in the directory, only shows the
     * names of the documents that match the regex.
     */
    @Override
    public void execute() {
        try {
            if (R) {
                output.write(dir.showTree(regex));
                return;
            }
            output.write(dir.toString(regex));
        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

}
