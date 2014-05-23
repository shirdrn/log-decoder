package org.shirdrn.log.decoder.common.types;

import org.shirdrn.log.decoder.common.handler.NumberValueHandler;
import org.shirdrn.log.decoder.utils.ByteUtils;

public final class Float4 extends NumberValueHandler<Float> {
	
	public Float4(String type) {
		super(type, 4);
	}
	
	@Override
	public Float handle(byte[] buffer) {
		return value = ByteUtils.decodeFloat(buffer);
	}

	@Override
	public Float valueOf(String value) {
		return Float.parseFloat(value);
	}
}