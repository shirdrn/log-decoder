package org.shirdrn.log.decoder.common.handler;


public abstract class AbstractValueHandler<D> implements ByteValueHandler<D> {

	protected String type;
	protected int byteCount;
	protected D value;
	
	public AbstractValueHandler(String type, int byteCount) {
		this.type = type;
		this.byteCount = byteCount;
	}
	
	@Override
	public String getType() {
		return type;
	}
	
	@Override
	public int byteCount() {
		return byteCount;
	}
	
	@Override
	public D valueOf() {
		return value;
	}
}
