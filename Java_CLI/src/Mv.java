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
public class Mv implements ICommand {

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
    public Mv(BufferedWriter output, BufferedWriter errors, int nrCrtCommand) {
        this.output = output;
        this.errors = errors;
        this.nrCrtCommand = nrCrtCommand;
    }

    /**
     *
     * @param sourcePath the relative or absolute path of the document to be
     * moved
     */
    public void setSourcePath(String sourcePath) {
        this.sourcePath = sourcePath;
    }

    /**
     *
     * @param destPath the relative or absolute path of the directory in which
     * the document will be moved
     */
    public void setDestPath(String destPath) {
        this.destPath = destPath;
    }

    /**
     * Moves the document found at sourcePath in the directory at destPath.
     */
    @Override
    public void execute() {
        String name;
        Document doc;
        Document destDoc;
        Directory destDir;
        ArrayList<Document> list;
        try {
            output.write(nrCrtCommand + "\n");
            errors.write(nrCrtCommand + "\n");
            list = Document.findDocuments(sourcePath);
            if (list.isEmpty()) {
                errors.write("mv: cannot move " + sourcePath + ": No such file or directory\n");
                return;
            }
            doc = list.get(0);
            list = Document.findDocuments(destPath);
            if (list.isEmpty() || (destDoc = list.get(0)) instanceof File) {
                errors.write("mv: cannot move into " + destPath + ": No such directory\n");
                return;
            }
            destDir = (Directory) destDoc;
            if (destDir.getDocumentIndex(doc.name) != -1) {
                errors.write("mv: cannot move " + sourcePath + ": Node exists at destination\n");
                return;
            }

            doc.removeDoc();
            doc.containingDirectory = destDir;
            destDir.documents.add(doc);
            Collections.sort(destDir.documents);
        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

}
