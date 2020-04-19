bool initGame(GameContext* context)
{
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

    context->globalScale = 4;
    context->globalSpeed = 1;
    context->originalWindowWidth = 320;
    context->originalWindowHeight = 200;
    context->windowWidth = (s32)(context->originalWindowWidth * context->globalScale);
    context->windowHeight = (s32)(context->originalWindowHeight * context->globalScale);
    strcpy(context->windowTitle, "Game");
    context->window = glfwCreateWindow(context->windowWidth, context->windowHeight, context->windowTitle, NULL, NULL);

    pthread_mutex_init(&context->__mutex, NULL);

    if (!context->window)
    {
        logError("GLFW window could not be created!\n");
        glfwTerminate();
        return false;
    }

    glfwGetWindowSize(context->window, &context->windowWidth, &context->windowHeight);
    glfwGetFramebufferSize(context->window, &context->framebufferWidth, &context->framebufferHeight);
    context->devicePixelRatio = (f32)context->framebufferWidth / (f32)context->windowWidth;

    glfwMakeContextCurrent(context->window);

    // glfwSetInputMode(context->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress);

    glCheckOpenGLVersion();

    // init graphics
    context->gfx = nvgCreateGLES2(NVG_STENCIL_STROKES | NVG_DEBUG);
    if (!context->gfx)
    {
        logError("Could not init nanovg.\n");
        glfwDestroyWindow(context->window);
        glfwTerminate();
		return false;
	}

    ma_decoder_init_file("music.wav", NULL, &context->decoder);

    // init audio
    if (!initAudio(context))
    {
        logError("Could not initialize audio.\n");
        return false;
    }

    // load fonts
    context->fontSprites[0] = loadFontSprite(context, "./font_1.png");
    context->fontSprites[1] = loadFontSprite(context, "./font_2.png");

    context->menu = (Menu*)xcalloc(1, sizeof(Menu));
    initMenu(context);

    glViewport(0, 0, context->framebufferWidth, context->framebufferHeight);
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

    context->time = (f32)glfwGetTime();
    return true;
}

void setWindowSize(GameContext* context, s32 width, s32 height)
{
    context->windowWidth = width;
    context->windowHeight = height;
    glfwSetWindowSize(context->window, context->windowWidth, context->windowHeight);
    glfwGetFramebufferSize(context->window, &context->framebufferWidth, &context->framebufferHeight);
    context->devicePixelRatio = (f32)context->framebufferWidth / (f32)context->windowWidth;
}

void setGlobalScale(GameContext* context, f32 scale)
{
    context->globalScale = max(scale, 1.0f);
    logDebug("set global scale to: %.2f\n", context->globalScale);

    s32 newWidth = (s32)(context->originalWindowWidth * context->globalScale);
    s32 newHeight = (s32)(context->originalWindowHeight * context->globalScale);
    setWindowSize(context, newWidth, newHeight);
}

void changeGlobalScale(GameContext* context, f32 deltaScale)
{
    setGlobalScale(context, context->globalScale + deltaScale);
}

void setGlobalSpeed(GameContext* context, f32 speed)
{
    context->globalSpeed = max(speed, 1.0f);
    logDebug("set global speed to: %.2f\n", context->globalSpeed);
}

void changeGlobalSpeed(GameContext* context, f32 deltaSpeed)
{
    setGlobalSpeed(context, context->globalSpeed + deltaSpeed);
}

void setInputButton(GameContext* context, s32 button, bool pressed)
{
    GameInput* input = &context->input;

    input->buttons[button].wasPressed = input->buttons[button].pressed && !pressed;
    input->buttons[button].pressed = pressed;
}

void setInputKey(GameContext* context, s32 key, bool pressed)
{
    GameInput* input = &context->input;

    input->keys[key].wasPressed = input->keys[key].pressed && !pressed;
    input->keys[key].pressed = pressed;
}

void inputGame(GameContext *context)
{
    // mouse position
    f64 xpos, ypos;
    glfwGetCursorPos(context->window, &xpos, &ypos);

    vec2 pos = vec2f((f32)floor(xpos), (f32)floor(ypos));
    pos = vec2Scalef(pos, 1/context->globalScale);
    context->input.pos = pos;

    // mouse buttons
    setInputButton(context, GAME_MOUSE_LEFT, glfwGetMouseButton(context->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    setInputButton(context, GAME_MOUSE_RIGHT, glfwGetMouseButton(context->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);

    // keyboard keys
    setInputKey(context, GAME_KEY_SPACE, glfwGetKey(context->window, GLFW_KEY_SPACE) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_APOSTROPHE, glfwGetKey(context->window, GLFW_KEY_APOSTROPHE) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_COMMA, glfwGetKey(context->window, GLFW_KEY_COMMA) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_MINUS, glfwGetKey(context->window, GLFW_KEY_MINUS) == GLFW_PRESS ||
                                        glfwGetKey(context->window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_PERIOD, glfwGetKey(context->window, GLFW_KEY_PERIOD) == GLFW_PRESS ||
                                         glfwGetKey(context->window, GLFW_KEY_KP_DECIMAL) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_SLASH, glfwGetKey(context->window, GLFW_KEY_SLASH) == GLFW_PRESS ||
                                        glfwGetKey(context->window, GLFW_KEY_KP_DIVIDE) == GLFW_PRESS);

    setInputKey(context, GAME_KEY_0, glfwGetKey(context->window, GLFW_KEY_0) == GLFW_PRESS ||
                                    glfwGetKey(context->window, GLFW_KEY_KP_0) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_1, glfwGetKey(context->window, GLFW_KEY_1) == GLFW_PRESS ||
                                    glfwGetKey(context->window, GLFW_KEY_KP_1) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_2, glfwGetKey(context->window, GLFW_KEY_2) == GLFW_PRESS ||
                                    glfwGetKey(context->window, GLFW_KEY_KP_2) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_3, glfwGetKey(context->window, GLFW_KEY_3) == GLFW_PRESS ||
                                    glfwGetKey(context->window, GLFW_KEY_KP_3) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_4, glfwGetKey(context->window, GLFW_KEY_4) == GLFW_PRESS ||
                                    glfwGetKey(context->window, GLFW_KEY_KP_4) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_5, glfwGetKey(context->window, GLFW_KEY_5) == GLFW_PRESS ||
                                    glfwGetKey(context->window, GLFW_KEY_KP_5) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_6, glfwGetKey(context->window, GLFW_KEY_6) == GLFW_PRESS ||
                                    glfwGetKey(context->window, GLFW_KEY_KP_6) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_7, glfwGetKey(context->window, GLFW_KEY_7) == GLFW_PRESS ||
                                    glfwGetKey(context->window, GLFW_KEY_KP_7) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_8, glfwGetKey(context->window, GLFW_KEY_8) == GLFW_PRESS ||
                                    glfwGetKey(context->window, GLFW_KEY_KP_8) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_9, glfwGetKey(context->window, GLFW_KEY_9) == GLFW_PRESS ||
                                    glfwGetKey(context->window, GLFW_KEY_KP_9) == GLFW_PRESS);

    setInputKey(context, GAME_KEY_SEMICOLON, glfwGetKey(context->window, GLFW_KEY_SEMICOLON) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_EQUAL, glfwGetKey(context->window, GLFW_KEY_EQUAL) == GLFW_PRESS ||
                                        glfwGetKey(context->window, GLFW_KEY_KP_EQUAL) == GLFW_PRESS);

    setInputKey(context, GAME_KEY_A, glfwGetKey(context->window, GLFW_KEY_A) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_B, glfwGetKey(context->window, GLFW_KEY_B) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_C, glfwGetKey(context->window, GLFW_KEY_C) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_D, glfwGetKey(context->window, GLFW_KEY_D) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_E, glfwGetKey(context->window, GLFW_KEY_E) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_F, glfwGetKey(context->window, GLFW_KEY_F) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_G, glfwGetKey(context->window, GLFW_KEY_G) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_H, glfwGetKey(context->window, GLFW_KEY_H) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_I, glfwGetKey(context->window, GLFW_KEY_I) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_J, glfwGetKey(context->window, GLFW_KEY_J) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_K, glfwGetKey(context->window, GLFW_KEY_K) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_L, glfwGetKey(context->window, GLFW_KEY_L) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_M, glfwGetKey(context->window, GLFW_KEY_M) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_N, glfwGetKey(context->window, GLFW_KEY_N) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_O, glfwGetKey(context->window, GLFW_KEY_O) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_P, glfwGetKey(context->window, GLFW_KEY_P) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_Q, glfwGetKey(context->window, GLFW_KEY_Q) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_R, glfwGetKey(context->window, GLFW_KEY_R) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_S, glfwGetKey(context->window, GLFW_KEY_S) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_T, glfwGetKey(context->window, GLFW_KEY_T) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_U, glfwGetKey(context->window, GLFW_KEY_U) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_V, glfwGetKey(context->window, GLFW_KEY_V) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_W, glfwGetKey(context->window, GLFW_KEY_W) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_X, glfwGetKey(context->window, GLFW_KEY_X) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_Y, glfwGetKey(context->window, GLFW_KEY_Y) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_Z, glfwGetKey(context->window, GLFW_KEY_Z) == GLFW_PRESS);

    setInputKey(context, GAME_KEY_LEFT_BRACKET, glfwGetKey(context->window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_BACKSLASH, glfwGetKey(context->window, GLFW_KEY_BACKSLASH) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_RIGHT_BRACKET, glfwGetKey(context->window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_GRAVE_ACCENT, glfwGetKey(context->window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS);

    setInputKey(context, GAME_KEY_ESC, glfwGetKey(context->window, GLFW_KEY_ESCAPE) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_ENTER, glfwGetKey(context->window, GLFW_KEY_ENTER) == GLFW_PRESS ||
                                        glfwGetKey(context->window, GLFW_KEY_KP_ENTER) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_TAB, glfwGetKey(context->window, GLFW_KEY_TAB) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_BACKSPACE, glfwGetKey(context->window, GLFW_KEY_BACKSPACE) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_INSERT, glfwGetKey(context->window, GLFW_KEY_INSERT) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_DELETE, glfwGetKey(context->window, GLFW_KEY_DELETE) == GLFW_PRESS);

    setInputKey(context, GAME_KEY_RIGHT, glfwGetKey(context->window, GLFW_KEY_RIGHT) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_LEFT, glfwGetKey(context->window, GLFW_KEY_LEFT) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_DOWN, glfwGetKey(context->window, GLFW_KEY_DOWN) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_UP, glfwGetKey(context->window, GLFW_KEY_UP) == GLFW_PRESS);

    setInputKey(context, GAME_KEY_PAGE_UP, glfwGetKey(context->window, GLFW_KEY_PAGE_UP) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_PAGE_DOWN, glfwGetKey(context->window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_HOME, glfwGetKey(context->window, GLFW_KEY_HOME) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_END, glfwGetKey(context->window, GLFW_KEY_END) == GLFW_PRESS);

    setInputKey(context, GAME_KEY_F1, glfwGetKey(context->window, GLFW_KEY_F1) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_F2, glfwGetKey(context->window, GLFW_KEY_F2) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_F3, glfwGetKey(context->window, GLFW_KEY_F3) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_F4, glfwGetKey(context->window, GLFW_KEY_F4) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_F5, glfwGetKey(context->window, GLFW_KEY_F5) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_F6, glfwGetKey(context->window, GLFW_KEY_F6) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_F7, glfwGetKey(context->window, GLFW_KEY_F7) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_F8, glfwGetKey(context->window, GLFW_KEY_F8) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_F9, glfwGetKey(context->window, GLFW_KEY_F9) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_F10, glfwGetKey(context->window, GLFW_KEY_F10) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_F11, glfwGetKey(context->window, GLFW_KEY_F11) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_F12, glfwGetKey(context->window, GLFW_KEY_F12) == GLFW_PRESS);

    setInputKey(context, GAME_KEY_ASTERISK, glfwGetKey(context->window, GLFW_KEY_KP_MULTIPLY) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_PLUS, glfwGetKey(context->window, GLFW_KEY_KP_ADD) == GLFW_PRESS);

    setInputKey(context, GAME_KEY_SHIFT, glfwGetKey(context->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
                                        glfwGetKey(context->window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_CTRL, glfwGetKey(context->window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
                                       glfwGetKey(context->window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);
    setInputKey(context, GAME_KEY_ALT, glfwGetKey(context->window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS ||
                                      glfwGetKey(context->window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS);
}

void inputCharCallback(GLFWwindow* window, u32 codepoint)
{
    GameContext* context = glfwGetWindowUserPointer(window);
    assert(context);

    logInfo("input char: %d\n", codepoint);
}

void updateGame(GameContext* context)
{
    GameInput* input = &context->input;

    if (isKeyPressed(input, GAME_KEY_CTRL) &&
        wasKeyPressed(input, GAME_KEY_P))
    {
        context->paused = !context->paused;
    }

    if (context->paused)
    {
        return;
    }

    if (context->room)
    {
        updateRoom(context);
    }
    else if (context->menu)
    {
        updateMenu(context);
    }
}

void renderGame(GameContext *context)
{
    NVGcontext* gfx = context->gfx;

    s32 framebufferWidth = context->framebufferWidth;
    s32 framebufferHeight = context->framebufferHeight;

    s32 windowWidth = context->windowWidth;
    s32 windowHeight = context->windowHeight;

    f32 devicePixelRatio = context->devicePixelRatio;

    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(gfx, windowWidth, windowHeight, devicePixelRatio);

    if (context->room)
    {
        renderRoom(context);
    }
    else if (context->menu)
    {
        renderMenu(context);
    }

    nvgEndFrame(gfx);
}

void presentGame(GameContext *context)
{
    glfwSwapBuffers(context->window);
    glfwPollEvents();

    f32 currentTime = glfwGetTime();
    context->deltaTime = (currentTime - context->time);

    while (context->deltaTime <= SECONDS_PER_FRAME)
    {
        currentTime = (f32)glfwGetTime();
        context->deltaTime = (currentTime - context->time);
    }

    context->time = currentTime;
    context->fps = (u32)(1.0f / context->deltaTime);
}
