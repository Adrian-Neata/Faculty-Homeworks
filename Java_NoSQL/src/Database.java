/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.util.ArrayList;
import java.util.Collections;

/**
 *
 * @author Adrian
 */
public class Database {

    private String name;
    private int maxCapacityNode;
    private int nrNodes;
    final ArrayList<Entity> entities;
    final ArrayList<Node> nodes;

    /**
     * The constructor of the objects; creates 2 new ArrayLists.
     */
    public Database() {
        entities = new ArrayList<>();
        nodes = new ArrayList<>();
    }

    /**
     * Sets the name, number of nodes, and the maximum capacity of a node in the database.
     * @param name the name of the database
     * @param nrNodes the number of nodes the database must contain at most
     * @param maxCapacityNode the number of entities a node must contain at most
     */
    public void initialize(String name, int nrNodes, int maxCapacityNode) {
        this.name = name;
        this.nrNodes = nrNodes;
        this.maxCapacityNode = maxCapacityNode;
    }

    /**
     * Creates a new entity for the database.
     *
     * @param tokens contains the name of an entity and the name and types of
     * the attributes found in that entity
     */
    public void createEntity(String[] tokens) {
        int nrAttributes;
        Entity entity = new Entity(tokens[1], Integer.parseInt(tokens[2]));
        nrAttributes = Integer.parseInt(tokens[3]);
        for (int i = 0; i < 2 * nrAttributes; i += 2) {
            switch (tokens[i + 5]) {
                case "Float":
                    entity.addAttribute(new FloatAttribute(tokens[i + 4], 0f));
                    break;
                case "Integer":
                    entity.addAttribute(new IntegerAttribute(tokens[i + 4], 0));
                    break;
                case "String":
                    entity.addAttribute(new StringAttribute(tokens[i + 4], null));
                    break;
                default:
                    break;
            }
        }
        entities.add(entity);
    }

    /**
     * Inserts a new entity in the database.
     *
     * @param tokens contains the name of an entity and the name and values of
     * the attributes found in that entity
     */
    public void insert(String[] tokens) {
        Entity entity;
        int i;
        int index;
        entity = whichEntity(tokens[1]);
        //set entitiy's values
        for (i = 0; i < entity.getNrAttributes(); i++) {
            switch (entity.getAttributeType(i)) {
                case "Float":
                    entity.setAttribute(i, Float.parseFloat(tokens[i + 2]));
                    break;
                case "Integer":
                    entity.setAttribute(i, Integer.parseInt(tokens[i + 2]));
                    break;
                case "String":
                    entity.setAttribute(i, tokens[i + 2]);
                    break;
                default:
                    break;
            }
        }
        index = 0;
        //creates new nodes if necessary
        while (nodes.size() < entity.rf) {
            nodes.add(new Node());
        }
        //goes to the first node that has space left
        while (index < nodes.size() && nodes.get(index).getNrEntities() == maxCapacityNode) {
            index++;
        }
        //add the entity in nodes
        for (i = 0; i < entity.rf; i++) {
            if (index == nodes.size()) {
                nodes.add(new Node());
            }
            nodes.get(index).addEntity(entity);
            index++;
        }
    }

    /**
     * Deletes, if found, the entities with the same name as entityName and the
     * same primary key as primaryKey.
     *
     * @param entityName the name of an entity
     * @param primaryKey the primary key of an entity
     * @return returns true if the entity has been found and deleted from the
     * database or else false
     */
    public boolean delete(String entityName, String primaryKey) {
        boolean found = false;
        int index;
        int nodeIndex = 0;
        Node node;
        while (nodeIndex < nodes.size()) {
            node = nodes.get(nodeIndex);
            index = node.searchEntity(entityName, primaryKey);
            if (index != -1) {
                found = true;
                node.removeEntity(index);
            }
            if (node.getNrEntities() == 0) {
                nodes.remove(nodeIndex);
            } else {
                nodeIndex++;
            }
        }
        return found;
    }

    /**
     * Updates an entity from the database replacing some of his attribute
     * values with new ones.
     *
     * @param tokens the name and primary key of an entity followed by the name
     * and new values of the attributes in that entity
     */
    public void update(String[] tokens) {
        int index;
        Entity entity;
        for (Node node : nodes) {
            index = node.searchEntity(tokens[1], tokens[2]);
            if (index != -1) {
                entity = node.getEntity(index);
                entity.update(tokens);
                entity.updateTimestamp();
            }
        }
    }

    /**
     *
     * @param name the name of an entity
     * @return returns the entity with the same name found in the list of
     * entities
     */
    public Entity whichEntity(String name) {
        int index = 0;
        for (Entity entity : entities) {
            if (entity.name.equals(name)) {
                break;
            }
            index++;
        }
        return entities.get(index);
    }

    /**
     *
     * @param entityName the name of an entity
     * @param primaryKey the primary key of an entity
     * @return returns a string containing the nodes in which the entity can be
     * found and its string representation
     */
    public String get(String entityName, String primaryKey) {
        String output = "";
        int index;
        int i = 1;
        Entity entity = whichEntity(entityName);
        Attribute attribute;
        boolean found = false;
        for (Node node : nodes) {
            index = node.searchEntity(entityName, primaryKey);
            if (index != -1) {
                if (found) {
                    output += " Nod" + i;
                } else {
                    output += "Nod" + i;
                }
                entity = node.getEntity(index);
                //entity.updateTimestamp();
                found = true;
            }
            i++;
        }
        if (found == true) {
            output += " " + entity.toString();
        } else {
            output = "NO INSTANCE FOUND";
        }
        return output;
    }

    /**
     *
     * @return returns a string representation of the database or the message EMPTY DB in case it's empty
     */
    public String SnapshotDB() {
        String output = "";
        int index = 1;
        for (Node node : nodes) {
            output += "Nod" + index + "\n";
            Collections.sort(node.entities, Entity.compareTimestamps);
            output += node.toString();
            index++;
        }
        if (output.equals("")) {
            output = "EMPTY DB\n";
        }
        return output;
    }

    /**
     * Deletes all instances of entities that are older than nano.
     * @param databaseName the name of the database
     * @param nano the time in nanoseconds
     */
    public void cleanup(String databaseName, long nano) {
        int index;
        int nodeIndex = 0;
        Node node;
        while (nodeIndex < nodes.size()) {
            node = nodes.get(nodeIndex);
            for (index = 0; index < node.getNrEntities(); index++) {
                if (node.getEntity(index).time < nano) {
                    break;
                }
            }
            while (index < node.getNrEntities()) {
                node.removeEntity(index);
            }
            if (node.getNrEntities() == 0) {
                nodes.remove(nodeIndex);
            } else {
                nodeIndex++;
            }
        }
    }
}
