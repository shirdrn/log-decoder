package org.shirdrn.log.decoder;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Date;
import java.util.Iterator;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.shirdrn.log.decoder.common.Context;
import org.shirdrn.log.decoder.common.EventPacket;
import org.shirdrn.log.decoder.common.LogDecoder;
import org.shirdrn.log.decoder.common.PacketFormatter;

public class LogDecoderImpl implements LogDecoder {

	private static final Log LOG = LogFactory.getLog(LogDecoderImpl.class);
	private String version;
	private final File logFile;
	private BytedLogFile bytedLogFile;
	private final Context context;
	private final PacketFormatter<String> formatter;
	
	public LogDecoderImpl(File logFile) {
		this.logFile = logFile;
		context = new Context();
		this.formatter = new EventDataPacketFormatter(context);
	}
	
	@Override
	public void setVersion(String version) throws Exception {
		if(version == null || version.trim().isEmpty()) {
			throw new IllegalArgumentException("version = " + version);
		}
		this.version = version;
	}

	@Override
	public void decode() {
		bytedLogFile = new BytedLogFile(context);
		Iterator<EventPacket> iter = bytedLogFile.iterator();
		while(iter.hasNext()) {
			EventPacket packet = iter.next();
			if(packet == null 
					|| packet.getEventId() == - Integer.MAX_VALUE) {
				continue;
			}
			String value = formatter.format(packet);
			LOG.info("Packet detail: " + 
					"value = " + value);
		}
	}

	@Override
	public void loadFile() {
		DataInputStream in = null;
		try {
			in = new DataInputStream(new FileInputStream(logFile));
			int readBytes = in.available();
			byte[] fileByteBuf = new byte[readBytes];
			in.read(fileByteBuf);
			LOG.info("Log file size: totalBytes = " + fileByteBuf.length);
			context.setFileByteBuf(fileByteBuf);
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			 try {
				in.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
	}
	
	@Override
	public String toString() {
		StringBuffer sb = new StringBuffer();
		sb.append("[")
		.append("version = ").append(version)
		.append("]");
		return sb.toString();
	}
	
	public static void main(String[] args) {
		Date d = new Date();
		System.out.println(d.getTime());
	}

}
