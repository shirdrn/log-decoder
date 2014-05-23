package org.shirdrn.log.decoder.utils;

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;

public class FixLengthBufferCache {

	private static final Map<Integer, byte[]> cachedBuffers = 
			new HashMap<Integer, byte[]>(0);
	private static final int maxCapacity = 8;
			
	public static byte[] allocate(int capacity) {
		// reuse allocated byte buffer
		byte[] buf = cachedBuffers.get(capacity);
		if(buf == null) {
			ByteBuffer b = ByteBuffer.allocate(capacity);
			buf = b.array();
			// just cache 8 byte buffers
			if(capacity <= maxCapacity) {
				cachedBuffers.put(capacity, buf);
			}
		}
		return buf;
	}
}
