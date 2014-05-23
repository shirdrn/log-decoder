package org.shirdrn.log.decoder.metadata;

import java.util.Map;

import org.shirdrn.log.decoder.common.Metadata;
import org.shirdrn.log.decoder.common.TypeParser;

public abstract class AbstractMetadata<K, V> implements Metadata<K, V> {

	protected Map<K, V> container;
	protected TypeParser<V> typeParser;
	
	public AbstractMetadata(String file) {
		initialize();
		load(file);
	}

	protected abstract void initialize();
	protected abstract void load(String file);
}
