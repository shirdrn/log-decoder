package org.shirdrn.log.decoder.metadata;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.Map.Entry;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.shirdrn.log.decoder.common.Addition;
import org.shirdrn.log.decoder.common.ArrayType;
import org.shirdrn.log.decoder.common.EventType;
import org.shirdrn.log.decoder.common.EventTypeId;
import org.shirdrn.log.decoder.common.Metadata;
import org.shirdrn.log.decoder.common.FieldType;
import org.shirdrn.log.decoder.common.ParamType;
import org.shirdrn.log.decoder.common.ParamType.FieldArray;
import org.shirdrn.log.decoder.common.ParamType.FieldPointer;
import org.shirdrn.log.decoder.common.TypeParser;
import org.shirdrn.log.decoder.common.handler.ByteValueHandler;
import org.shirdrn.log.decoder.utils.ParserUtils;

public final class EventTypeMetadata extends AbstractMetadata<EventTypeId, EventType> {

	private static final Log LOG = LogFactory.getLog(EventTypeMetadata.class);
	private static final String EVENT_TYPE_FILE = "event_types";
	private static final Metadata<EventTypeId, EventType> INSTANCE = new EventTypeMetadata();
	
	private Metadata<String, ByteValueHandler<?>> primitiveTypeMetadata;
	private Metadata<Integer, ParamType> paramTypeMetadata;
	
	private Pattern ADDITION_PATTERN;
	
	private EventTypeMetadata() {
		super(EVENT_TYPE_FILE);
	}
	
	public static Metadata<EventTypeId, EventType> newInstance() {
		return INSTANCE;
	}
	
	@Override
	protected void initialize() {
		this.container = new LinkedHashMap<EventTypeId, EventType>();	
		typeParser = new EventTypeParser();
		primitiveTypeMetadata = PrimitiveTypeMetadata.newInstance();
		paramTypeMetadata = ParamTypeMetadata.newInstance();
		ADDITION_PATTERN = Pattern.compile("([^\\(]+)\\(([^:]+):([^\\)]+)\\)");
	}
	
	@Override
	protected void load(String file) {
		LOG.info("Load event type configuration, read file...");
		InputStream in = null;
		BufferedReader reader = null;
		try {
			in = this.getClass().getClassLoader().getResourceAsStream(file);
			reader = new BufferedReader(new InputStreamReader(in));
			String line = null;
			while((line = reader.readLine()) != null) {
				line = line.trim();
				if(line.length() > 0 && !line.startsWith("#")) {
					try {
						parseLine(line);
					} catch (Exception e) {
						LOG.warn("Error line: " + line, e);
					}
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		LOG.debug("EVent type metadata: " + container);		
	}
	
	private void parseLine(String line) throws Exception {
		String[] a = line.split("\\s*,\\s*");
		if(ParserUtils.isValid(a, 4)) {
			int eventId = Integer.parseInt(a[0]);
			String eventName = a[1];
			boolean additionExisted = parseAdditionExisated(eventName);
			int paramCount = Integer.parseInt(a[2]);
			if(a.length == paramCount + 3) {
				Addition addition = null;
				if(additionExisted) {
					// get additions from parsed addition information
					AddiObj addiObj = parseAddition(eventName);
					eventName = addiObj.name;
					addition = new Addition();
					addition.setEventId(eventId);
					addition.setType(addiObj.type);
					addition.setValue(addiObj.value);
				}
				collect(addition, a, eventId, eventName, paramCount);
			} else {
				LOG.warn("Bad line format: line = " + line);
			}
		} else {
			LOG.warn("Bad line format: line = " + line);
		}
	}
	
	private void collect(Addition addition, String[] a, int eventId, 
			String eventName, int paramCount) {
		EventType eventType = container.get(eventId);
		EventType et = typeParser.parse(a, eventId, eventName, 3, paramCount);
		EventTypeId typeId = new EventTypeId();
		if(eventType == null) {
			eventType = et;
		}
		if(addition == null) {
			eventType.setAdditionExisted(false);
		} else {
			eventType.setAdditionExisted(true);
		}
		typeId.setEventId(eventId);
		typeId.setAddition(addition);
		container.put(typeId, eventType);
		eventType.setAddition(addition);
	}

	private AddiObj parseAddition(String eventName) {
		AddiObj addiObj = null;
		Matcher m = ADDITION_PATTERN.matcher(eventName);
		if(m.find()) {
			String name = m.group(1);
			String type = m.group(2);
			String value = m.group(3);
			addiObj = new AddiObj();
			addiObj.name = name;
			addiObj.type = type;
			addiObj.value = value;
		}
		return addiObj;
	}
	
	class AddiObj {
		String name;
		String type;
		String value;
	}

	private boolean parseAdditionExisated(String eventName) {
		boolean additionExisated = false;
		String f = eventName;
		if(f.indexOf(":") == -1) {
			additionExisated = false;
		} else {
			additionExisated = true;
		}
		return additionExisated;
	}
	
	@Override
	public EventType get(EventTypeId key) {
		return container.get(key);
	}
	
	class EventTypeParser implements TypeParser<EventType> {

		private final Pattern REF_TYPE_PTN = Pattern.compile("([^\\(]+)\\(@(\\d+)\\[(\\d+)\\]\\)");
		
		@Override
		public EventType parse(String[] a, int id, String name, int fromIndex, int paramCount) {
			EventType eventType = new EventType(id, name.toUpperCase());
			eventType.setParamCount(paramCount);
			for (int i = fromIndex; i < a.length; i++) {
				String type = a[i].toLowerCase();
				if(type.indexOf('[') != -1) {
					// 43,$CELLMEAS(uint8:5),4,uint8,uint8,uint8(@2[1]),uint8(@1[1])
					if(type.indexOf('@') != -1) {
						Matcher m = REF_TYPE_PTN.matcher(type);
						if(m.find()) {
							String t = m.group(1);
							ByteValueHandler<?> handler = primitiveTypeMetadata.get(t);
							int refTypeId = Integer.parseInt(m.group(2));
							int len = Integer.parseInt(m.group(3));
							ParamType pt = paramTypeMetadata.get(refTypeId);
							FieldPointer fp = new FieldPointer();
							fp.setParamType(pt);
							fp.setHandler(handler);
							fp.setLength(len);
							eventType.addField(t, handler.byteCount(), FieldType.POINTER, fp);
						}
					} else {
						ArrayType at = ArrayType.parseArrayType(type);
						paramCount += (at.getLength() - 1);
						eventType.setParamCount(paramCount);
						ByteValueHandler<?> handler = primitiveTypeMetadata.get(at.getType());
						FieldArray fa = new FieldArray();
						fa.setHandler(handler);
						fa.setLength(at.getLength());
						eventType.addField(at.getType(), handler.byteCount(), FieldType.ARRAY, fa);
					}
				} else {
					ByteValueHandler<?> handler = primitiveTypeMetadata.get(type);
					eventType.addField(type, handler.byteCount());
				}
			}
			return eventType;
		}
		
	}

	@Override
	public Iterator<Entry<EventTypeId, EventType>> iterator() {
		return container.entrySet().iterator();
	}

}
