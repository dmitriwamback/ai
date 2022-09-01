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
        void SetAttributeSource(AttribSource *source) {
            this->source = source;
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
        RenderBuffer buffer;
        std::vector<Mi::Texture> textures;

        ObjectRenderer() {}
        ObjectRenderer(RenderBuffer buffer) {}
        virtual float* GetVertices() { return nullptr; }
        virtual int GetVertexSize()  { return 0; }
    };
}

#endif