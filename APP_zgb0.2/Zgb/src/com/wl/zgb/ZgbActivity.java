package com.wl.zgb;


import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.TextView;
import android.widget.ToggleButton;

import com.android.debug.*;
import com.android.zgb.Globaldata;
import com.android.zgb.ZgbComCtrl;

public class ZgbActivity extends Activity implements OnClickListener{
	ToggleButton tgled,tgfan;
	static TextView tv_temp;
	static TextView tv_hum;
	
	private static boolean isExit = false;
	Handler mHandler=null;
	final String TAG="ZgbActivity";
	boolean ledstate=false,fanstate=false;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_zgb);
		initUI();
		
		Globaldata.zc=new ZgbComCtrl();
		
		Globaldata.SrvRcvIsrun=true;
		Globaldata.SrvSndIsrun=true;
		Globaldata.zc.start();
	}
	private void initUI() {
		tgled= (ToggleButton) findViewById(R.id.tg_led);
		tgfan=(ToggleButton) findViewById(R.id.tg_fan);
		tgfan.setOnClickListener(this);
		tgled.setOnClickListener(this);
		
		tv_hum=(TextView) findViewById(R.id.tv_show_hum);
		tv_temp=(TextView) findViewById(R.id.tv_show_temp);
		
	}
	@Override
	protected void onResume() {
	 /**
	  * 设置为横屏防止屏幕旋转后就重新将调用Ocreate的方法导致重入
	  */
	 if(getRequestedOrientation()!=ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE){
		 setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	 }
	 super.onResume();
	}
	
	
	@Override
	public void onClick(View v) {
		int ret;
		switch (v.getId()) {
		case R.id.tg_led:
			if(ledstate){
				ret = Globaldata.zc.Ledctrl(0);
				if(ret<0)
					Debug.D("led turn off error");
				else{
					Debug.D("led turn off ok");
				}				
				ledstate=false;
			}else{
				ret = Globaldata.zc.Ledctrl(1);
				if(ret<0)
					Debug.D("led turn on error");
				else{
					Debug.D("led turn on ok");
				}
				ledstate=true;
			}
			break;
		case R.id.tg_fan:
			if(fanstate){
				ret = Globaldata.zc.Fanctrl(0);
				if(ret<0)
					Debug.D("fan turn off error");
				else{
					Debug.D("fan turn off ok");
				}
				fanstate=false;
			}else{
				ret = Globaldata.zc.Fanctrl(1);
				if(ret<0)
					Debug.D("fan turn on error");
				else{
					Debug.D("fan turn on ok");
				}
				fanstate=true;
			}
			break;
		default:
			break;
		}
	}
	
	/*public后都无需传递mainHandler了*/
	public static Handler mainHandler =new Handler(){
		 
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch (msg.what) {
				case	0x01:
					if(msg.obj!=null)
					
						tv_hum.setText(msg.obj.toString());
					
					break;
				case	0x02:
					if(msg.obj!=null)
						
					
						tv_temp.setText(msg.obj.toString());
					
					break;		
				case    0x03:
					Debug.D("get msg 0x03");
					break;
			}
			
		}
	};
	

	
	@Override
	protected void onDestroy() {
		Globaldata.SrvRcvIsrun = false;/*就收数据的线程需要死掉*/
		Globaldata.SrvSndIsrun = false;/*发送数据的线程需要死掉*/
		Globaldata.zc.CloseZgb();/*JNI线程需要退出*/
		Debug.D("文件描述符关闭了");
		super.onDestroy();
	}
	
}
