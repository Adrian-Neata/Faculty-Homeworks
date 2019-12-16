/**
 *
 * @author Adrian
 */
public class LRUCache extends Memory implements ICache {

    private final int memorySize;
    private int usedMemory = 0;

    /**
     *
     * @param memorySize size of the cache memory
     */
    public LRUCache(int memorySize) {
        this.memorySize = memorySize;
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
        usedMemory++;
    }

    /**
     *
     * @param index the position of the object which must be removed from memory
     */
    @Override
    public void remove(int index) {
        if (index == -1) {
            index = 0;
        }
        memory.remove(index);
        usedMemory--;
    }

    /**
     *
     * @param name the name of the object which must be moved last in the cache
     * memory
     */
    public void reorder(String name) {
        int searchResultLru = searchMemory(name);
        memory.add(getElement(searchResultLru));
        memory.remove(searchResultLru);
    }

}
