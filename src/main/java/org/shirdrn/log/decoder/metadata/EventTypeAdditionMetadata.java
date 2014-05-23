package org.shirdrn.log.decoder.metadata;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.shirdrn.log.decoder.common.AdditionType;
import org.shirdrn.log.decoder.common.EventType;
import org.shirdrn.log.decoder.common.EventTypeId;
import org.shirdrn.log.decoder.common.Metadata;

public class EventTypeAdditionMetadata implements Metadata<Integer, AdditionType> {

	private static final Log LOG = LogFactory.getLog(EventTypeAdditionMetadata.class);
	// <eventId, additionType>
	private final Map<Integer, AdditionType> container;
	
	public EventTypeAdditionMetadata(Metadata<EventTypeId, EventType> eventTypeFactory) {
		container = new HashMap<Integer, AdditionType>();
		Iterator<Entry<EventTypeId, EventType>> iter = eventTypeFactory.iterator();
		while(iter.hasNext()) {
			Entry<EventTypeId, EventType> entry = iter.next();
			if(entry.getValue().isAdditionExisted()) {
				int eventId = entry.getKey().getEventId();
				// keep pair <eventId, additionType>
				if(!container.containsKey(eventId)) {
					AdditionType at = new AdditionType();
					at.setEventId(eventId);
					at.setType(entry.getKey().getAddition().getType());
					container.put(eventId, at);
				}
			}
		}
		LOG.debug("Addition type metadata: " + container);
	}
	
	@Override
	public Iterator<Entry<Integer, AdditionType>> iterator() {
		return container.entrySet().iterator();
	}

	@Override
	public AdditionType get(Integer key) {
		return container.get(key);
	}

}
