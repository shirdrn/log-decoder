package org.shirdrn.log.decoder.common.handler;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.shirdrn.log.decoder.EventDataPacket.ParamValue;

public abstract class NumberValueHandler<D> extends AbstractValueHandler<D> {

	private static final Log LOG = LogFactory.getLog(NumberValueHandler.class);
	
	public NumberValueHandler(String type, int byteCount) {
		super(type, byteCount);
	}

	@Override
	public ParamValue chooseBlock(int start, byte[] fileByteBuffer) {
		int end = start + byteCount;
		LOG.debug("Param data pos: type = " + type + ", start = " + start + ", end = " + end);
		ParamValue value = new ParamValue(type, start, end);
		value.setTotalByteCount(end - start);
		value.setExistsTerminateChar(false);
		return value;
	}

}
