void initRoom(GameContext* context)
{
    Room* room = context->room;

    for (s32 i = 0; i < FLOOR_TILE_ROWS; i++)
    {
        for (s32 j = 0; j < FLOOR_TILE_COLS; j++)
        {
            createImage(context, vec2i(j * FLOOR_TILE_WIDTH, 20 + i * FLOOR_TILE_HEIGHT), "floor_tile.png");
        }
    }

    for (s32 i = 0; i < BED_PAIRS_PER_ROW; i++)
    {
        for (s32 j = 0; j < BED_PAIRS_PER_COL; j++)
        {
            vec2 leftBedPos = vec2i(ROOM_PADDING + j * (BED_PAIR_WIDTH + BED_PAIR_H_SPACING), ROOM_PADDING + i * (BED_PAIR_HEIGHT + BED_PAIR_V_SPACING));
            vec2 rightBedPos = vec2i(ROOM_PADDING + j * (BED_PAIR_WIDTH + BED_PAIR_H_SPACING) + BED_WIDTH + VENTILATOR_WIDTH, ROOM_PADDING + i * (BED_PAIR_HEIGHT + BED_PAIR_V_SPACING));

            bool leftBedWithVentilator = chance(50);
            createBed(context, leftBedPos, i, j, 0, randomi(40, 60), leftBedWithVentilator);
            createBed(context, rightBedPos, i, j, 1, randomi(40, 60), !leftBedWithVentilator);

            createVentilator(context, vec2Addv(leftBedPos, vec2i(BED_WIDTH, 0)), i, j, leftBedWithVentilator ? -1 : 1);

            room->points[room->pointsCount++] = vec2Addv(leftBedPos, vec2f(-0.5f * VENTILATOR_WIDTH, 0.8f * BED_HEIGHT));
            room->points[room->pointsCount++] = vec2Addv(rightBedPos, vec2f(-0.5f * VENTILATOR_WIDTH, 0.66 * BED_HEIGHT));
            room->points[room->pointsCount++] = vec2Addv(rightBedPos, vec2f(BED_WIDTH + 0.5f * VENTILATOR_WIDTH, 0.8f *  BED_HEIGHT));

            room->points[room->pointsCount++] = vec2Addv(leftBedPos, vec2f(-0.5f * VENTILATOR_WIDTH, BED_HEIGHT + 0.75f * BED_PAIR_V_SPACING));
            room->points[room->pointsCount++] = vec2Addv(rightBedPos, vec2f(-0.5f * VENTILATOR_WIDTH, BED_HEIGHT + 0.75f * BED_PAIR_V_SPACING));
            room->points[room->pointsCount++] = vec2Addv(rightBedPos, vec2f(BED_WIDTH + 0.5f * VENTILATOR_WIDTH, BED_HEIGHT + 0.75f * BED_PAIR_V_SPACING));

            room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j, 0);
            room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j, 3);

            room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j, 1);
            room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j, 4);

            room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j, 2);
            room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j, 5);

            room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j, 3);
            room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j, 4);

            room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j, 4);
            room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j, 5);

            if (j > 0)
            {
                room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j - 1, 5);
                room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j, 3);

                room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j - 1, 2);
                room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j, 0);
            }

            if (i > 0)
            {
                room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i - 1, j, 3);
                room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j, 0);

                room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i - 1, j, 5);
                room->pathWays[room->pathWaysCount++] = pointIndexAt(context, i, j, 2);
            }
        }
    }

    createNurse(context, randomi(0, BED_PAIRS_PER_COL), randomi(0, BED_PAIRS_PER_COL), randomi(0, 6));

    createText(context, vec2i(context->originalWindowWidth - 300, 5), "txtLives", 10, NULL);
    createText(context, vec2i(context->originalWindowWidth - 200, 5), "txtDeaths", 10, NULL);
    createText(context, vec2i(context->originalWindowWidth - 100, 5), "txtRecovered", 10, NULL);

    Entity* entity = createRect(context, "rectGameOverBackdrop", vec2i(10, 10), vec2i(ORIGINAL_WINDOW_WIDTH - 20, ORIGINAL_WINDOW_HEIGHT - 20), NVG_GRAY_TRANSPARENT);
    setUIEntityStatus(entity, false);

    entity = createText(context, vec2i(0, 0), "txtGameOver", 15, NULL);
    setUITextBoundings(entity, vec2i(ORIGINAL_WINDOW_WIDTH, ORIGINAL_WINDOW_HEIGHT));
    setUITextHorizontalAlign(entity, TEXT_ALIGN_CENTER);
    setUITextVerticalAlign(entity, TEXT_ALIGN_MIDDLE);
    setUITextLineAlign(entity, TEXT_ALIGN_CENTER);
    setUITextMultiline(entity, true);
    setUIEntityStatus(entity, false);
}

Entity* createNurse(GameContext* context, s32 row, s32 col, s32 index)
{
    Entity* entity = createEntity(context, ENTITY_TYPE_NURSE);
    addNurseComponent(context, entity, row, col, index);

    s32 startPointIndex = pointIndexAt(context, row, col, index);
    vec2 startPoint = context->room->points[startPointIndex];
    addTransformComponent(context, entity, startPoint, VEC2_ONE);
    addSpriteComponentFromFile(context, entity, "nurse.png");
    entity->sprite.offset = vec2i(8, 24);
    return entity;
}

Entity* createBed(GameContext* context, vec2 position, s32 row, s32 col, s32 index, s32 life, bool ventilator)
{
    Entity* entity = createEntity(context, ENTITY_TYPE_BED);
    addTransformComponent(context, entity, position, VEC2_ONE);
    addBedComponent(context, entity, row, col, index, life);
    entity->bed.ventilator = ventilator;
    if (entity->bed.ventilator && index == 0)
        addSpriteComponentFromFile(context, entity, "left_bed_with_patient_with_mask.png");
    else if (entity->bed.ventilator && index == 1)
        addSpriteComponentFromFile(context, entity, "right_bed_with_patient_with_mask.png");
    else
        addSpriteComponentFromFile(context, entity, "bed_with_patient.png");
    return entity;
}

Entity* createVentilator(GameContext* context, vec2 position, s32 row, s32 col, s32 dir)
{
    Entity* entity = createEntity(context, ENTITY_TYPE_VENTILATOR);
    addTransformComponent(context, entity, position, VEC2_ONE);
    addSpriteComponentFromFile(context, entity, dir < 0 ? "ventilator_left.png" : "ventilator_right.png");
    entity->sprite.sprite.frames[0].dx = 0;
    entity->sprite.sprite.frames[0].dy = 0;
    addVentilatorComponent(context, entity, row, col, dir);
    return entity;
}

void addPointToPath(GameContext* context, Entity* entity, s32 pointIndex)
{
    NurseComponent* nurse = &entity->nurse;
    if (nurse->pathLength + 1 < MAX_POINTS_COUNT)
    {
        nurse->pathLength++;
        nurse->path[nurse->pathLastIndex] = pointIndex;
        nurse->pathLastIndex = (nurse->pathLastIndex + 1) % MAX_POINTS_COUNT;
    }
}

void resetPath(GameContext* context, Entity* entity)
{
    NurseComponent* nurse = &entity->nurse;
    nurse->pathLength = 0;
    nurse->pathIndex = 0;
    nurse->pathLastIndex = 0;
}

void updateNurse(GameContext* context)
{
    GameInput* input = &context->input;
    Room* room = context->room;

    Entity* entity = findEntityOfType(context, ENTITY_TYPE_NURSE);
    if (entity)
    {
        TransformComponent* transform = &entity->transform;

        NurseComponent* nurse = &entity->nurse;
        s32 row = nurse->row;
        s32 col = nurse->col;
        s32 index = nurse->index;

        switch (nurse->action)
        {
            case ACTION_WALK:
            {
                if (nurse->pathLength > 0)
                {
                    s32 pointIndex = nurse->path[nurse->pathIndex];
                    vec2 destPosition = room->points[pointIndex];
                    f32 distance = vec2Distance(transform->position, destPosition);
                    if (distance > 0.5f)
                    {
                        vec2 dir = vec2Normalize(vec2Subv(destPosition, transform->position));
                        f32 length = min(200 * context->deltaTime, distance);;
                        vec2 delta = vec2Mulf(dir, length);
                        transform->position = vec2Addv(transform->position, delta);
                    }
                    else
                    {
                        transform->position = destPosition;

                        pointRowColIndex(context, pointIndex, &nurse->row, &nurse->col, &nurse->index);

                        if (nurse->index == 0 || nurse->index == 2)
                            nurse->action = ACTION_CARE;
                        else if (nurse->index == 1)
                            nurse->action = ACTION_SWITCH;

                        nurse->pathLength--;
                        nurse->pathIndex = (nurse->pathIndex + 1) % MAX_POINTS_COUNT;
                    }
                }
                else
                {
                    resetPath(context, entity);
                    nurse->action = ACTION_IDLE;
                }
                break;
            }

            case ACTION_CARE:
            {
                Entity* entity = findBedAt(context, nurse->row, nurse->col, nurse->index == 0 ? 0 : 1);
                if (entity)
                {
                    if (entity->bed.life > 0)
                    {
                        entity->bed.life += randomi(3, 6);
                        entity->bed.life = clamp(entity->bed.life, 0, 100);
                    }
                }

                if (nurse->pathLength > 0)
                {
                    nurse->action = ACTION_WALK;
                }
                else
                {
                    resetPath(context, entity);
                    nurse->action = ACTION_IDLE;
                }

                break;
            }

            case ACTION_SWITCH:
            {
                Entity* entity = findVentilatorAt(context, nurse->row, nurse->col);
                if (entity)
                {
                    Entity* leftBed = findBedAt(context, nurse->row, nurse->col, 0);
                    bool leftBedHasLife = leftBed->bed.life > 0 && leftBed->bed.life < 100;

                    Entity* rightBed = findBedAt(context, nurse->row, nurse->col, 1);
                    bool rightBedHasLife = rightBed->bed.life > 0 && rightBed->bed.life < 100;

                    entity->ventilator.dir = entity->ventilator.dir < 0 ? 1 : -1;
                    if (entity->ventilator.dir < 0)
                    {
                        removeSpriteComponent(context, entity);
                        addSpriteComponentFromFile(context, entity, leftBedHasLife ? "ventilator_left.png" : "ventilator_left_no_patient.png");
                        entity->sprite.sprite.frames[0].dx = 0;
                        entity->sprite.sprite.frames[0].dy = 0;

                        if (leftBedHasLife)
                        {
                            leftBed->bed.ventilator = true;

                            removeSpriteComponent(context, leftBed);
                            addSpriteComponentFromFile(context, leftBed, "left_bed_with_patient_with_mask.png");
                        }

                        if (rightBedHasLife)
                        {
                            rightBed->bed.ventilator = false;

                            removeSpriteComponent(context, rightBed);
                            addSpriteComponentFromFile(context, rightBed, "bed_with_patient.png");
                        }
                    }
                    else if (entity->ventilator.dir > 0)
                    {
                        removeSpriteComponent(context, entity);
                        addSpriteComponentFromFile(context, entity, rightBedHasLife ? "ventilator_right.png" : "ventilator_right_no_patient.png");
                        entity->sprite.sprite.frames[0].dx = 0;
                        entity->sprite.sprite.frames[0].dy = 0;

                        if (leftBedHasLife)
                        {
                            leftBed->bed.ventilator = false;

                            removeSpriteComponent(context, leftBed);
                            addSpriteComponentFromFile(context, leftBed, "bed_with_patient.png");
                        }

                        if (rightBedHasLife)
                        {
                            rightBed->bed.ventilator = true;

                            removeSpriteComponent(context, rightBed);
                            addSpriteComponentFromFile(context, rightBed, "right_bed_with_patient_with_mask.png");
                        }
                    }
                }

                if (nurse->pathLength > 0)
                {
                    nurse->action = ACTION_WALK;
                }
                else
                {
                    resetPath(context, entity);
                    nurse->action = ACTION_IDLE;
                }

                break;
            }

            default:
            {
                if (isKeyPressed(input, GAME_KEY_UP))
                {
                    if (index >= 3)
                    {
                        index -= 3;
                    }
                    else if (index != 1 && row > 0)
                    {
                        row -= 1;
                        index += 3;
                    }
                }
                else if (isKeyPressed(input, GAME_KEY_DOWN))
                {
                    if (index < 3)
                    {
                        index += 3;
                    }
                    else if (index != 4 && row < BED_PAIRS_PER_ROW - 1)
                    {
                        row += 1;
                        index -= 3;
                    }
                }
                else if (isKeyPressed(input, GAME_KEY_LEFT))
                {
                    if (index > 3)
                    {
                        index -= 1;
                    }
                    else if ((index == 0 || index == 3) && col > 0)
                    {
                        col -= 1;
                        index += 2;
                    }
                }
                else if (isKeyPressed(input, GAME_KEY_RIGHT))
                {
                    if (index >= 3 && index < 5)
                    {
                        index += 1;
                    }
                    else if ((index == 2 ||index == 5) && col < BED_PAIRS_PER_COL - 1)
                    {
                        col += 1;
                        index -= 2;
                    }
                }

                if (pointIndexValid(context, row, col, index) &&
                    (row != nurse->row || col != nurse->col || index != nurse->index))
                {
                    resetPath(context, entity);
                    addPointToPath(context, entity, pointIndexAt(context, row, col, index));
                    nurse->action = ACTION_WALK;
                }
            }
        }
    }
}

void determinePath(GameContext* context, s32 startPointIndex, s32 endPointIndex, s32List* path)
{
    Room* room = context->room;

    s32 parents[MAX_POINTS_COUNT];
    for (s32 i = 0; i < MAX_POINTS_COUNT; i++)
        parents[i] = -1;

    s32ListAdd(path, startPointIndex);

    bool found = false;
    for (s32 i = 0; i < path->count; i++)
    {
        s32 pointIndex = path->items[i];
        if (pointIndex == endPointIndex)
        {
            found = true;
            break;
        }

        for (s32 k = 0; k < room->pathWaysCount - 1; k += 2)
        {
            if (pointIndex == room->pathWays[k])
            {
                if (!s32ListContains(path, room->pathWays[k + 1]))
                {
                    s32ListAdd(path, room->pathWays[k + 1]);
                    parents[room->pathWays[k + 1]] = pointIndex;
                }
            }
            else if (pointIndex == room->pathWays[k + 1])
            {
                if (!s32ListContains(path, room->pathWays[k]))
                {
                    s32ListAdd(path, room->pathWays[k]);
                    parents[room->pathWays[k]] = pointIndex;
                }
            }
        }
    }

    s32ListClear(path);

    if (found)
    {
        s32 pointIndex = endPointIndex;
        while (parents[pointIndex] != -1)
        {
            s32ListAdd(path, pointIndex);
            pointIndex = parents[pointIndex];
        }

        s32ListReverse(path);
    }
}

void updateBed(GameContext* context, Entity* entity)
{
    GameInput* input = &context->input;

    vec2 mousePos = input->pos;

    TransformComponent* transform = &entity->transform;
    SpriteComponent* sprite = &entity->sprite;
    BedComponent* bed = &entity->bed;

    rect r = rectf(transform->position.x, transform->position.y, sprite->sprite.frameWidth, sprite->sprite.frameHeight);
    bed->highlighted = rectContainsf(r, mousePos.x, mousePos.y);

    // if a whole second has passed
    if ((context->time - context->deltaTime) < floorf(context->time))
    {
        if (bed->life > 0 && bed->life < 100)
        {
            s32 dl = randomi(0, 3);
            bed->life += bed->ventilator ? dl : -dl;
            bed->life = clamp(bed->life, 0, 100);
        }
        else
        {
            removeSpriteComponent(context, entity);
            addSpriteComponentFromFile(context, entity, "bed_without_patient.png");

            if (bed->ventilator)
            {
                Entity* ventilator = findVentilatorAt(context, bed->row, bed->col);

                if (bed->index == 0)
                {
                    removeSpriteComponent(context, ventilator);
                    addSpriteComponentFromFile(context, ventilator, "ventilator_left_no_patient.png");
                    ventilator->sprite.sprite.frames[0].dx = 0;
                    ventilator->sprite.sprite.frames[0].dy = 0;
                }
                else
                {
                    removeSpriteComponent(context, ventilator);
                    addSpriteComponentFromFile(context, ventilator, "ventilator_right_no_patient.png");
                    ventilator->sprite.sprite.frames[0].dx = 0;
                    ventilator->sprite.sprite.frames[0].dy = 0;
                }
            }

            bed->ventilator = false;
        }
    }

    if (bed->highlighted && wasButtonPressed(input, GAME_MOUSE_LEFT))
    {
        Entity* nurse = findEntityOfType(context, ENTITY_TYPE_NURSE);
        if (nurse)
        {
            s32List path;
            s32ListInit(&path, s32ListDefaultOptions);

            s32 startPointIndex = nurse->nurse.pathLength > 0
                ? nurse->nurse.path[(nurse->nurse.pathLastIndex - 1) % MAX_POINTS_COUNT]
                : pointIndexAt(context, nurse->nurse.row, nurse->nurse.col, nurse->nurse.index);

            s32 endPointIndex = pointIndexAt(context, bed->row, bed->col, bed->index == 0 ? 0 : 2);
            determinePath(context, startPointIndex, endPointIndex, &path);

            for (s32 i = 0; i < path.count; i++)
            {
                addPointToPath(context, nurse, path.items[i]);
            }

            s32ListFree(&path);

            if (nurse->nurse.pathLength > 0)
            {
                nurse->nurse.action = ACTION_WALK;
            }
        }
    }
}

void updateVentilator(GameContext* context, Entity* entity)
{
    GameInput* input = &context->input;

    vec2 mousePos = input->pos;

    TransformComponent* transform = &entity->transform;
    SpriteComponent* sprite = &entity->sprite;
    VentilatorComponent* ventilator = &entity->ventilator;

    rect r = rectf(transform->position.x, transform->position.y, sprite->sprite.frameWidth, sprite->sprite.frameHeight);
    ventilator->highlighted = rectContainsf(r, mousePos.x, mousePos.y);

    if (ventilator->highlighted && wasButtonPressed(input, GAME_MOUSE_LEFT))
    {
        Entity* nurse = findEntityOfType(context, ENTITY_TYPE_NURSE);
        if (nurse)
        {
            s32List path;
            s32ListInit(&path, s32ListDefaultOptions);

            s32 startPointIndex = nurse->nurse.pathLength > 0
                ? nurse->nurse.path[(nurse->nurse.pathLastIndex - 1) % MAX_POINTS_COUNT]
                : pointIndexAt(context, nurse->nurse.row, nurse->nurse.col, nurse->nurse.index);

            s32 endPointIndex = pointIndexAt(context, ventilator->row, ventilator->col, 1);
            determinePath(context, startPointIndex, endPointIndex, &path);

            for (s32 i = 0; i < path.count; i++)
            {
                addPointToPath(context, nurse, path.items[i]);
            }

            s32ListFree(&path);

            if (nurse->nurse.pathLength > 0)
            {
                nurse->nurse.action = ACTION_WALK;
            }
        }
    }
}

void countBeds(GameContext* context, s32* lives, s32* deaths, s32* recovered)
{
    Room* room = context->room;

    for (s32 i = 0; i < room->entitiesCount; i++)
    {
        Entity* entity = room->entities[i];
        if (entity && entity->type == ENTITY_TYPE_BED)
        {
            if (entity->bed.life == 0)
                (*deaths)++;
            else if (entity->bed.life == 100)
                (*recovered)++;
            else
                (*lives)++;
        }
    }
}

void updateUI(GameContext* context)
{
    s32 lives = 0, deaths = 0, recovered = 0;
    countBeds(context, &lives, &deaths, &recovered);

    Entity* txtLives = findUIEntity(context, "txtLives");
    setUITextFormat(txtLives, "LIVES: %d", lives);

    Entity* txtDeaths = findUIEntity(context, "txtDeaths");
    setUITextFormat(txtDeaths, "DEATHS: %d", deaths);

    Entity* txtRecovered = findUIEntity(context, "txtRecovered");
    setUITextFormat(txtRecovered, "RECOVERED: %d", recovered);
}

bool gameOver(GameContext* context)
{
    s32 lives = 0, deaths = 0, recovered = 0;
    countBeds(context, &lives, &deaths, &recovered);

    if (lives == 0)
    {
        Entity* rectGameOverBackdrop = findUIEntity(context, "rectGameOverBackdrop");
        setUIEntityStatus(rectGameOverBackdrop, true);

        Entity* txtGameOver = findUIEntity(context, "txtGameOver");
        setUIEntityStatus(txtGameOver, true);

        if (deaths == 0)
        {
            setUIText(txtGameOver, "Game Over\n\nYou are a hero! :D\n\nA game made by acoto87 for LD46\nThanks for playing\n");
        }
        if (deaths > recovered)
        {
            setUIText(txtGameOver, "Game Over\n\nIt is tough, huh? :(\n\nA game made by acoto87 for LD46\nThanks for playing\n");
        }
        else
        {
            setUIText(txtGameOver, "Game Over\n\nWell, at least a few survived,\nyou are still a hero\n\nA game made by acoto87 for LD46\nThanks for playing\n");
        }

        return true;
    }

    return false;
}

void updateRoom(GameContext* context)
{
    Room* room = context->room;

    if (gameOver(context))
    {
        return;
    }

    updateNurse(context);

    for (s32 i = 0; i < room->entitiesCount; i++)
    {
        Entity* entity = room->entities[i];
        if (entity && entity->type == ENTITY_TYPE_BED)
        {
            updateBed(context, entity);
        }
    }

    for (s32 i = 0; i < room->entitiesCount; i++)
    {
        Entity* entity = room->entities[i];
        if (entity && entity->type == ENTITY_TYPE_VENTILATOR)
        {
            updateVentilator(context, entity);
        }
    }

    updateUI(context);
}

void renderRoom(GameContext* context)
{
    NVGcontext* gfx = context->gfx;

    nvgSave(gfx);
    nvgScale(gfx, context->globalScale, context->globalScale);

    Room* room = context->room;

    // for (s32 i = 0; i < room->pathWaysCount - 1; i += 2)
    // {
    //     vec2 point1 = room->points[room->pathWays[i]];
    //     vec2 point2 = room->points[room->pathWays[i + 1]];
    //     nvgStrokeLine(gfx, point1, point2, getColorFromList(4), 1.0f);
    // }

    // for (s32 i = 0; i < room->pointsCount; i++)
    // {
    //     vec2 point = room->points[i];
    //     nvgFillRect(gfx, rectf(point.x - 1, point.y - 1, 2, 2), getColorFromList(3));
    // }

    for (s32 i = 0; i < room->entitiesCount; i++)
    {
        Entity* entity = room->entities[i];
        if (entity)
        {
            renderEntity(context, entity);
        }
    }

    nvgRestore(gfx);
}

inline s32 pointIndexAt(GameContext* context, s32 row, s32 col, s32 index)
{
    return (row * BED_PAIRS_PER_COL + col) * 6 + index;
}

inline void pointRowColIndex(GameContext* context, s32 pointIndex, s32* row, s32* col, s32* index)
{
    s32 t = pointIndex / 6;
    *row = t / BED_PAIRS_PER_COL;
    *col = t % BED_PAIRS_PER_COL;
    *index = pointIndex % 6;
}

inline bool pointIndexValid(GameContext* context, s32 row, s32 col, s32 index)
{
    s32 pointIndex = pointIndexAt(context, row, col, index);
    return pointIndex >= 0 && pointIndex < context->room->pointsCount;
}