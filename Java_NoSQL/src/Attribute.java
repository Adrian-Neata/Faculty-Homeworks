/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Adrian
 */
public abstract class Attribute {

    private final String name;

    abstract Object getValue();

    abstract void setValue(Object value);

    /**
     *
     * @return returns a string representation of the object
     */
    @Override
    public abstract String toString();

    /**
     *
     * @param name the name of the attribute
     */
    public Attribute(String name) {
        this.name = name;
    }

    /**
     *
     * @return returns the name of the attribute
     */
    public String getName() {
        return name;
    }

}

class FloatAttribute extends Attribute {

    private Float value;

    public FloatAttribute(String name, Float value) {
        super(name);
        this.value = value;
    }

    @Override
    public Float getValue() {
        return value;
    }

    @Override
    public void setValue(Object value) {
        this.value = (Float) value;
    }

    @Override
    public String toString() {
        if (value == Math.ceil(value)) {
            return getName() + ":" + Math.round(value);
        } else {
            return getName() + ":" + getValue();
        }
    }

}

class IntegerAttribute extends Attribute {

    private Integer value;

    public IntegerAttribute(String name, Integer value) {
        super(name);
        this.value = value;
    }

    @Override
    public Integer getValue() {
        return value;
    }

    @Override
    public void setValue(Object value) {
        this.value = (Integer) value;
    }

    @Override
    public String toString() {
        return getName() + ":" + getValue();
    }
}

class StringAttribute extends Attribute {

    private String value;

    public StringAttribute(String name, String value) {
        super(name);
        this.value = value;
    }

    @Override
    public String getValue() {
        return value;
    }

    @Override
    public void setValue(Object value) {
        this.value = (String) value;
    }

    @Override
    public String toString() {
        return getName() + ":" + getValue();
    }
}
