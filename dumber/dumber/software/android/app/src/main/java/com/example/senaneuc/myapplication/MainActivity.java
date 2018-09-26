package com.example.senaneuc.myapplication;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.ClipData;
import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.Toast;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import static android.hardware.Sensor.TYPE_GAME_ROTATION_VECTOR;

public class MainActivity extends AppCompatActivity implements AdapterView.OnItemSelectedListener{

    private boolean connectionEnable = false;
    private boolean buttonPressed = false;
    private String previousState = "STOP";
    private RobotCom dumbyBluetooth = new RobotCom();
    private float sensibility = 20;

    private SensorManager mSensorManager;
    private Sensor mRotationSensor;

    private static final int SENSOR_DELAY = 500 * 1000; // 50ms
    private static final int FROM_RADS_TO_DEGS = -57;

    private float pitch = 0;
    private float roll = 0;

    private float pitch_init=0;
    private float roll_init=0;
    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR2)
    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mSensorManager = (SensorManager) getSystemService(Activity.SENSOR_SERVICE);
        mRotationSensor = mSensorManager.getDefaultSensor(TYPE_GAME_ROTATION_VECTOR);
        if(mRotationSensor == null)
            Toast.makeText(this, "Hardware issue",Toast.LENGTH_SHORT).show();
        else {
            Toast.makeText(this, mRotationSensor.getName(), Toast.LENGTH_SHORT).show();
            mSensorManager.registerListener(rtSensor, mRotationSensor, SENSOR_DELAY);
        }

        Spinner dropdown = findViewById(R.id.spinner1);
        Button button = findViewById(R.id.button);
        ArrayAdapter<String> adapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_dropdown_item, dumbyBluetooth.getDevicesBounds());
        dropdown.setAdapter(adapter);
        dropdown.setOnItemSelectedListener(this);

        button.setOnTouchListener(new View.OnTouchListener()

            {
                private int actionUpCount = 0;

                @Override
                public boolean onTouch (View view, MotionEvent motionEvent){
                switch (motionEvent.getAction()) {
                    case MotionEvent.ACTION_DOWN: {
                        //Toast.makeText(getApplicationContext(), "lul", Toast.LENGTH_SHORT).show();
                        try {
                            pitch_init = pitch;
                            roll_init = roll;
                            dumbyBluetooth.write("uu\r");
                            buttonPressed=true;

                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                    case MotionEvent.ACTION_UP: {
                        try {
                            actionUpCount++;
                            if (actionUpCount % 2 == 0) {
                                pitch_init=0;
                                roll_init=0;
                                buttonPressed=false;
                                dumbyBluetooth.write("rr\r");
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        //Toast.makeText(getApplicationContext(), "lal", Toast.LENGTH_SHORT).show();
                    }
                }
                return false;
            }
            });
        };
    @Override
    public void onItemSelected(AdapterView<?> parent, View v, int position, long id) {
        Toast.makeText(parent.getContext(), parent.getItemAtPosition(position).toString(),
                Toast.LENGTH_SHORT).show();
        try {
            dumbyBluetooth.selectDevice(position);
            connectionEnable = true;
        } catch (IOException e) {
            e.printStackTrace();
            connectionEnable = false;
        }
    }

    private SensorEventListener rtSensor = new SensorEventListener() {
        @Override
        public void onSensorChanged(SensorEvent sensorEvent) {
            if (sensorEvent.sensor == mRotationSensor) {

                    if (sensorEvent.values.length > 4) {
                        float[] truncatedRotationVector = new float[4];
                        System.arraycopy(sensorEvent.values, 0, truncatedRotationVector, 0, 4);
                        update(truncatedRotationVector);
                    } else {
                        update(sensorEvent.values);
                    }
                if(connectionEnable && buttonPressed){
                    Log.v("Test", "roll" + (roll_init-roll));
                    Log.v("Test", "pitch" + (pitch_init-pitch));
                    if(roll_init - roll < -sensibility && !previousState.equals("GAUCHE")) {
                        try {
                            dumbyBluetooth.write("T=-64000v\r");
                            previousState = "GAUCHE";
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }

                    if(roll_init - roll > sensibility && !previousState.equals("DROITE")) {
                        try {
                            dumbyBluetooth.write("T=64000[\r");
                            previousState = "DROITE";
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }

                    if(pitch_init - pitch > sensibility && !previousState.equals("AVANT")) {
                        try {
                            dumbyBluetooth.write("M=64000B\r");
                            previousState = "AVANT";
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }

                    if(pitch_init - pitch < -sensibility && !previousState.equals("ARRIERE")) {
                        try {
                            dumbyBluetooth.write("M=-64000o\r");
                            previousState = "ARRIERE";
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                    if(!previousState.equals("STOP") && pitch_init-pitch < 25 && pitch_init - pitch > -25 &&roll_init - roll < 25 && roll_init - roll > -25 ) {
                        try {
                            dumbyBluetooth.write("M=0@\r");
                            previousState="STOP";
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int i) {

        }

        private void update(float[] vectors) {
            float[] rotationMatrix = new float[9];
            SensorManager.getRotationMatrixFromVector(rotationMatrix, vectors);
            int worldAxisX = SensorManager.AXIS_X;
            int worldAxisZ = SensorManager.AXIS_Z;
            float[] adjustedRotationMatrix = new float[9];
            SensorManager.remapCoordinateSystem(rotationMatrix, worldAxisX, worldAxisZ, adjustedRotationMatrix);
            float[] orientation = new float[3];
            SensorManager.getOrientation(adjustedRotationMatrix, orientation);
            pitch = orientation[1] * FROM_RADS_TO_DEGS;
            roll = orientation[2] * FROM_RADS_TO_DEGS;

        }
    };

    @Override
    public void onNothingSelected(AdapterView<?> adapterView) {

    }
}
