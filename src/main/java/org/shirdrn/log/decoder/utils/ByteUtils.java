package org.shirdrn.log.decoder.utils;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;


public class ByteUtils {

	public static byte decodeByte(byte[] singleByteBuffer) {
		byte[] buf = singleByteBuffer;
        byte result  = buf[0];
		return result;
	}
	
	public static short decodeShort(byte[] doubleByteBuffer) {
		byte[] buf = doubleByteBuffer;
        short result  = (short) (buf[0] << 8 | buf[1]);
        result = Short.reverseBytes(result);
		return result;
	}
	
	public static int decodeInt(byte[] quadByteBuffer) {
		byte[] buf = quadByteBuffer;
        int result  = (int) (buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3]);
        result = Integer.reverse(result);
		return result;
	}
	
	public static long decodeLong(byte[] doubleQuadByteBuffer) {
		byte[] buf = doubleQuadByteBuffer;
        long result  = (long) (buf[0] << 56 | buf[1] << 48 
        		| buf[2] << 40 | buf[3] << 32 
        		| buf[4] << 24 | buf[5] << 16 
        		| buf[6] << 8 | buf[7]);
        result = Long.reverse(result);
		return result;
	}
	
	public static int decodeUnsignedByte(byte[] singleByteBuffer) {
		byte[] buf = singleByteBuffer;
		int firstByte = (0x000000FF & ((int) buf[0]));
        int result  = (char) firstByte;
		return result;
	}
	
	public static int decodeUnsignedShort(byte[] doubleByteBuffer) {
		byte[] buf = doubleByteBuffer;
		int firstByte = (0xFF & ((int)buf[1]));
        int secondByte = (0xFF & ((int)buf[0]));
        int result  = (char) (firstByte << 8 | secondByte);
		return result;
	}
	
	public static long decodeUnsignedInt(byte[] quadByteBuffer) {
		byte[] buf = quadByteBuffer;
		long firstByte = (0x000000FF & ((int)buf[3]));
        long secondByte = (0x000000FF & ((int)buf[2]));
        long thirdByte = (0x000000FF & ((int)buf[1]));
        long fourthByte = (0x000000FF & ((int)buf[0]));
        long result  = (firstByte << 24 | secondByte << 16 | thirdByte << 8 | fourthByte);
		return result & 0xFFFFFFFFFFFFFFFFL;
	}
	
	// http://tjmljw.iteye.com/blog/1767716
	// http://bbs.csdn.net/topics/60150732
	public static float decodeFloat(byte[] buffer) {
		int i = (buffer[3] & 0xFF) << 24 
				| (buffer[2] & 0xFF) << 16 
				| (buffer[1] & 0xFF) << 8 
				| (buffer[0] & 0xFF);
		return Float.intBitsToFloat(i);
	}
	
	public static double decodeUnsignedDouble(byte[] buffer) {
		long bits;
		bits = buffer[0] & 0xFF;
		bits &= 0xFF;
		bits |= ((long) buffer[1] << 8);
		bits &= 0xFFFF;
		bits |= ((long) buffer[2] << 16);
		bits &= 0xFFFFFF;
		bits |= ((long) buffer[3] << 24);
		bits &= 0xFFFFFFFFL;
		bits |= ((long) buffer[4] << 32);
		bits &= 0xFFFFFFFFFFL;
		bits |= ((long) buffer[5] << 40);
		bits &= 0xFFFFFFFFFFFFL;
		bits |= ((long) buffer[6] << 48);
		bits &= 0xFFFFFFFFFFFFFFL;
		bits |= ((long) buffer[7] << 56);
		return Double.longBitsToDouble(bits);
	}
	
	public static String decodeString(byte[] buf) {
		return new String(buf);
	}
	
	public static byte[] hexStringToBytes(String hexString) {
		if (hexString == null || hexString.equals("")) {
			return null;
		}
		hexString = hexString.toUpperCase();
		int length = hexString.length() / 2;
		char[] hexChars = hexString.toCharArray();
		byte[] d = new byte[length];
		for (int i = 0; i < length; i++) {
			int pos = i * 2;
			d[i] = (byte) (hexCharToByte(hexChars[pos]) << 4 | hexCharToByte(hexChars[pos + 1]));
		}
		return d;
	}
	
	private static byte hexCharToByte(char c) {
		return (byte) "0123456789ABCDEF".indexOf(c);
	}

	public static byte[] leftCompact(byte[] buf) {
		// optimize
		if(buf[0] != 0 && buf[buf.length-1] != 0) {
			return buf;
		}
		int i = 0;
		for (; i<buf.length; i++) {
			if(buf[i] != 0) {
				break;
			}
		}
		int len = buf.length - i;
		byte[] replica = new byte[len]; 
		System.arraycopy(buf, i, replica, 0, len);
		return replica;
	}
	
	public static byte[] rightCompact(byte[] buf) {
		// optimize
		if(buf[0] != 0 && buf[buf.length-1] != 0) {
			return buf;
		}
		int j = buf.length - 1;
		for (; j >= 0 ; j--) {
			if(buf[j] != 0) {
				break;
			}
		}
		int len = j + 1;
		byte[] replica = new byte[len]; 
		System.arraycopy(buf, 0, replica, 0, len);
		return replica;
	}
	
	public static byte[] compact(byte[] buf) {
		// optimize
		if(buf[0] != 0 && buf[buf.length-1] != 0) {
			return buf;
		}
		// left
		int i = 0;
		for (; i<buf.length; i++) {
			if(buf[i] != 0) {
				break;
			}
		}
		// right
		int j = buf.length - 1;
		for (; j >= 0 ; j--) {
			if(buf[j] != 0) {
				break;
			}
		}
		int len = j - i + 1;
		byte[] replica = new byte[len]; 
		System.arraycopy(buf, i, replica, 0, len);
		return replica;
	}
	
	public static void reverse(byte[] buf) {
		int len = buf.length / 2;
		for (int i = 0; i < len; i++) {
			byte tmp = buf[i];
			buf[i] = buf[buf.length - i - 1];
			buf[buf.length - i - 1] = tmp;
		}
	}
	
	public static String decodeTime(byte[] quadByteBuffer) {
		byte[] buf = quadByteBuffer;
		long firstByte = (0x000000FF & ((int)buf[3]));
        long secondByte = (0x000000FF & ((int)buf[2]));
        long thirdByte = (0x000000FF & ((int)buf[1]));
        long fourthByte = (0x000000FF & ((int)buf[0]));
        long result  = (firstByte << 24 | secondByte << 16 | thirdByte << 8 | fourthByte);
		result = result & 0xFFFFFFFFFFFFFFFFL;
		
		long n = result;
		long hour = (n >> 22) & 0x1F;
		long minute = (n >> 16) & 0x3F;
		long second = (n >> 10) & 0x3F;
		long msecond = (n & 0x03FF);
		
		StringBuffer time = new StringBuffer();
		time
			.append(hour).append(":")
			.append(minute).append(":")
			.append(second).append(".")
			.append(msecond);
		return getStringTime(time.toString(), "HH:mm:ss.SSS");
	}
	
	private static String getStringTime(String time, String format) {
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
		byte[] b = hexStringToBytes("E001");
		System.out.println((int) decodeShort(b));
		
		b = hexStringToBytes("3000B182");
		System.out.println(decodeTime(b));
	}

}
