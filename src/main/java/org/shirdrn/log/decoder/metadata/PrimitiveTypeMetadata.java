package org.shirdrn.log.decoder.metadata;

import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.Map.Entry;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.shirdrn.log.decoder.common.Metadata;
import org.shirdrn.log.decoder.common.handler.ByteValueHandler;
import org.shirdrn.log.decoder.common.types.Double8;
import org.shirdrn.log.decoder.common.types.Float4;
import org.shirdrn.log.decoder.common.types.Int16;
import org.shirdrn.log.decoder.common.types.Int32;
import org.shirdrn.log.decoder.common.types.Int8;
import org.shirdrn.log.decoder.common.types.String1;
import org.shirdrn.log.decoder.common.types.String128;
import org.shirdrn.log.decoder.common.types.String16;
import org.shirdrn.log.decoder.common.types.String24;
import org.shirdrn.log.decoder.common.types.String32;
import org.shirdrn.log.decoder.common.types.String48;
import org.shirdrn.log.decoder.common.types.String64;
import org.shirdrn.log.decoder.common.types.String8;
import org.shirdrn.log.decoder.common.types.UInt16;
import org.shirdrn.log.decoder.common.types.UInt32;
import org.shirdrn.log.decoder.common.types.UInt8;
import org.shirdrn.log.decoder.constants.Type;

public final class PrimitiveTypeMetadata implements Metadata<String, ByteValueHandler<?>> {

	private static final Log LOG = LogFactory.getLog(PrimitiveTypeMetadata.class);
	private static final Metadata<String, ByteValueHandler<?>> INSTANCE = new PrimitiveTypeMetadata();
	private static final LinkedHashMap<String, ByteValueHandler<?>> valueHandlers = 
			new LinkedHashMap<String, ByteValueHandler<?>>();
	static {
		valueHandlers.put(Type.STRING_1, new String1(Type.STRING_1));
		valueHandlers.put(Type.STRING_8, new String8(Type.STRING_8));
		valueHandlers.put(Type.STRING_16, new String16(Type.STRING_16));
		valueHandlers.put(Type.STRING_24, new String24(Type.STRING_24));
		valueHandlers.put(Type.STRING_32, new String32(Type.STRING_32));
		valueHandlers.put(Type.STRING_48, new String48(Type.STRING_48));
		valueHandlers.put(Type.STRING_64, new String64(Type.STRING_64));
		valueHandlers.put(Type.STRING_128, new String128(Type.STRING_128));
		
		valueHandlers.put(Type.INT_8, new Int8(Type.INT_8));
		valueHandlers.put(Type.INT_16, new Int16(Type.INT_16));
		valueHandlers.put(Type.INT_32, new Int32(Type.INT_32));
		valueHandlers.put(Type.UINT_8, new UInt8(Type.UINT_8));
		valueHandlers.put(Type.UINT_16, new UInt16(Type.UINT_16));
		valueHandlers.put(Type.UINT_32, new UInt32(Type.UINT_32));
		
		valueHandlers.put(Type.FLOAT_4, new Float4(Type.FLOAT_4));
		valueHandlers.put(Type.DOUBLE_8, new Double8(Type.DOUBLE_8));
		
		logDataTypes();
	}
	
	private PrimitiveTypeMetadata() {
	}
	
	private static void logDataTypes() {
		LOG.info("Load event type information: ");
		Iterator<Entry<String, ByteValueHandler<?>>> iter = valueHandlers.entrySet().iterator();
		LOG.info("[ TYPE ]\t:\t[ LENGTH ]" );
		while(iter.hasNext()) {
			Entry<String, ByteValueHandler<?>> entry = iter.next();
			LOG.info(entry.getKey() + "\t:\t" + entry.getValue().byteCount());
		}
	}
	
	public static Metadata<String, ByteValueHandler<?>> newInstance() {
		return INSTANCE;
	}
	
	@Override
	public ByteValueHandler<?> get(String key) {
		return valueHandlers.get(key);
	}

	@Override
	public Iterator<Entry<String, ByteValueHandler<?>>> iterator() {
		return valueHandlers.entrySet().iterator();
	}

}
