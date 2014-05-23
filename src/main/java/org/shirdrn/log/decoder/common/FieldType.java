package org.shirdrn.log.decoder.common;

public enum FieldType {

	NORMAL(0),
	ARRAY(1),
	POINTER(2);
	
	private final int code;
    private static final int FIRST_CODE = values()[0].code;
	
	FieldType(int code) {
		this.code = code;
	}
	
	public int getCode() {
		return this.code;
	}
	
	public static FieldType valueOf(int code) {
        final int current = (code & 0xff) - FIRST_CODE;
        return current < 0 || current >= values().length ? null : values()[current];
	}
}
