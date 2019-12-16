/**
 *
 * @author Adrian
 */
public interface ICache {

    /**
     *
     * @param index the position of the object which must be removed from memory
     */
    public void remove(int index);

    /**
     *
     * @param sub the object which must be added to the memory
     */
    public void add(Premium sub);
}
