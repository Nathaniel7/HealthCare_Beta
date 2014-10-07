package com.Nathaniel.healthcare.beta;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

public class SensorViewActivity extends Activity {	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.sensor_view);
	}

	@Override
	protected void onResume() {
		super.onResume();

		LinearLayout layout_sensorlist = (LinearLayout)findViewById(R.id.sensorlist_area);
		layout_sensorlist.removeAllViews();

		LinearLayout layout_sensorcontent_title = new LinearLayout(this);
		layout_sensorcontent_title.setOrientation(LinearLayout.HORIZONTAL);
		layout_sensorcontent_title.setBackgroundColor(Color.parseColor("#FF6A6A"));

		for(int i = 0; i < 5; i++) {
			TextView tv_content = new TextView(this);
			tv_content = new TextView(this);

			if(i == 0) {
				tv_content.setText("   Photo");
				tv_content.setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.35f));
			}
			else if(i == 1) {
				tv_content.setText("Brand");
				tv_content.setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.15f));
			}
			else if(i == 2) {
				tv_content.setText("Type");
				tv_content.setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.20f));
			}
			else if(i == 3) {
				tv_content.setText("Features");
				tv_content.setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.35f));
			}
			//					Log.i("### List Act ###", tv_content[i].getText().toString());
			layout_sensorcontent_title.addView(tv_content);
		}
		layout_sensorlist.addView(layout_sensorcontent_title);

		int[] sensors = AbstractionLib.getData();
		
		if(sensors[0] == 0) {
			//			Log.i("### List Act ###", "Select data not exist!");
			TextView tv = new TextView(this);
			tv.setText("연결된 센서가 존재하지 않습니다.");
			tv.setGravity(Gravity.CENTER);
			layout_sensorlist.addView(tv);
			Log.i("############", "..........");
		}

		////////장치의 상태를 나타낸다 ////////////////
		final int STATE_WORST = 0;
		final int STATE_GOOD = 1;
		final int STATE_WAIT = 2;
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
					iv_content.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT, 0.1f));
					layout_sensorcontent.addView(iv_content);
				}
				else if(j == 1) {	// Brand
					switch(sensors[i]) {
					case COMPANY_HANBACK:
//						Log.i("#######", "HANBACK ELECTRONIC");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("HANBACK");
						tv_content[j-1].setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.15f));
						break;
					default:
//						Log.i("#######", "Unknown Company");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("Unknown");
						tv_content[j-1].setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.15f));
						break;
					}
					layout_sensorcontent.addView(tv_content[j-1]);
				}
				else if(j == 2) {	// Type
					switch(sensors[i+1]) {
					case HANBACK_DIOXIDE_FRONT:
//						Log.i("#######", "DIOXIDE");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("Dioxide");
						tv_content[j-1].setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.20f));
						break;
					case HANBACK_DUST_FRONT:
//						Log.i("#######", "DUST");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("Dust");
						tv_content[j-1].setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.20f));
						break;
					case HANBACK_VOC_FRONT:
//						Log.i("#######", "VOC");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("VOC");
						tv_content[j-1].setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.20f));
						break;
					case HANBACK_THRMMTR_FRONT:
//						Log.i("#######", "THERMOMETER");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("Thermometer");
						tv_content[j-1].setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.20f));
						break;
					case HANBACK_WEATHER_FRONT:
//						Log.i("#######", "WEATHER");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("Weather");
						tv_content[j-1].setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.20f));
						break;
					}
					layout_sensorcontent.addView(tv_content[j-1]);
				}
				else if(j == 3) {	// Features
					switch(sensors[i+1]) {
					case HANBACK_DIOXIDE_FRONT:
//						Log.i("#######", "DIOXIDE");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("실내 공기 중의 이산화탄소 농도를 측정하며, 상황에 따른 안내가 가능합니다.");
						tv_content[j-1].setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.35f));
						break;
					case HANBACK_DUST_FRONT:
//						Log.i("#######", "DUST");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("실내 공기 중의 먼지 농도를 측정하며, 공기의 탁한 정도를 알 수 있습니다.");
						tv_content[j-1].setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.35f));
						break;
					case HANBACK_VOC_FRONT:
//						Log.i("#######", "VOC");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("공기 중 휘발성 농도를 측정하며, LPG 등 가스의 및 가연성 기체를 감지합니다.");
						tv_content[j-1].setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.35f));
						break;
					case HANBACK_THRMMTR_FRONT:
//						Log.i("#######", "THERMOMETER");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("적외선 센서로 인체 온도와 실내 온도를 동시에 체크가 가능합니다.");
						tv_content[j-1].setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.35f));
						break;
					case HANBACK_WEATHER_FRONT:
//						Log.i("#######", "WEATHER");
						tv_content[j-1] = new TextView(this);
						tv_content[j-1].setText("기압, 조도, 습도, 온도, 가속도를 측정 할 수 있습니다.");
						tv_content[j-1].setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.35f));
						break;
					}
					layout_sensorcontent.addView(tv_content[j-1]);
				}	
			}
			layout_sensorlist.addView(layout_sensorcontent);
		}
	}

}

