package org.shirdrn.log.decoder.utils;


public class ParserUtils {
	
	public static boolean isValid(String[] a, int minArrayLen) {
		if(a.length >= minArrayLen) {
			return true;
		}
		return false;
	}
}
