/* See LICENSE file for copyright and license details. */

/* appearance */
#include <X11/XF86keysym.h>
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 0;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 1;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 4;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 0;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;

// static const char *fonts[]          = { "Iosevka Nerd Font:size=16" };
// static const char dmenufont[]       = "Iosevka Nerd Font:size=16";
static const char *fonts[]          = { "Iosevka Nerd Font:size=10" };
static const char dmenufont[]       = "Iosevka Nerd Font:size=10";

static const char col_gray[]       = "#222222";
static const char col_gray1[]       = "#000000";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#478061";
static const char col_purple[]      = "#10521d";
static const unsigned int baralpha = 0x80;
static const unsigned int middy = 0x50;
static const unsigned int borderalpha = 0x00;

static const char *colors[][3]      = {
	/*               fg         bg         border   */
  // Greenline
	// [SchemeNorm] = { col_gray3, col_gray1, col_gray1 },
	//  [SchemeSel]  = { col_gray4, col_purple,  col_purple },

  //Normal
	[SchemeNorm] = { col_gray3, col_gray1, col_gray },
  [SchemeSel]  = { col_gray4, col_gray1,  col_gray2 },
};

static const unsigned int alphas[][3]      = {
    /*               fg      bg        border*/
    [SchemeNorm] = { OPAQUE, baralpha, baralpha },
	[SchemeSel]  = { OPAQUE, baralpha, middy },
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	// { "Gimp",     NULL,       NULL,       0,            1,           -1 },
	// { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	//  { "Thunar",  NULL,  NULL,  0,  1,  -1 },

/* class      instance    title       tags mask     isfloating   monitor    float x,y,w,h         floatborderpx*/
	{ "Gimp",     NULL,       NULL,       0,            1,           -1,        50,50,500,500,        5 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1,        50,50,500,500,        5 },
	{ "Thunar",   NULL,       NULL,       0     ,       1,           -1,        50,50,1300,800,        5 },
	{ "Pavucontrol", NULL,    NULL,       0     ,       1,           -1,        50,50,1300,800,        5 },
	{ "blueman-manager", NULL,    NULL,       0     ,       1,           -1,        50,50,1300,800,        5 },
	{ "Blueman-manager", NULL,    NULL,       0     ,       1,           -1,        50,50,1300,800,        5 },
	{ "nm-connection-editor", NULL,    NULL,       0     ,       1,           -1,        50,50,1300,800,        5 },
	{ "Nm-connection-editor", NULL,    NULL,       0     ,       1,           -1,        50,50,1300,800,        5 },
	{ "alacritty", NULL,    NULL,       0     ,       1,           -1,        50,50,1300,800,        5 },
	{ "Alacritty", NULL,    NULL,       0     ,       1,           -1,        50,50,1300,800,        5 },
	{ "kitty", NULL,    NULL,       0     ,       1,           -1,        50,50,1300,800,        5 },


};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	// { "[]=",      tile },    /* first entry is default */
	// { "><>",      NULL },    /* no layout function means floating behavior */
	// { "[M]",      monocle },
  
	{ "",      tile },    /* first entry is default */
	{ "[F]",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },

	// { "",      tile },    /* first entry is default */
	// { "",      NULL },    /* no layout function means floating behavior */
	// { "",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4,"-i", NULL };
static const char *termcmd[]  = { "st", NULL };
// static const char *tmux[] = { "st", "-e", "tmux new-session -A -s dev", NULL }; 
static const char *deftmux[] = { "st", "-e", "tmux" , NULL };
static const char *tmux[] = { "st", "-e", "tmux", "new-session", "-A", "-s", "dev", NULL };
static const char *tmux2[] = { "st", "-e", "tmux", "new-session", "-A", "-s", "serv", NULL };
static const char *pavucontrol[] = { "pavucontrol", NULL };
static const char *blueman_manager[] = { "blueman-manager", NULL };
static const char *network_editor[] = { "nm-connection-editor", NULL };
static const char *spotify[] = { "spotify", NULL };

// bindsym Print exec flameshot gui --path ~/Pictures/Screenshots/ --clipboard
// bindsym Control+Print exec flameshot full --path ~/Pictures/Screenshots/ 
// bindsym Shift+Print exec flameshot gui --clipboard
// bindsym Control+Shift+Print exec flameshot gui --upload
static const char *flameshot[] = { "flameshot", "gui", "--path", "/home/aidan/Pictures/Screenshots/", "--clipboard", NULL };
static const char *flameshotc[] = { "flameshot", "gui", "--clipboard",NULL };
static const char *flameshotoneshot[] = { "flameshot", "gui", "--path", "/home/aidan/Pictures/Screenshots/", "--accept-on-select", NULL };
static const char *ocr[] = {"ocrscreenshot", NULL };
static const char *qr[] = {"qrscreenshot", NULL };

static const char *mutecmd[] = { "pactl", "set-sink-mute", "0", "toggle", NULL };
static const char *volupcmd[] = { "pactl", "set-sink-volume", "0", "+10%", NULL };
static const char *voldowncmd[] = { "pactl", "set-sink-volume", "0", "-10%", NULL };

static const char *audiotoggle[] = { "playerctl", "play-pause", NULL };
static const char *audionext[] = { "playerctl", "next", NULL };
static const char *audioprev[] = { "playerctl", "previous", NULL };
// migrate from i3
static const char *brightup[] = { "brightnessctl", "set", "+10%", NULL };
static const char *brightdown[] = {"brightnessctl", "set", "10%-", NULL };

static const char *touchpad[] = {"touchpad", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
  { 0,          XF86XK_AudioMute, spawn, {.v = mutecmd } },
  { 0,          XF86XK_AudioLowerVolume, spawn, {.v = voldowncmd } },
  { 0,          XF86XK_AudioRaiseVolume, spawn, {.v = volupcmd } },
  { 0,          XF86XK_MonBrightnessUp, spawn, {.v = brightup} },
  { 0,          XF86XK_MonBrightnessDown, spawn, {.v = brightdown} },


  { 0,          XF86XK_AudioPlay, spawn, {.v = audiotoggle } },
  { 0,          XF86XK_AudioPrev, spawn, {.v = audioprev} },
  { 0,          XF86XK_AudioNext, spawn, {.v = audionext} },
// ❯ XF86AudioPlay XF86AudioPrev, XF86AudioNext
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_r,      quit,           {.i = 23} },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = deftmux } },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = pavucontrol } },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          {.v = blueman_manager } },
	{ MODKEY|ShiftMask,             XK_w,      spawn,          {.v = network_editor } },
	{ MODKEY,                       XK_Return, spawn,          {.v = tmux } },
	{ MODKEY,                       XK_slash,  spawn,          SHCMD("alacritty") },
	{ MODKEY|ShiftMask,              XK_slash,  spawn,          SHCMD("kitty") },

	// { 0,                            XK_F6,     spawn,          {.v = touchpad } },
  { MODKEY|ShiftMask,                            XK_t,     spawn,          SHCMD("~/.xtoggle_touchpad.sh") },
	{ 0,                            XK_Print,  spawn,          {.v = flameshot } },
  { 0|ShiftMask,                  XK_Print,  spawn,          {.v = flameshotc } },
  { 0|ControlMask,                XK_Print,  spawn,          {.v = ocr } },
  { 0|ControlMask|ShiftMask,      XK_Print,  spawn,          {.v = qr } },
  { MODKEY|ControlMask|ShiftMask, XK_f,      spawn,          {.v = flameshotoneshot } },

	{ MODKEY,                       XK_m,      spawn,          {.v = tmux2 } },
	{ MODKEY,                       XK_n,      spawn,          SHCMD("firefox") },
	{ MODKEY|ShiftMask,             XK_n,      spawn,          SHCMD("spotify") },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          SHCMD("thunar") },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask|ControlMask,             XK_i,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ControlMask,           XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ControlMask,           XK_f,      setlayout,      {.v = &layouts[1]} },
  { MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[2]} },



  { MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
  { MODKEY|ShiftMask,             XK_j,      focusmaster,    {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_d,      focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_d,      tagmon,         {.i = -1 } },
	{ MODKEY,                       XK_q,      focusmon,       {.i = -1 } },
	{ MODKEY|ControlMask,             XK_q,      tagmon,         {.i = -1 } },
  // { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },



	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	// { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	// { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	// { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },

	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

