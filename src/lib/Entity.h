#ifndef GP_3_ENTITY_H
#define GP_3_ENTITY_H

#include "Model.h"
#include "Transform.h"

#include <vector>

class Entity {
protected:
    Model* model = nullptr;
    Shader* shader = nullptr;
public:
    Transform transform;

    Entity(const std::string& modelPath, Shader* shader);
    Entity(Model* loadedModel, Shader* shader);
    Entity();
    virtual ~Entity() = default;

    void setModel(Model* newModel);
    void setShader(Shader* newShader);

    virtual void draw();
    virtual void update();
};


#endif //GP_3_ENTITY_H