/**
 *
 * @author Adrian
 */
public class FIFOCache extends Memory implements ICache {

    private final int memorySize;
    private int usedMemory = 0;

    /**
     *
     * @param memorySize size of the cache memory
     */
    public FIFOCache(int memorySize) {
        this.memorySize = memorySize;
    }

    /**
     *
     * @param sub the object which must be added to the memory
     */
    @Override
    public void add(Premium sub) {
        if (usedMemory >= memorySize) {
            remove(0);
        }
        memory.add(sub);
        usedMemory++;
    }

    /**
     *
     * @param index the position of the object which must be removed from memory
     */
    @Override
    public void remove(int index) {
        usedMemory--;
        memory.remove(index);
    }

}
