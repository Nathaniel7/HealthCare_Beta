package com.Nathaniel.healthcare.beta;

public class AbstractionLib {
	public native static int Test();
	public native static int TMconnectDevice();
	public native static int TFmakeDevthread();
	public native static int TSpressData();
	
	public native static int[] getData();
	
	static {
		System.loadLibrary("HealthCare"); // Library Load "libHealthCare.so"
	}
}
