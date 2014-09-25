package com.Nathaniel.healthcare.beta;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.os.SystemClock;
import android.widget.LinearLayout;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.GraphView.GraphViewData;
import com.jjoe64.graphview.GraphView.LegendAlign;
import com.jjoe64.graphview.GraphViewSeries;
import com.jjoe64.graphview.GraphViewSeries.GraphViewSeriesStyle;
import com.jjoe64.graphview.LineGraphView;

public class GraphViewActivity extends Activity {
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

	double v = 0;
	int[] sensors = new int[1000];

	GraphViewSeries series_du;
	GraphViewSeries series_vo;
	GraphViewSeries series_th;
	GraphViewSeries series_di;
	
	GraphViewData[] g_data_du = new GraphViewData[1];
	GraphViewData[] g_data_vo = new GraphViewData[1];
	GraphViewData[] g_data_th = new GraphViewData[1];
	GraphViewData[] g_data_di = new GraphViewData[1];

	int cnt_du = 0;
	int cnt_vo = 0;
	int cnt_th = 0;
	int cnt_di = 0;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.graph_view);
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		GraphViewData[] data = new GraphViewData[1];
		data[0] = new GraphViewData(0, 0);
		series_di = new GraphViewSeries("Dioxide", new GraphViewSeriesStyle(Color.rgb(255, 93, 68), 4), data);
		series_du = new GraphViewSeries("Dust", new GraphViewSeriesStyle(Color.rgb(70, 232, 130), 4), data);
		series_vo = new GraphViewSeries("VOC", new GraphViewSeriesStyle(Color.rgb(193, 255, 89), 4), data);
		series_th = new GraphViewSeries("Thermometer", new GraphViewSeriesStyle(Color.rgb(255, 163, 92), 4), data);

		// Graph Title
		GraphView graphView;
		graphView = new LineGraphView(this, "Sensors Graph");

		// add data
		graphView.addSeries(series_di);
		graphView.addSeries(series_du);
		graphView.addSeries(series_vo);
		graphView.addSeries(series_th);

		graphView.setViewPort(0, 50);	// first view start~end location  
		graphView.setScalable(true);	// enable scrolling and zoom

		// optional - legend
		graphView.setShowLegend(true);
		graphView.setLegendAlign(LegendAlign.MIDDLE);	// Legend location
		graphView.setLegendWidth(280);				// Legend Width

		LinearLayout layout = (LinearLayout)findViewById(R.id.graph_sub_layout);		
		layout = (LinearLayout) findViewById(R.id.graph_layout);
		layout.addView(graphView);

		new Thread(new Runnable() {
			@Override
			public void run() {//				
				while(true) {
					// 현재 UI 스레드가 아님으로 메시지 큐에 Runnable을 등록 함
					runOnUiThread(new Runnable() {
						public void run() {
							sensors = AbstractionLib.getData();
							//Log.i("RECV:", "2-DIOXIDE");
							for(int i = 0; sensors[i] != 0; i += 100) {
								switch(sensors[i+1]) {
								case HANBACK_DIOXIDE_FRONT:
									series_di.appendData(new GraphViewData(cnt_di+1, sensors[i+4]), true, 100);
									cnt_di++;
									break;
								case HANBACK_DUST_FRONT:
									series_du.appendData(new GraphViewData(cnt_du+1, sensors[i+4]), true, 100);
									cnt_du++;
									break;
								case HANBACK_VOC_FRONT:
									series_vo.appendData(new GraphViewData(cnt_vo+1, sensors[i+4]), true, 100);
									cnt_vo++;
									break;
								case HANBACK_THRMMTR_FRONT:
									series_th.appendData(new GraphViewData(cnt_th+1, sensors[i+4]), true, 100);
									cnt_th++;
									break;
								}
							}
						}
					});
					// 복잡한 작업 simulating	
					SystemClock.sleep(500);
				}

			}
		}).start();
	}//end resume
}
