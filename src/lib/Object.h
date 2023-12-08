#ifndef GP_3_OBJECT_H
#define GP_3_OBJECT_H

#include "Entity.h"
#include "Model.h"

class Object {
protected:
    Model* model = nullptr;
    Shader* shader = nullptr;
public:
    Entity entity;
    Object() = default;
    Object(Model* model, Shader* shader);
    Object(char* modelPath, Shader* shader);
    virtual ~Object() = default;

    void setModel(Model* model);
    void setShader(Shader* shader);

    virtual void draw();
    virtual void update();
};


#endif //GP_3_OBJECT_H
