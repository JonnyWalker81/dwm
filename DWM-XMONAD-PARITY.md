# DWM XMonad Parity - Project State

## Repository

- **GitHub**: https://github.com/JonnyWalker81/dwm
- **Local clone**: `~/Repositories/dwm`
- **Branch**: `xmonad-parity` (based on upstream DWM 6.8)
- **Upstream remote**: `upstream` -> `https://git.suckless.org/dwm`
- **Current commit**: `b65fcbadf1eb3b8e97a9bf37225f08594ec07af0`

## Current Status: Integrated into NixOS

DWM is fully integrated into the NixOS configuration and selectable as a login session in SDDM alongside XMonad, AwesomeWM, and Hyprland. The build succeeded and a `nixos-rebuild switch` will make it available.

**What works:**
- DWM compiles via NixOS overlay from the GitHub repo
- SDDM shows "dwm" as a selectable session at login
- All keybindings match XMonad config
- All layouts (tile, monocle, grid, three-column, floating) are functional
- Autostart script launches wallpaper, greenclip, emacs daemon, picom, status bar
- Status bar replicates xmobar info (weather, CPU, memory, disk, uptime, date/time)

**Not yet tested at runtime:**
- Actually logging into DWM session via SDDM (build verified, not yet switched)
- Status bar visual appearance (may need tuning)
- Picom compositing behavior under DWM vs XMonad
- Font Awesome icons in DWM bar (DWM bar is simpler than xmobar)

## NixOS Integration Architecture

```
flake.nix
  overlays (applied globally to all system configs)
    └── overlays/dwm.nix         # Overrides nixpkgs dwm with our patched build

machines/vm-shared.nix (line 233)
  └── services.xserver.windowManager.dwm.enable = true;
      # Creates SDDM session entry "dwm" (none+dwm.desktop)

users/cipher/home-manager.nix    # Deploys autostart + statusbar scripts
users/jrothberg/home-manager.nix # Same for jrothberg user

users/dwm/autostart.sh           # Source for ~/.local/share/dwm/autostart.sh
users/dwm/dwm-statusbar.sh       # Source for ~/.local/share/dwm/dwm-statusbar.sh
```

### NixOS Overlay (`~/nixos-config/overlays/dwm.nix`)

Overrides the nixpkgs `dwm` package with our GitHub source. The NixOS `windowManager.dwm` module uses whatever `pkgs.dwm` resolves to, so the overlay is the only integration point needed.

```nix
final: prev: {
  dwm = prev.dwm.overrideAttrs (old: {
    src = prev.fetchFromGitHub {
      owner = "JonnyWalker81";
      repo = "dwm";
      rev = "b65fcbadf1eb3b8e97a9bf37225f08594ec07af0";  # UPDATE after pushing changes
      sha256 = "sha256-ykUoxP4+LFi83ceGEChEKgUzlLf8D+q6EZfs/XrH1xo=";  # UPDATE after pushing changes
    };
    nativeBuildInputs = (old.nativeBuildInputs or [ ]) ++ [ prev.pkg-config ];
    buildInputs = (old.buildInputs or [ ]) ++ [
      prev.xorg.libX11 prev.xorg.libXft prev.xorg.libXinerama
      prev.freetype prev.fontconfig
    ];
  });
}
```

### Home-Manager Deployment (both `cipher` and `jrothberg`)

```nix
home.file.".local/share/dwm/autostart.sh" = {
  source = ../dwm/autostart.sh;
  executable = true;
};
home.file.".local/share/dwm/dwm-statusbar.sh" = {
  source = ../dwm/dwm-statusbar.sh;
  executable = true;
};
```

## Workflow: Updating DWM After Source Changes

1. Make changes in `~/Repositories/dwm` (edit `dwm.c`, `config.h`, etc.)
2. Build locally to verify: `nix-shell -p xorg.libX11.dev xorg.libXft.dev xorg.libXinerama.dev pkg-config freetype.dev fontconfig.dev gnumake gcc xorg.xorgproto --run "make clean && make"`
3. Commit and push:
   ```bash
   cd ~/Repositories/dwm
   git add -A && git commit -m "description"
   git push origin xmonad-parity
   ```
4. Get the new commit hash and source hash:
   ```bash
   REV=$(git rev-parse HEAD)
   nix-prefetch-url --unpack "https://github.com/JonnyWalker81/dwm/archive/${REV}.tar.gz"
   # Convert the output hash to SRI format:
   nix hash convert --to sri --hash-algo sha256 <hash-from-above>
   ```
5. Update `~/nixos-config/overlays/dwm.nix` with new `rev` and `sha256`
6. Rebuild NixOS:
   ```bash
   cd ~/nixos-config
   sudo nixos-rebuild switch --flake ".#vm-aarch64-prl"
   ```

## Source Modifications to `dwm.c`

- Added `gappx` field to `Monitor` struct for per-monitor gap tracking
- Added `focusmaster()` function -- focuses the master window (mod+v), toggles back if already on master
- Added `togglefullscr()` function -- actual fullscreen toggle (mod+m)
- Added `togglegaps()` function -- toggle gaps on/off (mod+Shift+g)
- Rewrote `tile()` layout to be gap-aware (respects `m->gappx` for inner/outer gaps)
- Added `grid()` layout function -- gap-aware grid layout
- Added `tcl()` layout function -- gap-aware three-column-layout (master centered)
- Applied `autostart` patch (from suckless.org) -- runs `~/.local/share/dwm/autostart.sh` on startup
- All new functions have proper forward declarations

### Other Modified/New Files

- `config.h` -- Full XMonad-matching config (see tables below)
- `config.mk` -- Replaced hardcoded `/usr/X11R6/` paths with `pkg-config` for NixOS compatibility
- `movestack.c` -- Swap focused window with next/prev in stack, included via `#include` in config.h
- `tcl.c` -- Downloaded from suckless.org (unused, implementation moved inline to dwm.c)
- `dwm.1` -- Modified by autostart patch
- `.gitignore` -- Ignores `*.o`, `dwm` binary, `*.tar.gz`
- `patches/` -- Downloaded reference patch files

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
`"coding", "web", "services", "work", "misc", "6", "7", "8", "9"` -- matches `myWorkspaces`

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

## Status Bar (`dwm-statusbar.sh`)

DWM reads status from `xsetroot -name`. The status bar script runs in a loop (2s interval) and replicates xmobar's right-side info:

```
  72F, Sunny  |  cpu: 5%  |  mem: 42%  |  hdd: 15G free  |  uptime: 3d 7h  |  Feb 06 2026 - 14:30 (22:30 UTC)
```

**Sections (matching xmobar):**
| Section | Source | xmobar Equivalent |
|---|---|---|
| Weather | `wttr.in/91105` (cached 10min) | `Run Com "scripts/wttr.sh"` |
| CPU | `/proc/stat` delta | `Run Cpu` |
| Memory | `free -m` | `Run Memory` |
| Disk | `df -h /` | `Run DiskU` |
| Uptime | `/proc/uptime` | `Run Uptime` |
| Date/Time | `date` local + UTC | `Run Com "xmobar-datetime.sh"` |

**Managed by**: `~/nixos-config/users/dwm/dwm-statusbar.sh` -> deployed to `~/.local/share/dwm/dwm-statusbar.sh` via home-manager.

## Autostart Script (`autostart.sh`)

Called by DWM's autostart patch on startup. Mirrors XMonad's `myStartupHook`:

| Process | Purpose | XMonad Equivalent |
|---|---|---|
| `feh --bg-fill` | Set wallpaper | `spawnOnce "feh --bg-fill ..."` |
| `greenclip daemon` | Clipboard manager | `spawnOnce "greenclip daemon"` |
| `emacs --daemon` | Emacs server | `spawnOnce "emacs --daemon"` |
| `picom --daemon` | Compositor | picom systemd service |
| `dwm-statusbar.sh` | Status bar loop | xmobar spawned in main |

**Managed by**: `~/nixos-config/users/dwm/autostart.sh` -> deployed to `~/.local/share/dwm/autostart.sh` via home-manager.

## Files Reference

### DWM Repository (`~/Repositories/dwm/`)
```
.git/
.gitignore          (ignores *.o, dwm binary, *.tar.gz)
config.def.h        (original upstream, untouched)
config.h            (XMonad-matching config)
config.mk           (modified: pkg-config for NixOS)
drw.c               (original)
drw.h               (original)
dwm.1               (modified by autostart patch)
dwm.c               (modified: gaps, focusmaster, togglefullscr, togglegaps, grid, tcl)
dwm.png             (original)
DWM-XMONAD-PARITY.md (this file)
LICENSE             (original)
Makefile            (original)
movestack.c         (movestack patch)
patches/            (downloaded reference patch files)
README              (original)
tcl.c               (downloaded reference, implementation is inline in dwm.c)
transient.c         (original)
util.c              (original)
util.h              (original)
```

### NixOS Config (`~/nixos-config/`) -- DWM-related files
```
overlays/dwm.nix                     # Overlay: builds DWM from GitHub source
machines/vm-shared.nix                # Line 233: windowManager.dwm.enable = true
users/dwm/autostart.sh               # Source: DWM autostart script
users/dwm/dwm-statusbar.sh           # Source: DWM status bar script
users/cipher/home-manager.nix         # Deploys dwm/ files to ~/.local/share/dwm/
users/jrothberg/home-manager.nix      # Same for jrothberg user
```

## Potential Future Work

- **Runtime testing**: Log into DWM session, verify everything works end-to-end
- **Status bar polish**: Consider `dwmblocks` or `slstatus` for click-to-expand, colored segments, etc. The current `xsetroot` approach is plain text only (no colors in DWM's built-in bar without the `status2d` patch)
- **Status2d patch**: Would allow colored status bar segments similar to xmobar's colored boxes
- **Systray patch**: Add system tray to DWM bar (xmobar has `trayer` integration)
- **Per-tag layouts**: DWM uses a single layout list cycled via mod+Space; XMonad remembers layout per-workspace
- **XMonad layouts not yet ported**: `threeRow`, `oneBig`, `space` (XMonad has 9 layouts vs DWM's 5)
- **Picom exclusion rules**: XMonad's picom config excludes xmobar from blur/shadows; DWM bar may need similar rules added to `~/.config/picom/picom.conf` or `users/common.nix`
- **Restart-in-place**: DWM doesn't have XMonad's `mod+q` recompile-and-restart; consider adding a `SIGHUP` handler or the `restart` patch
