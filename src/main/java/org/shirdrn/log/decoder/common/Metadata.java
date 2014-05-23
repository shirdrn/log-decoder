package org.shirdrn.log.decoder.common;

import java.util.Map.Entry;

public interface Metadata<K, V> extends Iterable<Entry<K, V>>{

	V get(K key);
	
}
