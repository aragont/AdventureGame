/* = common blocks for adventure === include file === */

/* - parameters --- current limits: */

                        // === ������� �������� ===
#define vocw 550        // ������ �������
#define loct 254        /* locations */
#define objt 127        /* objects */
#define fixt 200        /* locations for fixed objects */
#define sttt (objt*2)   /* different states of objects */
#define actw 60         /* "action" verbs */
#define ranm 400        /* random messages */
#define plcl 12         /* different player classifications. */

                        // === ���� ���� ===
#define movewr 0        // ����������� ��������
#define objcwr 1        // ������
#define actnwr 2        // �������� � ��������
#define specwr 3        // ����-�����


/* there are also limits which cannot be exceeded due to the structure */
/* of the database. these upper limits are: */
/*      1000 non-synonymous vocabulary words */
/*       255 locations */
/*       127 objects */
/*       511 random messages */
/*       255 states of objects */


/* --- messages --- */
int  abb;						// ����: ����� �� ������ ��������
unsigned  _rtext[ranm];			// ������ ������ ���������
#define    rtext(x) _rtext[(x)-1]
unsigned  _ltext[loct];			// - " -  ������� �������� �������
#define    ltext(x) _ltext[(x)-1]
unsigned  _stext[loct];         // - " -  �������� �������� �������
#define    stext(x) _stext[(x)-1]
unsigned  _ctext[plcl];			// - " -  ������������ ������
#define    ctext(x) _ctext[(x)-1]
unsigned  _cval[plcl];			// ������ ������������ ������
#define    cval(x) _cval[(x)-1]
unsigned  _ptext[objt];			// ������� �����. � ������.��������
#define    ptext(x) _ptext[(x)-1]
unsigned  _pstat[sttt];         // ������� �����. � ������.��������
#define    pstat(x) _pstat[(x)-1]
char _abrev[loct];              // �����: ���� ����������� ��������
#define    abrev(x) _abrev[(x)-1]


/* --- travels + actions --- */
unsigned  _trvkey[loct];        // ������� ������ ����� ������ ���.
#define    trvkey(x) _trvkey[(x)-1]
unsigned  _actkey[actw];        //  -"-    -"-  ������� �� ��������
#define    actkey(x) _actkey[(x)-1]


/* --- vocabulary --- */
int  nvoc;                      // ����� ���� � �������
int  _ktab[vocw];               // ����� - �������� ����
#define    ktab(x) _ktab[(x)-1]
long _atab[vocw];               // ����� - �� 4 �����
#define    atab(x) _atab[(x)-1]

/* --- state --- */
#define inipro  255				// ������. ��������� ��� ��������
#define nulobj  255				// ������������� ������
#define caried  1000			// ������ - ������ �����

int  loc;                       // ������� �������
int  tally;						// ����� ��� �� �������. ��������
int  treasr;					// ����� 1-��� �������-���������
int  rndini;					// ����.����� ������ ������
unsigned  tevent;				// ��������� ������� ����.�������
unsigned  eevent;				// ����� ������� ��������� �������
unsigned  tiniti;				// -"-  -"- �������������
int  _place[loct];				// ������� ������� ��� -[������]
#define    place(x) _place[(x)-1]
char _fixed[fixt];				//    ��� ������������� ��������
#define    fixed(x) _fixed[(x)-1]
char _prop[objt];				// �������� �������
#define    prop(x) _prop[(x)-1]
