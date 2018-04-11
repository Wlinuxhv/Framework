package com.android.zgb;

/*
 
	 1、public：public表明该数据成员、成员函数是对所有用户开放的，所有用户都可以直接进行调用 

     2、private：private表示私有，私有的意思就是除了class自己之外，任何人都不可以直接使用，私有财产神圣不可侵犯嘛，即便是子女，朋友，都不可以使用。 

     3、protected：protected对于子女、朋友来说，就是public的，可以自由使用，没有任何限制，而对于其他的外部class，protected就变成private。 

         	作用域       		当前类    同一package       子孙类             其他package 

           public        √         √             √           √ 

           protected     √          √             √           × 

           friendly      √          √             ×           × 

           private       √          ×             ×           ×

*/

public class Globaldata {

	public static Boolean SrvSndIsrun = false;
	public static Boolean SrvRcvIsrun = false;
	public static  ZgbComCtrl zc = null;
	
	public static final int CMD_LEDOP_ID = 0x11;
	public static final int CMD_FANOP_ID = 0x12;
	public static final int CMD_OFF_ID = 0x12;
	
	public static final int HANDLE_SERVER_ERROR = 5;
	public static final int HANDLE_GET_DATA = 6;
	public static final int HANDLE_GET_CARTE = 7;
	public static final int HANDLE_GET_CHECK_OUT = 8;
	public static final int HANDLE_GET_SUBMIT_ORDER = 9;
	
}