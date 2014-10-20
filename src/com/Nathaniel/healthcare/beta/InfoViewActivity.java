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

public class InfoViewActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.info_view);

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

		LinearLayout layout_infolist = (LinearLayout)findViewById(R.id.infolist_area);
		layout_infolist.removeAllViews();
		layout_infolist.setWeightSum(1.0f);
		
		LinearLayout layout_infocontent_title = new LinearLayout(this);
		layout_infocontent_title.setOrientation(LinearLayout.HORIZONTAL);
		layout_infocontent_title.setBackgroundColor(Color.parseColor("#FF6A6A"));

		LinearLayout.LayoutParams image_Lparam = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.15f);
		LinearLayout.LayoutParams type_Lparam = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.15f);
		LinearLayout.LayoutParams brand_Lparam = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.15f);
		LinearLayout.LayoutParams feature_Lparam = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.55f);
		
		for(int i = 0; i < 5; i++) {
			TextView tv_content = new TextView(this);
			tv_content = new TextView(this);

			if(i == 0) {
				tv_content.setText("Photo");
				tv_content.setLayoutParams(image_Lparam);
			}
			else if(i == 1) {
				tv_content.setText("Type");
				tv_content.setLayoutParams(brand_Lparam);
			}
			else if(i == 2) {
				tv_content.setText("Measurement");
				tv_content.setLayoutParams(type_Lparam);
			}
			else if(i == 3) {
				tv_content.setText("Features");
				tv_content.setLayoutParams(feature_Lparam);
			}
			//					Log.i("### List Act ###", tv_content[i].getText().toString());
			tv_content.setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
			tv_content.setTextAppearance(this, android.R.style.TextAppearance_Medium);
			layout_infocontent_title.addView(tv_content);
		}
		layout_infolist.addView(layout_infocontent_title);

		int[] sensors = AbstractionLib.getData();
		
		if(sensors[0] == 0) {
			//			Log.i("### List Act ###", "Select data not exist!");
			TextView tv = new TextView(this);
			tv.setText("연결된 센서가 존재하지 않습니다.");
			tv.setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
			tv.setTextAppearance(this, android.R.style.TextAppearance_Medium);
			layout_infolist.addView(tv);
			//Log.i("############", "..........");
		}

		//////센서가 출저회사를 나타낸다 ////////////////
		final int COMPANY_HANBACK = 0x76;
		///////센서의 종류를 나타낸다///////
		final int HANBACK_DIOXIDE_FRONT = 0x13;
		final int HANBACK_DUST_FRONT = 0x15;
		final int HANBACK_VOC_FRONT = 0x17;
		final int HANBACK_THRMMTR_FRONT = 0x18;
		final int HANBACK_WEATHER_FRONT = 0x12;

		for(int i = 0; sensors[i] != 0; i += 100) {
			LinearLayout layout_sensorcontent = new LinearLayout(this);
			layout_sensorcontent.setOrientation(LinearLayout.HORIZONTAL);
			layout_sensorcontent.setBackgroundColor(Color.parseColor("#BBDEF8"));
			layout_sensorcontent.setWeightSum(1.0f);
			
			ImageView iv_content = new ImageView(this);
			TextView[] tv_content = new TextView[3];

			for(int j = 0; j < 5; j++) {
				if(j == 0) {	// Photo
					switch(sensors[i+1]) {
					case HANBACK_DIOXIDE_FRONT:
//						Log.i("#######", "DIOXIDE");
						iv_content.setBackgroundResource(R.drawable.dioxide);
						break;
					case HANBACK_DUST_FRONT:
//						Log.i("#######", "DUST");
						iv_content.setBackgroundResource(R.drawable.dust);
						break;
					case HANBACK_VOC_FRONT:
//						Log.i("#######", "VOC");
						iv_content.setBackgroundResource(R.drawable.voc);
						break;
					case HANBACK_THRMMTR_FRONT:
//						Log.i("#######", "THERMOMETER");
						iv_content.setBackgroundResource(R.drawable.thermometer);
						break;
					case HANBACK_WEATHER_FRONT:
//						Log.i("#######", "WEATHER");
						iv_content.setBackgroundResource(R.drawable.weather);
						break;
					}
					iv_content.setLayoutParams(image_Lparam);
					layout_sensorcontent.addView(iv_content);
				}
				else if(j == 1) {	// Brand
					switch(sensors[i]) {
					case COMPANY_HANBACK:
//						Log.i("#######", "HANBACK ELECTRONIC");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("HANBACK");
						tv_content[j-1].setLayoutParams(brand_Lparam);
						break;
					default:
//						Log.i("#######", "Unknown Company");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("Unknown");
						tv_content[j-1].setLayoutParams(brand_Lparam);
						break;
					}
					tv_content[j-1].setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
					tv_content[j-1].setTextAppearance(this, android.R.style.TextAppearance_Medium);
					layout_sensorcontent.addView(tv_content[j-1]);
				}
				else if(j == 2) {	// Type
					switch(sensors[i+1]) {
					case HANBACK_DIOXIDE_FRONT:
//						Log.i("#######", "DIOXIDE");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("Dioxide");
						tv_content[j-1].setLayoutParams(type_Lparam);
						break;
					case HANBACK_DUST_FRONT:
//						Log.i("#######", "DUST");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("Dust");
						tv_content[j-1].setLayoutParams(type_Lparam);
						break;
					case HANBACK_VOC_FRONT:
//						Log.i("#######", "VOC");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("VOC");
						tv_content[j-1].setLayoutParams(type_Lparam);
						break;
					case HANBACK_THRMMTR_FRONT:
//						Log.i("#######", "THERMOMETER");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("Thermometer");
						tv_content[j-1].setLayoutParams(type_Lparam);
						break;
					case HANBACK_WEATHER_FRONT:
//						Log.i("#######", "WEATHER");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("Weather");
						tv_content[j-1].setLayoutParams(type_Lparam);
						break;
					}
					tv_content[j-1].setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
					tv_content[j-1].setTextAppearance(this, android.R.style.TextAppearance_Medium);
					layout_sensorcontent.addView(tv_content[j-1]);
				}
				else if(j == 3) {	// Features
					switch(sensors[i+1]) {
					case HANBACK_DIOXIDE_FRONT:
//						Log.i("#######", "DIOXIDE");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("실내 공기 중의 이산화탄소 농도를 측정하며, 상황에 따른 안내가 가능합니다.");
						tv_content[j-1].setLayoutParams(feature_Lparam);
						break;
					case HANBACK_DUST_FRONT:
//						Log.i("#######", "DUST");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("실내 공기 중의 먼지 농도를 측정하며, 공기의 탁한 정도를 알 수 있습니다.");
						tv_content[j-1].setLayoutParams(feature_Lparam);
						break;
					case HANBACK_VOC_FRONT:
//						Log.i("#######", "VOC");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("공기 중 휘발성 농도를 측정하며, LPG 등 가스의 및 가연성 기체를 감지합니다.");
						tv_content[j-1].setLayoutParams(feature_Lparam);
						break;
					case HANBACK_THRMMTR_FRONT:
//						Log.i("#######", "THERMOMETER");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("적외선 센서로 인체 온도와 실내 온도를 동시에 체크가 가능합니다.");
						tv_content[j-1].setLayoutParams(feature_Lparam);
						break;
					}
					tv_content[j-1].setTextAppearance(this, android.R.style.TextAppearance_Medium);
					layout_sensorcontent.addView(tv_content[j-1]);
				}	
			}
			layout_infolist.addView(layout_sensorcontent);
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
