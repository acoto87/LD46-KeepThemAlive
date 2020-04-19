#define FRAMES_PER_SECONDS 60
#define SECONDS_PER_FRAME (1.0f/FRAMES_PER_SECONDS)

#define PLAYBACK_FREQ 44100

#define ORIGINAL_WINDOW_WIDTH 320
#define ORIGINAL_WINDOW_HEIGHT 200

#define ROOM_PADDING 30

#define BED_WIDTH 16
#define BED_HEIGHT 32
#define VENTILATOR_WIDTH 12
#define VENTILATOR_HEIGHT 16

#define BED_PAIR_WIDTH (2 * BED_WIDTH + VENTILATOR_WIDTH)
#define BED_PAIR_HEIGHT (BED_HEIGHT)

#define BED_PAIRS_PER_COL 4
#define BED_PAIRS_PER_ROW 3

#define BED_PAIR_H_SPACING ((ORIGINAL_WINDOW_WIDTH - 2 * ROOM_PADDING - BED_PAIRS_PER_COL * BED_PAIR_WIDTH) / (BED_PAIRS_PER_COL - 1))
#define BED_PAIR_V_SPACING ((ORIGINAL_WINDOW_HEIGHT - 2 * ROOM_PADDING - BED_PAIRS_PER_ROW * BED_PAIR_HEIGHT) / (BED_PAIRS_PER_ROW - 1))

#define FLOOR_TILE_WIDTH 10
#define FLOOR_TILE_HEIGHT 10
#define FLOOR_TILE_ROWS ((ORIGINAL_WINDOW_HEIGHT - 20) / FLOOR_TILE_HEIGHT)
#define FLOOR_TILE_COLS (ORIGINAL_WINDOW_WIDTH / FLOOR_TILE_WIDTH)

#define isButtonPressed(input, btn) (input->buttons[btn].pressed)
#define wasButtonPressed(input, btn) (input->buttons[btn].wasPressed)
#define isKeyPressed(input, key) (input->keys[key].pressed)
#define wasKeyPressed(input, key) (input->keys[key].wasPressed)

#define getScaledSpeed(context, t) ((t) * (context)->globalSpeed)
#define getScaledTime(context, t) ((t) / (context)->globalSpeed)

void setGlobalScale(GameContext* context, f32 scale);
void changeGlobalScale(GameContext* context, f32 deltaScale);

void setGlobalSpeed(GameContext* context, f32 speed);
void changeGlobalSpeed(GameContext* context, f32 deltaSpeed);

#define setUIEntityStatus(uiEntity, value) ((uiEntity)->ui.enabled = (value))
#define setUITextBoundings(uiEntity, value) ((uiEntity)->text.boundings = (value))
#define setUITextHorizontalAlign(uiEntity, value) ((uiEntity)->text.horizontalAlign = (value))
#define setUITextVerticalAlign(uiEntity, value) ((uiEntity)->text.verticalAlign = (value))
#define setUITextLineAlign(uiEntity, value) ((uiEntity)->text.lineAlign = (value))
#define setUITextWrapping(uiEntity, value) ((uiEntity)->text.wrapping = (value))
#define setUITextColor(uiEntity, value) ((uiEntity)->text.fontColor = (value))
#define setUITextHighlight(uiEntity, index, count) \
    ({ ((uiEntity)->text.highlightIndex = (index)); \
       ((uiEntity)->text.highlightCount = (count)); })
#define setUITextHighlightColor(uiEntity, color) ((uiEntity)->text.highlightColor = (color))
#define setUITextMultiline(uiEntity, value) ((uiEntity)->text.multiline = (value))
#define setUITextLineHeight(uiEntity, value) ((uiEntity)->text.lineHeight = (value))
#define setUITextTrimming(uiEntity, value) ((uiEntity)->text.trimming = (value))
