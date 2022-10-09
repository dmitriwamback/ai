#if !defined(MI_COMPONENT_IMPLEMENTATION)
#define MI_COMPONENT_IMPLEMENTATION

namespace Mi {

    class AttribSource {
    public:
        std::string name;
        virtual void Update() {};
        virtual void Initialized() {};
    };

    class Attribute {
    public:
        void UseAttribute() {
            if (source == nullptr) return;
            if (!initialized) {
                source->Initialized();
                initialized = true;
            }
            source->Update();
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

        glm::mat4 CreateModelMatrix(glm::vec3 position, glm::vec3 size, glm::vec3 rotation) {
            glm::mat4 modelPosition = glm::translate(glm::mat4(1.f), position);
            glm::mat4 modelSize     = glm::scale(glm::mat4(1.f), size);
            glm::mat4 modelRotation = Mi::EulerAngles(rotation);

            return modelPosition * modelSize;
        }

        virtual void Update(Mi::Material material);
    };
}

#endif