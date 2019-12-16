import java.io.*;

/**
 *
 * @author Adrian
 */
public class Main {

    /**
     *
     * @param args [0] the input file, [1] the output file
     */
    public static void main(String[] args) {
        String command;
        String cacheType;
        int cacheSize;
        int numberOperations;
        String delims = " ";
        try {
            FileReader fr = new FileReader(args[0]);
            FileWriter fw = new FileWriter(args[1]);
            BufferedWriter bw = new BufferedWriter(fw);
            try (BufferedReader br = new BufferedReader(fr)) {
                cacheType = br.readLine();
                cacheSize = Integer.parseInt(br.readLine());
                Cache cache = new Cache(cacheSize);
                numberOperations = Integer.parseInt(br.readLine());
                for (int counter = 1; counter <= numberOperations; counter++) {
                    command = br.readLine();
                    String[] tokens = command.split(delims);
                    switch (tokens[0]) {
                        case "GET":
                            bw.write(cache.get(tokens[1], cacheType));
                            bw.newLine();
                            break;

                        case "ADD":
                            cache.add(tokens, cacheType);
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
