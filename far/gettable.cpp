/*
gettable.cpp

������ � ��������� ��������

*/

/* Revision: 1.07 21.05.2001 $ */

/*
Modify:
  21.05.2001 SVS
    ! struct MenuData|MenuItem
      ���� Selected, Checked, Separator � Disabled ������������� � DWORD Flags
    ! ��������� MENU_ - � ����
  06.05.2001 DJ
    ! �������� #include
  11.02.2001 SVS
    ! ��������� ��������� ���� � ����� � ����������� � ��������� MenuItem
  15.09.2000 IS
    + ������� DistrTableExist - ���������, ����������� �� ������� �
      �������������� ������ ��������, ���������� TRUE � ������ ������
  08.09.2000 tran 1.03
    + menu from registry
  27.08.2000 tran
    + hotkeys as numbers
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "global.hpp"
#include "plugin.hpp"
#include "fn.hpp"
#include "lang.hpp"
#include "vmenu.hpp"
#include "savefpos.hpp"

#if defined(__BORLANDC__)
static unsigned long CalcDifference(int *SrcTable,int *CheckedTable,unsigned char *DecodeTable);
#else
static unsigned long CalcDifference(int *SrcTable,int *CheckedTable,char *DecodeTable);
#endif

/* 15.09.2000 IS
   ���������, ����������� �� ������� � �������������� ������ ��������
*/
int DistrTableExist(void)
{
 return (CheckRegValue("CodeTables","Distribution"));
}
/* IS $ */

int GetTable(struct CharTableSet *TableSet,int AnsiText,int &TableNum,
             int &UseUnicode)
{
  int I;
  int mx=0;
  char ItemName[128],t[128],t2[128];

  if (AnsiText)
  {
    for (I=0;I<256;I++)
    {
      TableSet->DecodeTable[I]=TableSet->EncodeTable[I]=I;
      TableSet->LowerTable[I]=TableSet->UpperTable[I]=I;
    }
    for (I=0;I<256;I++)
    {
      char Str[1];
      Str[0]=I;
      CharToOemBuff(Str,Str,1);
      TableSet->DecodeTable[I]=Str[0];
      Str[0]=I;
      OemToCharBuff(Str,Str,1);
      TableSet->EncodeTable[I]=Str[0];
      TableSet->LowerTable[I]=(unsigned char)CharLower((LPTSTR)I);
      TableSet->UpperTable[I]=(unsigned char)CharUpper((LPTSTR)I);
    }
    strcpy(TableSet->TableName,"Win");
    return(TRUE);
  }

  VMenu TableList(MSG(MGetTableTitle),NULL,0,ScrY-4);
  TableList.SetFlags(VMENU_WRAPMODE);
  TableList.SetPosition(-1,-1,0,0);

  struct MenuItem ListItem;
  memset(&ListItem,0,sizeof(ListItem));
  ListItem.SetSelect(!TableNum);
  strcpy(ListItem.Name,MSG(MGetTableNormalText));
  TableList.AddItem(&ListItem);

  if (UseUnicode)
  {
    ListItem.SetSelect(TableNum==1);
    strcpy(ListItem.Name,"Unicode");
    TableList.AddItem(&ListItem);
    mx=1;
  }

  for (I=0;;I++)
  {
    char TableKey[512];
    if (!EnumRegKey("CodeTables",I,TableKey,sizeof(TableKey)))
      break;
    CharToOem(PointToName(TableKey),ItemName);
    sprintf(t,"CodeTables\\%s",ItemName);
    GetRegKey(t,"TableName",t2,ItemName,128);
    strcpy(ListItem.Name,t2);
    ListItem.SetSelect(I+1+UseUnicode == TableNum);
    TableList.AddItem(&ListItem);
  }

  TableList.AssignHighlights(FALSE);
  TableList.Process();
  int Pos=TableList.GetExitCode();

  if (Pos>UseUnicode && !PrepareTable(TableSet,Pos-1-UseUnicode))
    return(FALSE);

  UseUnicode=UseUnicode && Pos==1;


  if (Pos==-1)
    return(-1);
  TableNum=Pos;

  return(Pos!=0);
}


void DecodeString(char *Str,unsigned char *DecodeTable,int Length)
{
  if (Length==-1)
    Length=strlen(Str);
  while (Length--)
  {
    *Str=DecodeTable[(unsigned int)*Str];
    Str++;
  }
}


void EncodeString(char *Str,unsigned char *EncodeTable,int Length)
{
  if (Length==-1)
    Length=strlen(Str);
  while (Length--)
  {
    *Str=EncodeTable[(unsigned int)*Str];
    Str++;
  }
}


int DetectTable(FILE *SrcFile,struct CharTableSet *TableSet,int &TableNum)
{
  unsigned char DistrTable[256],FileDistr[256],FileData[4096];
  int ReadSize;
  if (!GetRegKey("CodeTables","Distribution",(BYTE *)DistrTable,(BYTE *)NULL,sizeof(DistrTable)))
    return(FALSE);
  SaveFilePos SavePos(SrcFile);
  fseek(SrcFile,0,SEEK_SET);

  int ProcessedSize=0;
  memset(FileDistr,0,sizeof(FileDistr));
  for (int Attempt=0;;Attempt++)
  {
    if ((ReadSize=fread(FileData,1,sizeof(FileData),SrcFile))==0)
      break;
    int TextData=TRUE;
    for (int I=4;I<ReadSize;I++)
    {
      int Ch=FileData[I];
      if (DistrTable[Ch]==0xff)
        continue;
      if (Ch!=FileData[I-1])
      {
        ProcessedSize++;
        if (++FileDistr[Ch]==254)
          break;
      }
      else
        if (Ch>127 && Ch==FileData[I-2] && Ch==FileData[I-3])
        {
          fseek(SrcFile,I-ReadSize+256,SEEK_CUR);
          TextData=FALSE;
          break;
        }
    }
    if (ProcessedSize>1024 || TextData && Attempt>4 || !TextData && Attempt>8)
      break;
  }

  if (ProcessedSize<10)
    return(FALSE);

  int MaxDistr=0,MaxFileDistr=0,I;
  for (I=0;I<sizeof(DistrTable)/sizeof(DistrTable[0]);I++)
    if (DistrTable[I]!=0xff && DistrTable[I]>MaxDistr)
      MaxDistr=DistrTable[I];
  for (I=0;I<sizeof(FileDistr)/sizeof(FileDistr[0]);I++)
    if (FileDistr[I]!=0xff && FileDistr[I]>MaxFileDistr)
      MaxFileDistr=FileDistr[I];

  int SrcTable[256],CheckedTable[256];
  for (I=0;I<sizeof(DistrTable)/sizeof(DistrTable[0]);I++)
    if (DistrTable[I]==0xff)
      SrcTable[I]=-1;
    else
      SrcTable[I]=MaxFileDistr*DistrTable[I];
  for (I=0;I<sizeof(FileDistr)/sizeof(FileDistr[0]);I++)
    if (FileDistr[I]==0xff)
      CheckedTable[I]=-1;
    else
      CheckedTable[I]=MaxDistr*FileDistr[I];

  unsigned long BestValue=CalcDifference(SrcTable,CheckedTable,NULL);
  int BestTable=-1;
  for (I=0;;I++)
  {
    char TableKey[512];
    if (!EnumRegKey("CodeTables",I,TableKey,sizeof(TableKey)))
      break;
#if defined(__BORLANDC__)
    unsigned char DecodeTable[256];
#else
    char DecodeTable[256];
#endif
    if (!GetRegKey(TableKey,"Mapping",(BYTE *)DecodeTable,(BYTE *)NULL,sizeof(DecodeTable)))
      return(FALSE);
    unsigned long CurValue=CalcDifference(SrcTable,CheckedTable,DecodeTable);
    if (CurValue<(5*BestValue)/6 || CurValue<BestValue && BestTable!=-1)
    {
      BestValue=CurValue;
      BestTable=I;
    }
  }
  if (BestTable==-1)
  {
    TableNum=0;
    return(FALSE);
  }
  if (!PrepareTable(TableSet,BestTable))
    return(FALSE);
  TableNum=BestTable+1;
  return(TRUE);
}


#if defined(__BORLANDC__)
static unsigned long CalcDifference(int *SrcTable,int *CheckedTable,unsigned char *DecodeTable)
#else
static unsigned long CalcDifference(int *SrcTable,int *CheckedTable,char *DecodeTable)
#endif
{
  unsigned char EncodeTable[256];
  int CheckedTableProcessed[256];
  int I;
  for (I=0;I<256;I++)
  {
    EncodeTable[I]=I;
    CheckedTableProcessed[I]=FALSE;
  }
  if (DecodeTable!=NULL)
    for (I=0;I<256;I++)
      EncodeTable[DecodeTable[I]]=I;
  unsigned long Diff=0;
  for (I=0;I<256;I++)
    if (SrcTable[I]!=-1)
    {
      int N=EncodeTable[I];
      Diff+=abs(SrcTable[I]-CheckedTable[N]);
      CheckedTableProcessed[N]=TRUE;
    }
  for (I=0;I<256;I++)
    if (SrcTable[I]!=-1 && !CheckedTableProcessed[I])
      Diff+=CheckedTable[I];
  return(Diff);
}


int PrepareTable(struct CharTableSet *TableSet,int TableNum)
{
  int I;
  for (I=0;I<256;I++)
  {
    TableSet->DecodeTable[I]=TableSet->EncodeTable[I]=I;
    TableSet->LowerTable[I]=TableSet->UpperTable[I]=I;
  }
  char TableKey[512];
  if (!EnumRegKey("CodeTables",TableNum,TableKey,sizeof(TableKey)))
    return(FALSE);
  if (!GetRegKey(TableKey,"Mapping",(BYTE *)TableSet->DecodeTable,(BYTE *)NULL,sizeof(TableSet->DecodeTable)))
    return(FALSE);
  strncpy(TableSet->TableName,PointToName(TableKey),sizeof(TableSet->TableName)-1);

  int EncodeSet[256];
  memset(EncodeSet,0,sizeof(EncodeSet));
  for (I=0;I<256;I++)
  {
    int Ch=TableSet->DecodeTable[I];
    if (!EncodeSet[Ch] || Ch>=128)
    {
      TableSet->EncodeTable[Ch]=I;
      EncodeSet[Ch]=TRUE;
    }
  }
  for (I=0;I<256;I++)
  {
    int Ch=TableSet->DecodeTable[I];
    TableSet->LowerTable[I]=TableSet->EncodeTable[LocalLower(Ch)];
    TableSet->UpperTable[I]=TableSet->EncodeTable[LocalUpper(Ch)];
  }
  return(TRUE);
}
