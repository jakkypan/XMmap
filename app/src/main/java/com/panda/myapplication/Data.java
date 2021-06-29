package com.panda.myapplication;

/**
 * @author panda
 * created at 2021/6/24 11:51 AM
 */
public class Data {
	public static String data() {
		StringBuilder content = new StringBuilder();
		for (int i = 0; i < 1000000; i++) {
			content.append("ABCDEFGHIGKLMNOPQRSEUVWXYZ");
		}
		return content.toString();
	}
}
