package org.shirdrn.log.decoder.utils;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class TestByteUtils {

	@Test
	public void getFromDoubleBytes() {
		byte[] bs = ByteUtils.hexStringToBytes("4C00");
		assertEquals(76, ByteUtils.decodeUnsignedShort(bs));
		
		bs = ByteUtils.hexStringToBytes("00F1");
		assertEquals(61696, ByteUtils.decodeUnsignedShort(bs));
		
		bs = ByteUtils.hexStringToBytes("2211");
		assertEquals(4386, ByteUtils.decodeUnsignedShort(bs));
	}
	
	@Test
	public void getFromQuadBytes() {
		byte[] bs = ByteUtils.hexStringToBytes("4C000000");
		assertEquals(76, ByteUtils.decodeUnsignedInt(bs));
		
		// 10*pow(16,5)+13*pow(16,4)+pow(16,2)+4*pow(16,1)+12
		bs = ByteUtils.hexStringToBytes("4C01AD00");
		assertEquals(11338060L, ByteUtils.decodeUnsignedInt(bs));
	}
}
