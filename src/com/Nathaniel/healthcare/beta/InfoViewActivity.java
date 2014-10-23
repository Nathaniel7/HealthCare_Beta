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
	interface HANBACK {
		int DIOXIDE_FRONT 	= 0x13;
		int DUST_FRONT 		= 0x15;
		int VOC_FRONT 		= 0x17;
		int THRMMTR_FRONT 	= 0x18;
	}

	interface STATUS
	{
		int NOT_YET_SUMMARY 	= 0x00;
		int GOOD			= 0x01;
		int NORMAL			= 0x02;
		int BAD				= 0x04;
		int WORST			= 0x08;
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.info_view);

		new Thread(new Runnable() {
			@Override
			public void run() {
				while(true) {
					runOnUiThread(new Runnable() {
						public void run() {
							onResume();
						}
					});

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

		LinearLayout.LayoutParams image_Lparam = new LinearLayout.LayoutParams(0, 193, 0.15f);
		LinearLayout.LayoutParams photo_Lparam = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.15f);
		LinearLayout.LayoutParams type_Lparam = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.15f);
		LinearLayout.LayoutParams measurement_Lparam = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.17f);
		LinearLayout.LayoutParams feature_Lparam = new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.53f);

		for(int i = 0; i < 5; i++) {
			TextView tv_content = new TextView(this);
			tv_content = new TextView(this);

			if(i == 0) {
				tv_content.setText("Photo");
				tv_content.setLayoutParams(photo_Lparam);
			}
			else if(i == 1) {
				tv_content.setText("Type");
				tv_content.setLayoutParams(type_Lparam);
			}
			else if(i == 2) {
				tv_content.setText("Measurement");
				tv_content.setLayoutParams(measurement_Lparam);
			}
			else if(i == 3) {
				tv_content.setText("Features");
				tv_content.setLayoutParams(feature_Lparam);
			}

			tv_content.setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
			tv_content.setTextAppearance(this, android.R.style.TextAppearance_Medium);
			layout_infocontent_title.addView(tv_content);
		}
		layout_infolist.addView(layout_infocontent_title);

		int[] sensors = AbstractionLib.getData();

		if(sensors[0] == 0) {
			TextView tv = new TextView(this);
			tv.setText("연결된 센서가 존재하지 않습니다.");
			tv.setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
			tv.setTextAppearance(this, android.R.style.TextAppearance_Medium);
			layout_infolist.addView(tv);
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

			for(int j = 0; j < 3; j++) {
				if(j == 0) {	// Type
					switch(sensors[i+1]) {
					case HANBACK.DIOXIDE_FRONT:
						tv_content[j] = new TextView(this);
						tv_content[j].setText("Dioxide");
						break;
					case HANBACK.DUST_FRONT:
						tv_content[j] = new TextView(this);
						tv_content[j].setText("Dust");
						break;
					case HANBACK.VOC_FRONT:
						tv_content[j] = new TextView(this);
						tv_content[j].setText("VOC");
						break;
					case HANBACK.THRMMTR_FRONT:
						tv_content[j] = new TextView(this);
						tv_content[j].setText("Thermometer");
						break;
					}

					tv_content[j].setLayoutParams(type_Lparam);
					tv_content[j].setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
					tv_content[j].setTextAppearance(this, android.R.style.TextAppearance_Medium);
				}
				else if(j == 1) {	// Measurement
					switch(sensors[i+1]) {
					case HANBACK.DIOXIDE_FRONT:
						tv_content[j] = new TextView(this);
						tv_content[j].setText(Integer.toString(sensors[i+4]) + " ppm  ");
						break;
					case HANBACK.DUST_FRONT:
						tv_content[j] = new TextView(this);
						tv_content[j].setText(Integer.toString(sensors[i+4]) + " ppb  ");
						break;
					case HANBACK.VOC_FRONT:
						tv_content[j] = new TextView(this);
						tv_content[j].setText(Integer.toString(sensors[i+4]/10) + " ppb  ");
						break;
					case HANBACK.THRMMTR_FRONT:
						tv_content[j] = new TextView(this);
						tv_content[j].setText(Float.toString(sensors[i+4]/(float)100) + " ℃  ");
						break;
					}

					tv_content[j].setLayoutParams(measurement_Lparam);
					tv_content[j].setGravity(Gravity.END);
					tv_content[j].setTextAppearance(this, android.R.style.TextAppearance_Large);
				}
				else if(j == 2) {
					switch(sensors[i+1]) {
					case HANBACK.DIOXIDE_FRONT:
						tv_content[j] = new TextView(this);

						switch(sensors[i+3]) {
						case STATUS.NOT_YET_SUMMARY:
							tv_content[j].setText("측정치 분석중입니다.");
							iv_content.setBackgroundResource(R.drawable.pass);
							break;
						case STATUS.WORST: 
							tv_content[j].setText("두통, 졸음, 집중력 저하, 심박 수 증가 및 메스꺼움이 발생할 수 있을 정도의 이산화탄소 농도가 측정되었습니다. 환기가 필요합니다.");
							iv_content.setBackgroundResource(R.drawable.dizziness);
							break;
						case STATUS.BAD: 
							tv_content[j].setText("약간의 졸음이 유발될 수 있을 정도의 이산화탄소 농도가 측정되었습니다. 적당한 환기가 요구됩니다.");
							iv_content.setBackgroundResource(R.drawable.dizziness);
							break;
						case STATUS.NORMAL: 
							tv_content[j].setText("실내에서의 일반적인 이산화탄소 농도가 측정되었습니다.");
							iv_content.setBackgroundResource(R.drawable.pass);
							break;
						case STATUS.GOOD: 
							tv_content[j].setText("실외에서 느낄 수 있는 상당히 쾌적한 수준의 이산화탄소의 농도가 측정되었습니다.");
							iv_content.setBackgroundResource(R.drawable.pass);
							break;
						}

						break;
					case HANBACK.DUST_FRONT:
						tv_content[j] = new TextView(this);

						switch(sensors[i+3]) {
						case STATUS.NOT_YET_SUMMARY:
							tv_content[j].setText("측정치 분석중입니다.");
							iv_content.setBackgroundResource(R.drawable.pass);
							break;
						case STATUS.WORST: 
							tv_content[j].setText("미세먼지의 농도가 상당히 높습니다. 실외라면 활동을 자제하시고 실내라면 환기가 필요합니다.");
							iv_content.setBackgroundResource(R.drawable.finedust);
							break;
						case STATUS.BAD: 
							tv_content[j].setText("미세먼지의 농도가 높습니다. 실외라면 장시간의 활동은 자제하시고 실내라면 적당한 환기가 요구됩니다.");
							iv_content.setBackgroundResource(R.drawable.finedust);
							break;
						case STATUS.NORMAL: 
							tv_content[j].setText("약간의 미세먼지가 존재하며, 환기가 필요할 수 있습니다.");
							iv_content.setBackgroundResource(R.drawable.pass);
							break;
						case STATUS.GOOD: 
							tv_content[j].setText("미세먼지가 거이 존재하지 않으며 상당히 쾌적한 상태입니다.");
							iv_content.setBackgroundResource(R.drawable.pass);
							break;
						}

						break;
					case HANBACK.VOC_FRONT:
						tv_content[j] = new TextView(this);

						switch(sensors[i+3]) {
						case STATUS.NOT_YET_SUMMARY:
							tv_content[j].setText("측정치 분석중입니다.");
							iv_content.setBackgroundResource(R.drawable.pass);
							break;
						case STATUS.WORST: 
							tv_content[j].setText("공기 중 휘발성 기체의 농도가 상당히 높아 화재위험이 상당히 높으므로 환기가 필요합니다.");
							iv_content.setBackgroundResource(R.drawable.fire);
							break;
						case STATUS.BAD: 
							tv_content[j].setText("공기 중 휘발성 기체의 농도가 평균치 이상으로 측정되었습니다. 적당한 환기가 요구됩니다.");
							iv_content.setBackgroundResource(R.drawable.fire);
							break;
						case STATUS.NORMAL: 
							tv_content[j].setText("공기 중 휘발성 기체의 농도가 안정치를 조금 넘어섰습니다. 적당한 환기가 필요할 수 있습니다.");
							iv_content.setBackgroundResource(R.drawable.pass);
							break;
						case STATUS.GOOD: 
							tv_content[j].setText("공기 중 휘발성 기체의 농도가 안정적입니다.");
							iv_content.setBackgroundResource(R.drawable.pass);
							break;
						}

						break;
					case HANBACK.THRMMTR_FRONT:
						tv_content[j] = new TextView(this);

						switch(sensors[i+3]) {
						case STATUS.NOT_YET_SUMMARY:
							tv_content[j].setText("측정치 분석중입니다.");
							iv_content.setBackgroundResource(R.drawable.pass);
							break;
						case STATUS.WORST: 
							tv_content[j].setText("저체온 혹은 상당한 고체온이 감지되었습니다. 고체온의 경우 신체활동이 불가능 할 정도의 고통을 동반하고 있는 상태일 수 있습니다. 병원 진료를 받아보기를 권합니다.");
							iv_content.setBackgroundResource(R.drawable.fever);
							break;
						case STATUS.BAD: 
							tv_content[j].setText("높은 체온이 감지되었습니다. 질병에 의한 요인일 수 있으므로 병원질료를 권장합니다.");
							iv_content.setBackgroundResource(R.drawable.fever);
							break;
						case STATUS.NORMAL: 
							tv_content[j].setText("체온이 안정치보다 조금은 높지만 괜찮은 수준입니다.");
							iv_content.setBackgroundResource(R.drawable.pass);
							break;
						case STATUS.GOOD: 
							tv_content[j].setText("체온이 안정적입니다.");
							iv_content.setBackgroundResource(R.drawable.pass);
							break;
						}

						break;
					}

					iv_content.setLayoutParams(image_Lparam);
					tv_content[j].setLayoutParams(feature_Lparam);
					tv_content[j].setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
					tv_content[j].setTextAppearance(this, android.R.style.TextAppearance_Medium);
				}
			}

			layout_sensorcontent.addView(iv_content);
			for(int k = 0; k < 3; k++)
				layout_sensorcontent.addView(tv_content[k]);

			layout_infolist.addView(layout_sensorcontent);
		}
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
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
