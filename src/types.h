#pragma once

#define WINDOW_TITLE_MAX_LENGTH 256
#define MAX_SPRITE_FRAME_COUNT 100
#define MAX_ENTITIES_COUNT 1000
#define MAX_POINTS_COUNT 100
#define MAX_PATHWAYS_COUNT 400

bool equalsS32(const s32 a, const s32 b)
{
    return a == b;
}

shlDeclareList(s32List, s32)
shlDefineList(s32List, s32)

#define s32ListDefaultOptions (s32ListOptions){0, equalsS32, NULL}

//
// Forward references to other structs that need a reference to these ones.
// See WarButtonComponent, WarState.building.
//
struct _GameContext;
struct _Entity;

typedef struct
{
    s32 spriteWidth;
    s32 spriteHeight;
    s32 lineHeight;
    s32 advance;
    rect data[95];
} FontData;

typedef struct _SpriteFrame
{
    u8 dx;
    u8 dy;
    u8 w;
    u8 h;
    u32 off;
    u8* data;
} SpriteFrame;

typedef struct _Sprite
{
    s32 image;
    s32 frameWidth;
    s32 frameHeight;
    s32 framesCount;
    SpriteFrame frames[MAX_SPRITE_FRAME_COUNT];
} Sprite;

// Input
typedef enum _GameMouseButtons
{
    GAME_MOUSE_LEFT,
    GAME_MOUSE_RIGHT,

    GAME_MOUSE_COUNT
} GameMouseButtons;

typedef enum _GameKeys
{
    GAME_KEY_NONE,

    GAME_KEY_SPACE,
    GAME_KEY_APOSTROPHE,
    GAME_KEY_ASTERISK,
    GAME_KEY_PLUS,
    GAME_KEY_COMMA,
    GAME_KEY_MINUS,
    GAME_KEY_PERIOD,
    GAME_KEY_SLASH,

    GAME_KEY_0,
    GAME_KEY_1,
    GAME_KEY_2,
    GAME_KEY_3,
    GAME_KEY_4,
    GAME_KEY_5,
    GAME_KEY_6,
    GAME_KEY_7,
    GAME_KEY_8,
    GAME_KEY_9,

    GAME_KEY_SEMICOLON,
    GAME_KEY_EQUAL,

    GAME_KEY_A,
    GAME_KEY_B,
    GAME_KEY_C,
    GAME_KEY_D,
    GAME_KEY_E,
    GAME_KEY_F,
    GAME_KEY_G,
    GAME_KEY_H,
    GAME_KEY_I,
    GAME_KEY_J,
    GAME_KEY_K,
    GAME_KEY_L,
    GAME_KEY_M,
    GAME_KEY_N,
    GAME_KEY_O,
    GAME_KEY_P,
    GAME_KEY_Q,
    GAME_KEY_R,
    GAME_KEY_S,
    GAME_KEY_T,
    GAME_KEY_U,
    GAME_KEY_V,
    GAME_KEY_W,
    GAME_KEY_X,
    GAME_KEY_Y,
    GAME_KEY_Z,

    GAME_KEY_LEFT_BRACKET,
    GAME_KEY_BACKSLASH,
    GAME_KEY_RIGHT_BRACKET,
    GAME_KEY_GRAVE_ACCENT,

    GAME_KEY_ESC,
    GAME_KEY_ENTER,
    GAME_KEY_TAB,
    GAME_KEY_BACKSPACE,
    GAME_KEY_INSERT,
    GAME_KEY_DELETE,

    GAME_KEY_RIGHT,
    GAME_KEY_LEFT,
    GAME_KEY_DOWN,
    GAME_KEY_UP,

    GAME_KEY_PAGE_UP,
    GAME_KEY_PAGE_DOWN,
    GAME_KEY_HOME,
    GAME_KEY_END,

    GAME_KEY_F1,
    GAME_KEY_F2,
    GAME_KEY_F3,
    GAME_KEY_F4,
    GAME_KEY_F5,
    GAME_KEY_F6,
    GAME_KEY_F7,
    GAME_KEY_F8,
    GAME_KEY_F9,
    GAME_KEY_F10,
    GAME_KEY_F11,
    GAME_KEY_F12,

    GAME_KEY_SHIFT,
    GAME_KEY_CTRL,
    GAME_KEY_ALT,

    GAME_KEY_COUNT
} GameKeys;

typedef struct _GameKeyButtonState
{
    // indicates if the key is pressed in the current frame
    bool pressed;

    // indicate if the key was pressed in the previous frame
    bool wasPressed;
} GameKeyButtonState;

typedef struct _GameInput
{
    // current mouse position
    vec2 pos;

    // state of the mouse buttons
    GameKeyButtonState buttons[GAME_MOUSE_COUNT];

    // state of the keys
    GameKeyButtonState keys[GAME_KEY_COUNT];

    // drag
    bool isDragging;
    bool wasDragging;
    vec2 dragPos;
    rect dragRect;
} GameInput;

typedef void (*GameRenderFunc)(struct _GameContext* context, struct _Entity* entity);

typedef u16 EntityId;

typedef struct _TransformComponent
{
    bool enabled;
    vec2 position;
    vec2 rotation;
    vec2 scale;
} TransformComponent;

typedef struct
{
    bool enabled;
    s32 resourceIndex;
    s32 frameIndex;
    vec2 offset;
    Sprite sprite;
} SpriteComponent;

typedef struct
{
    bool enabled;
    char* name;
} UIComponent;

typedef enum
{
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT,
    TEXT_ALIGN_TOP,
    TEXT_ALIGN_MIDDLE,
    TEXT_ALIGN_BOTTOM,

    TEXT_ALIGN_COUNT
} TextAlignment;

typedef enum
{
    TEXT_WRAP_NONE,
    TEXT_WRAP_CHAR
} TextWrapping;

typedef enum
{
    TEXT_TRIM_NONE,
    TEXT_TRIM_SPACES,
    TEXT_TRIM_ALL
} TextTrimming;

typedef struct
{
    bool enabled;
    char* text;
    s32 fontIndex;
    f32 fontSize;
    f32 lineHeight;
    u8Color fontColor;
    u8Color highlightColor;
    s32 highlightIndex;
    s32 highlightCount;
    vec2 boundings;
    TextAlignment horizontalAlign;
    TextAlignment verticalAlign;
    TextAlignment lineAlign;
    TextWrapping wrapping;
    TextTrimming trimming;
    bool multiline;
} TextComponent;

typedef enum _NurseAction
{
    ACTION_IDLE,
    ACTION_WALK,
    ACTION_CARE,
    ACTION_SWITCH,

    ACTION_COUNT
} NurseAction;

typedef struct _NurseComponent
{
    bool enabled;
    NurseAction action;

    s32 row;
    s32 col;
    s32 index;

    s32 pathLength;
    s32 pathIndex;
    s32 pathLastIndex;
    s32 path[MAX_POINTS_COUNT];
} NurseComponent;

typedef struct _BedComponent
{
    bool enabled;
    bool highlighted;
    bool ventilator;
    s32 row;
    s32 col;
    s32 index;
    s32 life;
} BedComponent;

typedef struct _VentilatorComponent
{
    bool enabled;
    bool highlighted;
    s32 row;
    s32 col;
    s32 dir;
} VentilatorComponent;

typedef struct _RectComponent
{
    bool enabled;
    vec2 size;
    NVGcolor color;
} RectComponent;

typedef enum _EntityType
{
    ENTITY_TYPE_NONE,
    ENTITY_TYPE_IMAGE,
    ENTITY_TYPE_NURSE,
    ENTITY_TYPE_BED,
    ENTITY_TYPE_VENTILATOR,
    ENTITY_TYPE_TEXT,
    ENTITY_TYPE_RECT,
    ENTITY_TYPE_BUTTON,

    ENTITY_TYPE_COUNT
} EntityType;

typedef struct _Entity
{
    EntityId id;
    EntityType type;
    TransformComponent transform;
    SpriteComponent sprite;
    UIComponent ui;
    TextComponent text;
    RectComponent rect;
    NurseComponent nurse;
    BedComponent bed;
    VentilatorComponent ventilator;
} Entity;

typedef struct _Room
{
    s32 entitiesCount;
    Entity* entities[MAX_ENTITIES_COUNT];

    s32 pointsCount;
    vec2 points[MAX_POINTS_COUNT];

    s32 pathWaysCount;
    int pathWays[MAX_PATHWAYS_COUNT];
} Room;

typedef struct _Menu
{
    s32 step;

    s32 entitiesCount;
    Entity* entities[MAX_ENTITIES_COUNT];
} Menu;

typedef struct _GameContext
{
    f32 time;
    f32 deltaTime;
    u32 fps;

    bool paused;

    f32 globalScale;
    f32 globalSpeed;

    s32 originalWindowWidth;
    s32 originalWindowHeight;
    s32 windowWidth;
    s32 windowHeight;
    s32 framebufferWidth;
    s32 framebufferHeight;
    f32 devicePixelRatio;
    char windowTitle[WINDOW_TITLE_MAX_LENGTH];
    GLFWwindow* window;

    pthread_mutex_t __mutex;

    NVGcontext* gfx;
    ma_device sfx;
    ma_decoder decoder;

    Sprite fontSprites[2];
    GameInput input;

    s32 staticEntityId;

    Room* room;
    Menu* menu;
} GameContext;
