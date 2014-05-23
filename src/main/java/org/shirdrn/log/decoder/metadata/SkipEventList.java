package org.shirdrn.log.decoder.metadata;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashSet;
import java.util.Set;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class SkipEventList {

	private static final Log LOG = LogFactory.getLog(SkipEventList.class);
	private static final String SKIP_EVENT_LIST_FILE = "skip_event_list";
	private static final Set<Integer> blackList = new HashSet<Integer>();
	private static final SkipEventList INSTANCE = new SkipEventList();

	private SkipEventList() {
		LOG.info("Load skip event list configuration...");
		InputStream in = null;
		BufferedReader reader = null;
		try {
			in = this.getClass().getClassLoader().getResourceAsStream(SKIP_EVENT_LIST_FILE);
			if(in != null) {
				reader = new BufferedReader(new InputStreamReader(in));
				String line = null;
				while((line = reader.readLine()) != null) {
					line = line.trim();
					if(!line.isEmpty() && !line.startsWith("#")) {
						if(line.indexOf("-") != -1) {
							String[] a = line.split("\\s*\\-\\s*");
							int start = Integer.parseInt(a[0]);
							int end = Integer.parseInt(a[1]);
							for(int i=start; i<=end; i++) {
								try {
									blackList.add(i);
								} catch (Exception e) { }
							}
						} else {
							try {
								blackList.add(Integer.parseInt(line));
							} catch (Exception e) { }
						}
					}
				}
			} else {
				LOG.warn("Can not find file: " + SKIP_EVENT_LIST_FILE);
			}
		} catch (IOException e) {
			LOG.error("Fail to read file " + SKIP_EVENT_LIST_FILE, e);
		} finally {
			if(reader != null) {
				try {
					reader.close();
				} catch (IOException e) {
					LOG.error(e);
				}
			}
		}
		LOG.debug("Skip eVent list: " + blackList);		
	}
	
	public boolean isInSkipList(int id) {
		return blackList.contains(id);
	}
	
	public static SkipEventList newInstance() {
		return INSTANCE;
	}
}
