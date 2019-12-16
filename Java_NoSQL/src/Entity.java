/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.util.ArrayList;
import java.util.Comparator;

/**
 *
 * @author Adrian
 */
public class Entity {

    String name;
    int rf;
    ArrayList<Attribute> attributes;
    long time;

    /**
     * The constructor of the objects.
     * @param name the name of the entity
     * @param rf replication factor (how many nodes must contain the entity)
     */
    public Entity(String name, int rf) {
        this.rf = rf;
        this.name = name;
        attributes = new ArrayList<>();
        time = System.nanoTime();
    }

    /**
     * Adds a new attribute in the entity.
     * @param a the attribute which must be added to the entity
     */
    public void addAttribute(Attribute a) {
        attributes.add(a);
    }

    /**
     *
     * @return returns the number of attributes the entity contains
     */
    public int getNrAttributes() {
        return attributes.size();
    }

    /**
     * Sets the value of an attribute.
     * @param index the position of an attribute from the list
     * @param value the new value of the attribute
     */
    public void setAttribute(int index, Object value) {
        attributes.get(index).setValue(value);
    }

    /**
     *
     * @param index the position of an attribute from the list
     * @return returns the attribute found at position index from the list
     */
    public Attribute getAttribute(int index) {
        return attributes.get(index);
    }

    /**
     *
     * @param index the position of an attribute from the list
     * @return returns a string ("Float", "Integer" or "String") which is the
     * type of value the attribute at position index from the list stores
     */
    public String getAttributeType(int index) {
        if (attributes.get(index) instanceof FloatAttribute) {
            return "Float";
        } else if (attributes.get(index) instanceof IntegerAttribute) {
            return "Integer";
        } else {
            return "String";
        }
    }

    /**
     *
     * @param key a value set to be compared with the primary key of the entity
     * @return returns true if the key is equal to the primary key of the
     * entity, or else false
     */
    public boolean checkKey(String key) {
        try {
            switch (getAttributeType(0)) {
                case "Float":
                    return (Float) attributes.get(0).getValue() == Float.parseFloat(key);
                case "Integer":
                    return (Integer) attributes.get(0).getValue() == Integer.parseInt(key);
                case "String":
                    return attributes.get(0).getValue().equals(key);
                default:
                    return false;
            }
        } catch (NumberFormatException ex) {
            return false; //in case the key is the wrong format which is the same as being different
        }
    }

    /**
     * Updates some if not all the attributes of the entity.
     * @param tokens contains the the name of the attributes and the new values
     * which must be replaced
     */
    public void update(String[] tokens) {
        int index;
        for (int i = 3; i < tokens.length; i += 2) {
            index = 0;
            for (Attribute attribute : attributes) {
                if (attribute.getName().equals(tokens[i])) {
                    break;
                }
                index++;
            }
            switch (getAttributeType(index)) {
                case "Float":
                    attributes.get(index).setValue(Float.parseFloat(tokens[i + 1]));
                    break;
                case "Integer":
                    attributes.get(index).setValue(Integer.parseInt(tokens[i + 1]));
                    break;
                case "String":
                    attributes.get(index).setValue(tokens[i + 1]);
                    break;
                default:
                    break;
            }
        }
    }

    /**
     * Class being used for the sorting of the list of entities from a node.
     */
    public static Comparator<Entity> compareTimestamps = new Comparator<Entity>() {
        @Override
        public int compare(Entity e1, Entity e2) {
            if (e1.time > e2.time) {
                return -1;
            } else if (e1.time < e2.time) {
                return 1;
            } else {
                return 0;
            }
        }
    };

    /**
     * Updates the timestamp of the entity with a new value in nanoseconds.
     */
    public void updateTimestamp() {
        this.time = System.nanoTime();
    }

    /**
     *
     * @return returns a string representation of the object
     */
    @Override
    public String toString() {
        String s = name;
        for (Attribute attribute : attributes) {
            s += " " + attribute.toString();
        }
        return s;
    }
}
