package com.example.techsavvy;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Method;
import java.util.Set;
import java.util.StringTokenizer;
import java.util.UUID;

public class Bluetooth extends AppCompatActivity {

    // GUI Components
    private TextView mBluetoothStatus;
    //private TextView mReadBuffer;
    private static TextView mdriveCmd;
    private static TextView msteerCmd;
    private static TextView mrpm;
    private static TextView mobsFront;
    private static TextView mobsRight;
    private static TextView mobsLeft;
    private static TextView mobsBack;
    private static TextView mlatitude;
    private static TextView mlongitude;
    private static TextView mcompass;
    private static TextView mdistance;
    private Button mListPairedDevicesBtn;
    private Button mDiscoverBtn;
    private BluetoothAdapter mBTAdapter;
    private Set<BluetoothDevice> mPairedDevices;
    private ArrayAdapter<String> mBTArrayAdapter;
    private ListView mDevicesListView;
    private CheckBox mStart;
    private CheckBox mStop;
    private static float mlat_rx;
    private static float mlong_rx;
    static double srcLat = 0.00;
    static double srcLng = 0.00;
    static double dstLat = 0.00;
    static double dstLng = 0.00;
    static double currentLat = 0.00;
    static double currentLng = 0.00;
    private static float mcompass_rx;
    static boolean cmdStartStop = false;
    static boolean flagStartButtonPressed;
    static int mapButtonCnt = 0;


    private final String TAG = MainActivity.class.getSimpleName();
    private Handler mHandler; // Our main handler that will receive callback notifications
    private ConnectedThread mConnectedThread; // bluetooth background worker thread to send and receive data
    private BluetoothSocket mBTSocket = null; // bi-directional client-to-client data path

    //private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"); // "random" unique identifier for Moto
    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    // #defines for identifying shared types between calling functions
    private final static int REQUEST_ENABLE_BT = 1; // used to identify adding bluetooth names
    private final static int MESSAGE_READ = 2; // used in bluetooth handler to identify message update
    private final static int CONNECTING_STATUS = 3; // used in bluetooth handler to identify message status

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth2);
        mBluetoothStatus = (TextView)findViewById(R.id.bluetoothStatus);
        Toolbar toolbar = findViewById(R.id.toolbar_bt);
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);


        //mReadBuffer = (TextView) findViewById(R.id.readBuffer);
        mlatitude = (TextView) findViewById(R.id.lat_rx);
        mlongitude = (TextView) findViewById(R.id.long_rx);
        mcompass = (TextView) findViewById(R.id.compass_rx);
        mdriveCmd =(TextView) findViewById(R.id.motor_drive_rx);
         msteerCmd=(TextView) findViewById(R.id.motor_steer_rx);
       mrpm=(TextView) findViewById(R.id.rpm_rx);
        mobsFront=(TextView) findViewById(R.id.obsFront_rx);
        mobsRight=(TextView) findViewById(R.id.obsRight_rx);
       mobsLeft=(TextView) findViewById(R.id.obsLeft_rx);
        mobsBack=(TextView) findViewById(R.id.obsBack_rx);
        mdistance = (TextView) findViewById(R.id.Distance_GPS);
        mStart = (CheckBox)findViewById(R.id.start);

        mBTArrayAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1);
        mBTAdapter = BluetoothAdapter.getDefaultAdapter(); // get a handle on the bluetooth radio


        // Ask for location permission if not already allowed
        if(ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED)
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, 1);

        Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
        startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        mBluetoothStatus.setText("Bluetooth enabled");
        Toast.makeText(getApplicationContext(),"Bluetooth turned on",Toast.LENGTH_SHORT).show();

        mHandler = new Handler(){
            public void handleMessage(android.os.Message msg){
                if(msg.what == MESSAGE_READ){
                    String readMessage = new String();
                    try {
                        readMessage = new String((byte[]) msg.obj, "UTF-8");
                        Log.d("HANDLER", readMessage);
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                    }
                    //mReadBuffer.setText(readMessage);
                    stringParse(readMessage);
                }
                if(msg.what == CONNECTING_STATUS){
                    if(msg.arg1 == 1) {
                        mBluetoothStatus.setText("Connected to Device: " + (String) (msg.obj));
                    }

                    else {
                        mBluetoothStatus.setText("Connection Failed");
                    }
                }
            }
        };

        if (mBTArrayAdapter == null) {
            // Device does not support Bluetooth
            mBluetoothStatus.setText("Status: Bluetooth not found");
            Toast.makeText(getApplicationContext(),"Bluetooth device not found!",Toast.LENGTH_SHORT).show();
        }
        else {

            mStart.setOnClickListener(new View.OnClickListener(){
                @Override
                public void onClick(View v){
                    if(mStart.isChecked())

                    {
                        if(mConnectedThread != null) //First check to make sure thread created
                            mConnectedThread.write("S1");
                    }
                    else
                    {
                        if(mConnectedThread != null) //First check to make sure thread created
                            mConnectedThread.write("S0");
                    }
                }
            });
        }
    }

    public void mapButtonOnClick(View view){
        Intent intent = new Intent(getApplicationContext(), MapsActivity.class);
        //intent.putExtra("placeNumber", 12);

        //Hard Coded SJSU values
        /*double latitude =  37.3352;
        double longitude = -121.8811;*/

        //Send location to Maps
        intent.putExtra("latitude", currentLat);
        intent.putExtra("longitude", currentLng);
        startActivity(intent);
    }


    //Parse received data from bluetooth
    private static void stringParse(String str) {
        StringTokenizer st = new StringTokenizer(str);
        try{
            while (st.hasMoreElements()) {
                String token = (String)st.nextElement();
                if(!token.isEmpty() && token.split(":")[1]!="")
                {
                    String item = token.split(":")[0];
                    String value = token.split(":")[1];
                        if (item.equalsIgnoreCase("Lat")) {
                            if (value != null && (!value.isEmpty())&& isNumeric(value)) {
                                mlat_rx = Float.parseFloat(value);
                                currentLat = (double)mlat_rx;
                                //currentLat = 37.335003;
                            }
                            mlatitude.setText(value);
                        } else if (item.equalsIgnoreCase("Long")) {
                            if (value != null && (!value.isEmpty())&& isNumeric(value)) {
                                mlong_rx = Float.parseFloat(value);
                                currentLng = (double)mlong_rx;
                                //currentLng = -121.887446;
                            }
                            mlongitude.setText(value);
                        }
                    else if(item.equalsIgnoreCase("Bearing"))
                    {
                        //mcompass_rx = Float.parseFloat(value);
                        mcompass.setText(value);
                    }
                    else if(item.equalsIgnoreCase("DrvCmd"))
                    {
                        mdriveCmd.setText(value);
                    }
                    else if(item.equalsIgnoreCase("RPM"))
                    {
                        mrpm.setText(value);
                    }

                    else if(item.equalsIgnoreCase("StrCmd"))
                    {
                        msteerCmd.setText(value);
                    }
                    else if(item.equalsIgnoreCase("ObsFront"))
                    {
                        mobsFront.setText(value);
                    }
                    else if(item.equalsIgnoreCase("ObsRight"))
                    {
                        mobsRight.setText(value);
                    }
                    else if(item.equalsIgnoreCase("ObsLeft"))
                    {
                        mobsLeft.setText(value);
                    }
                    else if(item.equalsIgnoreCase("ObsBack"))
                    {
                        mobsBack.setText(value);
                    }
                    else if(item.equalsIgnoreCase("Distance"))
                    {
                        mdistance.setText(value);
                    }

                }
            }//end while
        } // end try
        catch(ArrayIndexOutOfBoundsException exc){
        }
    }

    public static boolean isNumeric(String str) {
        try {
            Double.parseDouble(str);
            return true;
        } catch(NumberFormatException e){
            return false;
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.bt_toolbar_icon, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            // action with ID action_refresh was selected
            case R.id.bt_icon:
                if (mBTAdapter.isEnabled()) {
                    BluetoothEnable();
                }
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    // Enter here after user selects "yes" or "no" to enabling radio
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent Data){
        // Check which request we're responding to
        if (requestCode == REQUEST_ENABLE_BT) {
            // Make sure the request was successful
            if (resultCode == RESULT_OK) {
                // The user picked a contact.
                // The Intent's data Uri identifies which contact was selected.
                mBluetoothStatus.setText("Enabled");
            }
            else
                mBluetoothStatus.setText("Disabled");
        }
    }

    private void BluetoothEnable(){
        mBluetoothStatus.setText("Connecting...");
        /*
        Moto_G Details
        final String address = "68:C4:4D:72:C1:6F";
        final String name = "Moto G Play";
        */

        final String address = "98:D3:33:80:67:F9";
        final String name = "Tech_Savy";

        Log.i("BluetoothEnable", "Inside test function");
        // Spawn a new thread to avoid blocking the GUI one
        new Thread()
        {
            public void run() {
                boolean fail = false;

                BluetoothDevice device = mBTAdapter.getRemoteDevice(address);

                if (device != null){
                    Log.d("BluetoothEnable", device.toString());
                }

                try {
                    mBTSocket = createBluetoothSocket(device);
                } catch (IOException e) {
                    fail = true;
                    //Toast.makeText(getBaseContext(), "Socket creation failed", Toast.LENGTH_SHORT).show();
                    Log.d("BluetoothEnable", "Socket failed e1");
                    e.printStackTrace();
                }
                // Establish the Bluetooth socket connection.
                try {
                    mBTSocket.connect();
                } catch (IOException e) {
                    try {
                        fail = true;
                        mBTSocket.close();
                        mHandler.obtainMessage(CONNECTING_STATUS, -1, -1)
                                .sendToTarget();
                    } catch (IOException e2) {
                        //insert code to deal with this
                        //Toast.makeText(getBaseContext(), "Socket creation failed", Toast.LENGTH_SHORT).show();
                        Log.d("BluetoothEnable", "Socket failed e2");
                        e.printStackTrace();
                    }
                }
                if(fail == false) {
                    mConnectedThread = new ConnectedThread(mBTSocket);
                    mConnectedThread.start();

                    mHandler.obtainMessage(CONNECTING_STATUS, 1, -1, name)
                            .sendToTarget();
                }
            }
        }.start();
    }

    private BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException {
        try {
            final Method m = device.getClass().getMethod("createInsecureRfcommSocketToServiceRecord", UUID.class);
            return (BluetoothSocket) m.invoke(device, BTMODULEUUID);
        } catch (Exception e) {
            Log.e(TAG, "Could not create Insecure RFComm Connection",e);
        }
        return  device.createRfcommSocketToServiceRecord(BTMODULEUUID);
    }

    private class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            // Get the input and output streams, using temp objects because
            // member streams are final
            try {

                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run() {
            byte[] buffer = new byte[8192];  // buffer store for the stream
            int bytes; // bytes returned from read()

            // Keep listening to the InputStream until an exception occurs
            while (true) {

                try {
                    // Read from the InputStream
                    bytes = mmInStream.available();
                    if(bytes != 0) {
                        buffer = new byte[8192];
                        SystemClock.sleep(100); //pause and wait for rest of data. Adjust this depending on your sending speed.
                        bytes = mmInStream.available(); // how many bytes are ready to be read?
                        bytes = mmInStream.read(buffer, 0, bytes); // record how many bytes we actually read
                        mHandler.obtainMessage(MESSAGE_READ, bytes, -1, buffer)
                                .sendToTarget(); // Send the obtained bytes to the UI activity
                    }
                    //Intent intent = getIntent();
                    //Get Longitude and Latitude Values
                    //Boolean cmdStartStop_intent = getIntent().getExtras().getBoolean("start_stop_command");

                    //int cmdStartStop_intent = intent.getIntExtra("start_stop_command", 5);


                    if(flagStartButtonPressed)
                    {
                        //Convert destination values to string
                        //boolean cmdStartStop_intent = intent.getBooleanExtra("start_stop_command", true);
                        //Boolean cmdStartStop_intent = getIntent()("start_stop_command");
                        String latitudeStr = "Lat:" + String.valueOf(dstLat);
                        String longitudeStr = "Long:" + String.valueOf(dstLng);
                        if(cmdStartStop == true) {
                        //if(cmdStartStop_intent == 1) {
                            mConnectedThread.write("S1");
                            Log.i("Bluetooth start","start");
                            //cmdStartStop_intent = 0;
                            //cmdStartStop = false;
                        }
                        //else if((cmdStartStop_intent == 0 )) {
                        else if(cmdStartStop == false ) {
                            mConnectedThread.write("S0");
                            Log.i("Bluetooth stop","stop");
                            //cmdStartStop_intent = true;
                        }

                        SystemClock.sleep(100);
                        mConnectedThread.write(latitudeStr);
                        SystemClock.sleep(100);
                        mConnectedThread.write(longitudeStr);
                        SystemClock.sleep(100);
                        mConnectedThread.write("0");
                        Log.d("Lat for Dst", latitudeStr);
                        Log.d("Long for Dst", longitudeStr);
                        dstLat = dstLng = 0.00;
                        //Toast.makeText(this, "Button pressed", Toast.LENGTH_SHORT).show();
                        flagStartButtonPressed = false;
                    }
                }
                catch (IOException e) {
                    e.printStackTrace();
                    break;
                }
            }
        }

        /* Call this from the main activity to send data to the remote device */
        public void write(String input) {
            byte[] bytes = input.getBytes();           //converts entered String into bytes
            try {
                mmOutStream.write(bytes);
                Log.d("Writing", input);
            } catch (IOException e) { }
        }

        /* Call this from the main activity to shutdown the connection */
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) { }
        }
    }
}


