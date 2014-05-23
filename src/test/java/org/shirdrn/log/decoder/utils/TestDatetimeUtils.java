package org.shirdrn.log.decoder.utils;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class TestDatetimeUtils {

	public static String getStringDatetime(long time, String format) {
		DateFormat df = new SimpleDateFormat(format);
		Date d = new Date(time);
		return df.format(d);
	}
	
	public static String getStringTime(String time, String format) {
		DateFormat df = new SimpleDateFormat(format);
		Date d = null;
		try {
			d = df.parse(time);
		} catch (ParseException e) {
			e.printStackTrace();
		}
		return df.format(d);
	}
	
	public static void main(String[] args) {
		long time = 2192638001L;
		System.out.println(getStringDatetime(time, "yyyy-MM-dd HH:mm:ss.SSS"));
	}
}
