/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 30;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 30;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 30;       /* bar height */
static const char *fonts[]          = { "Menlo for Powerline:size=14", "Twemoji:pixelsize=12:antialias=true:autohint=true" };
static const char dmenufont[]       = "Menlo for Powerline:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_magenta[]     = "#ff6ac1";
static const char col_gray5[]       = "#282a36";
static const char col_pink1[]       = "#edccea";
static const char col_pink2[]       = "#d696d1";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray5, col_gray2 },
	[SchemeSel]  = { col_gray5, col_pink1,  col_magenta },
	[SchemeStatus]  = { col_gray3, col_gray5,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { col_gray5, col_pink1,  "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm]  = { col_gray3, col_gray5,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]  = { col_gray5, col_pink1,  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm]  = { col_gray3, col_gray5,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* Monitors IDs : 2 - 0 - 1 */
        /* class                instance        title           tags mask  isfloating  isterminal   noswallow monitor */
        { "Gimp",               NULL,           NULL,           0,          1,          0,          0,         -1 },
        { "Brave-browser",      NULL,           "Messenger",    1 << 1,     0,          0,          0,          2 },
        { "Brave-browser",      NULL,           NULL,           0,          0,          0,          0,          2 },
        { "St",                 NULL,           NULL,           0,          0,          1,          0,         -1 },
        { "Ankama Launcher",    NULL,           NULL,           1 << 8,     0,          0,          0,          0 },
        { "dofus.exe",          NULL,           NULL,           1 << 8,     0,          0,          0,          0 },
        { "Spotify",            NULL,           NULL,           1 << 2,     0,          0,          0,          2 },
        { "scrot",              NULL,           NULL,           0,          0,          0,          0,         -1 },
        { "factorio",           NULL,           NULL,           1 << 8,     0,          0,          0,         -1 },
        { "discord",            NULL,           NULL,           1 << 1,     0,          0,          0,          2 },
        { "TelegramDesktop",    NULL,           NULL,           1 << 1,     0,          0,          0,          2 },
        { "Calculator",         NULL,           NULL,           0,          1,          0,          0,         -1 },
        { "xfreerdp",           NULL,           NULL,           0,          1,          0,          1,         -1 },
        { NULL,                 "oriwotw.exe",  NULL,           0,          1,          0,          1,         -1 },
        { "md2html-electron",   NULL,           NULL,           0,          1,          0,          1,         -1 },
        { "Yad",                NULL,           NULL,           0,          1,          0,          1,         -1 },
        { "explorer.exe",       NULL,           NULL,           0,          1,          0,          1,         -1 },
        { NULL,                 NULL,           "Event Tester", 0,          1,          0,          1,         -1 }, /* xev */
        { "dmenu",              NULL,           NULL,           0,          1,          0,          1,         -1 },
        { "dev",                NULL,           NULL,           0,          1,          0,          1,         -1 },
        { NULL,                 NULL,           "dev",          0,          1,          0,          1,         -1 },
        { "Minecraft",          NULL,           NULL,           0,          1,          0,          1,         -1 },
        { "Minecraft Launcher", NULL,           NULL,           0,          1,          0,          1,         -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int decorhints  = 1;    /* 1 means respect decoration hints */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "◫",      tile },    /* first entry is default */
	{ "🐟",      NULL },    /* no layout function means floating behavior */
	{ "▣",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define DECRVOL XF86XK_AudioLowerVolume
#define INCRVOL XF86XK_AudioRaiseVolume
#define MUTEVOL XF86XK_AudioMute

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-h", "30", "-l", "10", "-c", "-fn", dmenufont, "-nb", col_gray5, "-nf", col_gray3, "-sb", col_pink1, "-sf", col_gray5, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *rangercmd[]  = { "st", "ranger", "--cmd=tab_close", NULL };
static const char *dwmbindings[] = { "st", "dwmbindings", NULL };
static const char *printscreen[] = { "printscreen", NULL }; 
static const char *incrvol[] = { "volmgr", "--change-volume", "+1", NULL };
static const char *decrvol[] = { "volmgr", "--change-volume", "-1", NULL };
static const char *mutevol[] = { "volmgr", "--toggle-mute", NULL };
static const char *shutrest[] = { "shut-rest", NULL };
static const char *lockscreen[] = { "slock", NULL };
static const char *notes[] = { "notes", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,			            XK_e,	   spawn,          {.v = rangercmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY,                       XK_a,      setmfact,       {.f = +0.05} },
	{ MODKEY|Mod1Mask,              XK_h,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_l,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_y,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrohgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { False,                        XK_Print,  spawn,          {.v = printscreen } },
    { MODKEY,                       XK_F1,     spawn,          {.v = dwmbindings } },
    { False,                        INCRVOL,   spawn,          {.v = incrvol } },
    { False,                        DECRVOL,   spawn,          {.v = decrvol } },
    { False,                        MUTEVOL,   spawn,          {.v = mutevol } },
    { MODKEY|ShiftMask|ControlMask, XK_BackSpace, spawn,       {.v = shutrest } },
    { MODKEY|ShiftMask|ControlMask, XK_l,      spawn,          {.v = lockscreen } },
    { MODKEY,                       XK_n,      spawn,          {.v = notes } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};

