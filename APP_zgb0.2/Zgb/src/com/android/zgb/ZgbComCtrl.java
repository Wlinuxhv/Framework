package com.android.zgb;

import android.os.Handler;
import android.os.Message;

import com.android.debug.Debug;
import com.wl.zgb.ZgbActivity;

public class ZgbComCtrl extends Thread{
	static{
		System.loadLibrary("zgb_jni");
	}
	
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
	
	public  void  ReportHumTemp(String retpoString) {
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
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		
		Globaldata.zc.InitZgb();
		super.run();
		Debug.D("before loop in java");
		while (Globaldata.SrvRcvIsrun) {
			try {
				sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	public  native int InitZgb();
	public  native int CloseZgb();
	public  native int Ledctrl(int on);
	public  native int Fanctrl(int on);
}
