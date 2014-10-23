package com.Nathaniel.healthcare.beta;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.os.SystemClock;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

public class SensorViewActivity extends Activity {
	interface HANBACK {
		int COMPANY			= 0x76;
		
		int DIOXIDE_FRONT 	= 0x13;
		int DUST_FRONT 		= 0x15;
		int VOC_FRONT 		= 0x17;
		int THRMMTR_FRONT 	= 0x18;
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.sensor_view);

		new Thread(new Runnable() {
			@Override
			public void run() {//				
				while(true) {
					// 현재 UI 스레드가 아님으로 메시지 큐에 Runnable을 등록 함
					runOnUiThread(new Runnable() {
						public void run() {
							onResume();
						}
					});
					// 복잡한 작업 simulating	
					SystemClock.sleep(250);
				}

			}
		}).start();
	}

	@Override
	protected void onResume() {
		super.onResume();

		LinearLayout layout_sensorlist = (LinearLayout)findViewById(R.id.sensorlist_area);
		layout_sensorlist.removeAllViews();
		layout_sensorlist.setWeightSum(1.0f);
		
		LinearLayout layout_sensorcontent_title = new LinearLayout(this);
		layout_sensorcontent_title.setOrientation(LinearLayout.HORIZONTAL);
		layout_sensorcontent_title.setBackgroundColor(Color.parseColor("#FF6A6A"));

		LinearLayout.LayoutParams image_Lparam = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.15f);
		LinearLayout.LayoutParams brand_Lparam = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.14f);
		LinearLayout.LayoutParams type_Lparam = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.16f);
		LinearLayout.LayoutParams feature_Lparam = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.55f);
		
		for(int i = 0; i < 5; i++) {
			TextView tv_content = new TextView(this);
			tv_content = new TextView(this);
			
			if(i == 0) {
				tv_content.setText("Photo");
				tv_content.setLayoutParams(image_Lparam);
			}
			else if(i == 1) {
				tv_content.setText("Brand");
				tv_content.setLayoutParams(brand_Lparam);
			}
			else if(i == 2) {
				tv_content.setText("Type");
				tv_content.setLayoutParams(type_Lparam);
			}
			else if(i == 3) {
				tv_content.setText("Features");
				tv_content.setLayoutParams(feature_Lparam);
			}

			tv_content.setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
			tv_content.setTextAppearance(this, android.R.style.TextAppearance_Medium);
			layout_sensorcontent_title.addView(tv_content);
		}
		layout_sensorlist.addView(layout_sensorcontent_title);

		int[] sensors = AbstractionLib.getData();
		
		if(sensors[0] == 0) {
			//			Log.i("### List Act ###", "Select data not exist!");
			TextView tv = new TextView(this);
			tv.setText("연결된 센서가 존재하지 않습니다.");
			tv.setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
			tv.setTextAppearance(this, android.R.style.TextAppearance_Medium);
			layout_sensorlist.addView(tv);
			//Log.i("############", "..........");
		}

		
		for(int i = 0; sensors[i] != 0; i += 100) {
			LinearLayout layout_sensorcontent = new LinearLayout(this);
			layout_sensorcontent.setOrientation(LinearLayout.HORIZONTAL);
			if(i == 0)			layout_sensorcontent.setBackgroundColor(Color.parseColor("#CFE4F8"));
			else if(i == 100)	layout_sensorcontent.setBackgroundColor(Color.parseColor("#D1FADD"));
			else if(i == 200)	layout_sensorcontent.setBackgroundColor(Color.parseColor("#FFFDDD"));
			else if(i == 300)	layout_sensorcontent.setBackgroundColor(Color.parseColor("#F8D5CD"));
			layout_sensorcontent.setWeightSum(1.0f);
			
			ImageView iv_content = new ImageView(this);
			TextView[] tv_content = new TextView[3];

			for(int j = 0; j < 5; j++) {
				if(j == 0) {	// Photo
					switch(sensors[i+1]) {
					case HANBACK.DIOXIDE_FRONT:
						iv_content.setBackgroundResource(R.drawable.dioxide);
						break;
					case HANBACK.DUST_FRONT:
						iv_content.setBackgroundResource(R.drawable.dust);
						break;
					case HANBACK.VOC_FRONT:
						iv_content.setBackgroundResource(R.drawable.voc);
						break;
					case HANBACK.THRMMTR_FRONT:
						iv_content.setBackgroundResource(R.drawable.thermometer);
						break;
					}

					iv_content.setLayoutParams(image_Lparam);
					layout_sensorcontent.addView(iv_content);
				}
				else if(j == 1) {	// Brand
					switch(sensors[i]) {
					case HANBACK.COMPANY:
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("HANBACK");
						break;
					default:
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("Unknown");
						break;
					}
					
					tv_content[j-1].setLayoutParams(brand_Lparam);
					tv_content[j-1].setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
					tv_content[j-1].setTextAppearance(this, android.R.style.TextAppearance_Medium);
					layout_sensorcontent.addView(tv_content[j-1]);
				}
				else if(j == 2) {	// Type
					switch(sensors[i+1]) {
					case HANBACK.DIOXIDE_FRONT:
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("Dioxide");
						break;
					case HANBACK.DUST_FRONT:
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("Dust");
						break;
					case HANBACK.VOC_FRONT:
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("VOC");
						break;
					case HANBACK.THRMMTR_FRONT:
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("Thermometer");
						break;
					}
					
					tv_content[j-1].setLayoutParams(type_Lparam);
					tv_content[j-1].setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
					tv_content[j-1].setTextAppearance(this, android.R.style.TextAppearance_Medium);
					layout_sensorcontent.addView(tv_content[j-1]);
				}
				else if(j == 3) {	// Features
					switch(sensors[i+1]) {
					case HANBACK.DIOXIDE_FRONT:
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("실내 공기 중의 이산화탄소 농도를 측정하며, 상황에 따른 안내가 가능합니다.");
						break;
					case HANBACK.DUST_FRONT:
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("실내 공기 중의 먼지 농도를 측정하며, 공기의 탁한 정도를 알 수 있습니다.");
						break;
					case HANBACK.VOC_FRONT:
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("공기 중 휘발성 농도를 측정하며, LPG 등 가스의 및 가연성 기체를 감지합니다.");
						break;
					case HANBACK.THRMMTR_FRONT:
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("적외선 센서로 인체 온도와 실내 온도를 동시에 체크가 가능합니다.");
						break;
					}
					
					tv_content[j-1].setLayoutParams(feature_Lparam);
					tv_content[j-1].setTextAppearance(this, android.R.style.TextAppearance_Medium);
					layout_sensorcontent.addView(tv_content[j-1]);
				}	
			}
			layout_sensorlist.addView(layout_sensorcontent);
		}
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		switch (keyCode) {
		case KeyEvent.KEYCODE_BACK:
			// 첫번째 방법		
			finish();
			// 두번째 방법
			System.exit(0);
			// 세번째 방법
			android.os.Process.killProcess(android.os.Process.myPid());

			break;
		}

		return super.onKeyDown(keyCode, event);
	}
}

