import java.util.ArrayList;

/**
 *
 * @author Adrian
 */
public class LFUCache extends Memory implements ICache {

    private final int memorySize;
    private int usedMemory = 0;
    private final ArrayList<Integer> timestamp;

    /**
     *
     * @param memorySize size of the cache memory
     */
    public LFUCache(int memorySize) {
        this.memorySize = memorySize;
        this.timestamp = new ArrayList<>();
    }

    /**
     *
     * @param sub the object which must be added to the memory
     */
    @Override
    public void add(Premium sub) {
        if (usedMemory >= memorySize) {
            remove(-1);
        }
        memory.add(sub);
        timestamp.add(0);
        usedMemory++;
    }

    /**
     *
     * @param lfu the position of the object which must be removed from memory
     */
    @Override
    public void remove(int lfu) {
        if (lfu == -1) {
            int counter = 0;
            lfu = 0;
            for (Integer uses : timestamp) {
                if (uses < timestamp.get(lfu)) {
                    lfu = counter;
                }
                counter++;
            }
        }
        memory.remove(lfu);
        timestamp.remove(lfu);
        usedMemory--;
    }

    /**
     *
     * @param index the position of the object which was just used and its
     * timestamp
     */
    public void incStamp(int index) {
        timestamp.set(index, timestamp.get(index) + 1);
    }
}
