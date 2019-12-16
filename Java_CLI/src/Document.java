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
public class Document implements Comparable<Document> {

    String name;
    Directory containingDirectory = null;

    /**
     * The root directory of the document system, it has no name.
     */
    public static Directory rootDirectory = new Directory("");

    /**
     * The current directory of the document system, starts off with the root
     * directory.
     */
    public static Directory currentDirectory = rootDirectory;

    /**
     *
     * @param doc the document to be compared
     * @return the value 0 if the document's name is equal to this document's
     * name; a value less than 0 if this name is lexicographically less than the
     * name of the document; and a value greater than 0 if this name is
     * lexicographically greater than the name of the document.
     */
    @Override
    public int compareTo(Document doc) {
        return name.compareTo(doc.name);
    }

    /**
     *
     * @return a string representing the absolute path of the document
     */
    public String getAbsolutePath() {
        String path = name;
        Directory dir = containingDirectory;
        if (dir == null) {
            return "/";
        }
        while (dir != null) {
            path = dir.name + "/" + path;
            dir = dir.containingDirectory;
        }
        return path;
    }

    /**
     *
     * @return true if a document with the same name was found and removed or
     * false if it doesn't exist
     */
    public boolean removeDoc() {
        int index;
        Directory dir = containingDirectory;
        if (dir == null || (index = dir.getDocumentIndex(name)) == -1) {
            return false;
        }
        dir.documents.remove(index);
        return true;
    }

    private static ArrayList<Document> findMatches(String name, ArrayList<Document> list) {
        Directory dir;
        ArrayList<Document> newList = new ArrayList<>();
        if (name.equals(".") || name.equals("")) {
            return list;
        }   //nothing happens
        if (name.equals("..")) {
            for (Document doc : list) {
                if (doc.containingDirectory != null) {
                    newList.add(doc.containingDirectory);
                }
            }
            return newList;
        }   //go back to the directory that contains the document
        if (name.equals("*")) {
            for (Document doc1 : list) {
                if (doc1 instanceof Directory) {
                    dir = (Directory) doc1;
                    for (Document doc2 : dir.documents) {
                        newList.add(doc2);
                    }
                }
            }
            return newList;
        }   //if the document is a directory everything goes in the list
        if (name.startsWith("*")) {
            name = name.substring(1, name.length());
            for (Document doc1 : list) {
                if (doc1 instanceof Directory) {
                    dir = (Directory) doc1;
                    for (Document doc2 : dir.documents) {
                        if (doc2.name.endsWith(name)) {
                            newList.add(doc2);
                        }
                    }
                }
            }
            return newList;
        }   //if the document is a directory everything that ends with what's on the right of the "*" goes in the list
        if (name.endsWith("*")) {
            name = name.substring(0, name.length() - 1);
            for (Document doc1 : list) {
                if (doc1 instanceof Directory) {
                    dir = (Directory) doc1;
                    for (Document doc2 : dir.documents) {
                        if (doc2.name.startsWith(name)) {
                            newList.add(doc2);
                        }
                    }
                }
            }
            return newList;
        }   //if the document is a directory everything that starts with what's on the left of the "*" goes in the list
        if (name.contains("*")) {
            String s1 = name.substring(0, name.indexOf("*"));
            String s2 = name.substring(name.indexOf("*") + 1, name.length());
            for (Document doc1 : list) {
                if (doc1 instanceof Directory) {
                    dir = (Directory) doc1;
                    for (Document doc2 : dir.documents) {
                        if (doc2.name.startsWith(s1) && doc2.name.endsWith(s2)) {
                            newList.add(doc2);
                        }
                    }
                }
            }
            return newList;
        }   //if the document is a directory everything that ends with what's on the right of the "*" 
        //and starts with what's on the left of the "*" goes in the list
        for (Document doc1 : list) {
            if (doc1 instanceof Directory) {
                dir = (Directory) doc1;
                for (Document doc2 : dir.documents) {
                    if (doc2.name.equals(name)) {
                        newList.add(doc2);
                    }
                }
            }
        }
        return newList;
    }   //if the document is a directory everything that has the same name as the parameter goes in the list

    /**
     *
     * @param path the relative or absolute path of the document/-s to be found.
     * May contain '*'.
     * @return a list of documents found at the path
     */
    public static ArrayList<Document> findDocuments(String path) {
        int index = 0;
        String[] tokens = path.split("/");
        ArrayList<Document> list = new ArrayList<>();

        if (path.startsWith("/")) {
            index++;
            list.add(Document.rootDirectory);
        } else {
            list.add(Document.currentDirectory);
        }

        while (index < tokens.length) {
            list = Document.findMatches(tokens[index], list);
            index++;
        }
        return list;
    }
}

class File extends Document {

    public File(String name) {
        super.name = name;
    }

    public void setContainingDirectory(Directory containingDirectory) {
        this.containingDirectory = containingDirectory;
    }

    public File copy() {
        File file = new File(this.name);
        file.containingDirectory = this.containingDirectory;
        return file;
    }
}
