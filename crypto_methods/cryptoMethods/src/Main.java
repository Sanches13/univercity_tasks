import java.util.HashMap;
import java.util.Map;

public class Main {
    public static int[] x1 = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1};
    public static int[] x2 = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1};
    public static int[] x3 = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1};
    public static int[] x4 = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    public static int[] x1x2 = get_x1x2(x1, x2);
    public static int[] x1x3 = get_x1x2(x1, x3);
    public static int[] x1x4 = get_x1x2(x1, x4);
    public static int[] x2x3 = get_x1x2(x2, x3);
    public static int[] x2x4 = get_x1x2(x2, x4);
    public static int[] x3x4 = get_x1x2(x3, x4);
    public static int[] x1x2x3 = get_x1x2x3(x1, x2, x3);
    public static int[] x1x2x4 = get_x1x2x3(x1, x2, x4);
    public static int[] x1x3x4 = get_x1x2x3(x1, x3, x4);
    public static int[] x2x3x4 = get_x1x2x3(x2, x3, x4);
    public static int[] x1x2x3x4 = get_x1x2x3x4(x1, x2, x3, x4);

    public static int[] x1INV = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    public static int[] x2INV = {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0};
    public static int[] x3INV = {1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0};
    public static int[] x4INV = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
    public static int[] x1x2INV = get_x1x2(x1INV, x2INV);
    public static int[] x1x3INV = get_x1x2(x1INV, x3INV);
    public static int[] x1x4INV = get_x1x2(x1INV, x4INV);
    public static int[] x2x3INV = get_x1x2(x2INV, x3INV);
    public static int[] x2x4INV = get_x1x2(x2INV, x4INV);
    public static int[] x3x4INV = get_x1x2(x3INV, x4INV);
    public static int[] x1x2x3INV = get_x1x2x3(x1INV, x2INV, x3INV);
    public static int[] x1x2x4INV = get_x1x2x3(x1INV, x2INV, x4INV);
    public static int[] x1x3x4INV = get_x1x2x3(x1INV, x3INV, x4INV);
    public static int[] x2x3x4INV = get_x1x2x3(x2INV, x3INV, x4INV);
    public static int[] x1x2x3x4INV = get_x1x2x3x4(x1INV, x2INV, x3INV, x4INV);

    public static void main(String[] args) {
        int[] f = {0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1};

        System.out.println("1. Сбалансированность.");
        checkBalance(f);
        System.out.println("___________________________________________________________");

        System.out.println("2. Нелинейность.");
        checkNonlinearity(f);
        System.out.println("___________________________________________________________");

        System.out.println("f(x1=0): " + getDifference(f, new int[]{0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0}));
        System.out.println("f(x1=1): " + getDifference(f, new int[]{1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1}));
        System.out.println("f(x2=0): " + getDifference(f, new int[]{0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0}));
        System.out.println("f(x2=1): " + getDifference(f, new int[]{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1}));
        System.out.println("f(x3=0): " + getDifference(f, new int[]{0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1}));
        System.out.println("f(x3=1): " + getDifference(f, new int[]{1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1}));
        System.out.println("f(x4=0): " + getDifference(f, new int[]{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}));
        System.out.println("f(x4=1): " + getDifference(f, new int[]{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1}));

    }

    public static int[] get_x1x2(int[] x1, int[] x2) {
        int[] x1x2 = new int[16];
        for(int i = 0; i < 16; i++)
            x1x2[i] = x1[i] ^ x2[i];
        return x1x2;
    }

    public static int[] get_x1x2x3(int[] x1, int[] x2, int[] x3) {
        int[] x1x2x3 = new int[16];
        for(int i = 0; i < 16; i++)
            x1x2x3[i] = x1[i] ^ x2[i] ^ x3[i];
        return x1x2x3;
    }

    public static int[] get_x1x2x3x4(int[] x1, int[] x2, int[] x3, int[] x4) {
        int[] x1x2x3x4 = new int[16];
        for(int i = 0; i < 16; i++)
            x1x2x3x4[i] = x1[i] ^ x2[i] ^ x3[i] ^ x4[i];
        return x1x2x3x4;
    }

    public static int getDifference(int[] f, int[] x) {
        int count = 0;
        for(int i = 0; i < 16; i++)
            if(f[i] != x[i])
                count++;
        return count;
    }

    public static int getNumberOfOnes(int[] f) {
        int count = 0;
        for(int i = 0; i < 16; i++)
            if(f[i] == 1)
                count++;
        return count;
    }

    public static void checkBalance(int[] f) {
        System.out.println("Число единиц: " + getNumberOfOnes(f) +
                ", число нулей:" + (16 - getNumberOfOnes(f)));
        if(getNumberOfOnes(f) == 8)
            System.out.println("Так как количество нулей и единиц совпадает, то функция сбалансирована. P(0) = P(1) = 0.5");
        else
            System.out.println("Так как количество нулей и единиц не совпадает, то функция не сбалансирована.");
    }

    public static void getTable(int[] f) {
        System.out.printf("%4s%4s%4s%4s%4s%7s%7s%7s%7s%7s%7s%9s%9s%9s%9s%13s\n", "f", "x1", "x2", "x3", "x4", "x1+x2", "x1+x3", "x1+x4", "x2+x3", "x2+x4", "x3+x4", "x1+x2+x3", "x1+x2+x4", "x1+x3+x4", "x2+x3+x4", "x1+x2+x3+x4");
        for(int i = 0; i < 16; i++)
            System.out.printf("%4d%4d%4d%4d%4d%7d%7d%7d%7d%7d%7d%9d%9d%9d%9d%13d\n", f[i], x1[i], x2[i], x3[i], x4[i], x1x2[i], x1x3[i], x1x4[i], x2x3[i], x2x4[i], x3x4[i], x1x2x3[i], x1x2x4[i], x1x3x4[i], x2x3x4[i], x1x2x3x4[i]);
    }

    public static void getNonlinearityDegree(int[] f) {
        //System.out.printf("\n%4d%4d%4d%4d%4d%7d%7d%7d%7d%7d%7d%9d%9d%9d%9d%13d\n", getDifference(f, f), getDifference(f, x1), getDifference(f, x2), getDifference(f, x3), getDifference(f, x4), getDifference(f, x1x2), getDifference(f, x1x3), getDifference(f, x1x4), getDifference(f, x2x3), getDifference(f, x2x4), getDifference(f, x3x4), getDifference(f, x1x2x3), getDifference(f, x1x2x4), getDifference(f, x1x3x4), getDifference(f, x2x3x4), getDifference(f, x1x2x3x4));
        System.out.println("Степень нелинейности x1: " + getDifference(f, x1));
        System.out.println("Степень нелинейности x2: " + getDifference(f, x2));
        System.out.println("Степень нелинейности x3: " + getDifference(f, x3));
        System.out.println("Степень нелинейности x4: " + getDifference(f, x4));
        System.out.println("Степень нелинейности x1x2: " + getDifference(f, x1x2));
        System.out.println("Степень нелинейности x1x3: " + getDifference(f, x1x3));
        System.out.println("Степень нелинейности x1x4: " + getDifference(f, x1x4));
        System.out.println("Степень нелинейности x2x3: " + getDifference(f, x2x3));
        System.out.println("Степень нелинейности x2x4: " + getDifference(f, x2x4));
        System.out.println("Степень нелинейности x3x4: " + getDifference(f, x3x4));
        System.out.println("Степень нелинейности x1x2x3: " + getDifference(f, x1x2x3));
        System.out.println("Степень нелинейности x1x2x4: " + getDifference(f, x1x2x4));
        System.out.println("Степень нелинейности x1x3x4: " + getDifference(f, x1x3x4));
        System.out.println("Степень нелинейности x2x3x4: " + getDifference(f, x2x3x4));
        System.out.println("Степень нелинейности x1x2x3x4: " + getDifference(f, x1x2x3x4));

        System.out.println();
        System.out.println("Степень нелинейности xINV1: " + getDifference(f, x1INV));
        System.out.println("Степень нелинейности x2INV: " + getDifference(f, x2INV));
        System.out.println("Степень нелинейности x3INV: " + getDifference(f, x3INV));
        System.out.println("Степень нелинейности x4INV: " + getDifference(f, x4INV));
        System.out.println("Степень нелинейности x1x2INV: " + getDifference(f, x1x2INV));
        System.out.println("Степень нелинейности x1x3INV: " + getDifference(f, x1x3INV));
        System.out.println("Степень нелинейности x1x4INV: " + getDifference(f, x1x4INV));
        System.out.println("Степень нелинейности x2x3INV: " + getDifference(f, x2x3INV));
        System.out.println("Степень нелинейности x2x4INV: " + getDifference(f, x2x4INV));
        System.out.println("Степень нелинейности x3x4INV: " + getDifference(f, x3x4INV));
        System.out.println("Степень нелинейности x1x2x3INV: " + getDifference(f, x1x2x3INV));
        System.out.println("Степень нелинейности x1x2x4INV: " + getDifference(f, x1x2x4INV));
        System.out.println("Степень нелинейности x1x3x4INV: " + getDifference(f, x1x3x4INV));
        System.out.println("Степень нелинейности x2x3x4INV: " + getDifference(f, x2x3x4INV));
        System.out.println("Степень нелинейности x1x2x3x4INV: " + getDifference(f, x1x2x3x4INV));
    }

    public static void getMinNonlinearityDegree(int[] f) {
        Map<String, Integer> map = new HashMap<>();
        map.put("x1", getDifference(f, x1));
        map.put("x2", getDifference(f, x2));
        map.put("x3", getDifference(f, x3));
        map.put("x4", getDifference(f, x4));
        map.put("x1x2", getDifference(f, x1x2));
        map.put("x1x3", getDifference(f, x1x3));
        map.put("x1x4", getDifference(f, x1x4));
        map.put("x2x3", getDifference(f, x2x3));
        map.put("x2x4", getDifference(f, x2x4));
        map.put("x3x4", getDifference(f, x3x4));
        map.put("x1x2x3", getDifference(f, x1x2x3));
        map.put("x1x2x4", getDifference(f, x1x2x4));
        map.put("x1x3x4", getDifference(f, x1x3x4));
        map.put("x2x3x4", getDifference(f, x2x3x4));
        map.put("x1x2x3x4", getDifference(f, x1x2x3x4));

        String min = "";
        int minValue = 16;
        for(Map.Entry<String, Integer> entry: map.entrySet()) {
            if(entry.getValue() < minValue) {
                minValue = entry.getValue();
                min = entry.getKey();
            }
        }
        System.out.println("При " + min + " нелинейность " + minValue + "-го порядка");
    }
    public static void checkNonlinearity(int[] f) {
        getTable(f);
        getNonlinearityDegree(f);
        getMinNonlinearityDegree(f);
    }
}