#ifndef __COLORS_HPP__
#define __COLORS_HPP__
/*
colors.hpp

�����

*/

/* Revision: 1.06 21.05.2001 $ */

/*
Modify:
  21.05.2001 SVS
    + COL_MENUDISABLEDTEXT
  13.12.2000 SVS
    + COL_LASTPALETTECOLOR
  04.12.2000 SVS
    + COL_*DIALOG*DISABLED - ����������� ������� ������� �
      ��������� DIF_DISABLE
  13.09.2000 tran 1.03
    + COL_COMMANDLINEPREFIX
  18.07.2000 tran 1.02
    + COL_VIEWERSCROLLBAR
  11.07.2000 SVS
    + 1 ��������� ������� ����� ������ ������� :-)
  29.06.2000 SVS
    + �������� �������� ��� Menu ScrollBar
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

// ����������� ����� FAR Manager
enum {
    F_BLACK=0,
    F_BLUE=FOREGROUND_BLUE,
    F_GREEN=FOREGROUND_GREEN,
    F_CYAN=FOREGROUND_BLUE|FOREGROUND_GREEN,
    F_RED=FOREGROUND_RED,
    F_MAGENTA=FOREGROUND_BLUE|FOREGROUND_RED,
    F_BROWN=FOREGROUND_GREEN|FOREGROUND_RED,
    F_LIGHTGRAY=FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED,
    F_DARKGRAY=FOREGROUND_INTENSITY,
    F_LIGHTBLUE=FOREGROUND_BLUE|FOREGROUND_INTENSITY,
    F_LIGHTGREEN=FOREGROUND_GREEN|FOREGROUND_INTENSITY,
    F_LIGHTCYAN=FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY,
    F_LIGHTRED=FOREGROUND_RED|FOREGROUND_INTENSITY,
    F_LIGHTMAGENTA=FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY,
    F_YELLOW=FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY,
    F_WHITE=FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY,
    B_BLACK=0,
    B_BLUE=BACKGROUND_BLUE,
    B_GREEN=BACKGROUND_GREEN,
    B_CYAN=BACKGROUND_BLUE|BACKGROUND_GREEN,
    B_RED=BACKGROUND_RED,
    B_MAGENTA=BACKGROUND_BLUE|BACKGROUND_RED,
    B_BROWN=BACKGROUND_GREEN|BACKGROUND_RED,
    B_LIGHTGRAY=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED,
    B_DARKGRAY=BACKGROUND_INTENSITY,
    B_LIGHTBLUE=BACKGROUND_BLUE|BACKGROUND_INTENSITY,
    B_LIGHTGREEN=BACKGROUND_GREEN|BACKGROUND_INTENSITY,
    B_LIGHTCYAN=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_INTENSITY,
    B_LIGHTRED=BACKGROUND_RED|BACKGROUND_INTENSITY,
    B_LIGHTMAGENTA=BACKGROUND_BLUE|BACKGROUND_RED|BACKGROUND_INTENSITY,
    B_YELLOW=BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY,
    B_WHITE=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY,
    F_MASK=F_WHITE,
    B_MASK=B_WHITE
};


enum PaletteColors
{
  COL_FIRSTPALETTECOLOR=1000,

  COL_MENUTEXT=COL_FIRSTPALETTECOLOR,
  COL_MENUSELECTEDTEXT,
  COL_MENUHIGHLIGHT,
  COL_MENUSELECTEDHIGHLIGHT,
  COL_MENUBOX,
  COL_MENUTITLE,

  COL_HMENUTEXT,
  COL_HMENUSELECTEDTEXT,
  COL_HMENUHIGHLIGHT,
  COL_HMENUSELECTEDHIGHLIGHT,

  COL_PANELTEXT,
  COL_PANELSELECTEDTEXT,
  COL_PANELHIGHLIGHTTEXT,
  COL_PANELINFOTEXT,
  COL_PANELCURSOR,
  COL_PANELSELECTEDCURSOR,
  COL_PANELTITLE,
  COL_PANELSELECTEDTITLE,
  COL_PANELCOLUMNTITLE,
  COL_PANELTOTALINFO,
  COL_PANELSELECTEDINFO,

  COL_DIALOGTEXT,
  COL_DIALOGHIGHLIGHTTEXT,
  COL_DIALOGBOX,
  COL_DIALOGBOXTITLE,
  COL_DIALOGHIGHLIGHTBOXTITLE,
  COL_DIALOGEDIT,
  COL_DIALOGBUTTON,
  COL_DIALOGSELECTEDBUTTON,
  COL_DIALOGHIGHLIGHTBUTTON,
  COL_DIALOGHIGHLIGHTSELECTEDBUTTON,

  COL_DIALOGMENUTEXT,
  COL_DIALOGMENUSELECTEDTEXT,
  COL_DIALOGMENUHIGHLIGHT,
  COL_DIALOGMENUSELECTEDHIGHLIGHT,

  COL_WARNDIALOGTEXT,
  COL_WARNDIALOGHIGHLIGHTTEXT,
  COL_WARNDIALOGBOX,
  COL_WARNDIALOGBOXTITLE,
  COL_WARNDIALOGHIGHLIGHTBOXTITLE,
  COL_WARNDIALOGEDIT,
  COL_WARNDIALOGBUTTON,
  COL_WARNDIALOGSELECTEDBUTTON,
  COL_WARNDIALOGHIGHLIGHTBUTTON,
  COL_WARNDIALOGHIGHLIGHTSELECTEDBUTTON,

  COL_KEYBARNUM,
  COL_KEYBARTEXT,
  COL_KEYBARBACKGROUND,

  COL_COMMANDLINE,

  COL_CLOCK,

  COL_VIEWERTEXT,
  COL_VIEWERSELECTEDTEXT,
  COL_VIEWERSTATUS,

  COL_EDITORTEXT,
  COL_EDITORSELECTEDTEXT,
  COL_EDITORSTATUS,

  COL_HELPTEXT,
  COL_HELPHIGHLIGHTTEXT,
  COL_HELPTOPIC,
  COL_HELPSELECTEDTOPIC,
  COL_HELPBOX,
  COL_HELPBOXTITLE,

  COL_PANELDRAGTEXT,
  COL_DIALOGEDITUNCHANGED,
  COL_PANELSCROLLBAR,
  COL_HELPSCROLLBAR,
  COL_PANELBOX,
  COL_PANELSCREENSNUMBER,
  COL_DIALOGEDITSELECTED,
  COL_COMMANDLINESELECTED,
  COL_VIEWERARROWS,

  /* $ 11.07.2000 SVS
    + 1 ��������� ������� ����� ������ ������� :-)
  */
  COL_PRIVATEPOSITION_FOR_XRENZNAETCHEGO,
  /* SVS $ */
  /* $ 29.06.2000 SVS
    �������� �������� ��� Menu ScrollBar
  */
  COL_DIALOGMENUSCROLLBAR,  // ��� ������ History
  COL_MENUSCROLLBAR,            // ��� �������� ����
  /* SVS $ */
  /* $ 18.07.2000 tran
     scrollbar ��� viewer*/
  COL_VIEWERSCROLLBAR,
  /* tran 18.07.2000 $ */
  /* $ 13.09.2000 tran
     comman list prefix */
  COL_COMMANDLINEPREFIX,
  /* tran 13.09.2000 $ */
  /* $ 04.12.2000 SVS
     COL_*DIALOG*DISABLED - ����������� ������� ������� � ��������� DIF_DISABLE */
  COL_DIALOGDISABLED,
  COL_DIALOGEDITDISABLED,
  COL_DIALOGLISTDISABLED,
  COL_WARNDIALOGDISABLED,
  COL_WARNDIALOGEDITDISABLED,
  COL_WARNDIALOGLISTDISABLED,
  /* SVS 04.12.2000 $ */


  COL_MENUDISABLEDTEXT,

  /* $ 13.12.2000 SVS
     ���� ����� ������ ���� ����� ���������!!!!!!
  */
  COL_LASTPALETTECOLOR
  /* SVS $ */
};

#endif  // __COLORS_HPP__
