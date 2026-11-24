#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

using namespace sf;
using namespace std;

// Shayan
const int BOARD_N = 10;
const int SHIP_COUNT = 5;
vector<int> shipLengths = { 5, 4, 3, 3, 2 };

int p1Ships[BOARD_N][BOARD_N];
int p2Ships[BOARD_N][BOARD_N];
int p1Shots[BOARD_N][BOARD_N];
int p2Shots[BOARD_N][BOARD_N];

vector<int> p1Health(SHIP_COUNT, 0);
vector<int> p2Health(SHIP_COUNT, 0);
vector<int> p1Row(SHIP_COUNT, -1);
vector<int> p2Row(SHIP_COUNT, -1);
vector<int> p1Col(SHIP_COUNT, -1);
vector<int> p2Col(SHIP_COUNT, -1);
vector<int> p1Dir(SHIP_COUNT, 0);
vector<int> p2Dir(SHIP_COUNT, 0);

int activePlayer = 0;
bool singlePlayer = true;

const int START_AMMO = 40;
const float BOT_DELAY = 0.75f;
const float SHOT_LAND_TIME = 0.65f;
const float SHOT_DONE_TIME = 1.3f;

bool limitedAmmo = false;
int ammoLeft[2];
int hitsLanded[2];
int shipsSunk[2];

vector<int> botTargetR;
vector<int> botTargetC;
float botTimer = 0.0f;

int coinWinner = 0;

bool shotActive = false;
bool shotResolved = false;
bool lastShotHit = false;
float shotTimer = 0.0f;
int firingPlayer = 0;
int targetR = 0;
int targetC = 0;

bool turnPassed = false;
bool gameOver = false;
int matchWinner = -1;

// Abbas
const int TILE = 38;
const int LEFT_BOARD_X = 60;
const int RIGHT_BOARD_X = 560;
const int BOARD_TOP = 115;

RenderWindow app;
Font mainFont;
Clock totalClock;

const float CANNON_Y = 305.0f;
const float LEFT_CANNON_X = 26.0f;
const float RIGHT_CANNON_X = 974.0f;

const int MENU_COUNT = 5;
const float MENU_X = 340.0f;
const float MENU_TOP = 175.0f;
const float MENU_GAP = 68.0f;
const float MENU_W = 320.0f;
const float MENU_H = 52.0f;

bool wasShotActive = false;
bool wasShotResolved = false;
bool wasGameOver = false;

// Muneeb
const int SCR_MENU = 0;
const int SCR_SETUP = 1;
const int SCR_COIN = 2;
const int SCR_FIGHT = 3;
const int SCR_OVER = 4;
const int SCR_HANDOFF = 5;

int currentScreen = SCR_MENU;
int nextAfterHandoff = SCR_FIGHT;
int placingStep = 0;
int placeDir = 0;

float curMouseX = 0.0f;
float curMouseY = 0.0f;
bool uiClicked = false;

const float COIN_SPIN_TIME = 2.0f;
float coinTimer = 0.0f;

bool moveMode = false;
int pickedShipId = -1;
int savedShipPlayer = 0;
int savedShipR = -1;
int savedShipC = -1;
int savedShipDir = 0;
int moveDir = 0;
bool repositionUsed[2];

bool reconMode = false;
bool reconUsed[2];
int reconRow[2];
int reconCol[2];
int reconFound[2];

// Shayan
int getShipCell(int p, int r, int c)
{
    if (p == 0)
    {
        return p1Ships[r][c];
    }
    return p2Ships[r][c];
}

void setShipCell(int p, int r, int c, int val)
{
    if (p == 0)
    {
        p1Ships[r][c] = val;
    }
    else
    {
        p2Ships[r][c] = val;
    }
}

int getShotCell(int p, int r, int c)
{
    if (p == 0)
    {
        return p1Shots[r][c];
    }
    return p2Shots[r][c];
}

void setShotCell(int p, int r, int c, int val)
{
    if (p == 0)
    {
        p1Shots[r][c] = val;
    }
    else
    {
        p2Shots[r][c] = val;
    }
}

int getHealth(int p, int idx)
{
    if (p == 0)
    {
        return p1Health[idx];
    }
    return p2Health[idx];
}

void damageShip(int p, int idx)
{
    if (p == 0)
    {
        p1Health[idx]--;
    }
    else
    {
        p2Health[idx]--;
    }
}

int getShipRow(int p, int idx)
{
    if (p == 0)
    {
        return p1Row[idx];
    }
    return p2Row[idx];
}

int getShipCol(int p, int idx)
{
    if (p == 0)
    {
        return p1Col[idx];
    }
    return p2Col[idx];
}

int getShipDir(int p, int idx)
{
    if (p == 0)
    {
        return p1Dir[idx];
    }
    return p2Dir[idx];
}

void saveShipMeta(int p, int idx, int r, int c, int dir)
{
    if (p == 0)
    {
        p1Row[idx] = r;
        p1Col[idx] = c;
        p1Dir[idx] = dir;
    }
    else
    {
        p2Row[idx] = r;
        p2Col[idx] = c;
        p2Dir[idx] = dir;
    }
}

void clearMatchData()
{
    for (int r = 0; r < BOARD_N; r++)
    {
        for (int c = 0; c < BOARD_N; c++)
        {
            p1Ships[r][c] = 0;
            p2Ships[r][c] = 0;
            p1Shots[r][c] = 0;
            p2Shots[r][c] = 0;
        }
    }

    for (int i = 0; i < SHIP_COUNT; i++)
    {
        p1Health[i] = shipLengths[i];
        p2Health[i] = shipLengths[i];
        p1Row[i] = -1;
        p2Row[i] = -1;
        p1Col[i] = -1;
        p2Col[i] = -1;
        p1Dir[i] = 0;
        p2Dir[i] = 0;
    }

    for (int p = 0; p < 2; p++)
    {
        ammoLeft[p] = START_AMMO;
        hitsLanded[p] = 0;
        shipsSunk[p] = 0;
    }
    botTargetR.clear();
    botTargetC.clear();

    activePlayer = 0;
    placingStep = 0;
    placeDir = 0;

    botTimer = 0.0f;
    coinWinner = 0;
    shotActive = false;
    shotResolved = false;
    lastShotHit = false;
    shotTimer = 0.0f;
    firingPlayer = 0;
    targetR = 0;
    targetC = 0;
    turnPassed = false;
    gameOver = false;
    matchWinner = -1;
}

void setupMatchMode(bool onePlayer, bool ammoMode)
{
    singlePlayer = onePlayer;
    limitedAmmo = ammoMode;
    clearMatchData();
}

void setActivePlayer(int p)
{
    activePlayer = p;
}

void clearTurnPassed()
{
    turnPassed = false;
}

string getName(int p)
{
    if (singlePlayer && p == 1)
    {
        return "Computer";
    }
    if (p == 0)
    {
        return "Player 1";
    }
    return "Player 2";
}

int getViewerId()
{
    if (singlePlayer)
    {
        return 0;
    }
    return activePlayer;
}

int countAliveShips(int p)
{
    int alive = 0;
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (getHealth(p, i) > 0)
        {
            alive++;
        }
    }
    return alive;
}

int totalFleetHealth(int p)
{
    int sum = 0;
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        sum += getHealth(p, i);
    }
    return sum;
}

bool hasAmmo(int p)
{
    if (!limitedAmmo)
    {
        return true;
    }
    if (ammoLeft[p] > 0)
    {
        return true;
    }
    return false;
}

// Muneeb
void clearUiClick()
{
    uiClicked = false;
}

bool canPlaceHere(int p, int startR, int startC, int len, int dir)
{
    if (startR < 0 || startC < 0)
    {
        return false;
    }

    if (dir == 1)
    {
        if (startR + len > BOARD_N)
        {
            return false;
        }
    }
    else
    {
        if (startC + len > BOARD_N)
        {
            return false;
        }
    }

    for (int k = 0; k < len; k++)
    {
        int r = startR;
        int c = startC;
        if (dir == 1)
        {
            r = startR + k;
        }
        else
        {
            c = startC + k;
        }

        if (getShipCell(p, r, c) != 0 || getShotCell(p, r, c) != 0)
        {
            return false;
        }
    }

    return true;
}

void writeShipToGrid(int p, int id, int startR, int startC, int dir)
{
    saveShipMeta(p, id, startR, startC, dir);
    int len = shipLengths[id];

    for (int k = 0; k < len; k++)
    {
        if (dir == 1)
        {
            setShipCell(p, startR + k, startC, id + 1);
        }
        else
        {
            setShipCell(p, startR, startC + k, id + 1);
        }
    }
}

// Shayan
void decideLimitedWinner()
{
    if (hitsLanded[0] > hitsLanded[1])
    {
        matchWinner = 0;
    }
    else if (hitsLanded[1] > hitsLanded[0])
    {
        matchWinner = 1;
    }
    else if (shipsSunk[0] > shipsSunk[1])
    {
        matchWinner = 0;
    }
    else if (shipsSunk[1] > shipsSunk[0])
    {
        matchWinner = 1;
    }
    else
    {
        matchWinner = -1;
    }
    gameOver = true;
}

void switchTurn(int nextP)
{
    if (nextP == activePlayer)
    {
        return;
    }
    activePlayer = nextP;
    lastShotHit = false;
    botTimer = 0.0f;
    turnPassed = true;
}

void placeComputerFleet()
{
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        bool done = false;
        while (!done)
        {
            int r = rand() % BOARD_N;
            int c = rand() % BOARD_N;
            int dir = rand() % 2;

            if (canPlaceHere(1, r, c, shipLengths[i], dir))
            {
                writeShipToGrid(1, i, r, c, dir);
                done = true;
            }
        }
    }
}

void tossCoin()
{
    coinWinner = rand() % 2;
    activePlayer = coinWinner;
    lastShotHit = false;
}

void launchShot(int shooterP, int r, int c)
{
    if (shotActive)
    {
        return;
    }
    if (!hasAmmo(shooterP))
    {
        return;
    }
    if (getShotCell(1 - shooterP, r, c) != 0)
    {
        return;
    }

    firingPlayer = shooterP;
    targetR = r;
    targetC = c;
    shotActive = true;
    shotResolved = false;
    shotTimer = 0.0f;

    if (limitedAmmo)
    {
        ammoLeft[shooterP]--;
    }
}

void addBotTargets(int r, int c)
{
    botTargetR.push_back(r - 1);
    botTargetC.push_back(c);
    botTargetR.push_back(r + 1);
    botTargetC.push_back(c);
    botTargetR.push_back(r);
    botTargetC.push_back(c - 1);
    botTargetR.push_back(r);
    botTargetC.push_back(c + 1);
}

void botChooseShot()
{
    int pickR = -1;
    int pickC = -1;

    while (!botTargetR.empty() && pickR == -1)
    {
        int last = (int)botTargetR.size() - 1;
        int r = botTargetR[last];
        int c = botTargetC[last];
        botTargetR.pop_back();
        botTargetC.pop_back();

        if (r >= 0 && r < BOARD_N && c >= 0 && c < BOARD_N && getShotCell(0, r, c) == 0)
        {
            pickR = r;
            pickC = c;
        }
    }

    while (pickR == -1)
    {
        int r = rand() % BOARD_N;
        int c = rand() % BOARD_N;
        if (getShotCell(0, r, c) == 0)
        {
            pickR = r;
            pickC = c;
        }
    }

    launchShot(1, pickR, pickC);
}

void resolveShot()
{
    shotResolved = true;
    int enemy = 1 - firingPlayer;
    int shipVal = getShipCell(enemy, targetR, targetC);

    if (shipVal > 0)
    {
        lastShotHit = true;
        setShotCell(enemy, targetR, targetC, 2);

        int shipId = shipVal - 1;
        damageShip(enemy, shipId);
        hitsLanded[firingPlayer]++;
        if (getHealth(enemy, shipId) == 0)
        {
            shipsSunk[firingPlayer]++;
        }

        if (singlePlayer && firingPlayer == 1)
        {
            addBotTargets(targetR, targetC);
        }
    }
    else
    {
        lastShotHit = false;
        setShotCell(enemy, targetR, targetC, 1);
    }
}

void finishShot()
{
    shotActive = false;
    shotResolved = false;
    int enemy = 1 - firingPlayer;

    if (totalFleetHealth(enemy) == 0)
    {
        matchWinner = firingPlayer;
        gameOver = true;
        return;
    }

    if (limitedAmmo && !hasAmmo(0) && !hasAmmo(1))
    {
        decideLimitedWinner();
        return;
    }

    if (lastShotHit && hasAmmo(firingPlayer))
    {
        return;
    }

    int nextP = enemy;
    if (!hasAmmo(nextP))
    {
        nextP = firingPlayer;
    }
    switchTurn(nextP);
}

void updateBattle(float dt)
{
    if (shotActive)
    {
        shotTimer += dt;
        if (shotTimer >= SHOT_LAND_TIME && !shotResolved)
        {
            resolveShot();
        }
        else if (shotTimer >= SHOT_DONE_TIME)
        {
            finishShot();
        }
        return;
    }

    if (limitedAmmo)
    {
        if (!hasAmmo(0) && !hasAmmo(1))
        {
            decideLimitedWinner();
            return;
        }
        if (!hasAmmo(activePlayer))
        {
            switchTurn(1 - activePlayer);
            return;
        }
    }

    if (singlePlayer && activePlayer == 1)
    {
        botTimer += dt;
        if (botTimer >= BOT_DELAY)
        {
            botTimer = 0.0f;
            botChooseShot();
        }
    }
}

// Abbas
bool makeTone(SoundBuffer& buf, float hz, float seconds)
{
    const int SAMPLE_RATE = 44100;
    const float PI = 3.14159f;
    int sampleCount = (int)((float)SAMPLE_RATE * seconds);
    vector<short> samples(sampleCount);

    for (int i = 0; i < sampleCount; i++)
    {
        float progress = (float)i / (float)sampleCount;
        float fade = 1.0f - progress;
        float wave = (float)sin(2.0f * PI * hz * (float)i / (float)SAMPLE_RATE);
        samples[i] = (short)(wave * fade * 10000.0f);
    }

    vector<SoundChannel> channels;
    channels.push_back(SoundChannel::Mono);
    return buf.loadFromSamples(&samples[0], samples.size(), 1, SAMPLE_RATE, channels);
}

void updateAudio(Sound& click, Sound& fire, Sound& hit, Sound& miss, Sound& win)
{
    if (uiClicked)
    {
        click.play();
        clearUiClick();
    }

    if (shotActive && !wasShotActive)
    {
        fire.play();
    }
    wasShotActive = shotActive;

    if (shotResolved && !wasShotResolved)
    {
        if (lastShotHit)
        {
            hit.play();
        }
        else
        {
            miss.play();
        }
    }
    wasShotResolved = shotResolved;

    if (gameOver && !wasGameOver)
    {
        win.play();
    }
    wasGameOver = gameOver;
}

void drawStr(string msg, float x, float y, int sz, Color col)
{
    Text label(mainFont, msg, sz);
    label.setPosition({ x, y });
    label.setFillColor(col);
    app.draw(label);
}

void drawStrCenter(string msg, float y, int sz, Color col)
{
    Text label(mainFont, msg, sz);
    FloatRect box = label.getLocalBounds();
    float x = 500.0f - (box.position.x + box.size.x * 0.5f);
    label.setPosition({ x, y });
    label.setFillColor(col);
    app.draw(label);
}

void fillBox(float x, float y, float w, float h, Color col)
{
    RectangleShape r({ w, h });
    r.setPosition({ x, y });
    r.setFillColor(col);
    app.draw(r);
}

void drawBackground()
{
    for (int i = 0; i < 5; i++)
    {
        fillBox(0.0f, (float)(i * 80), 1000.0f, 80.0f, Color(10 + i * 4, 20 + i * 7, 46 + i * 10));
    }
    fillBox(0.0f, 400.0f, 1000.0f, 200.0f, Color(12, 42, 82));
    fillBox(0.0f, 400.0f, 1000.0f, 3.0f, Color(65, 115, 165));
}

// Muneeb
bool insideBox(float x, float y, float w, float h)
{
    if (curMouseX >= x && curMouseX <= x + w && curMouseY >= y && curMouseY <= y + h)
    {
        return true;
    }
    return false;
}

bool mouseToGrid(int bx, int by, int& outR, int& outC)
{
    int c = (int)floor((curMouseX - (float)bx) / (float)TILE);
    int r = (int)floor((curMouseY - (float)by) / (float)TILE);

    if (r < 0 || r >= BOARD_N || c < 0 || c >= BOARD_N)
    {
        return false;
    }

    outR = r;
    outC = c;
    return true;
}

// Abbas
void drawVessel(float x, float y, int len, int dir, Color bodyCol)
{
    float span = (float)(len * TILE);
    Color trim(bodyCol.r * 3 / 4, bodyCol.g * 3 / 4, bodyCol.b * 3 / 4);

    if (dir == 0)
    {
        ConvexShape hull(5);
        hull.setPoint(0, Vector2f(x + 4.0f, y + 6.0f));
        hull.setPoint(1, Vector2f(x + span - 18.0f, y + 6.0f));
        hull.setPoint(2, Vector2f(x + span - 3.0f, y + TILE * 0.5f));
        hull.setPoint(3, Vector2f(x + span - 18.0f, y + TILE - 6.0f));
        hull.setPoint(4, Vector2f(x + 4.0f, y + TILE - 6.0f));
        hull.setFillColor(bodyCol);
        hull.setOutlineColor(Color(20, 24, 34));
        hull.setOutlineThickness(2.0f);
        app.draw(hull);

        fillBox(x + 12.0f, y + 11.0f, span - 36.0f, TILE - 22.0f, trim);
        fillBox(x + span * 0.36f, y + 9.0f, 18.0f, TILE - 18.0f, Color(215, 220, 230));

        CircleShape turret(4.0f);
        turret.setOrigin({ 4.0f, 4.0f });
        turret.setPosition({ x + span * 0.36f + 9.0f, y + TILE * 0.5f });
        turret.setFillColor(Color(55, 60, 70));
        app.draw(turret);
    }
    else
    {
        ConvexShape hull(5);
        hull.setPoint(0, Vector2f(x + 6.0f, y + 4.0f));
        hull.setPoint(1, Vector2f(x + TILE - 6.0f, y + 4.0f));
        hull.setPoint(2, Vector2f(x + TILE - 6.0f, y + span - 18.0f));
        hull.setPoint(3, Vector2f(x + TILE * 0.5f, y + span - 3.0f));
        hull.setPoint(4, Vector2f(x + 6.0f, y + span - 18.0f));
        hull.setFillColor(bodyCol);
        hull.setOutlineColor(Color(20, 24, 34));
        hull.setOutlineThickness(2.0f);
        app.draw(hull);

        fillBox(x + 11.0f, y + 12.0f, TILE - 22.0f, span - 36.0f, trim);
        fillBox(x + 9.0f, y + span * 0.36f, TILE - 18.0f, 18.0f, Color(215, 220, 230));

        CircleShape turret(4.0f);
        turret.setOrigin({ 4.0f, 4.0f });
        turret.setPosition({ x + TILE * 0.5f, y + span * 0.36f + 9.0f });
        turret.setFillColor(Color(55, 60, 70));
        app.draw(turret);
    }
}

void drawCannon(float x, int facing)
{
    if (facing == 0)
    {
        fillBox(x, CANNON_Y - 6.0f, 38.0f, 12.0f, Color(75, 80, 90));
    }
    else
    {
        fillBox(x - 38.0f, CANNON_Y - 6.0f, 38.0f, 12.0f, Color(75, 80, 90));
    }

    CircleShape wheel(17.0f);
    wheel.setOrigin({ 17.0f, 17.0f });
    wheel.setPosition({ x, CANNON_Y });
    wheel.setFillColor(Color(115, 85, 50));
    app.draw(wheel);
}

void drawBoard(int p, int bx, int by, bool revealAll, float t)
{
    fillBox((float)bx - 4.0f, (float)by - 4.0f,
        (float)(BOARD_N * TILE) + 8.0f, (float)(BOARD_N * TILE) + 8.0f,
        Color(10, 20, 35, 210));

    for (int r = 0; r < BOARD_N; r++)
    {
        for (int c = 0; c < BOARD_N; c++)
        {
            int waveG = (int)(72.0f + 20.0f * sin(t * 2.1f + (float)r * 0.65f + (float)c * 0.65f));
            fillBox((float)(bx + c * TILE + 1), (float)(by + r * TILE + 1),
                (float)(TILE - 2), (float)(TILE - 2), Color(18, waveG, 148));
        }
    }

    for (int i = 0; i < SHIP_COUNT; i++)
    {
        int sr = getShipRow(p, i);
        int sc = getShipCol(p, i);
        if (sr < 0 || sc < 0)
        {
            continue;
        }

        bool destroyed = (getHealth(p, i) == 0);
        if (revealAll || destroyed)
        {
            Color col(128, 138, 156);
            if (destroyed)
            {
                col = Color(155, 60, 60);
            }
            drawVessel((float)(bx + sc * TILE), (float)(by + sr * TILE),
                shipLengths[i], getShipDir(p, i), col);
        }
    }

    for (int r = 0; r < BOARD_N; r++)
    {
        for (int c = 0; c < BOARD_N; c++)
        {
            int mark = getShotCell(p, r, c);
            if (mark == 0)
            {
                continue;
            }

            float cx = (float)bx + ((float)c + 0.5f) * (float)TILE;
            float cy = (float)by + ((float)r + 0.5f) * (float)TILE;

            if (mark == 1)
            {
                CircleShape dot(5.0f);
                dot.setOrigin({ 5.0f, 5.0f });
                dot.setPosition({ cx, cy });
                dot.setFillColor(Color(235, 245, 255));
                app.draw(dot);
            }
            else if (mark == 2)
            {
                CircleShape hit(12.0f);
                hit.setOrigin({ 12.0f, 12.0f });
                hit.setPosition({ cx, cy });
                hit.setFillColor(Color(255, 70, 20, 210));
                app.draw(hit);
            }
        }
    }
}

// Muneeb
void drawGhostPreview(int bx, int by, int p, int len, int dir)
{
    int r = 0;
    int c = 0;
    if (!mouseToGrid(bx, by, r, c))
    {
        return;
    }

    bool valid = canPlaceHere(p, r, c, len, dir);
    Color tint(240, 45, 45, 130);
    if (valid)
    {
        tint = Color(45, 235, 85, 130);
    }

    for (int k = 0; k < len; k++)
    {
        int rr = r;
        int cc = c;
        if (dir == 1)
        {
            rr = r + k;
        }
        else
        {
            cc = c + k;
        }

        if (rr < BOARD_N && cc < BOARD_N)
        {
            fillBox((float)(bx + cc * TILE + 1), (float)(by + rr * TILE + 1),
                (float)(TILE - 2), (float)(TILE - 2), tint);
        }
    }
}

// Abbas
void renderMenu()
{
    drawStrCenter("BATTLESHIPS", 55.0f, 64, Color(255, 210, 40));

    string labels[MENU_COUNT] = {
        "1 Player - Classic",
        "1 Player - Limited Ammo",
        "2 Players - Classic",
        "2 Players - Limited Ammo",
        "Quit"
    };

    for (int i = 0; i < MENU_COUNT; i++)
    {
        float by = MENU_TOP + (float)i * MENU_GAP;
        Color btnCol(30, 68, 118);
        if (insideBox(MENU_X, by, MENU_W, MENU_H))
        {
            btnCol = Color(58, 122, 195);
        }

        fillBox(MENU_X, by, MENU_W, MENU_H, btnCol);
        drawStrCenter(labels[i], by + 11.0f, 23, Color::White);
    }
}

void renderSetup(float t)
{
    string header = getName(activePlayer) + " - Ship " + to_string(placingStep + 1)
        + " of 5 (Length " + to_string(shipLengths[placingStep])
        + ")   [R / Right Click: Rotate]";
    drawStr(header, 60.0f, 28.0f, 23, Color::White);

    if (singlePlayer)
    {
        drawStr("Place your fleet on the left grid. Computer ships stay hidden.",
            60.0f, 64.0f, 19, Color(255, 215, 70));
    }
    else
    {
        drawStr("Other player: look away while ships are being placed.",
            60.0f, 64.0f, 19, Color(255, 215, 70));
    }

    drawBoard(activePlayer, LEFT_BOARD_X, BOARD_TOP, true, t);
    drawGhostPreview(LEFT_BOARD_X, BOARD_TOP, activePlayer, shipLengths[placingStep], placeDir);
}

void renderCoin()
{
    int shownPlayer = coinWinner;
    if (coinTimer < COIN_SPIN_TIME)
    {
        shownPlayer = ((int)(coinTimer * 6.0f)) % 2;
    }

    CircleShape coin(68.0f);
    coin.setOrigin({ 68.0f, 68.0f });
    coin.setPosition({ 500.0f, 255.0f });
    coin.setFillColor(Color(240, 190, 30));
    app.draw(coin);

    drawStrCenter(getName(shownPlayer), 240.0f, 24, Color(20, 25, 35));
    if (coinTimer < COIN_SPIN_TIME)
    {
        drawStrCenter("Flipping coin...", 380.0f, 32, Color::White);
    }
    else
    {
        drawStrCenter(getName(coinWinner) + " attacks first! Click to start.", 380.0f, 30, Color(110, 255, 130));
    }
}

void renderHandoff()
{
    fillBox(160.0f, 140.0f, 680.0f, 310.0f, Color(10, 16, 28, 230));
    drawStrCenter(getName(activePlayer) + ", get ready!", 185.0f, 48, Color(255, 215, 50));
    drawStrCenter("Opponent must look away from the screen", 275.0f, 26, Color::White);
    drawStrCenter("Click anywhere to continue", 365.0f, 26, Color(90, 245, 120));
}

void drawReconMarker(int v)
{
    if (!reconUsed[v] || reconRow[v] < 0)
    {
        return;
    }
    Color col(90, 230, 255, 115);
    if (reconFound[v] == 1)
    {
        col = Color(255, 100, 220, 135);
    }
    fillBox((float)(RIGHT_BOARD_X + reconCol[v] * TILE + 4),
        (float)(BOARD_TOP + reconRow[v] * TILE + 4),
        (float)(TILE - 8), (float)(TILE - 8), col);
}

void drawCannonShot(int v)
{
    drawCannon(LEFT_CANNON_X, 0);
    drawCannon(RIGHT_CANNON_X, 1);
    if (!shotActive)
    {
        return;
    }

    float startX = LEFT_CANNON_X;
    float endX = (float)RIGHT_BOARD_X + ((float)targetC + 0.5f) * (float)TILE;
    if (firingPlayer != v)
    {
        startX = RIGHT_CANNON_X;
        endX = (float)LEFT_BOARD_X + ((float)targetC + 0.5f) * (float)TILE;
    }
    float endY = (float)BOARD_TOP + ((float)targetR + 0.5f) * (float)TILE;

    if (shotTimer < SHOT_LAND_TIME)
    {
        float p = shotTimer / SHOT_LAND_TIME;
        CircleShape ball(8.0f);
        ball.setOrigin({ 8.0f, 8.0f });
        ball.setPosition({ startX + (endX - startX) * p, CANNON_Y + (endY - CANNON_Y) * p });
        ball.setFillColor(Color(25, 25, 30));
        app.draw(ball);
    }
    else
    {
        float p = (shotTimer - SHOT_LAND_TIME) / (SHOT_DONE_TIME - SHOT_LAND_TIME);
        int alpha = (int)(255.0f * (1.0f - p));
        Color boomCol(190, 230, 255, alpha);
        if (lastShotHit)
        {
            boomCol = Color(255, 130, 15, alpha);
        }
        float rad = 6.0f + 40.0f * p;
        CircleShape boom(rad);
        boom.setOrigin({ rad, rad });
        boom.setPosition({ endX, endY });
        boom.setFillColor(boomCol);
        app.draw(boom);
    }
}

void drawBattleHud(int v)
{
    if (currentScreen == SCR_OVER)
    {
        drawStr(getName(0) + " - Hits: " + to_string(hitsLanded[0]) + ", Sunk: " + to_string(shipsSunk[0]), 60.0f, 524.0f, 20, Color::White);
        drawStr(getName(1) + " - Hits: " + to_string(hitsLanded[1]) + ", Sunk: " + to_string(shipsSunk[1]), 560.0f, 524.0f, 20, Color::White);
        drawStrCenter("Click anywhere to return to the Main Menu", 560.0f, 20, Color(110, 255, 130));
        return;
    }

    if (limitedAmmo)
    {
        drawStr(getName(0) + " Ammo: " + to_string(ammoLeft[0]) + "   |   " + getName(1) + " Ammo: " + to_string(ammoLeft[1]), 60.0f, 514.0f, 18, Color::White);
    }

    if (reconUsed[v] && reconRow[v] >= 0)
    {
        string res = "Water";
        if (reconFound[v] == 1)
        {
            res = "Ship";
        }
        drawStr("Recon: " + res + " at R" + to_string(reconRow[v] + 1) + ", C" + to_string(reconCol[v] + 1), 560.0f, 514.0f, 18, Color(120, 235, 255));
    }

    string pState = "Available";
    if (repositionUsed[v])
    {
        pState = "Used";
    }
    string tState = "Available";
    if (reconUsed[v])
    {
        tState = "Used";
    }
    drawStr("Reposition [P]: " + pState + "      Recon [T]: " + tState, 60.0f, 540.0f, 18, Color(255, 215, 60));

    if (moveMode && pickedShipId == -1)
    {
        drawStr("Move Mode: Click an undamaged ship on your left grid (P: Cancel).", 60.0f, 568.0f, 18, Color(100, 255, 130));
    }
    else if (moveMode)
    {
        drawStr("Move Mode: Click new spot on your left grid (R / Right Click: Rotate).", 60.0f, 568.0f, 18, Color(100, 255, 130));
    }
    else if (reconMode)
    {
        drawStr("Recon Mode: Click one unshot enemy square to scan it (T: Cancel).", 60.0f, 568.0f, 18, Color(120, 235, 255));
    }
}

void renderBattle(float t)
{
    fillBox(0.0f, 508.0f, 1000.0f, 92.0f, Color(9, 16, 30, 225));

    int v = getViewerId();
    string topMsg = getName(activePlayer) + "'s turn";
    if (currentScreen == SCR_OVER)
    {
        if (matchWinner == -1)
        {
            topMsg = "MATCH DRAW - Click for Menu";
        }
        else
        {
            topMsg = getName(matchWinner) + " WINS! - Click for Menu";
        }
    }
    else if (lastShotHit && !shotActive)
    {
        topMsg = getName(activePlayer) + " scored a HIT! Bonus shot!";
    }
    drawStrCenter(topMsg, 18.0f, 34, Color(255, 215, 60));

    drawStr("Your Fleet (Alive: " + to_string(countAliveShips(v)) + ")",
        (float)LEFT_BOARD_X, 78.0f, 21, Color::White);
    drawStr("Enemy Waters (Alive: " + to_string(countAliveShips(1 - v)) + ")",
        (float)RIGHT_BOARD_X, 78.0f, 21, Color::White);

    bool showEnemy = false;
    if (currentScreen == SCR_OVER)
    {
        showEnemy = true;
    }
    drawBoard(v, LEFT_BOARD_X, BOARD_TOP, true, t);
    drawBoard(1 - v, RIGHT_BOARD_X, BOARD_TOP, showEnemy, t);

    if (currentScreen == SCR_FIGHT)
    {
        if (moveMode && pickedShipId != -1)
        {
            drawGhostPreview(LEFT_BOARD_X, BOARD_TOP, v, shipLengths[pickedShipId], moveDir);
        }
        drawReconMarker(v);
    }

    int hr = 0;
    int hc = 0;
    if (currentScreen == SCR_FIGHT && (activePlayer == 0 || !singlePlayer) && !shotActive && !moveMode)
    {
        if (mouseToGrid(RIGHT_BOARD_X, BOARD_TOP, hr, hc))
        {
            RectangleShape cursor({ (float)(TILE - 2), (float)(TILE - 2) });
            cursor.setPosition({
                (float)(RIGHT_BOARD_X + hc * TILE + 1),
                (float)(BOARD_TOP + hr * TILE + 1)
            });
            cursor.setFillColor(Color::Transparent);
            cursor.setOutlineColor(Color::Yellow);
            cursor.setOutlineThickness(2.0f);
            app.draw(cursor);
        }
    }

    drawCannonShot(v);
    drawBattleHud(v);
}

// Muneeb
void eraseShipFromGrid(int p, int id, int startR, int startC, int dir)
{
    int len = shipLengths[id];

    for (int k = 0; k < len; k++)
    {
        if (dir == 1)
        {
            setShipCell(p, startR + k, startC, 0);
        }
        else
        {
            setShipCell(p, startR, startC + k, 0);
        }
    }
    saveShipMeta(p, id, -1, -1, dir);
}

void cancelReposition()
{
    if (pickedShipId != -1)
    {
        writeShipToGrid(savedShipPlayer, pickedShipId, savedShipR, savedShipC, savedShipDir);
        pickedShipId = -1;
    }
    moveMode = false;
}

void resetAbilities()
{
    for (int p = 0; p < 2; p++)
    {
        repositionUsed[p] = false;
        reconUsed[p] = false;
        reconRow[p] = -1;
        reconCol[p] = -1;
        reconFound[p] = -1;
    }
    moveMode = false;
    pickedShipId = -1;
    savedShipPlayer = 0;
    savedShipR = -1;
    savedShipC = -1;
    savedShipDir = 0;
    moveDir = 0;
    reconMode = false;
    coinTimer = 0.0f;
}

void startHandoff(int targetScreen)
{
    cancelReposition();
    reconMode = false;
    nextAfterHandoff = targetScreen;
    currentScreen = SCR_HANDOFF;
}

void startCoinScreen()
{
    tossCoin();
    coinTimer = 0.0f;
    currentScreen = SCR_COIN;
}

void updateScreens(float dt)
{
    if (currentScreen == SCR_COIN)
    {
        coinTimer += dt;
    }
    else if (currentScreen == SCR_FIGHT)
    {
        if (gameOver)
        {
            cancelReposition();
            reconMode = false;
            currentScreen = SCR_OVER;
        }
        else if (turnPassed)
        {
            clearTurnPassed();
            cancelReposition();
            reconMode = false;
            if (!singlePlayer)
            {
                startHandoff(SCR_FIGHT);
            }
        }
    }
}

void clickMenu(bool leftBtn)
{
    if (!leftBtn)
    {
        return;
    }

    for (int i = 0; i < MENU_COUNT; i++)
    {
        if (!insideBox(MENU_X, MENU_TOP + (float)i * MENU_GAP, MENU_W, MENU_H))
        {
            continue;
        }

        uiClicked = true;
        if (i == 4)
        {
            app.close();
            return;
        }

        bool onePlayer = false;
        if (i == 0 || i == 1)
        {
            onePlayer = true;
        }

        bool ammoMode = false;
        if (i == 1 || i == 3)
        {
            ammoMode = true;
        }

        setupMatchMode(onePlayer, ammoMode);
        resetAbilities();
        currentScreen = SCR_SETUP;
        return;
    }
}

void clickSetup(bool leftBtn)
{
    if (!leftBtn)
    {
        placeDir = 1 - placeDir;
        return;
    }

    int r = 0;
    int c = 0;
    if (mouseToGrid(LEFT_BOARD_X, BOARD_TOP, r, c))
    {
        if (canPlaceHere(activePlayer, r, c, shipLengths[placingStep], placeDir))
        {
            writeShipToGrid(activePlayer, placingStep, r, c, placeDir);
            uiClicked = true;
            placingStep++;

            if (placingStep == SHIP_COUNT)
            {
                placingStep = 0;
                placeDir = 0;

                if (singlePlayer)
                {
                    placeComputerFleet();
                    startCoinScreen();
                }
                else if (activePlayer == 0)
                {
                    setActivePlayer(1);
                    startHandoff(SCR_SETUP);
                }
                else
                {
                    startCoinScreen();
                }
            }
        }
    }
}

void clickReposition()
{
    int r = 0;
    int c = 0;
    if (!mouseToGrid(LEFT_BOARD_X, BOARD_TOP, r, c))
    {
        return;
    }

    if (pickedShipId == -1)
    {
        int val = getShipCell(activePlayer, r, c);
        if (val > 0)
        {
            int id = val - 1;
            if (getHealth(activePlayer, id) == shipLengths[id])
            {
                pickedShipId = id;
                savedShipPlayer = activePlayer;
                savedShipR = getShipRow(activePlayer, id);
                savedShipC = getShipCol(activePlayer, id);
                savedShipDir = getShipDir(activePlayer, id);
                moveDir = savedShipDir;
                eraseShipFromGrid(activePlayer, id, savedShipR, savedShipC, savedShipDir);
                uiClicked = true;
            }
        }
    }
    else
    {
        if (r == savedShipR && c == savedShipC && moveDir == savedShipDir)
        {
            cancelReposition();
        }
        else if (canPlaceHere(activePlayer, r, c, shipLengths[pickedShipId], moveDir))
        {
            writeShipToGrid(activePlayer, pickedShipId, r, c, moveDir);
            repositionUsed[activePlayer] = true;
            pickedShipId = -1;
            moveMode = false;
        }
        else
        {
            cancelReposition();
        }
        uiClicked = true;
    }
}

void clickRecon()
{
    int r = 0;
    int c = 0;
    if (!mouseToGrid(RIGHT_BOARD_X, BOARD_TOP, r, c))
    {
        return;
    }

    int enemy = 1 - activePlayer;
    if (getShotCell(enemy, r, c) != 0)
    {
        return;
    }

    reconRow[activePlayer] = r;
    reconCol[activePlayer] = c;
    if (getShipCell(enemy, r, c) > 0)
    {
        reconFound[activePlayer] = 1;
    }
    else
    {
        reconFound[activePlayer] = 0;
    }
    reconUsed[activePlayer] = true;
    reconMode = false;
    uiClicked = true;
}

void clickBattle(bool leftBtn)
{
    if (shotActive)
    {
        return;
    }
    if (singlePlayer && activePlayer == 1)
    {
        return;
    }

    if (!leftBtn)
    {
        if (moveMode && pickedShipId != -1)
        {
            moveDir = 1 - moveDir;
        }
        return;
    }

    if (moveMode)
    {
        clickReposition();
        return;
    }
    if (reconMode)
    {
        clickRecon();
        return;
    }

    int r = 0;
    int c = 0;
    if (mouseToGrid(RIGHT_BOARD_X, BOARD_TOP, r, c))
    {
        launchShot(activePlayer, r, c);
    }
}

void handleMouseClick(bool leftBtn)
{
    switch (currentScreen)
    {
    case SCR_MENU:
        clickMenu(leftBtn);
        break;
    case SCR_SETUP:
        clickSetup(leftBtn);
        break;
    case SCR_COIN:
        if (leftBtn && coinTimer >= COIN_SPIN_TIME)
        {
            uiClicked = true;
            if (singlePlayer)
            {
                currentScreen = SCR_FIGHT;
            }
            else
            {
                startHandoff(SCR_FIGHT);
            }
        }
        break;
    case SCR_FIGHT:
        clickBattle(leftBtn);
        break;
    case SCR_OVER:
        if (leftBtn)
        {
            uiClicked = true;
            currentScreen = SCR_MENU;
        }
        break;
    case SCR_HANDOFF:
        if (leftBtn)
        {
            uiClicked = true;
            currentScreen = nextAfterHandoff;
        }
        break;
    }
}

void handleKeyPress(Keyboard::Key key)
{
    if (key == Keyboard::Key::R)
    {
        if (currentScreen == SCR_SETUP)
        {
            placeDir = 1 - placeDir;
        }
        else if (currentScreen == SCR_FIGHT && moveMode && pickedShipId != -1)
        {
            moveDir = 1 - moveDir;
        }
        return;
    }

    if (currentScreen != SCR_FIGHT || shotActive)
    {
        return;
    }
    if (singlePlayer && activePlayer == 1)
    {
        return;
    }

    if (key == Keyboard::Key::P && !repositionUsed[activePlayer])
    {
        if (moveMode)
        {
            cancelReposition();
        }
        else
        {
            moveMode = true;
            pickedShipId = -1;
            reconMode = false;
        }
    }
    else if (key == Keyboard::Key::T && !reconUsed[activePlayer])
    {
        cancelReposition();
        if (reconMode)
        {
            reconMode = false;
        }
        else
        {
            reconMode = true;
        }
    }
}

// Abbas
bool loadSystemFont()
{
    vector<string> paths = {
        "arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf"
    };

    for (int i = 0; i < (int)paths.size(); i++)
    {
        if (filesystem::exists(paths[i]) && mainFont.openFromFile(paths[i]))
        {
            return true;
        }
    }
    return false;
}

int main()
{
    // Shayan
    srand((unsigned int)time(0));

    // Abbas
    app.create(VideoMode({ 1000, 600 }), "Battleships", Style::Titlebar | Style::Close);
    app.setFramerateLimit(60);

    if (!loadSystemFont())
    {
        return 1;
    }

    SoundBuffer clickBuf;
    SoundBuffer fireBuf;
    SoundBuffer hitBuf;
    SoundBuffer missBuf;
    SoundBuffer winBuf;
    if (!makeTone(clickBuf, 520.0f, 0.05f) ||
        !makeTone(fireBuf, 115.0f, 0.24f) ||
        !makeTone(hitBuf, 80.0f, 0.45f) ||
        !makeTone(missBuf, 240.0f, 0.18f) ||
        !makeTone(winBuf, 650.0f, 0.75f))
    {
        return 1;
    }

    Sound clickSfx(clickBuf);
    Sound fireSfx(fireBuf);
    Sound hitSfx(hitBuf);
    Sound missSfx(missBuf);
    Sound winSfx(winBuf);

    // Shayan
    clearMatchData();

    // Muneeb
    resetAbilities();

    // Abbas
    Clock frameClock;

    while (app.isOpen())
    {
        float elapsed = totalClock.getElapsedTime().asSeconds();
        float dt = frameClock.restart().asSeconds();
        if (dt > 0.1f)
        {
            dt = 0.1f;
        }

        // Muneeb
        Vector2i mpos = Mouse::getPosition(app);
        curMouseX = (float)mpos.x;
        curMouseY = (float)mpos.y;

        optional<Event> ev = app.pollEvent();
        while (ev.has_value())
        {
            if (ev->is<Event::Closed>())
            {
                app.close();
            }

            const Event::KeyPressed* keyEv = ev->getIf<Event::KeyPressed>();
            if (keyEv != nullptr)
            {
                handleKeyPress(keyEv->code);
            }

            const Event::MouseButtonPressed* mouseEv = ev->getIf<Event::MouseButtonPressed>();
            if (mouseEv != nullptr)
            {
                curMouseX = (float)mouseEv->position.x;
                curMouseY = (float)mouseEv->position.y;
                handleMouseClick(mouseEv->button == Mouse::Button::Left);
            }

            ev = app.pollEvent();
        }

        // Shayan
        if (currentScreen == SCR_FIGHT)
        {
            updateBattle(dt);
        }

        // Muneeb
        updateScreens(dt);

        // Abbas
        updateAudio(clickSfx, fireSfx, hitSfx, missSfx, winSfx);

        app.clear(Color(10, 20, 42));
        drawBackground();

        switch (currentScreen)
        {
        case SCR_MENU:
            renderMenu();
            break;
        case SCR_SETUP:
            renderSetup(elapsed);
            break;
        case SCR_COIN:
            renderCoin();
            break;
        case SCR_HANDOFF:
            renderHandoff();
            break;
        case SCR_FIGHT:
        case SCR_OVER:
            renderBattle(elapsed);
            break;
        }

        app.display();
    }

    return 0;
}