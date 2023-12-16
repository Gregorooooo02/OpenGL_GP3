#ifndef GP_3_ENTITY_H
#define GP_3_ENTITY_H

#include "Model.h"
#include "Transform.h"

#include <vector>

class Entity {
public:
    std::vector<std::unique_ptr<Entity>> children;
    Model *model = nullptr;
    Entity* parent = nullptr;
    Transform transform;

    // Constructor
    Entity(Model &model, float startScale, int num, bool empty = false);

    // Add a child to the scene graph
    void addChild(Model &model, float startScale, int num, bool empty = false);
    // Draw the entity
    void draw(Shader &shader, int houses);
    // Update transform if it was changed
    void updateSelfAndChild();
    // Force update transform
    void forceUpdateSelfAndChild();
    // Swap color
    void swapColor();
    // Get node at index
    Entity *getNodeAtIndex(int index);

private:
    int idNumber;
    glm::vec4 color;
    glm::vec4 colorSelected = {1,0,0,1};
    glm::vec4 colorBasic;
    bool selected = false;
    bool empty = false;
    bool lighting = true;
    bool instanced = false;

    GLuint modelBuffer;
    GLuint colorBuffer;
};


#endif //GP_3_ENTITY_H