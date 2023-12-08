#ifndef GP_3_INSTANCEDOBJECT_H
#define GP_3_INSTANCEDOBJECT_H

#include "Object.h"

class InstancedObject : Object {
    unsigned int instanceMatrixBuffer = 0;
    void prepareInstanceMatricesBuffer();
    void updateInstanceMatricesBuffer();
public:
    InstancedObject();
    InstancedObject(Model* model, Shader* shader, std::vector<Entity*> entities);
    ~InstancedObject() override = default;

    void update() override;
    void draw() override;
    void addInstanceEntity(const Entity &entity);
    std::vector<Entity*> instanceEntities;
};


#endif //GP_3_INSTANCEDOBJECT_H
