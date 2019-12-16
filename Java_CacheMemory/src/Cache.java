/**
 *
 * @author Adrian
 */
public class Cache {
    
    int cacheSize;
    FIFOCache fifo;
    LRUCache lru;
    LFUCache lfu;
    Memory memory;
    
    /**
     *
     * @param cacheSize size of the cache memory
     */
    public Cache(int cacheSize) {
        this.cacheSize = cacheSize;
        fifo = new FIFOCache(cacheSize);
        lru = new LRUCache(cacheSize);
        lfu = new LFUCache(cacheSize);
        memory = new Memory();
    }
    
    /**
     *
     * @param tokens the parameters of the command from the input file
     * @param cacheType "FIFO", "LRU" or "LFU" depending on which type of cache memory is being used
     */
    public void add(String[] tokens, String cacheType) {
        int searchResultMemory = memory.searchMemory(tokens[1]);
        Premium sub = new Premium();
        sub.setName(tokens[1]);
        sub.setNrBasicSub(Integer.parseInt(tokens[2]));
        if (tokens.length > 3) {
            sub.setNrPremiumSub(Integer.parseInt(tokens[3]));
        }
        memory.add(searchResultMemory, sub);
        
        switch (cacheType) {
            case "FIFO":
                int searchResultFifo = fifo.searchMemory(tokens[1]);
                if (searchResultFifo != -1) {
                    fifo.remove(searchResultFifo);
                }
                break;
            case "LRU":
                int searchResultLru = lru.searchMemory(tokens[1]);
                if (searchResultLru != -1) {
                    lru.remove(searchResultLru);
                }
                break;
            case "LFU":
                int searchResultLfu = lfu.searchMemory(tokens[1]);
                if (searchResultLfu != -1) {
                    lfu.remove(searchResultLfu);
                }
                break;
            default:
                break;
        }
    }
    
    /**
     *
     * @param name the name of the object that needs to be checked
     * @param cacheType "FIFO", "LRU" or "LFU" depending on which type of cache memory is being used
     * @return the string which needs to be written in the output file as a result of the GET command
     */
    public String get(String name, String cacheType) {
        int searchResultMemory = memory.searchMemory(name);
        String returnString = ("2");
        switch (cacheType) {
            case "FIFO":
                int searchResultFifo = fifo.searchMemory(name);
                if (searchResultFifo != -1) {
                    returnString = ("0 " + memory.getElement(searchResultMemory).showSubType());
                } else if (searchResultMemory != -1) {
                    fifo.add(memory.getElement(searchResultMemory));
                    returnString = ("1 " + memory.getElement(searchResultMemory).showSubType());
                }
                break;
            case "LRU":
                int searchResultLru = lru.searchMemory(name);
                if (searchResultLru != -1) {
                    lru.reorder(name);
                    returnString = ("0 " + memory.getElement(searchResultMemory).showSubType());
                } else if (searchResultMemory != -1) {
                    lru.add(memory.getElement(searchResultMemory));
                    returnString = ("1 " + memory.getElement(searchResultMemory).showSubType());
                }
                break;
            case "LFU":
                int searchResultLfu = lfu.searchMemory(name);
                if (searchResultLfu != -1) {
                    lfu.incStamp(searchResultLfu);
                    returnString = ("0 " + memory.getElement(searchResultMemory).showSubType());
                } else if (searchResultMemory != -1) {
                    lfu.add(memory.getElement(searchResultMemory));
                    returnString = ("1 " + memory.getElement(searchResultMemory).showSubType());
                }
                break;
            default:
                break;
        }
        return returnString;
    }
    
}
