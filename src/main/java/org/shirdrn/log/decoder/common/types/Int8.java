package org.shirdrn.log.decoder.common.types;

import org.shirdrn.log.decoder.common.handler.NumberValueHandler;
import org.shirdrn.log.decoder.utils.ByteUtils;

public final class Int8 extends NumberValueHandler<Byte> {
	
	public Int8(String type) {
		super(type, 1);
	}
	
	@Override
	public Byte handle(byte[] buffer) {
		return value = ByteUtils.decodeByte(buffer);
	}

	@Override
	public Byte valueOf(String value) {
		return Byte.parseByte(value);
	}
	
}