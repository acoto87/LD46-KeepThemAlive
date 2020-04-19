
Entity* createEntity(GameContext* context, EntityType type);
void removeEntity(GameContext* context, Entity* entity);

Entity* findEntity(GameContext* context, EntityId id);
Entity* findEntityOfType(GameContext* context, EntityType type);
Entity* findBedAt(GameContext* context, s32 row, s32 col, s32 index);
Entity* findVentilatorAt(GameContext* context, s32 row, s32 col);

Entity* createImage(GameContext* context, vec2 position, const char* fileName);
Entity* createRect(GameContext* context, const char* name, vec2 position, vec2 size, NVGcolor color);
Entity* createText(GameContext* context, vec2 position, const char* name, f32 fontSize, const char* text);

void addTransformComponent(GameContext* context, Entity* entity, vec2 position, vec2 scale);
void addSpriteComponent(GameContext* context, Entity* entity, Sprite sprite);
void addSpriteComponentFromFile(GameContext* context, Entity* entity, const char* fileName);
void addUIComponent(GameContext* context, Entity* entity, const char* name);
void addTextComponent(GameContext* context, Entity* entity, s32 fontIndex, f32 fontSize, const char* text);
void addNurseComponent(GameContext* context, Entity* entity, s32 row, s32 col, s32 index);
void addBedComponent(GameContext* context, Entity* entity, s32 row, s32 col, s32 index, s32 life);
void addVentilatorComponent(GameContext* context, Entity* entity, s32 row, s32 col, s32 dir);
void addRectComponent(GameContext* context, Entity* entity, vec2 size, NVGcolor color);

void removeSpriteComponent(GameContext* context, Entity* entity);

void renderImage(GameContext* context, Entity* entity);
void renderCharacter(GameContext* context, Entity* entity);
void renderEntity(GameContext* context, Entity* entity);
