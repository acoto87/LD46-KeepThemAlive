void initRoom(GameContext* context);

Entity* createNurse(GameContext* context, s32 row, s32 col, s32 index);
Entity* createBed(GameContext* context, vec2 position, s32 row, s32 col, s32 index, s32 life, bool ventilator);
Entity* createVentilator(GameContext* context, vec2 position, s32 row, s32 col, s32 dir);

void updateRoom(GameContext* context);
void renderRoom(GameContext* context);

s32 pointIndexAt(GameContext* context, s32 row, s32 col, s32 index);
void pointRowColIndex(GameContext* context, s32 pointIndex, s32* row, s32* col, s32* index);
bool pointIndexValid(GameContext* context, s32 row, s32 col, s32 index);