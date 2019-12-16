abstract class Subscription {

    private String name;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

}

class Free extends Subscription {

}

class Basic extends Free {

    private int nrBasicSub;

    public int getNrBasicSub() {
        return nrBasicSub;
    }

    public void setNrBasicSub(int nrBasicSub) {
        this.nrBasicSub = nrBasicSub;
    }

}

/**
 *
 * @author Adrian
 */
public class Premium extends Basic {

    private int nrPremiumSub;

    /**
     *
     * @return the number of premium subscriptions left
     */
    public int getNrPremiumSub() {
        return nrPremiumSub;
    }

    /**
     *
     * @param nrPremiumSub the number of premium subscriptions
     */
    public void setNrPremiumSub(int nrPremiumSub) {
        this.nrPremiumSub = nrPremiumSub;
    }

    /**
     *
     * @return "Premium" if the object still has premium subscribtions, "Basic" if it has only basic subscriptions or "Free"
     */
    public String showSubType() {
        if (getNrPremiumSub() > 0) {
            setNrPremiumSub(getNrPremiumSub() - 1);
            return "Premium";
        } else if (super.getNrBasicSub() > 0) {
            super.setNrBasicSub(super.getNrBasicSub() - 1);
            return "Basic";
        } else {
            return "Free";
        }
    }

}
