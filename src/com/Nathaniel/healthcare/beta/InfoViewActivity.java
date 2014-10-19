package com.Nathaniel.healthcare.beta;

import android.app.Activity;
import android.os.Bundle;
import android.os.SystemClock;
import android.view.KeyEvent;
import android.widget.LinearLayout;

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
