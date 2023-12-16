#ifndef GP_3_INSTANCEDENTITY_H
#define GP_3_INSTANCEDENTITY_H

#include "Entity.h"

class InstancedEntity : public Entity {
    unsigned int instanceMatrixBuffer = 0;

    void prepareInstanceMatrixBuffer();
    void updateInstanceMatrixBuffer();
public:
    InstancedEntity();
    InstancedEntity(Model* model, Shader* shader, std::vector<Transform*> transforms);
    ~InstancedEntity() = default;

    void draw() override;
    void update() override;

    std::vector<Transform*> instanceTransforms;
};


#endif //GP_3_INSTANCEDENTITY_H
