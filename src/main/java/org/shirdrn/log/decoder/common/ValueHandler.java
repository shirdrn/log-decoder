package org.shirdrn.log.decoder.common;


public interface ValueHandler<S, D> {

	String getType();
	D handle(S buffer);
	
	D valueOf();
	D valueOf(String value);
}
