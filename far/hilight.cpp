/*
hilight.cpp

Files highlighting

*/

/* Revision: 1.03 20.10.2000 $ */

/*
Modify:
  20.10.2000 SVS
    ! �������� ��ਡ�� Enctripted � ������� ������ �������᪫�祭��
      ��� Include & Exclude ��ਡ�⮢.
  13.07.2000 SVS
    ! ������� ���४樨 �� �ᯮ�짮����� new/delete/realloc
  07.07.2000 IS
    + �᫨ ������ ctrl+r � ����, � ����⠭����� ���祭�� �� 㬮�砭��.
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#include "headers.hpp"
#pragma hdrstop

/* $ 30.06.2000 IS
   �⠭����� ���������
*/
#include "internalheaders.hpp"
/* IS $ */


HighlightFiles::HighlightFiles()
{
 /* $ 07.07.2000 IS
   ���� ��� �뭥ᥭ � �⤥���� �㭪��, �⮡� ����� �뫮 ��� �ᯮ�짮����
   ����୮
 */
 InitHighlightFiles();
 /* IS $ */
}

void HighlightFiles::InitHighlightFiles()
{
  HiData=NULL;
  HiDataCount=0;
  while (1)
  {
    char RegKey[80],Mask[sizeof(HiData->Masks)];
    sprintf(RegKey,"Highlight\\Group%d",HiDataCount);
    if (!GetRegKey(RegKey,"Mask",Mask,"",sizeof(Mask)))
      break;
    struct HighlightData *NewHiData,*CurHiData;
    if ((NewHiData=(struct HighlightData *)realloc(HiData,sizeof(*HiData)*(HiDataCount+1)))==NULL)
      break;
    HiData=NewHiData;
    CurHiData=&HiData[HiDataCount];
    memset(CurHiData,0,sizeof(*CurHiData));
    strcpy(CurHiData->Masks,Mask);
    CurHiData->IncludeAttr=GetRegKey(RegKey,"IncludeAttributes",0);
    CurHiData->ExcludeAttr=GetRegKey(RegKey,"ExcludeAttributes",0);
    CurHiData->Color=GetRegKey(RegKey,"NormalColor",0);
    CurHiData->SelColor=GetRegKey(RegKey,"SelectedColor",0);
    CurHiData->CursorColor=GetRegKey(RegKey,"CursorColor",0);
    CurHiData->CursorSelColor=GetRegKey(RegKey,"SelectedCursorColor",0);
    CurHiData->MarkChar=GetRegKey(RegKey,"MarkChar",0);
    HiDataCount++;
  }
  StartHiDataCount=HiDataCount;
}


HighlightFiles::~HighlightFiles()
{
  /* $ 13.07.2000 SVS
     �� �� �� ��뢠� ����� ����� �१ new :-)
  */
  free(HiData);
  /* SVS $ */
}


void HighlightFiles::GetHiColor(char *Path,int Attr,unsigned char &Color,
     unsigned char &SelColor,unsigned char &CursorColor,
     unsigned char &CursorSelColor,unsigned char &MarkChar)
{
  Color=SelColor=CursorColor=CursorSelColor=MarkChar=0;
  for (int I=0;I<HiDataCount;I++)
  {
    struct HighlightData *CurHiData=&HiData[I];
    if ((Attr & CurHiData->IncludeAttr)==CurHiData->IncludeAttr &&
        (Attr & CurHiData->ExcludeAttr)==0)
    {
      char ArgName[NM],*NamePtr=CurHiData->Masks;
      while ((NamePtr=GetCommaWord(NamePtr,ArgName))!=NULL)
        if (Path==NULL && (strcmp(ArgName,"*")==0 || strcmp(ArgName,"*.*")==0) ||
            Path!=NULL && CmpName(ArgName,Path))
        {
          if (Path!=NULL && Path[0]=='.' && Path[1]=='.' && Path[2]==0 &&
              strcmp(ArgName,"..")!=0)
            continue;
          Color=CurHiData->Color;
          SelColor=CurHiData->SelColor;
          CursorColor=CurHiData->CursorColor;
          CursorSelColor=CurHiData->CursorSelColor;
          MarkChar=CurHiData->MarkChar;
          return;
        }
    }
  }
}


void HighlightFiles::HiEdit(int MenuPos)
{
  struct MenuItem HiMenuItem;
  HiMenuItem.Checked=HiMenuItem.Separator=*HiMenuItem.UserData=HiMenuItem.UserDataSize=0;

  {
    VMenu HiMenu(MSG(MHighlightTitle),NULL,0,ScrY-4);
    HiMenu.SetHelp("Highlight");
    HiMenu.SetFlags(MENU_WRAPMODE|MENU_SHOWAMPERSAND);
    HiMenu.SetPosition(-1,-1,0,0);
    HiMenu.SetBottomTitle(MSG(MHighlightBottom));
    for (int I=0;I<HiDataCount;I++)
    {
      struct HighlightData *CurHiData=&HiData[I];
      sprintf(HiMenuItem.Name,"%c%c%c%c%c%c � %c%c%c%c%c%c � %.60s",
        (CurHiData->IncludeAttr & FILE_ATTRIBUTE_READONLY) ? 'R':'.',
        (CurHiData->IncludeAttr & FILE_ATTRIBUTE_HIDDEN) ? 'H':'.',
        (CurHiData->IncludeAttr & FILE_ATTRIBUTE_SYSTEM) ? 'S':'.',
        (CurHiData->IncludeAttr & FILE_ATTRIBUTE_ARCHIVE) ? 'A':'.',
        (CurHiData->IncludeAttr & FILE_ATTRIBUTE_COMPRESSED) ? 'C':
          ((CurHiData->IncludeAttr & FILE_ATTRIBUTE_ENCRYPTED)?'E':'.'),
        (CurHiData->IncludeAttr & FILE_ATTRIBUTE_DIRECTORY) ? 'F':'.',
        (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_READONLY) ? 'R':'.',
        (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_HIDDEN) ? 'H':'.',
        (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_SYSTEM) ? 'S':'.',
        (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_ARCHIVE) ? 'A':'.',
        (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_COMPRESSED) ? 'C':
          ((CurHiData->ExcludeAttr & FILE_ATTRIBUTE_ENCRYPTED)?'E':'.'),
        (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_DIRECTORY) ? 'F':'.',
        CurHiData->Masks);
      HiMenuItem.Selected=(I==MenuPos);
      HiMenu.AddItem(&HiMenuItem);
    }
    *HiMenuItem.Name=0;
    HiMenuItem.Selected=(HiDataCount==MenuPos);
    HiMenu.AddItem(&HiMenuItem);

    {
      HiMenu.Show();
      while (1)
      {
        Panel *LeftPanel=CtrlObject->LeftPanel;
        Panel *RightPanel=CtrlObject->RightPanel;
        while (!HiMenu.Done())
        {
          int SelectPos=HiMenu.GetSelectPos();
          int ItemCount=HiMenu.GetItemCount();
          switch(HiMenu.ReadInput())
          {
            /* $ 07.07.2000 IS
              �᫨ ������ ctrl+r, � ����⠭����� ���祭�� �� 㬮�砭��.
            */
            case KEY_CTRLR:
              if (Message(MSG_WARNING,2,MSG(MHighlightTitle),
                            MSG(MHighlightWarning),MSG(MHighlightAskRestore),
                            MSG(MYes),MSG(MCancel))!=0)
                 break;
              DeleteKeyTree("Highlight");
              SetHighlighting();
              HiMenu.Hide();
              delete(HiData);
              InitHighlightFiles();
              LeftPanel->Update(UPDATE_KEEP_SELECTION);
              LeftPanel->Redraw();
              RightPanel->Update(UPDATE_KEEP_SELECTION);
              RightPanel->Redraw();
              HiEdit(0);
              return;
            /* IS $ */
            case KEY_DEL:
              if (SelectPos<HiMenu.GetItemCount()-1)
              {
                if (Message(MSG_WARNING,2,MSG(MHighlightTitle),
                            MSG(MHighlightAskDel),HiData[SelectPos].Masks,
                            MSG(MDelete),MSG(MCancel))!=0)
                  break;
                for (int I=SelectPos+1;I<ItemCount;I++)
                  HiData[I-1]=HiData[I];
                HiDataCount--;
                HiData=(struct HighlightData *)realloc(HiData,sizeof(*HiData)*(HiDataCount+1));
                HiMenu.Hide();
                SaveHiData();
                LeftPanel->Update(UPDATE_KEEP_SELECTION);
                LeftPanel->Redraw();
                RightPanel->Update(UPDATE_KEEP_SELECTION);
                RightPanel->Redraw();
                HiEdit(SelectPos);
                return;
              }
              break;
            case KEY_INS:
              if (EditRecord(SelectPos,TRUE))
              {
                HiMenu.Hide();
                SaveHiData();
                LeftPanel->Update(UPDATE_KEEP_SELECTION);
                LeftPanel->Redraw();
                RightPanel->Update(UPDATE_KEEP_SELECTION);
                RightPanel->Redraw();
                HiEdit(SelectPos);
                return;
              }
              break;
            case KEY_ENTER:
            case KEY_F4:
              if (SelectPos<HiMenu.GetItemCount()-1)
                if (EditRecord(SelectPos,FALSE))
                {
                  HiMenu.Hide();
                  SaveHiData();
                  LeftPanel->Update(UPDATE_KEEP_SELECTION);
                  LeftPanel->Redraw();
                  RightPanel->Update(UPDATE_KEEP_SELECTION);
                  RightPanel->Redraw();
                  HiEdit(SelectPos);
                  return;
                }
              break;
            default:
              HiMenu.ProcessInput();
              break;
          }
        }
        if (HiMenu.GetExitCode()!=-1)
        {
          HiMenu.ClearDone();
          HiMenu.WriteInput(KEY_F4);
          continue;
        }
        break;
      }
    }
  }
}


void HighlightFiles::SaveHiData()
{
  int I;
  for (I=0;I<HiDataCount;I++)
  {
    struct HighlightData *CurHiData=&HiData[I];
    char RegKey[80];
    sprintf(RegKey,"Highlight\\Group%d",I);
    SetRegKey(RegKey,"Mask",CurHiData->Masks);
    SetRegKey(RegKey,"IncludeAttributes",CurHiData->IncludeAttr);
    SetRegKey(RegKey,"ExcludeAttributes",CurHiData->ExcludeAttr);
    SetRegKey(RegKey,"NormalColor",CurHiData->Color);
    SetRegKey(RegKey,"SelectedColor",CurHiData->SelColor);
    SetRegKey(RegKey,"CursorColor",CurHiData->CursorColor);
    SetRegKey(RegKey,"SelectedCursorColor",CurHiData->CursorSelColor);
    SetRegKey(RegKey,"MarkChar",CurHiData->MarkChar);
  }
  for (I=HiDataCount;I<StartHiDataCount;I++)
  {
    char RegKey[80];
    sprintf(RegKey,"Highlight\\Group%d",I);
    DeleteRegKey(RegKey);
  }
}


static void IncludeExcludeAttrib(int FocusPos,struct DialogItem *Item, int FocusPosSet, int FocusPosSkip)
{
  if(FocusPos == FocusPosSet && Item[FocusPosSet].Selected && Item[FocusPosSkip].Selected)
    Item[FocusPosSkip].Selected=0;
  if(FocusPos == FocusPosSkip && Item[FocusPosSkip].Selected && Item[FocusPosSet].Selected)
    Item[FocusPosSet].Selected=0;
}

int HighlightFiles::EditRecord(int RecPos,int New)
{
  const char *HistoryName="Masks";
  static struct DialogData HiEditDlgData[]={
  /* 00 */DI_DOUBLEBOX,3,1,72,20,0,0,0,0,(char *)MHighlightEditTitle,
  /* 01 */DI_TEXT,5,2,0,0,0,0,0,0,(char *)MHighlightMasks,
  /* 02 */DI_EDIT,5,3,70,3,1,(DWORD)HistoryName,DIF_HISTORY,0,"",
  /* 03 */DI_TEXT,3,4,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 04 */DI_TEXT,5,5,0,0,0,0,DIF_BOXCOLOR,0,(char *)MHighlightIncludeAttr,
  /* 05 */DI_CHECKBOX,5,6,0,0,0,0,0,0,(char *)MHighlightRO,
  /* 06 */DI_CHECKBOX,5,7,0,0,0,0,0,0,(char *)MHighlightHidden,
  /* 07 */DI_CHECKBOX,5,8,0,0,0,0,0,0,(char *)MHighlightSystem,
  /* 08 */DI_CHECKBOX,5,9,0,0,0,0,0,0,(char *)MHighlightArchive,
  /* 09 */DI_CHECKBOX,5,10,0,0,0,0,0,0,(char *)MHighlightCompressed,
  /* 10 */DI_CHECKBOX,5,11,0,0,0,0,0,0,(char *)MHighlightEncrypted,
  /* 11 */DI_CHECKBOX,5,12,0,0,0,0,0,0,(char *)MHighlightFolder,
  /* 12 */DI_TEXT,37,5,0,0,0,0,DIF_BOXCOLOR,0,(char *)MHighlightExcludeAttr,
  /* 13 */DI_CHECKBOX,37,6,0,0,0,0,0,0,(char *)MHighlightRO,
  /* 14 */DI_CHECKBOX,37,7,0,0,0,0,0,0,(char *)MHighlightHidden,
  /* 15 */DI_CHECKBOX,37,8,0,0,0,0,0,0,(char *)MHighlightSystem,
  /* 16 */DI_CHECKBOX,37,9,0,0,0,0,0,0,(char *)MHighlightArchive,
  /* 17 */DI_CHECKBOX,37,10,0,0,0,0,0,0,(char *)MHighlightCompressed,
  /* 18 */DI_CHECKBOX,37,11,0,0,0,0,0,0,(char *)MHighlightEncrypted,
  /* 19 */DI_CHECKBOX,37,12,0,0,0,0,0,0,(char *)MHighlightFolder,
  /* 20 */DI_TEXT,-1,13,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,(char *)MHighlightColors,
  /* 21 */DI_BUTTON,5,14,0,0,0,0,0,0,(char *)MHighlightNormal,
  /* 22 */DI_BUTTON,5,15,0,0,0,0,0,0,(char *)MHighlightSelected,
  /* 23 */DI_BUTTON,37,14,0,0,0,0,0,0,(char *)MHighlightCursor,
  /* 24 */DI_BUTTON,37,15,0,0,0,0,0,0,(char *)MHighlightSelectedCursor,
  /* 25 */DI_TEXT,3,16,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 26 */DI_TEXT,7,17,0,0,0,0,0,0,(char *)MHighlightMarkChar,
  /* 27 */DI_FIXEDIT,5,17,5,18,0,0,0,0,"",
  /* 28 */DI_TEXT,3,18,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 29 */DI_BUTTON,0,19,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
  /* 30 */DI_BUTTON,0,19,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
  };
  MakeDialogItems(HiEditDlgData,HiEditDlg);
  struct HighlightData EditData;
  int ExitCode=0;

  if (!New && RecPos<HiDataCount)
    EditData=HiData[RecPos];
  else
    memset(&EditData,0,sizeof(EditData));

  strcpy(HiEditDlg[2].Data,EditData.Masks);
  HiEditDlg[5].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_READONLY)!=0;
  HiEditDlg[6].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_HIDDEN)!=0;
  HiEditDlg[7].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_SYSTEM)!=0;
  HiEditDlg[8].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_ARCHIVE)!=0;
  HiEditDlg[9].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_COMPRESSED)!=0;
  HiEditDlg[10].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_ENCRYPTED)!=0;
  HiEditDlg[11].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_DIRECTORY)!=0;
  HiEditDlg[13].Selected=(EditData.ExcludeAttr & FILE_ATTRIBUTE_READONLY)!=0;
  HiEditDlg[14].Selected=(EditData.ExcludeAttr & FILE_ATTRIBUTE_HIDDEN)!=0;
  HiEditDlg[15].Selected=(EditData.ExcludeAttr & FILE_ATTRIBUTE_SYSTEM)!=0;
  HiEditDlg[16].Selected=(EditData.ExcludeAttr & FILE_ATTRIBUTE_ARCHIVE)!=0;
  HiEditDlg[17].Selected=(EditData.ExcludeAttr & FILE_ATTRIBUTE_COMPRESSED)!=0;
  HiEditDlg[18].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_ENCRYPTED)!=0;
  HiEditDlg[19].Selected=(EditData.ExcludeAttr & FILE_ATTRIBUTE_DIRECTORY)!=0;

  *HiEditDlg[27].Data=EditData.MarkChar;

  int FocusPos;
  while (ExitCode!=29)
  {
    SaveScreen SaveScr;
    Dialog Dlg(HiEditDlg,sizeof(HiEditDlg)/sizeof(HiEditDlg[0]));
    Dlg.SetHelp("Highlight");
    Dlg.SetPosition(-1,-1,76,22);
    Dlg.Show();
    while (!Dlg.Done())
    {
       Dlg.ReadInput();
       Dlg.ProcessInput();
       FocusPos=Dialog::SendDlgMessage((HANDLE)&Dlg,DM_GETFOCUS,0,0);
       // ��ࠡ�⠥� �������᪫�祭��
       if(FocusPos >= 5 && FocusPos <= 19)
       {
         IncludeExcludeAttrib(FocusPos,HiEditDlg,5,13); // Read only
         IncludeExcludeAttrib(FocusPos,HiEditDlg,6,14); // Archive
         IncludeExcludeAttrib(FocusPos,HiEditDlg,7,15); // Hidden
         IncludeExcludeAttrib(FocusPos,HiEditDlg,8,16); // System
         IncludeExcludeAttrib(FocusPos,HiEditDlg,9,17); // Compressed
         IncludeExcludeAttrib(FocusPos,HiEditDlg,10,18); // Encrypted
         IncludeExcludeAttrib(FocusPos,HiEditDlg,11,19); // Folder
         Dlg.FastShow();
       }
    }
//    Dlg.GetDialogObjectsData(); //??
    if ((ExitCode=Dlg.GetExitCode())<0)
      return(FALSE);
    switch(ExitCode)
    {
      case 30:
        return(FALSE);
      case 21:
        GetColorDialog(EditData.Color);
        break;
      case 22:
        GetColorDialog(EditData.SelColor);
        break;
      case 23:
        GetColorDialog(EditData.CursorColor);
        break;
      case 24:
        GetColorDialog(EditData.CursorSelColor);
        break;
    }
    Dlg.InitDialogObjects();
  }
  if (*HiEditDlg[2].Data==0)
    return(FALSE);
  strcpy(EditData.Masks,HiEditDlg[2].Data);
  EditData.IncludeAttr=EditData.ExcludeAttr=0;
  if (HiEditDlg[5].Selected)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_READONLY;
  if (HiEditDlg[6].Selected)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_HIDDEN;
  if (HiEditDlg[7].Selected)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_SYSTEM;
  if (HiEditDlg[8].Selected)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_ARCHIVE;
  if (HiEditDlg[9].Selected)
  {
    EditData.IncludeAttr|=FILE_ATTRIBUTE_COMPRESSED;
    EditData.IncludeAttr&=~FILE_ATTRIBUTE_ENCRYPTED;
  }
  else if (HiEditDlg[10].Selected)
  {
    EditData.IncludeAttr&=~FILE_ATTRIBUTE_COMPRESSED;
    EditData.IncludeAttr|=FILE_ATTRIBUTE_ENCRYPTED;
  }
  if (HiEditDlg[11].Selected)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_DIRECTORY;
  if (HiEditDlg[13].Selected)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_READONLY;
  if (HiEditDlg[14].Selected)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_HIDDEN;
  if (HiEditDlg[15].Selected)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_SYSTEM;
  if (HiEditDlg[16].Selected)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_ARCHIVE;
  if (HiEditDlg[17].Selected)
  {
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_COMPRESSED;
    EditData.ExcludeAttr&=~FILE_ATTRIBUTE_ENCRYPTED;
  }
  else if (HiEditDlg[18].Selected)
  {
    EditData.ExcludeAttr&=~FILE_ATTRIBUTE_COMPRESSED;
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_ENCRYPTED;
  }
  if (HiEditDlg[19].Selected)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_DIRECTORY;
  EditData.MarkChar=*HiEditDlg[27].Data;
  if (!New && RecPos<HiDataCount)
    HiData[RecPos]=EditData;
  if (New)
  {
    struct HighlightData *NewHiData;
    HiDataCount++;
    if ((NewHiData=(struct HighlightData *)realloc(HiData,sizeof(*HiData)*HiDataCount))==NULL)
      return(FALSE);
    HiData=NewHiData;
    for (int I=HiDataCount-1;I>RecPos;I--)
      HiData[I]=HiData[I-1];
    HiData[RecPos]=EditData;
  }
  return(TRUE);
}
