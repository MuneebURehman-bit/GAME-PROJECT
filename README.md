# Battleships

A 2D Battleships game for one or two players, built in C++ with SFML 3.1.0. Destroy all of the enemy's ships before they destroy yours.

## Requirements

- C++ compiler
- [SFML 3.1.0](https://www.sfml-dev.org/)
- Developed in Visual Studio Code

## How to run

Open the project folder in VS Code, open a terminal, and run:

```
<ADD YOUR RUN COMMAND HERE>
```

A menu screen will appear once the game starts.

## Game modes

The menu has four buttons:

| Button | Description |
|---|---|
| 1 Player - Classic | Play against the computer with unlimited ammo |
| 1 Player - Limited Ammo | Play against the computer with 40 shots |
| 2 Player - Classic | Two players take turns on the same screen, unlimited ammo |
| 2 Player - Limited Ammo | Two players, 40 shots each |

## How to play

### Placing your ships

Each player places 5 ships on a 10x10 grid. The ships have lengths 5, 4, 3, 3 and 2.

- Move the cursor to choose a position.
- Press **R** to rotate the ship between horizontal and vertical.
- **Click** to set the ship and move on to the next one.

When all ships are placed, a coin toss decides who goes first.

### Taking turns

The game is turn based. On your turn, click a cell on the opponent's grid to fire a cannon shot.

- **Orange circle:** hit
- **White dot:** miss
- A hit gives you another shot. A miss passes the turn.
- An enemy ship becomes visible once it has been completely destroyed.
- The number of ships still alive is shown above each grid.
- Your opponent's shots are also shown on your own grid. Their misses appear as white dots, and any of your ships they hit shows an orange circle on the struck square.

The first player to destroy all 5 enemy ships wins.

### 1 Player mode

You play against the computer. Both grids are visible at the same time. You can always see your own ships, but the computer's ships stay hidden until destroyed.

### 2 Player mode

The current player's board is shown on the left, with their own ships and any enemy ships they have destroyed visible. At the end of each turn, a handover screen asks the other player to look away before the next turn begins.

### Limited Ammo mode

Each player has 40 shots. The remaining ammo is shown below the grid and updates in real time.

If the ammo runs out before all ships are destroyed, the winner is decided as follows:

1. The player with more successful shots wins.
2. If tied, the player who destroyed more ships wins.
3. If still tied, the game is a draw.

## Special abilities

Both players have two abilities in every mode, shown below their grid. Each ability can be used **once per match**.

### Reposition

Shown as `Reposition[P]: Available/Used`.

1. Press **P**.
2. Click any one of your ships that has not been hit.
3. Move it to a new position (press **R** to rotate it).

### Recon

Shown as `Recon[T]: Available/Used`.

1. Press **T**.
2. Click any square on the opponent's grid to scan it.

The result is shown as a message, for example `Recon results: Ship at R9, C6`, where R is the row and C is the column. If the square is empty, the message says `Water` instead of `Ship`. The scanned square stays highlighted on all of your later turns.

## End of game

When the game ends, a victory or draw message is shown. Click anywhere to return to the menu.
