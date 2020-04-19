Entity* createEntity(GameContext* context, EntityType type)
{
    Entity* entity = (Entity*)xmalloc(sizeof(Entity));

    entity->id = ++context->staticEntityId;
    entity->type = type;

    entity->transform = (TransformComponent){0};
    entity->sprite = (SpriteComponent){0};
    entity->ui = (UIComponent){0};
    entity->nurse = (NurseComponent){0};
    entity->bed = (BedComponent){0};

    if (context->room)
    {
        Room* room = context->room;
        room->entities[room->entitiesCount++] = entity;
    }
    else if (context->menu)
    {
        Menu* menu = context->menu;
        menu->entities[menu->entitiesCount++] = entity;
    }

    return entity;
}

void removeEntity(GameContext* context, Entity* entity)
{
    removeSpriteComponent(context, entity);
}

Entity* createText(GameContext* context, vec2 position, const char* name, f32 fontSize, const char* text)
{
    Entity* entity = createEntity(context, ENTITY_TYPE_TEXT);
    addTransformComponent(context, entity, position, VEC2_ONE);
    addUIComponent(context, entity, name);
    addTextComponent(context, entity, 1, fontSize, text);
    return entity;
}

Entity* createImage(GameContext* context, vec2 position, const char* fileName)
{
    Entity* entity = createEntity(context, ENTITY_TYPE_IMAGE);
    addTransformComponent(context, entity, position, VEC2_ONE);
    addSpriteComponentFromFile(context, entity, fileName);
    return entity;
}

Entity* createRect(GameContext* context, const char* name, vec2 position, vec2 size, NVGcolor color)
{
    Entity* entity = createEntity(context, ENTITY_TYPE_RECT);
    addTransformComponent(context, entity, position, VEC2_ONE);
    addUIComponent(context, entity, name);
    addRectComponent(context, entity, size, color);
    return entity;
}

Entity* findEntity(GameContext* context, EntityId id)
{
    Entity* result = NULL;

    Room* room = context->room;
    if (room)
    {
        for (s32 i = 0; i < room->entitiesCount; i++)
        {
            Entity* entity = room->entities[i];
            if (entity && entity->id == id)
            {
                result = entity;
                break;
            }
        }
    }

    Menu* menu = context->menu;
    if (menu)
    {
        for (s32 i = 0; i < menu->entitiesCount; i++)
        {
            Entity* entity = menu->entities[i];
            if (entity && entity->id == id)
            {
                result = entity;
                break;
            }
        }
    }

    return result;
}

Entity* findEntityOfType(GameContext* context, EntityType type)
{
    Entity* result = NULL;

    Room* room = context->room;
    if (room)
    {
        for (s32 i = 0; i < room->entitiesCount; i++)
        {
            Entity* entity = room->entities[i];
            if (entity && entity->type == type)
            {
                result = entity;
                break;
            }
        }
    }

    Menu* menu = context->menu;
    if (menu)
    {
        for (s32 i = 0; i < menu->entitiesCount; i++)
        {
            Entity* entity = menu->entities[i];
            if (entity && entity->type == type)
            {
                result = entity;
                break;
            }
        }
    }

    return result;
}

Entity* findUIEntity(GameContext* context, const char* name)
{
    Entity* result = NULL;

    Room* room = context->room;
    if (room)
    {
        for (s32 i = 0; i < room->entitiesCount; i++)
        {
            Entity* entity = room->entities[i];
            if (entity)
            {
                bool isUIEntity = entity->type == ENTITY_TYPE_TEXT ||
                                  entity->type == ENTITY_TYPE_RECT ||
                                  entity->type == ENTITY_TYPE_BUTTON;

                if (isUIEntity && strEquals(entity->ui.name, name))
                {
                    result = entity;
                    break;
                }
            }
        }
    }

    Menu* menu = context->menu;
    if (menu)
    {
        for (s32 i = 0; i < menu->entitiesCount; i++)
        {
            Entity* entity = menu->entities[i];
            if (entity)
            {
                bool isUIEntity = entity->type == ENTITY_TYPE_TEXT ||
                                  entity->type == ENTITY_TYPE_RECT ||
                                  entity->type == ENTITY_TYPE_BUTTON;

                if (isUIEntity && strEquals(entity->ui.name, name))
                {
                    result = entity;
                    break;
                }
            }
        }
    }

    return result;
}

Entity* findBedAt(GameContext* context, s32 row, s32 col, s32 index)
{
    Entity* result = NULL;

    Room* room = context->room;
    if (room)
    {
        for (s32 i = 0; i < room->entitiesCount; i++)
        {
            Entity* entity = room->entities[i];
            if (entity && entity->type == ENTITY_TYPE_BED)
            {
                BedComponent* bed = &entity->bed;
                if (bed->row == row && bed->col == col && bed->index == index)
                {
                    result = entity;
                    break;
                }

            }
        }
    }

    return result;
}

Entity* findVentilatorAt(GameContext* context, s32 row, s32 col)
{
    Entity* result = NULL;

    Room* room = context->room;
    if (room)
    {
        for (s32 i = 0; i < room->entitiesCount; i++)
        {
            Entity* entity = room->entities[i];
            if (entity && entity->type == ENTITY_TYPE_VENTILATOR)
            {
                VentilatorComponent* ventilator = &entity->ventilator;
                if (ventilator->row == row && ventilator->col == col)
                {
                    result = entity;
                    break;
                }

            }
        }
    }

    return result;
}

void clearUIText(Entity* uiText)
{
    if (uiText->text.text)
    {
        free(uiText->text.text);
        uiText->text.text = NULL;
        uiText->text.enabled = false;
    }
}

void setUIText(Entity* uiText, const char* text)
{
    clearUIText(uiText);

    if (text)
    {
        uiText->text.text = (char*)xmalloc(strlen(text) + 1);
        strcpy(uiText->text.text, text);
        uiText->text.enabled = true;
    }
}

void setUITextFormatv(Entity* uiText, const char* format, va_list args)
{
    if (!format)
    {
        setUIText(uiText, NULL);
        return;
    }

	char buffer[256];
    vsprintf(buffer, format, args);
    setUIText(uiText, buffer);
}

void setUITextFormat(Entity* uiText, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    setUITextFormatv(uiText, format, args);
    va_end(args);
}

void addTransformComponent(GameContext* context, Entity* entity, vec2 position, vec2 scale)
{
    entity->transform = (TransformComponent){0};
    entity->transform.enabled = true;
    entity->transform.position = position;
    entity->transform.rotation = VEC2_ZERO;
    entity->transform.scale = scale;
}

void addSpriteComponent(GameContext* context, Entity* entity, Sprite sprite)
{
    entity->sprite = (SpriteComponent){0};
    entity->sprite.enabled = true;
    entity->sprite.frameIndex = 0;
    entity->sprite.resourceIndex = 0;
    entity->sprite.sprite = sprite;
}

void addSpriteComponentFromFile(GameContext* context, Entity* entity, const char* fileName)
{
    int w, h, n;
    unsigned char *data = stbi_load(fileName, &w, &h, &n, 4);

    Sprite sprite = createSprite(context, w, h, data);
    sprite.frames[0].dx = 2;
    sprite.frames[0].dx = 2;
    sprite.frames[0].w = sprite.frameWidth - 4;
    sprite.frames[0].h = sprite.frameHeight - 4;

    addSpriteComponent(context, entity, sprite);

    stbi_image_free(data);
}

void removeSpriteComponent(GameContext* context, Entity* entity)
{
    freeSprite(context, entity->sprite.sprite);

    entity->sprite = (SpriteComponent){0};
}

void addUIComponent(GameContext* context, Entity* entity, const char* name)
{
    entity->ui = (UIComponent){0};
    entity->ui.enabled = true;
    entity->ui.name = (char*)name;
}

void addTextComponent(GameContext* context, Entity* entity, s32 fontIndex, f32 fontSize, const char* text)
{
    entity->text = (TextComponent){0};
    entity->text.enabled = true;
    entity->text.fontIndex = fontIndex;
    entity->text.fontSize = fontSize;
    entity->text.lineHeight = 0;
    entity->text.fontColor = FONT_NORMAL_COLOR;
    entity->text.highlightColor = FONT_HIGHLIGHT_COLOR;
    entity->text.highlightIndex = NO_HIGHLIGHT;
    entity->text.highlightCount = 0;
    entity->text.boundings = VEC2_ZERO;
    entity->text.horizontalAlign = TEXT_ALIGN_LEFT;
    entity->text.verticalAlign = TEXT_ALIGN_TOP;
    entity->text.lineAlign = TEXT_ALIGN_LEFT;
    entity->text.wrapping = TEXT_WRAP_NONE;
    entity->text.multiline = false;

    setUIText(entity, text);
}

void addNurseComponent(GameContext* context, Entity* entity, s32 row, s32 col, s32 index)
{
    entity->nurse = (NurseComponent){0};
    entity->nurse.enabled = true;
    entity->nurse.action = ACTION_IDLE;
    entity->nurse.row = row;
    entity->nurse.col = col;
    entity->nurse.index = index;
    entity->nurse.pathIndex = 0;
    entity->nurse.pathLastIndex = 0;
    entity->nurse.pathLength = 0;
}

void addBedComponent(GameContext* context, Entity* entity, s32 row, s32 col, s32 index, s32 life)
{
    entity->bed = (BedComponent){0};
    entity->bed.enabled = true;
    entity->bed.row = row;
    entity->bed.col = col;
    entity->bed.index = index;
    entity->bed.life = life;
}

void addVentilatorComponent(GameContext* context, Entity* entity, s32 row, s32 col, s32 dir)
{
    entity->ventilator = (VentilatorComponent){0};
    entity->ventilator.enabled = true;
    entity->ventilator.row = row;
    entity->ventilator.col = col;
    entity->ventilator.dir = dir;
}

void addRectComponent(GameContext* context, Entity* entity, vec2 size, NVGcolor color)
{
    entity->rect = (RectComponent){0};
    entity->rect.enabled = true;
    entity->rect.size = size;;
    entity->rect.color = color;
}

void renderImage(GameContext* context, Entity* entity)
{
    NVGcontext* gfx = context->gfx;

    TransformComponent* transform = &entity->transform;
    SpriteComponent* sprite = &entity->sprite;

    if (sprite->enabled && sprite->frameIndex >= 0)
    {
        nvgSave(gfx);

        if (sprite->sprite.framesCount > 1)
        {
            SpriteFrame frame = getSpriteFrame(context, sprite->sprite, sprite->frameIndex);
            updateSpriteImage(context, sprite->sprite, frame.data);

            nvgTranslate(gfx, -frame.dx, -frame.dy);
        }

        nvgTranslate(gfx, transform->position.x, transform->position.y);
        renderSprite(context, sprite->sprite, VEC2_ZERO, transform->scale);
        nvgRestore(gfx);
    }
}

void renderText(GameContext* context, Entity* entity)
{
    NVGcontext* gfx = context->gfx;

    TransformComponent* transform = &entity->transform;
    UIComponent* ui = &entity->ui;
    TextComponent* text = &entity->text;

    if (ui->enabled && text->enabled && text->text)
    {
        nvgSave(gfx);
        nvgTranslate(gfx, transform->position.x, transform->position.y);
        nvgScale(gfx, transform->scale.x, transform->scale.y);

        NVGfontParams params;
        params.fontIndex = text->fontIndex;
        params.fontSize = text->fontSize;
        params.lineHeight = text->lineHeight;
        params.fontColor = u8ColorToNVGcolor(text->fontColor);
        params.highlightColor = u8ColorToNVGcolor(text->highlightColor);
        params.highlightIndex = text->highlightIndex;
        params.highlightCount = text->highlightCount;
        params.boundings = text->boundings;
        params.horizontalAlign = textAlignToNVGalign(text->horizontalAlign);
        params.verticalAlign = textAlignToNVGalign(text->verticalAlign);
        params.lineAlign = textAlignToNVGalign(text->lineAlign);
        params.wrapping = textWrappingToNVGwrap(text->wrapping);
        params.trimming = textTrimmingToNVGtrim(text->trimming);
        params.fontSprite = context->fontSprites[text->fontIndex];
        params.fontData = fontsData[text->fontIndex];

        if (entity->text.multiline)
            nvgMultiSpriteText(gfx, text->text, 0, 0, params);
        else
            nvgSingleSpriteText(gfx, text->text, 0, 0, params);

        nvgRestore(gfx);
    }
}

void renderNurse(GameContext* context, Entity* entity)
{
    NVGcontext* gfx = context->gfx;

    TransformComponent* transform = &entity->transform;
    SpriteComponent* sprite = &entity->sprite;

    if (sprite->enabled && sprite->frameIndex >= 0)
    {
        nvgSave(gfx);

        if (sprite->sprite.framesCount > 1)
        {
            SpriteFrame frame = getSpriteFrame(context, sprite->sprite, sprite->frameIndex);
            updateSpriteImage(context, sprite->sprite, frame.data);

            nvgTranslate(gfx, -frame.dx, -frame.dy);
        }

        nvgTranslate(gfx, -sprite->offset.x, -sprite->offset.y);

        nvgTranslate(gfx, transform->position.x, transform->position.y);
        renderSprite(context, sprite->sprite, VEC2_ZERO, transform->scale);
        nvgRestore(gfx);
    }
}

void renderBed(GameContext* context, Entity* entity)
{
    NVGcontext* gfx = context->gfx;

    TransformComponent* transform = &entity->transform;
    SpriteComponent* sprite = &entity->sprite;
    BedComponent* bed = &entity->bed;

    if (sprite->enabled && sprite->frameIndex >= 0)
    {
        nvgSave(gfx);

        if (sprite->sprite.framesCount > 1)
        {
            SpriteFrame frame = getSpriteFrame(context, sprite->sprite, sprite->frameIndex);
            updateSpriteImage(context, sprite->sprite, frame.data);

            nvgTranslate(gfx, -frame.dx, -frame.dy);
        }

        nvgTranslate(gfx, -sprite->offset.x, -sprite->offset.y);
        nvgTranslate(gfx, transform->position.x, transform->position.y);
        nvgScale(gfx, transform->scale.x, transform->scale.y);

        nvgSave(gfx);
        nvgFillRect(gfx, recti(2, 6, sprite->sprite.frameWidth - 4, sprite->sprite.frameHeight - 4), NVG_BLACK_TRANSPARENT);
        nvgRestore(gfx);

        if (bed->highlighted)
        {
            nvgSave(gfx);
            renderSprite(context, sprite->sprite, VEC2_ZERO, vec2f(1.1f, 1.1f));
            nvgRestore(gfx);
        }

        renderSprite(context, sprite->sprite, VEC2_ZERO, VEC2_ONE);

        nvgStrokeRect(gfx, recti(2, -4, 12, 4), NVG_BLACK, 1.0f);

        NVGcolor color;
        if (bed->life < 40)
            color = NVG_RED;
        else if (bed->life < 80)
            color = NVG_CYAN;
        else
            color = NVG_GREEN;

        f32 percent = bed->life / 100.0f;
        nvgFillRect(gfx, recti(3, -3, percent * 10, 2), color);

        nvgRestore(gfx);
    }
}

void renderVentilator(GameContext* context, Entity* entity)
{
    NVGcontext* gfx = context->gfx;

    TransformComponent* transform = &entity->transform;
    SpriteComponent* sprite = &entity->sprite;
    VentilatorComponent* ventilator = &entity->ventilator;

    if (sprite->enabled && sprite->frameIndex >= 0)
    {
        nvgSave(gfx);

        if (sprite->sprite.framesCount > 1)
        {
            SpriteFrame frame = getSpriteFrame(context, sprite->sprite, sprite->frameIndex);
            updateSpriteImage(context, sprite->sprite, frame.data);

            nvgTranslate(gfx, -frame.dx, -frame.dy);
        }

        nvgTranslate(gfx, -sprite->offset.x, -sprite->offset.y);
        nvgTranslate(gfx, transform->position.x, transform->position.y);
        nvgScale(gfx, transform->scale.x, transform->scale.y);

        if (ventilator->highlighted)
        {
            nvgSave(gfx);
            renderSprite(context, sprite->sprite, VEC2_ZERO, vec2f(1.1f, 1.1f));
            nvgRestore(gfx);
        }

        renderSprite(context, sprite->sprite, VEC2_ZERO, VEC2_ONE);
        nvgRestore(gfx);
    }
}

void renderRect(GameContext* context, Entity* entity)
{
    NVGcontext* gfx = context->gfx;

    TransformComponent* transform = &entity->transform;
    UIComponent* ui = &entity->ui;
    RectComponent* rect = &entity->rect;

    if (ui->enabled && rect->enabled)
    {
        nvgSave(gfx);

        nvgTranslate(gfx, transform->position.x, transform->position.y);
        nvgFillRect(gfx, rectf(0, 0, rect->size.x, rect->size.y), rect->color);

        nvgRestore(gfx);
    }
}

void renderEntity(GameContext* context, Entity* entity)
{
    static GameRenderFunc renderFuncs[ENTITY_TYPE_COUNT] =
    {
        NULL,               // ENTITY_TYPE_NONE
        renderImage,        // ENTITY_TYPE_IMAGE
        renderNurse,        // ENTITY_TYPE_NURSE,
        renderBed,          // ENTITY_TYPE_BED,
        renderVentilator,   // ENTITY_TYPE_VENTILATOR,
        renderText,         // ENTITY_TYPE_TEXT,
        renderRect,         // ENTITY_TYPE_RECT,
        NULL,               // ENTITY_TYPE_BUTTON,
    };

    NVGcontext* gfx = context->gfx;

    if (entity->id)
    {
        GameRenderFunc renderFunc = renderFuncs[(s32)entity->type];
        if (!renderFunc)
        {
            logError("Entity of type %d can't be render. renderFunc = NULL\n", entity->type);
            return;
        }

        nvgSave(gfx);
        renderFunc(context, entity);
        nvgRestore(gfx);
    }
}