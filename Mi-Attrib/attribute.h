#if !defined(MI_COMPONENT_IMPLEMENTATION)
#define MI_COMPONENT_IMPLEMENTATION

namespace Mi {

    class AttribSource {
    public:
        std::string name;
        virtual void Update(Transform transform) {};
        virtual void Initialized() {};
    };

    class Attribute {
    public:
        void UseAttribute(Transform transform) {
            if (source == nullptr) return;
            if (!initialized) {
                source->Initialized();
                initialized = true;
            }
            source->Update(transform);
        }
        AttribSource* GetSource() {
            return source;
        }

        Attribute* SetAttributeSource(AttribSource *source) {
            this->source = source;
            return this;
        }
        bool HasAttribSource() {
            return source != nullptr;
        }
    private:
        bool initialized = false;
        AttribSource* source;
    };

    class ObjectAttribute: public AttribSource {
    public:
        RenderBuffer buffer;
        Mi::Scripting::Script script;

        template<class T>
        T CastTo() {
            return dynamic_cast<T>(this);
        }
    };

    class ObjectRenderer: public AttribSource {
    public:
        Material material{};
        RenderBuffer* buffer;
        std::vector<Mi::Texture> textures;

        ObjectRenderer() {}
        ObjectRenderer(RenderBuffer buffer) {}
        virtual float* GetVertices() { return nullptr; }
        virtual int GetVertexSize()  { return 0; }
        void SetRenderBuffer(RenderBuffer *buffer) { this->buffer = buffer; }

        glm::mat4 CreateModelMatrix(Transform transform) {
            glm::mat4 modelPosition = glm::translate(glm::mat4(1.f), transform.position);
            glm::mat4 modelSize     = glm::scale(glm::mat4(1.f), transform.size);
            glm::mat4 modelRotation = Mi::EulerAngles(transform.rotation);

            return modelPosition * modelRotation * modelSize;
        }

        virtual void Render(Transform transform) {}
    };
}

#endif