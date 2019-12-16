/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.util.ArrayList;

/**
 *
 * @author Adrian
 */
public class Node {

    ArrayList<Entity> entities;

    /**
     * The constructor of the objects; creates a new ArrayList.
     */
    public Node() {
        entities = new ArrayList<>();
    }

    /**
     *
     * @return returns the number of entities stored in the node
     */
    public int getNrEntities() {
        return entities.size();
    }

    /**
     * Adds a new entity in the node.
     * @param entity the entity which must be added to the node
     */
    public void addEntity(Entity entity) {
        Attribute attribute;
        Entity entityAux = new Entity(entity.name, entity.rf);
        for (int i = 0; i < entity.getNrAttributes(); i++) {
            attribute = entity.getAttribute(i);
            switch (entity.getAttributeType(i)) {
                case "Float":
                    entityAux.addAttribute(new FloatAttribute(attribute.getName(), (Float) attribute.getValue()));
                    break;
                case "Integer":
                    entityAux.addAttribute(new IntegerAttribute(attribute.getName(), (Integer) attribute.getValue()));
                    break;
                case "String":
                    entityAux.addAttribute(new StringAttribute(attribute.getName(), (String) attribute.getValue()));
                    break;
                default:
                    break;
            }
        }
        entities.add(entityAux);
    }

    /**
     *
     * @param index the position of an entity from the list
     * @return returns the entity found at position index from the list
     */
    public Entity getEntity(int index) {
        return entities.get(index);
    }

    /**
     * Removes the entity found at position index from the list.
     * @param index the position of an entity from the list
     */
    public void removeEntity(int index) {
        entities.remove(index);
    }

    /**
     *
     * @param entityName the name of an entity
     * @param primaryKey the primary key of an entity
     * @return returns the index at which the entity can be found or -1 if it doesn't exist
     */
    public int searchEntity(String entityName, String primaryKey) {
        int index = -1;
        Entity entity;
        for (int i = 0; i < getNrEntities(); i++) {
            entity = getEntity(i);
            if (entity.name.equals(entityName)) {
                if (entity.checkKey(primaryKey)) {
                    index = i;
                    break;
                }
            }
        }
        return index;
    }
    
    /**
     *
     * @return returns a string representation of the object
     */
    @Override
    public String toString() {
        String s = "";
        for (Entity entity : entities) {
            s += entity.toString() + "\n";
        }
        return s;
    }
}
