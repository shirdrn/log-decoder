package org.shirdrn.log.decoder;

import java.util.Iterator;

import org.shirdrn.log.decoder.common.Context;
import org.shirdrn.log.decoder.common.EventPacket;

public class BytedLogFile implements Iterable<EventPacket> {

	private final byte[] fileByteBuf;
	private final Iterator<EventPacket> iterator;
	private final Context context;
	
	public BytedLogFile(Context context) {
		this.context = context;
		this.fileByteBuf = this.context.getFileByteBuf();
		this.iterator = new BytedLogFileIterator(this);
	}
	
	@Override
	public Iterator<EventPacket> iterator() {
		return this.iterator;
	}

	public byte[] getFileByteBuf() {
		return fileByteBuf;
	}

	public Context getContext() {
		return context;
	}

}
