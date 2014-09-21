package com.Nathaniel.healthcare.beta;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends Activity {	
//	Thread TTD = new Thread(new Abstraction_Thread(0));
	Thread TM = new Thread(new Abstraction_Thread(1));
	Thread TF = new Thread(new Abstraction_Thread(2));
	Thread TS = new Thread(new Abstraction_Thread(3));
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
	
//		TTD.start();
		TM.start();
		TF.start();
		TS.start();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
	
	class Abstraction_Thread implements Runnable {
		private int n = 0;
		
		public Abstraction_Thread(int num) {
			this.n = num;
		}
		
		public void run() {
			switch(n) {
			case 0:
				AbstractionLib.Test();
				break;
			case 1:
				AbstractionLib.TMconnectDevice();
				break;
			case 2:
				AbstractionLib.TFmakeDevthread();
				break;
			case 3:
				AbstractionLib.TSpressData();
				break;
			}
		}
	}
}
