/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.*;

/**
 *
 * @author Adrian
 */
public class Tema2 {

    /**
     * @param args args [0] the input file
     */
    public static void main(String[] args) {
        String command;
        Database data = new Database();
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(args[0] + "_out"))) {
            try (BufferedReader br = new BufferedReader(new FileReader(args[0]))) {
                while ((command = br.readLine()) != null) {
                    String[] tokens = command.split(" ");
                    switch (tokens[0]) {
                        case "CREATEDB":
                            int nrNodes = Integer.parseInt(tokens[2]);
                            int maxCapacityNode = Integer.parseInt(tokens[3]);
                            data.initialize(tokens[1], nrNodes, maxCapacityNode);
                            break;
                        case "CREATE":
                            data.createEntity(tokens);
                            break;
                        case "INSERT":
                            data.insert(tokens);
                            break;
                        case "DELETE":
                            if (!data.delete(tokens[1], tokens[2])) {
                                bw.write("NO INSTANCE TO DELETE\n");
                            }
                            break;
                        case "UPDATE":
                            data.update(tokens);
                            break;
                        case "GET":
                            bw.write(data.get(tokens[1], tokens[2]));
                            bw.newLine();
                            break;
                        case "SNAPSHOTDB":
                            bw.write(data.SnapshotDB());
                            break;
                        case "CLEANUP":
                            data.cleanup(tokens[1], Long.parseLong(tokens[2]));
                            break;
                        default:
                            break;
                    }
                }
                bw.close();
                br.close();
            }
        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

}
