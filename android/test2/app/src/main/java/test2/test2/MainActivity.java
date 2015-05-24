package test2.test2;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.lamerman.FileDialog;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.Vector;
import java.lang.Math;


public class MainActivity extends ActionBarActivity {

    private Vector<EnvironmentObject> environmentObjs;
    private Vector<Double> robotPosition;
    private TextView podglad;

    public void buttonSimClicked(View v){
        Toast.makeText(getApplicationContext(),"Pozycja robota: " +robotPosition.get(0).toString() +
                " " + robotPosition.get(1).toString() + " " + robotPosition.get(2).toString() ,Toast.LENGTH_LONG).show();
        podglad.setText("Wynik symulacji:\n");
        simulation(5);
    }

    private void simulation(double katInterwal)
    {
        for(double kat = -120.0f; kat < 120.0f; kat += katInterwal)
        {
            double P10x = robotPosition.get(0);
            double P10z = robotPosition.get(2);
            double u1x = Math.sin(Math.toRadians(kat));
            double u1z = Math.cos(Math.toRadians(kat));

            double dist = 3.00;                              //jako ze 300 cm
            double xInterBest = 0;
            double zInterBest = 0;
            int pointIDBest1 = 0;
            int pointIDBest2 = 0;
            for(int i=0; i<environmentObjs.size(); i++)
            {
                EnvironmentObject line = environmentObjs.get(i);
              //  double detA = -u1x*line.u2z + u1z*line.u2x;
                double detA = -line.u2x*u1z + u1x*line.u2z;
                double dPx = P10x - line.point1.get(0);
                double dPz = P10z - line.point1.get(2) ;
                if(Math.abs(detA) > 0.001)
                {

                    double t = (1/detA)*(-u1z*dPx + u1x*dPz);                   //dla linii
                    double s = (1/detA)*(-line.u2z*dPx + line.u2x*dPz);         //dla robota
                    if(t<=1 && t >=0 && s > 0)
                    {
                        double xInter = P10x + s*u1x;
                        double zInter = P10z + s*u1z;
                        double distance = Math.sqrt((xInter-P10x)*(xInter-P10x) + (zInter-P10z)*(zInter-P10z));
                        if(distance <= dist)
                        {
                            dist= distance;
                            xInterBest = xInter;
                            zInterBest = zInter;
                            pointIDBest1 = line.point1ID;
                            pointIDBest2 = line.point2ID;
                        }
                    }
                }
            }
        //    String formatout = String.format("%.2f");
            podglad.append(Double.toString(kat) + " " + Double.toString(pointIDBest1) + " " + Double.toString(pointIDBest2) + " " + Double.toString(dist) + "\n\n");
        }
    }

    public void buttonClicked (View v){
        Toast.makeText(getApplicationContext(),"Otwieram plik symulacyjny", Toast.LENGTH_LONG).show();
        Intent intent = new Intent(getBaseContext(),FileDialog.class);
        intent.putExtra(FileDialog.START_PATH, "/sdcard/");
        intent.putExtra(FileDialog.CAN_SELECT_DIR, false);
        startActivityForResult(intent, 0);
    }

    public synchronized void onActivityResult(final int requestCode, int resultCode, final Intent data)
    {
        String filePath = data.getStringExtra(FileDialog.RESULT_PATH);
        //Toast.makeText(getApplicationContext(),filePath, Toast.LENGTH_LONG).show();
        if(filePath.isEmpty())
        {
            Toast.makeText(getApplicationContext(),"Brak pliku!",Toast.LENGTH_LONG).show();
            return;
        }
        loadOBJFile(filePath);
    }

    protected void loadOBJFile(String filePath)
    {
        environmentObjs = new Vector<EnvironmentObject>();
        podglad = (TextView) findViewById(R.id.logger);
        String allValues;
        StringBuffer stringBuffer = new StringBuffer();
        try {
            FileInputStream fis = new FileInputStream (new File(filePath));
            BufferedReader inputReader = new BufferedReader (new InputStreamReader(fis));

            while ((allValues = inputReader.readLine()) != null) {
                stringBuffer.append(allValues + "\n");
            }

            fis.close();
        } catch (Exception e) {
            podglad.setText(e.getMessage());
            return;
        }
        String allValues2 = stringBuffer.toString();

        Vector<Vector<Double>> points = new Vector<Vector<Double>>();
        try {
            if (allValues2 != null) {
                int len = allValues2.length();
                for (int i = 0; i < len-1; i++) {
                    if (allValues2.charAt(i) == '\n' && allValues2.charAt(i + 1) == 'v') {
                        i += 2;
                        Vector<Double> pointVec = new Vector<Double>();
                        for (int k = 0; k < 3; k++) {
                            while (!((allValues2.charAt(i) <= '9' && allValues2.charAt(i) >= '0') || allValues2.charAt(i) == '-')) i += 1;
                            int z = i;
                            while (allValues2.charAt(z) == '.' || allValues2.charAt(z) == '-' || (allValues2.charAt(z) <= '9' && allValues2.charAt(z) >= '0')) z += 1;
                            String valueString = allValues2.substring(i, z);
                            Log.d("LICZBA:", valueString);
                        //    podglad.append(" ");
                        //    podglad.append(valueString);
                            i = z;
                            pointVec.add(Double.parseDouble(valueString));
                        }
                    //    podglad.append("\n");
                        i-=1;
                        points.add(pointVec);
                    }
                    if (allValues2.charAt(i) == '\n' && allValues2.charAt(i + 1) == 'l') {
                        i += 2;
                        EnvironmentObject oneObject = new EnvironmentObject();
                        for(int k=0; k<2; k++) {

                            while (!(allValues2.charAt(i) <= '9' && allValues2.charAt(i) >= '0')) i += 1;
                            int z = i;
                            while (allValues2.charAt(z) <= '9' && allValues2.charAt(z) >= '0') z += 1;
                            String valueString = allValues2.substring(i,z);
                            i = z;
                            if(k==0)
                                oneObject.point1ID = Integer.parseInt(valueString);
                            else
                                oneObject.point2ID = Integer.parseInt(valueString);
                        }
                        oneObject.point1 = points.get(oneObject.point1ID-1);
                        oneObject.point2 = points.get(oneObject.point2ID-1);
                        environmentObjs.add(oneObject);
                     //   podglad.append("\n");
                     //   podglad.append(oneObject.point1ID.toString() + " " + oneObject.point2ID.toString());
                        Log.d("L parametry:", oneObject.point1ID.toString() + " " + oneObject.point2ID.toString());
                        i-=1;
                    }
                }
                //Wylistowanie wszystkich linii oraz obliczenie wektorow u
                for(int i=0; i<environmentObjs.size(); i++)
                {
                    environmentObjs.get(i).countU();
                    EnvironmentObject tmp = environmentObjs.get(i);
                    podglad.append(tmp.point1ID.toString() + " " + tmp.point2ID.toString() + ": \n\tfrom: ");
                    for(int z=0; z<3; z++)
                        podglad.append(tmp.point1.get(z).toString() + " ");
                    podglad.append("\n\tto: ");
                    for(int z=0; z<3; z++)
                        podglad.append(tmp.point2.get(z).toString() + " ");
                    podglad.append("\n");
                }
            }
        } catch (Exception e) {
            podglad.setText(e.toString());
            return;
        }
        Button simButton = (Button) findViewById(R.id.buttonSim);
        simButton.setEnabled(true);
        Toast.makeText(getApplicationContext(),"POWODZENIE!",Toast.LENGTH_LONG).show();
    }

    Button b1;
   // Button bS;
   @Override
   protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        b1 = (Button) findViewById(R.id.button);
       // bS = (Button) findViewById(R.id.simulationButton);
       robotPosition = new Vector<Double>();
       robotPosition.add(0.0);
       robotPosition.add(0.0);
       robotPosition.add(0.0);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);

        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            Toast obiekcik = Toast.makeText(getApplicationContext(), "Za 5s telefon wybuchnie", Toast.LENGTH_SHORT);
            obiekcik.show();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
