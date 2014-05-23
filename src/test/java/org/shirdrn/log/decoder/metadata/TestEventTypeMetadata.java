package org.shirdrn.log.decoder.metadata;

import java.util.Iterator;
import java.util.Map.Entry;

import org.junit.Test;
import org.shirdrn.log.decoder.common.EventType;
import org.shirdrn.log.decoder.common.EventTypeId;
import org.shirdrn.log.decoder.common.Metadata;
import org.shirdrn.log.decoder.metadata.EventTypeMetadata;

public class TestEventTypeMetadata {

	private final Metadata<EventTypeId, EventType> metadata = 
			EventTypeMetadata.newInstance();
	
	@Test
	public void iterate() {
		Iterator<Entry<EventTypeId, EventType>> iter = metadata.iterator();
		while(iter.hasNext()) {
			Entry<EventTypeId, EventType> entry = iter.next();
			System.out.println("=> " + entry.getKey() + "\t" + entry.getValue());
			EventType p = entry.getValue().getNext();
			if(p != null) {
				System.out.println("=> " + entry.getKey() + "\t" + p);
				 p = p.getNext();
			}
		}
	}
	
}
