package com.wl.zgb.bean;


import com.android.debug.Debug;
import com.android.zgb.Globaldata;
import com.android.zgb.ZgbComCtrl;
import com.wl.zgb.ZgbActivity;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.Toast;

public class SensorData extends Thread{
	
	private static int  temp;
	private static int hum;
	private static String ReportString = "welcome";
	public int getTemp() {
		return temp;
	}
	public void setTemp(int temp) {
		this.temp = temp;
	}
	public int getHum() {
		return hum;
	}
	public void setHum(int hum) {
		this.hum = hum;
	}
	
	public static void  ReportHumTemp(String retpoString) {
		ReportString = retpoString; 
		Message msgh = new Message();
		Message msgt = new Message();
		msgt.what =0x01;
		msgt.obj=""+ ReportString;
		ZgbActivity.mainHandler.sendMessage(msgt);
		msgh.what =0x02;
		msgh.obj=""+ ReportString;
		ZgbActivity.mainHandler.sendMessage(msgh);
	}
}
