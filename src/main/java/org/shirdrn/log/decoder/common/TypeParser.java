package org.shirdrn.log.decoder.common;

public interface TypeParser<T> {

	T parse(String[] a , int id, 
			String name, int fromIndex, int paramCount);
}
