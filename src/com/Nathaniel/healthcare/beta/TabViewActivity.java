package com.Nathaniel.healthcare.beta;

import android.app.TabActivity;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.view.MenuItem;
import android.widget.TabHost;

@SuppressWarnings("deprecation")
public class TabViewActivity extends TabActivity {
	Resources res;
	static TabHost tabHost;
	Intent intent;

	Thread TTD = new Thread(new Abstraction_Thread(0));
	Thread TM = new Thread(new Abstraction_Thread(1));
	Thread TF = new Thread(new Abstraction_Thread(2));
	Thread TS = new Thread(new Abstraction_Thread(3));

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.tab_host);

//		try {
//			Runtime.getRuntime().exec("su");
//		} catch (IOException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
		
		//		TTD.start();
		TM.start();
		TF.start();
		TS.start();

		res = getResources();
		tabHost = getTabHost();

		intent = new Intent().setClass(this, SensorViewActivity.class).addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		tabHost.addTab(tabHost.newTabSpec("tab1").setIndicator("Sensors").setContent(intent));
		
		intent = new Intent().setClass(this, GraphViewActivity.class).addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		tabHost.addTab(tabHost.newTabSpec("tab2").setIndicator("Graph").setContent(intent));

		tabHost.setCurrentTab(1);
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
