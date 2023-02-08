import java.io.BufferedReader;
import java.io.FileReader;
import java.util.LinkedList;

/*
Комбинирующий генератор 7 вар
 */

public class Main {
    public static final int RSLOS1_SIZE = 6;
    public static final int RSLOS2_SIZE = 5;
    public static final int IDLING = 11;
    public static final int GAMMA_SIZE = 16;

    public static void main(String[] args) {
        LinkedList<Integer> RSLOS1 = new LinkedList<>();
        LinkedList<Integer> RSLOS2 = new LinkedList<>();

        try(BufferedReader br = new BufferedReader(new FileReader("task.txt"))) {
            for (int i = 0; i < RSLOS1_SIZE; i++)
                RSLOS1.add(br.read() - '0');
            for (int i = 0; i < RSLOS2_SIZE; i++)
                RSLOS2.add(br.read() - '0');
        } catch(Exception ex) {
            System.out.println(ex.getMessage());
        }

        LinkedList<Integer> gamma = new LinkedList<>();

        System.out.println("------------------------------------------------------------------------");
        System.out.println("                                   Холостой ход");
        System.out.println("------------------------------------------------------------------------");
        for(int i = 0, step = 1; i < IDLING + GAMMA_SIZE; i++, step++) {
            if(i == IDLING) {
                gamma.clear();
                System.out.println("------------------------------------------------------------------------");
                System.out.println("                         Вычисляем значение гаммы");
                System.out.println("------------------------------------------------------------------------");
                step = 1;
            }

//            System.out.println(step + ". Начальное состояние: RSLOS1: " + RSLOS1 + "; RSLOS2: " + RSLOS2);
            System.out.printf("%d ", step);
            for(int j = 0; j < RSLOS1_SIZE; j++)
                System.out.print(RSLOS1.get(j));
            System.out.print(" ");

            for(int j = 0; j < RSLOS2_SIZE; j++)
                System.out.print(RSLOS2.get(j));
            System.out.print(" ");

            int f = getF(RSLOS1);

//            System.out.println("Значение функции f: " + f);
            changeRSLOS1(RSLOS1);
            System.out.printf("%d ", f);

            changeRSLOS2(RSLOS2, gamma, f);
            for(int j = 0; j < gamma.size(); j++)
                System.out.print(gamma.get(j));
            System.out.println();
//            System.out.println("Конечное состояние: RSLOS1: " + RSLOS1 + "; RSLOS2: " + RSLOS2 + "; Gamma: " + gamma);
        }
    }

    public static int getF(LinkedList<Integer> RSLOS1) {
        return RSLOS1.get(2) ^ RSLOS1.get(3) * RSLOS1.get(4);
    }

    public static void changeRSLOS1(LinkedList<Integer> RSLOS1) {
        RSLOS1.addFirst(RSLOS1.get(0) ^ RSLOS1.get(4));
        RSLOS1.removeLast();
    }
    public static void changeRSLOS2(LinkedList<Integer> RSLOS2, LinkedList<Integer> gamma, int f) {
        RSLOS2.addFirst(RSLOS2.get(0) ^ RSLOS2.get(2));
        gamma.addFirst(RSLOS2.removeLast() ^ f);
    }
}