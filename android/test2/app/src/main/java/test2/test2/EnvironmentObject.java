package test2.test2;

import java.util.Vector;

/**
 * Created by Dorian on 2015-05-13.
 */
public class EnvironmentObject {
    Vector<Double> point1;
    Vector<Double> point2;
    Integer point1ID;
    Integer point2ID;

    double u2x;
    double u2z;


    public EnvironmentObject() {
        point1 = new Vector<Double>();
        point2 = new Vector<Double>();
        point1ID = 0;
        point2ID = 0;
        u2x = 0;
        u2z = 0;
    }

    public void countU() {
        u2x = point2.get(0) - point1.get(0);
        u2z = point2.get(2) - point1.get(2);
    }
}

