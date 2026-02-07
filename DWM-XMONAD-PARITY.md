# DWM XMonad Parity - Project State

## Branch
`xmonad-parity` in `~/Repositories/dwm` (based on upstream DWM 6.8)

## What Was Done

### Source Modifications to `dwm.c`
- Added `gappx` field to `Monitor` struct for per-monitor gap tracking
- Added `focusmaster()` function — focuses the master window (mod+v), toggles back if already on master
- Added `togglefullscr()` function — actual fullscreen toggle (mod+m)
- Added `togglegaps()` function — toggle gaps on/off (mod+Shift+g)
- Rewrote `tile()` layout to be gap-aware (respects `m->gappx` for inner/outer gaps)
- Added `grid()` layout function — gap-aware grid layout
- Added `tcl()` layout function — gap-aware three-column-layout (master centered)
- Applied `autostart` patch (from suckless.org) — runs `~/.local/share/dwm/autostart.sh` on startup
- All new functions have proper forward declarations

### New Files Created
- `movestack.c` — swap focused window with next/prev in stack (mod+Shift+j/k), included via `#include` in config.h
- `tcl.c` — downloaded from suckless.org (unused, implementation moved inline to dwm.c)
- `config.h` — full configuration matching XMonad (see details below)
- `movestack.c` — movestack patch implementation
- `.gitignore` — ignores `*.o`, `dwm` binary, `*.tar.gz`
- `patches/` directory — contains downloaded patch files for reference
- `~/.local/share/dwm/autostart.sh` — startup script (feh wallpaper, greenclip, emacs --daemon, picom)

### `config.mk` Changes
- Replaced hardcoded `/usr/X11R6/` paths with `pkg-config` for NixOS compatibility
- Build requires nix-shell: `nix-shell -p xorg.libX11.dev xorg.libXft.dev xorg.libXinerama.dev pkg-config freetype.dev fontconfig.dev gnumake gcc xorg.xorgproto --run "make"`

## config.h Settings (Matching XMonad)

### Appearance
| Setting | Value | XMonad Source |
|---|---|---|
| `borderpx` | `2` | `myBorderWidth = 2` |
| `gappx` | `8` | `myWindowSpacing = 8` |
| `snap` | `32` | default |
| `showbar` | `1` | xmobar always visible |
| `topbar` | `1` | xmobar at top |
| `fonts` | Ubuntu Bold 13px, Mononoki Bold 13px, Font Awesome 6 Free/Brands 13px | `.xmobarrc` fonts |
| `resizehints` | `0` | disabled for even gaps |
| `mfact` | `0.50` | `ResizableTall 1 (3/100) (1/2)` |

### Colors (Omarchy/Nord Theme)
| Element | Hex | XMonad Source |
|---|---|---|
| Normal BG | `#24283B` | `myNormalBorderColor` |
| Normal FG | `#a9b1d6` | xmobar `ppHidden` color |
| Normal Border | `#24283B` | `myNormalBorderColor` |
| Selected BG | `#2E3440` | xmobar `bgColor` |
| Selected FG | `#B4F9F8` | `myFocusedBorderColor` / `ppCurrent` |
| Selected Border | `#B4F9F8` | `myFocusedBorderColor` |

### Tags (Workspaces)
`"coding", "web", "services", "work", "misc", "6", "7", "8", "9"` — matches `myWorkspaces`

### Window Rules
| Class | Float | XMonad Source |
|---|---|---|
| `MPlayer` | yes | `className =? "MPlayer" --> doFloat` |
| `Gimp` | yes | `className =? "Gimp" --> doFloat` |

### Layouts
| Symbol | Function | XMonad Equivalent |
|---|---|---|
| `[]=` | `tile` | `tall` (ResizableTall) |
| `[M]` | `monocle` | `monocle` (Full) |
| `HHH` | `grid` | `grid` (Grid 16/10) |
| `\|\|\|` | `tcl` | `threeColMid` (ThreeColMid) |
| `><>` | floating | `floats` (simplestFloat) |

### Keybindings (All Match XMonad)
| Key | DWM Function | XMonad Equivalent |
|---|---|---|
| `mod+Shift+Return` | spawn terminal (ghostty) | `spawn $ XMonad.terminal conf` |
| `mod+p` | spawn rofi run | `spawn "rofi -show run"` |
| `mod+w` | spawn rofi window | `spawn "rofi -show window"` |
| `mod+r` | spawn greenclip/rofi clipboard | `clipboardy` |
| `mod+Shift+p` | spawn gmrun | `spawn "gmrun"` |
| `mod+Shift+c` | killclient | `kill` |
| `mod+b` | togglebar | `sendMessage ToggleStruts` |
| `mod+Tab` | focusstack +1 | `windows W.focusDown` |
| `mod+j` | focusstack +1 | `windows W.focusDown` |
| `mod+k` | focusstack -1 | `windows W.focusUp` |
| `mod+v` | focusmaster | `windows W.focusMaster` |
| `mod+Return` | zoom (swap master) | `windows W.swapMaster` |
| `mod+Shift+j` | movestack +1 | `windows W.swapDown` |
| `mod+Shift+k` | movestack -1 | `windows W.swapUp` |
| `mod+h` | setmfact -0.05 | `sendMessage Shrink` |
| `mod+l` | setmfact +0.05 | `sendMessage Expand` |
| `mod+,` | incnmaster +1 | `sendMessage (IncMasterN 1)` |
| `mod+.` | incnmaster -1 | `sendMessage (IncMasterN (-1))` |
| `mod+m` | togglefullscr | `toggleFullscreen` |
| `mod+Space` | cycle layout | `sendMessage NextLayout` |
| `mod+Shift+Space` | togglefloating | `setLayout $ XMonad.layoutHook conf` |
| `mod+t` | togglefloating (re-tile) | `withFocused $ windows . W.sink` |
| `mod+Shift+g` | togglegaps | `decWindowSpacing` / `incWindowSpacing` |
| `mod+s` | screenshot full | `screenshot.sh` |
| `mod+Shift+s` | screenshot selection | `screenshot.sh -s` |
| `mod+Shift+q` | quit | `io (exitWith ExitSuccess)` |
| `mod+[1-9]` | view tag N | `windows $ W.greedyView` |
| `mod+Shift+[1-9]` | tag client to N | `windows $ W.shift` |

### Mouse Bindings
| Button | Function | XMonad Equivalent |
|---|---|---|
| `mod+button1` | movemouse (float+drag) | `mouseMoveWindow` |
| `mod+button2` | togglefloating | `windows W.shiftMaster` (raise) |
| `mod+button3` | resizemouse (float+resize) | `mouseResizeWindow` |

## Git Status
- Branch: `xmonad-parity`
- Modified: `config.mk`, `dwm.1`, `dwm.c`
- New untracked: `config.h`, `movestack.c`, `tcl.c`, `patches/`, `.gitignore`
- Build artifacts (ignored): `*.o`, `dwm` binary
- **Nothing committed yet** — all changes are uncommitted

## Build Status
- Compiles successfully with only 2 benign warnings (unused `system()` return in autostart patch)
- Binary: `~/Repositories/dwm/dwm` (82K)

## Next Steps: Add DWM as a Login Session
To make DWM selectable at login (SDDM), you need to:

1. **Add DWM to NixOS configuration** — either:
   - Use `services.xserver.windowManager.dwm.enable = true;` with a custom package overlay pointing to `~/Repositories/dwm`
   - Or create a custom `.desktop` session file for the display manager

2. **NixOS overlay approach** (recommended):
   ```nix
   # In flake.nix or overlays/
   dwm = prev.dwm.overrideAttrs (old: {
     src = /home/cipher/Repositories/dwm;
   });
   ```
   Then enable in `machines/vm-shared.nix`:
   ```nix
   services.xserver.windowManager.dwm.enable = true;
   ```

3. **Status bar**: DWM reads status from `xsetroot -name`. You'll need a status bar script or use `dwmblocks`/`slstatus` to replicate xmobar functionality. Alternatively, you can run xmobar separately and pipe to `xsetroot`.

4. **Rebuild NixOS**: `sudo nixos-rebuild switch --flake ".#<config-name>"`

## Files Reference
```
~/Repositories/dwm/
├── .git/
├── .gitignore          (new)
├── config.def.h        (original, untouched)
├── config.h            (new - XMonad-matching config)
├── config.mk           (modified - pkg-config for NixOS)
├── drw.c               (original)
├── drw.h               (original)
├── dwm.1               (modified by autostart patch)
├── dwm.c               (modified - gaps, focusmaster, togglefullscr, togglegaps, grid, tcl layouts)
├── dwm.png             (original)
├── LICENSE              (original)
├── Makefile             (original)
├── movestack.c         (new - movestack patch)
├── patches/            (new - downloaded reference patches)
├── README              (original)
├── tcl.c               (new - downloaded reference, implementation is inline in dwm.c)
├── transient.c         (original)
├── util.c              (original)
└── util.h              (original)

~/.local/share/dwm/
└── autostart.sh        (new - startup script: feh, greenclip, emacs, picom)
```
