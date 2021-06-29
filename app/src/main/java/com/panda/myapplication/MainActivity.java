package com.panda.myapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.panda.xmmap.XMmap;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {
	private XMmap xmmap;
	File normal;
	File normalBuffer;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		File dir = getDir("test", MODE_PRIVATE);
		if (!dir.exists()) {
			dir.mkdirs();
		}
		String testLog = new File(dir, "mmap.log").getAbsolutePath();
		xmmap = new XMmap(testLog, false);
		normal = new File(dir, "normal.log");
		if (!dir.exists()) {
			try {
				dir.createNewFile();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		normalBuffer = new File(dir, "normal-buffer.log");
		if (!dir.exists()) {
			try {
				normalBuffer.createNewFile();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public void mmapWrite(View view) {
		xmmap.write(Data.data().getBytes());
	}

	public void mmapRead(View view) {
		byte[] bytes = xmmap.read();
//		Log.e("222", new String(bytes));
	}

	public void fileWrite(View view) {
		String content = Data.data();
		FileWriter fileWriter = null;
		try {
			fileWriter = new FileWriter(normal);
			long start = System.currentTimeMillis();
			fileWriter.write(content);
			Log.e("【xmmap】", "filewriter write time: " + (System.currentTimeMillis() - start));
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				fileWriter.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public void fileRead(View view) {
		FileReader fileReader = null;
		try {
			fileReader = new FileReader(normal);
			long start = System.currentTimeMillis();
			char[] buf = new char[4096];
			int len;
			StringBuffer sb = new StringBuffer();
			while((len=fileReader.read(buf)) != -1) {
//				sb.append(new String(buf,0,len));
			}
			Log.e("【xmmap】", "filereader read time: " + (System.currentTimeMillis() - start));
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				fileReader.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public void fileBufferWrite(View view) {
		String content = Data.data();
		BufferedWriter bufferedWriter = null;
		try {
			bufferedWriter = new BufferedWriter(new FileWriter(normalBuffer));
			long start = System.currentTimeMillis();
			bufferedWriter.write(content);
			Log.e("【xmmap】", "filebufferwriter write time: " + (System.currentTimeMillis() - start));
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				bufferedWriter.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public void fileBufferRead(View view) {
		BufferedReader bufferedReader = null;
		try {
			bufferedReader = new BufferedReader(new FileReader(normalBuffer));
			long start = System.currentTimeMillis();
			char[] buf = new char[4096];
			int len;
			StringBuffer sb = new StringBuffer();
			while((len=bufferedReader.read(buf)) != -1){
//				sb.append(new String(buf,0,len));
			}
			Log.e("【xmmap】", "filebufferreader read time: " + (System.currentTimeMillis() - start));
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				bufferedReader.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
}