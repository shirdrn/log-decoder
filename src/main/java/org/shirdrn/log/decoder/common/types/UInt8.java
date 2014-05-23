package org.shirdrn.log.decoder.common.types;

import org.shirdrn.log.decoder.common.handler.NumberValueHandler;
import org.shirdrn.log.decoder.utils.ByteUtils;

public final class UInt8 extends NumberValueHandler<Integer> {
	
	public UInt8(String type) {
		super(type, 1);
	}
	
	@Override
	public Integer handle(byte[] buffer) {
		return value = ByteUtils.decodeUnsignedByte(buffer);
	}

	@Override
	public Integer valueOf(String value) {
		return Integer.parseInt(value);
	}
}