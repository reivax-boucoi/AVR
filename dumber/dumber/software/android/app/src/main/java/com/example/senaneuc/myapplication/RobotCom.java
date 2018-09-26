package com.example.senaneuc.myapplication;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.ParcelUuid;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;


public class RobotCom {
    private BluetoothAdapter bluetoothAdapter;
    private Set<BluetoothDevice> bondedDevices;
    private OutputStream outputStream;
    /*private InputStream inStream;
    private Object[] devices = (Object []) bondedDevices.toArray();
    private String[] devicesName;

    public static final String startDumby = "u";
    public static final String stopDumby = "S";
    public static final String goForward = "R";
    public static final String goBackward = "T";
    public static final String goLeft = "i";
    public static final String goRight = "o";
    public static final String getBattery = "V";
    public static final String stopMove = "y";
*/
    public RobotCom() {
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    }

    public List<String> getDevicesBounds(){
        List<String> blueToothArray = new ArrayList<String>();
        bondedDevices = bluetoothAdapter.getBondedDevices();
        if(bondedDevices.size() > 0) {
            for(BluetoothDevice device: bondedDevices){
                blueToothArray.add(device.getName());
            }
        }
        Log.v("Test", "Je passe là");
        return blueToothArray;
    }

    public void selectDevice(int position) throws IOException {
        Object[] devices = (Object []) bondedDevices.toArray();
        BluetoothDevice device = (BluetoothDevice) devices[position];
        ParcelUuid[] uuids = device.getUuids();
        BluetoothSocket socket = device.createRfcommSocketToServiceRecord(uuids[0].getUuid());
        socket.connect();
        outputStream = socket.getOutputStream();
        //inStream = socket.getInputStream();
    }

    public void write(String s) throws IOException {
        outputStream.write(s.getBytes());
    }
    public void writeWithCheckSum(String s) throws IOException {
        s += checksumCalc(s);
        s+= "\r";
        outputStream.write(s.getBytes());
    }

    /*private char sendCmdRobot(String cmd) throws IOException {
        switch(cmd) {
            case startDumby:
                break;
            case stopDumby:
                break;
            case goForward:
                break;
            case goBackward:
                break;
            case goLeft:
                break;
            case goRight:
                break;
            case getBattery:
                break;
            case stopMove:
                break;
            default:
        }
        return 0;
    }*/

    private char checksumCalc(String s) {
        char checkCode = 0;
        for (int i = 0; s.charAt(i) != '\r'; i++) {
            checkCode ^= s.charAt(i);
        }
        return checkCode;
    }
}
