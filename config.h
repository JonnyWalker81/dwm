/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows (XMonad: myBorderWidth = 2) */
static const unsigned int snap      = 32;       /* snap pixel */
static const int gappx              = 8;        /* gap pixel between windows (XMonad: myWindowSpacing = 8) */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Ubuntu:weight=bold:pixelsize=13:antialias=true:hinting=true",
                                        "Mononoki:weight=bold:pixelsize=13:antialias=true:hinting=true",
                                        "Font Awesome 6 Free Solid:pixelsize=13:antialias=true:hinting=true",
                                        "Font Awesome 6 Brands:pixelsize=13:antialias=true:hinting=true" };
static const char dmenufont[]       = "Ubuntu:weight=bold:pixelsize=13:antialias=true:hinting=true";

/* Omarchy / Nord color scheme matching XMonad config */
static const char col_normbg[]      = "#24283B"; /* normal background - Omarchy code background */
static const char col_normfg[]      = "#a9b1d6"; /* normal foreground - Omarchy soft blue-gray */
static const char col_normborder[]  = "#24283B"; /* unfocused border - myNormalBorderColor */
static const char col_selbg[]       = "#2E3440"; /* selected background - Nord dark */
static const char col_selfg[]       = "#B4F9F8"; /* selected foreground - Omarchy bright cyan */
static const char col_selborder[]   = "#B4F9F8"; /* focused border - myFocusedBorderColor */

static const char *colors[][3]      = {
	/*               fg            bg            border   */
	[SchemeNorm] = { col_normfg,   col_normbg,   col_normborder },
	[SchemeSel]  = { col_selfg,    col_selbg,    col_selborder  },
};

/* tagging - matching XMonad workspaces */
static const char *tags[] = { "coding", "web", "services", "work", "misc", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "MPlayer",  NULL,       NULL,       0,            1,           -1 },
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] (XMonad: 1/2) */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals (0 for even gaps) */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

#include "movestack.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "=[]",      tile },    /* first entry is default - master right, stack left */
	{ "[M]",      monocle }, /* like XMonad "monocle" (Full) */
	{ "HHH",      grid },   /* like XMonad "grid" */
	{ "|||",      tcl },    /* like XMonad "threeColMid" (three column) */
	{ "><>",      NULL },    /* floating - like XMonad "floats" */
};

/* key definitions */
/* XMonad uses mod1Mask (Left Alt) as modkey */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_normbg, "-nf", col_normfg, "-sb", col_selbg, "-sf", col_selfg, NULL };
static const char *termcmd[]  = { "ghostty", NULL };
static const char *rofiruncmd[] = { "rofi", "-show", "run", NULL };
static const char *rofiwindowcmd[] = { "rofi", "-show", "window", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */

	/* --- Launchers (matching XMonad keybindings) --- */
	/* mod+Shift+Return: Launch terminal (ghostty) */
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	/* mod+p: Launch rofi (run mode) - XMonad uses rofi instead of dmenu */
	{ MODKEY,                       XK_p,      spawn,          {.v = rofiruncmd } },
	/* mod+w: Launch rofi (window mode) */
	{ MODKEY,                       XK_w,      spawn,          {.v = rofiwindowcmd } },
	/* mod+r: Clipboard manager (greenclip via rofi) */
	{ MODKEY,                       XK_r,      spawn,          SHCMD("rofi -modi \"clipboard:greenclip print\" -show clipboard -run-command '{cmd}'") },
	/* mod+Shift+p: Launch gmrun */
	{ MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("gmrun") },

	/* --- Window management (matching XMonad keybindings) --- */
	/* mod+Shift+c: Kill focused window */
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	/* mod+b: Toggle status bar */
	{ MODKEY,                       XK_b,      togglebar,      {0} },

	/* --- Focus navigation --- */
	/* mod+Tab: Focus next window */
	{ MODKEY,                       XK_Tab,    focusstack,     {.i = +1 } },
	/* mod+j: Focus next window */
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	/* mod+k: Focus previous window */
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	/* mod+v: Focus master window */
	{ MODKEY,                       XK_v,      focusmaster,    {0} },

	/* --- Window swapping --- */
	/* mod+Return: Swap focused window with master (zoom) */
	{ MODKEY,                       XK_Return, zoom,           {0} },
	/* mod+Shift+j: Swap focused with next window */
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	/* mod+Shift+k: Swap focused with previous window */
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },

	/* --- Master area resizing --- */
	/* mod+h: Shrink master area */
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	/* mod+l: Expand master area */
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },

	/* --- Master count --- */
	/* mod+,: Increment master count */
	{ MODKEY,                       XK_comma,  incnmaster,     {.i = +1 } },
	/* mod+.: Decrement master count */
	{ MODKEY,                       XK_period, incnmaster,     {.i = -1 } },

	/* --- Fullscreen toggle (like XMonad mod+m toggleFullscreen) --- */
	{ MODKEY,                       XK_m,      togglefullscr,  {0} },

	/* --- Layout switching --- */
	/* mod+Space: Cycle forward through layout algorithms */
	{ MODKEY,                       XK_space,  cyclelayout,    {.i = +1} },
	/* mod+Shift+Space: Reset layout to default (toggle floating) */
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },

	/* --- Push window back into tiling --- */
	/* mod+t: Push window back into tiling (like XMonad) */
	{ MODKEY,                       XK_t,      togglefloating, {0} },

	/* --- Gap controls --- */
	/* mod+Shift+g: Toggle gaps on/off */
	{ MODKEY|ShiftMask,             XK_g,      togglegaps,     {0} },

	/* --- Screenshots (matching XMonad) --- */
	/* mod+s: Full screen screenshot */
	{ MODKEY,                       XK_s,      spawn,          SHCMD("/home/cipher/nixos-config/scripts/screenshot.sh") },
	/* mod+Shift+s: Selection screenshot */
	{ MODKEY|ShiftMask,             XK_s,      spawn,          SHCMD("/home/cipher/nixos-config/scripts/screenshot.sh -s") },

	/* --- View all tags / tag all --- */
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },

	/* --- Multi-monitor --- */
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	/* --- Quit --- */
	/* mod+Shift+q: Quit DWM (like XMonad quit) */
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

	/* --- Workspace switching (mod+[1..9]) and moving (mod+Shift+[1..9]) --- */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	/* mod+button1: Float window and move by dragging (like XMonad) */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	/* mod+button2: Raise window to top of stack (like XMonad) */
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	/* mod+button3: Float window and resize by dragging (like XMonad) */
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
