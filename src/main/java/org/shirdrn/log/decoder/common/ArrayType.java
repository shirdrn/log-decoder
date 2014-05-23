package org.shirdrn.log.decoder.common;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ArrayType {

	private String type;
	private int length;
	
	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

	public int getLength() {
		return length;
	}

	public void setLength(int length) {
		this.length = length;
	}
	
	private static final Pattern ARRAY_TYPE_PATTERN = Pattern.compile("([a-zA-Z]+\\d*)\\[(\\d+)\\]");
	
	public static ArrayType parseArrayType(String type) {
		String oriType = type;
		ArrayType at = new ArrayType();
		Matcher m = ARRAY_TYPE_PATTERN.matcher(oriType);
		String t = null;
		int arrayLenth = 0;
		if(m.find()) {
			t = m.group(1);
			arrayLenth = Integer.parseInt(m.group(2));
			at.type = t;
			at.length = arrayLenth;
		}
		return at;
	}

	@Override
	public String toString() {
		return "[type = " + type + ", length = " + length + "]";
	}
	
}
