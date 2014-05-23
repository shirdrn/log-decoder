package org.shirdrn.log.decoder.common.types;

import org.shirdrn.log.decoder.common.handler.NumberValueHandler;
import org.shirdrn.log.decoder.utils.ByteUtils;

public final class Int16 extends NumberValueHandler<Short> {
	
	public Int16(String type) {
		super(type, 2);
	}
	
	@Override
	public Short handle(byte[] buffer) {
		return ByteUtils.decodeShort(buffer);
	}

	@Override
	public Short valueOf(String value) {
		return this.value = Short.parseShort(value);
	}
}