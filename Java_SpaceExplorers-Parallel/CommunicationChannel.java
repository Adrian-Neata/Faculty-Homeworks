import java.util.*;
import java.util.concurrent.*;

/**
 * Class that implements the channel used by headquarters and space explorers to communicate.
 */
public class CommunicationChannel {

	private ConcurrentHashMap<Long, Integer> threadID_parent;
	private Set<String> frequenciesToBeDecoded;
	private Set<String> decodedFrequencies;
	private Queue<Message> messagesToSpaceExplorers;
	private Queue<Message> messagesToHeadQuarters;
	private Boolean exit = false;
	/**
	 * Creates a {@code CommunicationChannel} object.
	 */
	public CommunicationChannel() {
		threadID_parent = new ConcurrentHashMap<Long, Integer>();
		frequenciesToBeDecoded = Collections.newSetFromMap(new ConcurrentHashMap<String, Boolean>());
		decodedFrequencies = Collections.newSetFromMap(new ConcurrentHashMap<String, Boolean>());
		messagesToSpaceExplorers = new ConcurrentLinkedQueue<Message>();
		messagesToHeadQuarters = new ConcurrentLinkedQueue<Message>();
	}

	/**
	 * Puts a message on the space explorer channel (i.e., where space explorers write to and 
	 * headquarters read from).
	 * 
	 * @param message
	 *            message to be put on the channel
	 */
	public void putMessageSpaceExplorerChannel(Message message) {
		messagesToHeadQuarters.add(message);
	}

	/**
	 * Gets a message from the space explorer channel (i.e., where space explorers write to and
	 * headquarters read from).
	 * 
	 * @return message from the space explorer channel
	 */
	public Message getMessageSpaceExplorerChannel() {
		Message msg = messagesToHeadQuarters.poll();
		return msg;
	}

	/**
	 * Puts a message on the headquarters channel (i.e., where headquarters write to and 
	 * space explorers read from).
	 * 
	 * @param message
	 *            message to be put on the channel
	 */
	public void putMessageHeadQuarterChannel(Message message) {
		String data = message.getData();
		if(data.equals("EXIT") == true) {
			exit = true;
		} else{
			if(data.equals("NO_PARENT") == true){
				threadID_parent.put(Thread.currentThread().getId(), -1);
			}
			if(data.equals("NO_PARENT") == false) {
				if(data.equals("END") == false) {
					if(decodedFrequencies.contains(data) == true) {
						threadID_parent.put(Thread.currentThread().getId(), message.getCurrentSolarSystem());	
					}
					if(decodedFrequencies.contains(data) == false && frequenciesToBeDecoded.contains(data) == false) {
						frequenciesToBeDecoded.add(data);
						message.setParentSolarSystem(threadID_parent.get(Thread.currentThread().getId()));
						messagesToSpaceExplorers.add(message);
					}
				}
			}
		}
	}

	/**
	 * Gets a message from the headquarters channel (i.e., where headquarters write to and
	 * space explorer read from).
	 * 
	 * @return message from the header quarter channel
	 */
	public Message getMessageHeadQuarterChannel() {
		Message msg = null;
		if(exit) {
			msg = new Message(-1, "EXIT");
		} else{
			if(frequenciesToBeDecoded.isEmpty() == false) {
				msg = messagesToSpaceExplorers.poll();
				if(msg != null) {
					decodedFrequencies.add(msg.getData());
					frequenciesToBeDecoded.remove(msg.getData());
					messagesToSpaceExplorers.remove(msg);
				}
			}
		}
		return msg;
	}
}
