package com.example.enulatorcache;


import android.app.Activity;
import android.os.Bundle;

import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends Activity {
	
	private Button btnStart = null;
	
	static{
		System.loadLibrary("EnulatorCache");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		findViewById(R.id.btn_start).setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				if(start()==0){
					Toast.makeText(MainActivity.this, "�����������", Toast.LENGTH_SHORT).show();
				}else{
					Toast.makeText(MainActivity.this, "��ģ����������", Toast.LENGTH_SHORT).show();
				}
			}
		});
	}
	
	
	private native int start();
}
