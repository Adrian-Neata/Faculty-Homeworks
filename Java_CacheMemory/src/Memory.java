import java.util.ArrayList;

/**
 *
 * @author Adrian
 */
public class Memory {

    protected ArrayList<Premium> memory = new ArrayList<>();

    /**
     *
     * @param index the position of an object from the memory
     * @return the element found at position index
     */
    public Premium getElement(int index) {
        return memory.get(index);
    }

    /**
     *
     * @param name the name of the object which is searched in the cache memory
     * @return -1 if the object hasn't been found, or the position of the object
     * if it has
     */
    public int searchMemory(String name) {
        int counter = 0;
        for (Premium element : memory) {
            if (element.getName().equals(name)) {
                return counter;
            }
            counter++;
        }
        return -1;
    }

    /**
     *
     * @param index -1 if the object must be added or the position of the object
     * which must be overwritten
     * @param sub the object which must be added to the memory
     */
    public void add(int index, Premium sub) {
        if (index == -1) {
            memory.add(sub);
        } else {
            memory.set(index, sub);
        }
    }

}
