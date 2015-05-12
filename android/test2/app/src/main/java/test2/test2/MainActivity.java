package test2.test2;

import android.content.Context;
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

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Vector;



public class MainActivity extends ActionBarActivity {

    Vector<EnvironmentObject> environmentObjs;

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
        TextView podglad = (TextView) findViewById(R.id.logger);
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
        //podglad.setText(allValues2);
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
                            podglad.append(" ");
                            podglad.append(valueString);
                            i = z;
                            pointVec.add(Double.parseDouble(valueString));
                        }
                        podglad.append("\n");
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
                        podglad.append("\n");
                        podglad.append(oneObject.point1ID.toString() + " " + oneObject.point2ID.toString());
                        Log.d("L parametry:", oneObject.point1ID.toString() + " " + oneObject.point2ID.toString());
                        i-=1;
                    }
                }
            }
        } catch (Exception e) {
            podglad.setText(e.toString());
            return;
        }
        Toast.makeText(getApplicationContext(),"POWODZENIE!",Toast.LENGTH_LONG).show();
    }

    Button b1;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        b1 = (Button) findViewById(R.id.button);
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
