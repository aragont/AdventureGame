/* = common blocks for adventure === include file === */
/* �� ��� ��� 䠩� ����� � �����祭�묨 ��������ﬨ. �.�. */

/* - parameters --- current limits: */

                        /* === 5����5� ���㕁�� === */
#define vocw 550        /* 5����5 ㊍��5� */
#define loct 254        /* locations */
#define objt 127        /* objects */
#define fixt 200        /* locations for fixed objects */
#define sttt (objt*2)   /* different states of objects */
#define actw 60         /* "action" verbs */
#define ranm 400        /* random messages */
#define plcl 12         /* different player classifications. */

                        /* === � ㊍� === */
#define movewr 0        /* ���5������� �������� */
#define objcwr 1        /* ��'ekt */
#define actnwr 2        /* ����� c ��'ektom */
#define specwr 3        /* ㎅6-㊍�� */


/* there are also limits which cannot be exceeded due to the structure */
/* of the database. these upper limits are: */
/*      1000 non-synonymous vocabulary words */
/*       255 locations */
/*       127 objects */
/*       511 random messages */
/*       255 states of objects */


/* --- messages --- */
int  abb;                      /* ����: ����� �� ������ ���㞌�� */
unsigned  _rtext[ranm];        /* ��5�� 5����� ㍍������ */
#define    rtext(x) _rtext[(x)-1]
unsigned  _ltext[loct];        /* - " -  ������� ���㞌�� ����6�� */
#define    ltext(x) _ltext[(x)-1]
unsigned  _stext[loct];        /* - " -  ��5�� ���㞌�� ����6��*/
#define    stext(x) _stext[(x)-1]
unsigned  _ctext[plcl];        /* - " -  ���������6�� ��5��� */
#define    ctext(x) _ctext[(x)-1]
unsigned  _cval[plcl];         /* ��5��� ���������6�� ��5��� */
#define    cval(x) _cval[(x)-1]
unsigned  _ptext[objt];        /* ������� ㍍��. o き���.��'ektob*/
#define    ptext(x) _ptext[(x)-1]
unsigned  _pstat[sttt];        /* ������� ㍍��. o き���.��'ektob*/
#define    pstat(x) _pstat[(x)-1]
char _abrev[loct];             /* �����: ���� ㍉5������� ���㞌��*/
#define    abrev(x) _abrev[(x)-1]


/* --- travels + actions --- */
unsigned  _trvkey[loct];       /* ��6� ��2��� �5��� ������ ���. */
#define    trvkey(x) _trvkey[(x)-1]
unsigned  _actkey[actw];       /*  -"-    -"-  5���6�� ha ����� */
#define    actkey(x) _actkey[(x)-1]


/* --- vocabulary --- */
int  nvoc;                     /* 2�㊍ ㊍� b ㊍��5� */
int  _ktab[vocw];              /* 2�㊞ - ���2���� ㊍� */
#define    ktab(x) _ktab[(x)-1]
long _atab[vocw];              /* ㊍�� - �� 4 ����� */
#define    atab(x) _atab[(x)-1]

/* --- state --- */
#define inipro  255           /* ��2���. ����� ��� ㍉5���� */
#define nulobj  255           /* ��������� ��"ekt */
#define caried  1000          /* mapkep - ��'ekt ���� */

int  loc;                      /* ���� ����6�� */
int  tally;                    /* 2�㊍ ��� �� ����5��. ㍉5���� */
int  treasr;                   /* homep 1-��� ��'ekta-㍉5����� */
int  rndini;                   /* ㊠2.2�㊍ ������ ��5 */
unsigned  tevent;              /* ������ ��6� ㊠2.㍀� */
unsigned  eevent;              /* ����6 ��6� ㊠2����� ㍀� */
unsigned  tiniti;              /* -"-  -"- ���6������6�� */
int  _place[loct];             /* ����6�� ��7��� ��� -[������] */
#define    place(x) _place[(x)-1]
char _fixed[fixt];             /*    ��� ����5������� ��'ektob */
#define    fixed(x) _fixed[(x)-1]
char _prop[objt];              /* き�� ��'ekt� */
#define    prop(x) _prop[(x)-1]