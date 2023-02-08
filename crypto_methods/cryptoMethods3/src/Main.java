import java.io.BufferedReader;
import java.io.FileReader;

public class Main {
    public static final int KEY_SIZE = 9;
    public static void main(String[] args) {
        StringBuilder str = new StringBuilder();
        try(BufferedReader br = new BufferedReader(new FileReader("text.txt"))) {
            int nextChar = -1;
            while((nextChar = br.read()) != -1)
                str.append(Character.toString(nextChar));
        } catch(Exception ex) {
            System.out.println(ex.getMessage());
        }


        String string = str.toString().toLowerCase();
        string = string.replace(" ", "прб").replace(".", "тчк").replace(":", "двт");
        string = string.concat("кнц");

        printAsTable(string, new int[]{1, 2, 3, 4, 5, 6, 7, 8, 9});
        System.out.println("-------------------------------------");
        printAsTable(string, new int[]{6, 3, 1, 8, 7, 9, 5, 2, 4});

        System.out.println(getCipherText(string, new int[]{6, 3, 1, 8, 7, 9, 5, 2, 4}));
    }

    public static void printAsTable(String string, int[] ind) {
        System.out.println(ind[0] + "\t" + ind[1] + "\t" + ind[2] + "\t" + ind[3] + "\t" + ind[4] + "\t" + ind[5] + "\t" + ind[6] + "\t" + ind[7] + "\t" + ind[8] + "\t");
        for(int i = 0; i < string.length(); i += KEY_SIZE)
            System.out.println(string.charAt(i + ind[0] - 1) + "\t" +
                    string.charAt(i + ind[1] - 1) + "\t" +
                    string.charAt(i + ind[2] - 1) + "\t" +
                    string.charAt(i + ind[3] - 1) + "\t" +
                    string.charAt(i + ind[4] - 1) + "\t" +
                    string.charAt(i + ind[5] - 1) + "\t" +
                    string.charAt(i + ind[6] - 1) + "\t" +
                    string.charAt(i + ind[7] - 1) + "\t" +
                    string.charAt(i + ind[8] - 1) + "\t");
    }

    public static String getCipherText(String string, int[] ind) {
        StringBuilder str = new StringBuilder();
        for(int i = 0; i < string.length(); i += KEY_SIZE)
            str.append(string.charAt(i + ind[0] - 1)).
                append(string.charAt(i + ind[1] - 1)).
                append(string.charAt(i + ind[2] - 1)).
                append(string.charAt(i + ind[3] - 1)).
                append(string.charAt(i + ind[4] - 1)).
                append(string.charAt(i + ind[5] - 1)).
                append(string.charAt(i + ind[6] - 1)).
                append(string.charAt(i + ind[7] - 1)).
                append(string.charAt(i + ind[8] - 1));
        return str.toString();
    }
}