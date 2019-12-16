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
public class Cp implements ICommand {

    private String destPath;
    private String sourcePath;
    private final int nrCrtCommand;
    private final BufferedWriter output;
    private final BufferedWriter errors;

    /**
     *
     * @param output the file in which the output goes
     * @param errors the file in which the errors go
     * @param nrCrtCommand the current number of the command
     */
    public Cp(BufferedWriter output, BufferedWriter errors, int nrCrtCommand) {
        this.nrCrtCommand = nrCrtCommand;
        this.output = output;
        this.errors = errors;
    }

    /**
     *
     * @param sourcePath the relative or absolute path of the document that will
     * be copied
     */
    public void setSourcePath(String sourcePath) {
        this.sourcePath = sourcePath;
    }

    /**
     *
     * @param destPath the relative or absolute path of the directory in which
     * the document will be copied
     */
    public void setDestPath(String destPath) {
        this.destPath = destPath;
    }

    /**
     * Copies the document found at sourcePath and puts the copy in destPath.
     */
    @Override
    public void execute() {
        String name;
        Document doc;
        Directory destDir;
        ArrayList<Document> list;
        Document sourceDoc = null;
        try {
            output.write(nrCrtCommand + "\n");
            errors.write(nrCrtCommand + "\n");
            name = sourcePath.substring(sourcePath.lastIndexOf('/') + 1, sourcePath.length());

            if (sourcePath.equals(".") == false) {
                list = Document.findDocuments(sourcePath);
                if (list.isEmpty()) {
                    errors.write("cp: cannot copy " + sourcePath + ": No such file or directory\n");
                    return;
                }
                sourceDoc = list.get(0);
            }
            list = Document.findDocuments(destPath);
            if (list.isEmpty() || (doc = list.get(0)) instanceof File) {
                errors.write("cp: cannot copy into " + destPath + ": No such directory\n");
                return;
            }
            destDir = (Directory) doc;
            if (destDir.getDocumentIndex(name) != -1) {
                errors.write("cp: cannot copy " + sourcePath + ": Node exists at destination\n");
                return;
            }

            if (sourcePath.equals(".")) {
                Directory dir = Document.currentDirectory.copy();
                dir.containingDirectory = destDir;
                destDir.documents.add(dir);
                Collections.sort(destDir.documents);
                return;
            }

            if (sourceDoc instanceof Directory) {
                Directory dir = ((Directory) sourceDoc).copy();
                dir.containingDirectory = destDir;
                destDir.documents.add(dir);
            }
            if (sourceDoc instanceof File) {
                File file = ((File) sourceDoc).copy();
                file.containingDirectory = destDir;
                destDir.documents.add(file);
            }
            Collections.sort(destDir.documents);
        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

}
