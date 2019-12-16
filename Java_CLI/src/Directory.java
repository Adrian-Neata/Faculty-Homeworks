/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import java.util.ArrayList;

/**
 *
 * @author adrian
 */
public class Directory extends Document {

    ArrayList<Document> documents;

    /**
     *
     * @param name the name of the directory
     */
    public Directory(String name) {
        this.documents = new ArrayList<>();
        super.name = name;
    }

    /**
     *
     * @param containingDirectory the directory that contains this directory
     */
    public void setContainingDirectory(Directory containingDirectory) {
        this.containingDirectory = containingDirectory;
    }

    /**
     *
     * @param name the name of a document
     * @return the index of the document with the same name found in the
     * directory or -1 if it doesn't exist
     */
    public int getDocumentIndex(String name) {
        int index = 0;
        for (Document doc : documents) {
            if (name.equals(doc.name)) {
                return index;
            }
            index++;
        }
        return -1;
    }

    /**
     *
     * @return a copy of the directory and all its document
     */
    public Directory copy() {
        File file;
        Document d;
        Directory directory;
        Directory dir = new Directory(this.name);
        dir.containingDirectory = this.containingDirectory;
        for (Document doc : this.documents) {
            if (doc instanceof File) {
                d = ((File) doc).copy();
            } else {
                d = ((Directory) doc).copy();
            }
            d.containingDirectory = dir;
            dir.documents.add(d);
        }
        return dir;
    }

    /**
     *
     * @return a string containing the entire tree of the directory
     */
    public String showTree() {
        String s = this.toString();
        for (Document doc : documents) {
            if (doc instanceof Directory) {
                s = s + ((Directory) doc).showTree();
            }
        }
        return s;
    }

    /**
     *
     * @return a string containing the absolute paths of the directory and its
     * documents
     */
    @Override
    public String toString() {
        String s = this.getAbsolutePath() + ":\n";
        for (Document doc : documents) {
            s = s + doc.getAbsolutePath() + " ";
        }
        if (s.endsWith(" ")) {
            s = s.substring(0, s.length() - 1);
        }           //removes the extra space at the end of the line
        s = s + "\n\n";
        return s;
    }

    /**
     *
     * @param regex the regular expression to which the names of the documents
     * found in the directory are to be matched
     * @return a string containing the entire tree of the directory, only
     * showing the documents that match the regex
     */
    public String showTree(String regex) {
        String s = this.toString(regex);
        for (Document doc : documents) {
            if (doc instanceof Directory) {
                s = s + ((Directory) doc).showTree(regex);
            }
        }
        return s;
    }

    /**
     *
     * @param regex the regular expression to which the names of the documents
     * found in the directory are to be matched
     * @return a string containing the absolute paths of the directory and its
     * documents that match the regex with their name
     */
    public String toString(String regex) {
        String s = this.getAbsolutePath() + ":\n";
        for (Document doc : documents) {
            if (doc.name.matches(regex)) {
                s = s + doc.getAbsolutePath() + " ";
            }
        }
        if (s.endsWith(" ")) {
            s = s.substring(0, s.length() - 1);
        }           //removes the extra space at the end of the line
        s = s + "\n\n";
        return s;
    }
}
