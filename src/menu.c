Menu* initMenu(GameContext* context)
{
    Menu* menu = (Menu*)xcalloc(1, sizeof(Menu));
    menu->step = 0;

    Entity* entity;

    entity = createText(context, vec2i(0, 20), "Title", 15, "Keep Them Alive");
    setUITextBoundings(entity, vec2i(context->originalWindowWidth, context->originalWindowHeight));
    setUITextHorizontalAlign(entity, TEXT_ALIGN_CENTER);

    entity = createText(context, vec2i(0, 90), "Desc", 6, "There is only one ventilator for each pair of beds.\nSwitch it from one patient to another.");
    setUITextBoundings(entity, vec2i(ORIGINAL_WINDOW_WIDTH, 200));
    setUITextHorizontalAlign(entity, TEXT_ALIGN_CENTER);
    setUITextLineAlign(entity, TEXT_ALIGN_CENTER);
    setUITextWrapping(entity, TEXT_WRAP_CHAR);
    setUITextMultiline(entity, true);

    entity = createText(context, vec2i(0, 120), "Warning1", 10, "You just need to keep the patients alive");
    setUITextBoundings(entity, vec2i(context->originalWindowWidth, context->originalWindowHeight));
    setUITextHorizontalAlign(entity, TEXT_ALIGN_CENTER);
    setUIEntityStatus(entity, false);

    entity = createText(context, vec2i(0, 145), "Warning2", 10, "Seriously, keep them alive");
    setUITextBoundings(entity, vec2i(context->originalWindowWidth, context->originalWindowHeight));
    setUITextHorizontalAlign(entity, TEXT_ALIGN_CENTER);
    setUIEntityStatus(entity, false);

    entity = createText(context, vec2i(0, 170), "Warning3", 10, "Did you hear me?");
    setUITextBoundings(entity, vec2i(context->originalWindowWidth, context->originalWindowHeight));
    setUITextHorizontalAlign(entity, TEXT_ALIGN_CENTER);
    setUIEntityStatus(entity, false);

    entity = createText(context, vec2i(20, 0), "FootNote", 6, "Play with keyboard or mouse. Press Enter or Space to continue.");
    setUITextBoundings(entity, vec2i(context->originalWindowWidth - 25, context->originalWindowHeight));
    setUITextHorizontalAlign(entity, TEXT_ALIGN_CENTER);
    setUITextVerticalAlign(entity, TEXT_ALIGN_BOTTOM);
    setUITextLineAlign(entity, TEXT_ALIGN_CENTER);
    setUITextMultiline(entity, true);

    vec2 leftBedPos = vec2i((ORIGINAL_WINDOW_WIDTH / 2) - (BED_PAIR_WIDTH / 2), 50);
    vec2 rightBedPos = vec2i(leftBedPos.x + BED_WIDTH + VENTILATOR_WIDTH, 50);
    createBed(context, leftBedPos, 0, 0, 0, randomi(40, 60), true);
    createBed(context, rightBedPos, 0, 0, 1, randomi(40, 60), false);
    createVentilator(context, vec2Addv(leftBedPos, vec2i(BED_WIDTH, 0)), 0, 0, -1);

    return menu;
}

void updateMenu(GameContext* context)
{
    GameInput* input = &context->input;

    Menu* menu = context->menu;

    if ((context->time - context->deltaTime) < floorf(context->time))
    {
        Entity* ventilator = NULL;
        Entity* leftBed = NULL;
        Entity* rightBed = NULL;

        for (s32 i = 0; i < menu->entitiesCount; i++)
        {
            Entity* entity = menu->entities[i];
            if (entity)
            {
                if (entity->type == ENTITY_TYPE_VENTILATOR)
                    ventilator = entity;
                else if (entity->type == ENTITY_TYPE_BED)
                {
                    if (leftBed)
                        rightBed = entity;
                    else
                        leftBed = entity;
                }
            }
        }

        ventilator->ventilator.dir *= -1;
        if (ventilator->ventilator.dir < 0)
        {
            removeSpriteComponent(context, ventilator);
            addSpriteComponentFromFile(context, ventilator, "ventilator_left.png");

            leftBed->bed.ventilator = true;

            removeSpriteComponent(context, leftBed);
            addSpriteComponentFromFile(context, leftBed, "left_bed_with_patient_with_mask.png");

            removeSpriteComponent(context, rightBed);
            addSpriteComponentFromFile(context, rightBed, "bed_with_patient.png");
        }
        else
        {
            removeSpriteComponent(context, ventilator);
            addSpriteComponentFromFile(context, ventilator, "ventilator_right.png");

            rightBed->bed.ventilator = true;

            removeSpriteComponent(context, leftBed);
            addSpriteComponentFromFile(context, leftBed, "bed_with_patient.png");

            removeSpriteComponent(context, rightBed);
            addSpriteComponentFromFile(context, rightBed, "right_bed_with_patient_with_mask.png");
        }
    }

    if (wasKeyPressed(input, GAME_KEY_ENTER) ||
        wasKeyPressed(input, GAME_KEY_SPACE))
    {
        menu->step++;

        if (menu->step >= 4)
        {
            for (s32 i = 0; i < menu->entitiesCount; i++)
            {
                Entity* entity = menu->entities[i];
                if (entity)
                {
                    removeEntity(context, entity);
                }
            }

            context->room = (Room*)xcalloc(1, sizeof(Room));
            initRoom(context);
        }
        else
        {
            Entity* warning1 = findUIEntity(context, "Warning1");
            setUIEntityStatus(warning1, menu->step >= 1);

            Entity* warning2 = findUIEntity(context, "Warning2");
            setUIEntityStatus(warning2, menu->step >= 2);

            Entity* warning3 = findUIEntity(context, "Warning3");
            setUIEntityStatus(warning3, menu->step >= 3);
        }
    }
}

void renderMenu(GameContext* context)
{
    NVGcontext* gfx = context->gfx;

    nvgSave(gfx);
    nvgScale(gfx, context->globalScale, context->globalScale);

    Menu* menu = context->menu;

    for (s32 i = 0; i < menu->entitiesCount; i++)
    {
        Entity* entity = menu->entities[i];
        if (entity)
        {
            renderEntity(context, entity);
        }
    }

    nvgRestore(gfx);
}
