package com.android.zgb;

/*
 
	 1��public��public���������ݳ�Ա����Ա�����Ƕ������û����ŵģ������û�������ֱ�ӽ��е��� 

     2��private��private��ʾ˽�У�˽�е���˼���ǳ���class�Լ�֮�⣬�κ��˶�������ֱ��ʹ�ã�˽�вƲ���ʥ�����ַ����������Ů�����ѣ���������ʹ�á� 

     3��protected��protected������Ů��������˵������public�ģ���������ʹ�ã�û���κ����ƣ��������������ⲿclass��protected�ͱ��private�� 

         	������       		��ǰ��    ͬһpackage       ������             ����package 

           public        ��         ��             ��           �� 

           protected     ��          ��             ��           �� 

           friendly      ��          ��             ��           �� 

           private       ��          ��             ��           ��

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