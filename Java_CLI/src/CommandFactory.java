/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import java.io.*;

/**
 *
 * @author adrian
 */
public class CommandFactory {

    private int nrCrtCommand = 0;
    private static final CommandFactory INSTANCE = new CommandFactory();

    /**
     *
     * @return the only instance of the class (singleton)
     */
    public static CommandFactory getInstance() {
        return INSTANCE;
    }

    /**
     *
     * @param line the line containing the command and its parameters
     * @param output the file in which the output goes
     * @param errors the file in which the errors go
     * @return a new instance of the class corresponding to the command given
     * @throws NoSuchMethodException if the line does not contain a valid
     * command
     */
    public ICommand createCommand(String line, BufferedWriter output, BufferedWriter errors) throws NoSuchMethodException {
        String[] tokens = line.split(" ");
        nrCrtCommand++;
        switch (tokens[0]) {
            case "ls":
                Ls commandLs = new Ls(output, errors, nrCrtCommand);
                if (line.contains("-R")) {
                    commandLs.setR(true);
                    line = line.replace(" -R", "");
                } else {
                    commandLs.setR(false);
                }
                if (line.contains("|")) {
                    String regex = tokens[tokens.length - 1];
                    regex = regex.replace("\"", "");
                    commandLs.setGrep(new Grep(output, errors, regex));
                    line = line.substring(0, line.indexOf('|') - 1);
                }
                tokens = line.split(" ");
                if (tokens.length == 1) {
                    commandLs.setPath(".");
                } else {
                    commandLs.setPath(tokens[1]);
                }
                return commandLs;
            case "pwd":
                return new Pwd(output, errors, nrCrtCommand);
            case "cd":
                Cd commandCd = new Cd(output, errors, nrCrtCommand);
                commandCd.setPath(tokens[1]);
                return commandCd;
            case "cp":
                Cp commandCp = new Cp(output, errors, nrCrtCommand);
                commandCp.setSourcePath(tokens[1]);
                commandCp.setDestPath(tokens[2]);
                return commandCp;
            case "mv":
                Mv commandMv = new Mv(output, errors, nrCrtCommand);
                commandMv.setSourcePath(tokens[1]);
                commandMv.setDestPath(tokens[2]);
                return commandMv;
            case "rm":
                Rm commandRm = new Rm(output, errors, nrCrtCommand);
                commandRm.setPath(tokens[1]);
                return commandRm;
            case "touch":
                Touch commandTouch = new Touch(output, errors, nrCrtCommand);
                commandTouch.setPath(tokens[1]);
                return commandTouch;
            case "mkdir":
                Mkdir commandMkdir = new Mkdir(output, errors, nrCrtCommand);
                commandMkdir.setPath(tokens[1]);
                return commandMkdir;
        }
        throw new NoSuchMethodException();
    }
}
