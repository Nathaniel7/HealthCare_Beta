package com.Nathaniel.healthcare.beta;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.os.SystemClock;
import android.view.KeyEvent;
import android.view.ViewGroup;
import android.widget.LinearLayout;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.GraphView.GraphViewData;
import com.jjoe64.graphview.GraphView.LegendAlign;
import com.jjoe64.graphview.GraphViewSeries;
import com.jjoe64.graphview.GraphViewSeries.GraphViewSeriesStyle;
import com.jjoe64.graphview.GraphViewStyle.GridStyle;
import com.jjoe64.graphview.LineGraphView;

@SuppressLint("NewApi")
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
	
	GraphView graphView01;
	GraphView graphView02;
	GraphView graphView03;
	GraphView graphView04;
		
	GraphViewSeries series_du;
	GraphViewSeries series_vo;
	GraphViewSeries series_th;
	GraphViewSeries series_di;

	GraphViewData[] g_data_du = new GraphViewData[1];
	GraphViewData[] g_data_vo = new GraphViewData[1];
	GraphViewData[] g_data_th = new GraphViewData[1];
	GraphViewData[] g_data_di = new GraphViewData[1];

	int cnt_th = 0;
	int cnt_du = 0;
	int cnt_vo = 0;
	int cnt_di = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.graph_view);
	}

	@Override
	protected void onResume() {
		super.onResume();

		// setGraphView(title, maxY_axis, minY_axis, num_Label, line_under_color);
		graphView01 = setGraphView("Thermometer(℃)"	, 100, 0, 6, Color.rgb(232, 168, 163));
		graphView02 = setGraphView("Dust(ppb)", 600, 0, 6, Color.rgb(142, 189, 232));
		graphView03 = setGraphView("VOC(ppb)", 1000, 0, 6, Color.rgb(216, 164, 255));
		graphView04 = setGraphView("Dioxide(ppm)", 2500, 0, 6, Color.rgb(255, 201, 164));
		
		GraphViewData[] data = new GraphViewData[1];
		data[0] = new GraphViewData(0, 0);
		series_th = new GraphViewSeries("Thermometer", new GraphViewSeriesStyle(Color.rgb(232, 81, 69), 4), data);
		series_du = new GraphViewSeries("Dust(ppb)", new GraphViewSeriesStyle(Color.rgb(49, 145, 232), 4), data);
		series_vo = new GraphViewSeries("VOC(ppb)", new GraphViewSeriesStyle(Color.rgb(172, 60, 255), 4), data);
		series_di = new GraphViewSeries("Dioxide(ppm)", new GraphViewSeriesStyle(Color.rgb(255, 139, 60), 4), data);

		graphView01.addSeries(series_th);
//		graphView01.setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.5f));
		graphView02.addSeries(series_du);
//		graphView02.setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.5f));
		graphView03.addSeries(series_vo);
//		graphView03.setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.5f));
		graphView04.addSeries(series_di);
//		graphView04.setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 0.5f));
		
		
		LinearLayout sub_layout_graph1 = new LinearLayout(this);
		sub_layout_graph1.setBackgroundColor(Color.rgb(255, 255, 255));
		sub_layout_graph1.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT, 0.4f));
		sub_layout_graph1.addView(graphView01);
		
		LinearLayout sub_layout_graph2 = new LinearLayout(this);
		sub_layout_graph2.setBackgroundColor(Color.rgb(230, 230, 230));
		sub_layout_graph2.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT, 0.4f));
		sub_layout_graph2.addView(graphView02);
		
		LinearLayout sub_layout_graph3 = new LinearLayout(this);
		sub_layout_graph3.setBackgroundColor(Color.rgb(230, 230, 230));
		sub_layout_graph3.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT, 0.4f));
		sub_layout_graph3.addView(graphView03);
		
		LinearLayout sub_layout_graph4 = new LinearLayout(this);
		sub_layout_graph4.setBackgroundColor(Color.rgb(255, 255, 255));
		sub_layout_graph4.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT, 0.4f));
		sub_layout_graph4.addView(graphView04);
		
		LinearLayout layout_graph1 = new LinearLayout(this);
		layout_graph1.setOrientation(LinearLayout.HORIZONTAL);
		layout_graph1.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT, 0.4f));
		layout_graph1.addView(sub_layout_graph1);
		layout_graph1.addView(sub_layout_graph2);
		
		LinearLayout layout_graph2 = new LinearLayout(this);
		layout_graph2.setOrientation(LinearLayout.HORIZONTAL);
		layout_graph2.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT, 0.4f));
		layout_graph2.addView(sub_layout_graph3);
		layout_graph2.addView(sub_layout_graph4);
		
		LinearLayout layout = (LinearLayout) findViewById(R.id.graph_layout);
		layout.addView(layout_graph1);
		layout.addView(layout_graph2);
		

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
									series_di.appendData(new GraphViewData(cnt_di+1, sensors[i+4]), true, 1000);
									cnt_di++;
									break;
								case HANBACK_DUST_FRONT:
									series_du.appendData(new GraphViewData(cnt_du+1, sensors[i+4]), true, 1000);
									cnt_du++;
									break;
								case HANBACK_VOC_FRONT:
									series_vo.appendData(new GraphViewData(cnt_vo+1, sensors[i+4]/10), true, 1000);
									cnt_vo++;
									break;
								case HANBACK_THRMMTR_FRONT:
									series_th.appendData(new GraphViewData(cnt_th+1, sensors[i+4]/100), true, 1000);
									cnt_th++;
									break;
								}
							}
						}
					});
					// 복잡한 작업 simulating	
					SystemClock.sleep(250);
				}
			}
		}).start();
	}//end resume
	
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
	
	public GraphView setGraphView(String title, int maxY_axis, int minY_axis, int num_Label, int line_under_color)
	{
		LineGraphView lineView = new LineGraphView(this, title);
		lineView.setDrawBackground(true);
		lineView.setBackgroundColor(line_under_color);
	
		GraphView graphView;
		graphView = lineView;
		graphView.setViewPort(0, 200);	// first view start~end location  
		graphView.setScalable(true);	// enable scrolling and zoom
		graphView.setScrollable(true);
		graphView.setShowLegend(false);
		graphView.setDisableTouch(true);
//		graphView.setShowLegend(true);
		graphView.setManualYAxisBounds(maxY_axis, minY_axis);
		graphView.setLegendAlign(LegendAlign.BOTTOM);				// Legend location
//		graphView.getGraphViewStyle().setLegendWidth(250);			// Legend Width
//		graphView.getGraphViewStyle().setLegendMarginBottom(10);	// Legend Width
//		graphView.getGraphViewStyle().setLegendSpacing(10);			// Legends Space
		graphView.getGraphViewStyle().setGridStyle(GridStyle.BOTH);
		graphView.getGraphViewStyle().setGridColor(Color.rgb(100, 100, 100));

		graphView.setHorizontalLabels(new String[] {"", "Time", ""});
		graphView.getGraphViewStyle().setTextSize(25);
		graphView.getGraphViewStyle().setHorizontalLabelsColor(Color.BLACK);
		graphView.getGraphViewStyle().setVerticalLabelsColor(Color.rgb(50, 50, 50));
		// the number of Y-axis Label
		graphView.getGraphViewStyle().setNumVerticalLabels(num_Label);	
		
		return graphView;
	}
}

