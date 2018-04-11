package com.android.debug;

import android.util.Log;

public class Debug {
	private static boolean isShowDebugInfo = true;
	private static boolean isShowInfo = true;
	private static final String TAG = "ZGB_DEBUG";
	
	
	public static void D(String debug){
		if(isShowDebugInfo){
			Log.d(TAG, debug);
		}
	}
	
	public static void I(String info){
		if(isShowInfo){
			Log.d(TAG, info);
		}
	}
}



