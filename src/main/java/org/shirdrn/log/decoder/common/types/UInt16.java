package org.shirdrn.log.decoder.common.types;

import org.shirdrn.log.decoder.common.handler.NumberValueHandler;
import org.shirdrn.log.decoder.utils.ByteUtils;

public final class UInt16 extends NumberValueHandler<Integer> {
	
	public UInt16(String type) {
		super(type, 2);
	}
	
	@Override
	public Integer handle(byte[] buffer) {
		return value = ByteUtils.decodeUnsignedShort(buffer);
	}

	@Override
	public Integer valueOf(String value) {
		return Integer.parseInt(value);
	}
}