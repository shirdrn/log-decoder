package org.shirdrn.log.decoder.metadata;

import java.util.Iterator;
import java.util.Map.Entry;

import org.junit.Test;
import org.shirdrn.log.decoder.common.Metadata;
import org.shirdrn.log.decoder.common.ParamType;
import org.shirdrn.log.decoder.metadata.ParamTypeMetadata;

public class TestPrimitiveTypeMetadata {

	private final Metadata<Integer, ParamType> metadata = 
			ParamTypeMetadata.newInstance();
	
	@Test
	public void iterate() {
		Iterator<Entry<Integer, ParamType>> iter = metadata.iterator();
		while(iter.hasNext()) {
			Entry<Integer, ParamType> entry = iter.next();
			System.out.println("=> " + entry.getKey() + "\t" + entry.getValue());
		}
	}
	
}
