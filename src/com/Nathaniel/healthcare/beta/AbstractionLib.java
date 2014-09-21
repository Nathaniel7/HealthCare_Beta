package com.Nathaniel.healthcare.beta;

public class AbstractionLib {
	public native static void Test();
	public native static void TMconnectDevice();
	public native static void TFmakeDevthread();
	public native static void TSpressData();
	
	static {
		System.loadLibrary("HealthCare"); // Library Load "libHealthCare.so"
	}
}
