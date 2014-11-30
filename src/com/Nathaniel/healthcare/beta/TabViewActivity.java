package com.Nathaniel.healthcare.beta;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import android.annotation.SuppressLint;
import android.app.TabActivity;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.os.Environment;
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

	@SuppressLint("SimpleDateFormat")
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


		// 폴더가 없으면 생성     
		File dir = new File(Environment.getExternalStorageDirectory().getPath(), "Healthcare");
		if(!dir.exists()) {
			dir.mkdir();
		}


		// START - res/raw/sensors.json 파일을 sdcard/Healthcare로 복사
		File file = new File(Environment.getExternalStorageDirectory().getPath(), "Healthcare/sensors.json");
		File tmp_file = new File(Environment.getExternalStorageDirectory().getPath(), "Healthcare/tmp.json");

		try {
			InputStream in = getResources().openRawResource(R.raw.sensors);
			FileOutputStream out = new FileOutputStream(tmp_file);
			byte[] buff = new byte[1024];
			int read = 0;

			while ((read = in.read(buff)) > 0) {
				out.write(buff, 0, read);
			}

			in.close();
			out.close();

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		if(file.length() < tmp_file.length()) {
//			Log.i("####################", file.length() + " < " + tmp_file.length());
			tmp_file.renameTo(file);
		}
		else {
//			Log.i("####################", file.length() + " > " + tmp_file.length());
			tmp_file.delete();
		}
		// END - res/raw/sensors.json 파일을 sdcard/Healthcare로 복사
		
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

		intent = new Intent().setClass(this, InfoViewActivity.class).addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		tabHost.addTab(tabHost.newTabSpec("tab3").setIndicator("Information").setContent(intent));

		tabHost.setCurrentTab(0);
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
