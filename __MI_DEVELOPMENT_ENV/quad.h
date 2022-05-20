namespace __MI_DEVELOPMENT_ENVIRONMENT {

    uint32_t vao;
    uint32_t vbo;

    uint32_t tex;
    Mi::IO::Character ch;

    void InitQuad(Mi::IO::Character character) {
        ch = character;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        float vertices[24] = {
            -character.size.x * 0.001f,  character.size.y * 0.001f, 0.0f, 0.0f,
             character.size.x * 0.001f,  character.size.y * 0.001f, 1.0f, 0.0f,
             character.size.x * 0.001f, -character.size.y * 0.001f, 1.0f, 1.0f,

            -character.size.x * 0.001f,  character.size.y * 0.001f, 0.0f, 0.0f,
            -character.size.x * 0.001f, -character.size.y * 0.001f, 0.0f, 1.0f,
             character.size.x * 0.001f, -character.size.y * 0.001f, 1.0f, 1.0f
        };  


        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }

    void RenderQuad(Shader shader) {
        shader.use();

        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, ch.texid);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
    }
}