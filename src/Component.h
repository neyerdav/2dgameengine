#ifndef COMONENT_H
#define COMONENT_H

class Entity;

class Component {
    public:
    Entity* owner;
        virtual void Initialize() {};
        virtual void Update(float deltaTime) {};
        virtual void Render() {};
};

#endif