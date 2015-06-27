//cd c:\Users\IceCold\Documents\Visual Studio 2013\WebSites\WebSite2\advent
//cl /clr Advent.cpp /link /dll /out:..\bin\Advent.dll
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <io.h>
#include <direct.h>
#include "..\common\advcommo.h"
#include "..\common\advexter.h"
#include <atlstr.h>
#using <system.dll>
#using <mscorlib.dll>
#using <system.web.dll>

using namespace System;
using namespace System::Text;
using namespace System::Threading;
using namespace System::Net;
using namespace System::Web;
using namespace System::Web::UI::WebControls;

#define adv_data  "data.adv"
#define adv_data2 "\\advent\\data.adv"
#define wrd2(x) _wrd2[(x)-1]
#define wrd1(x) _wrd1[(x)-1]
#define blksiz    512
#define mask      (~(blksiz-1l))
#define adv_text  "text.adv"
#define adv_text2 "\\advent\\text.dat"
#define empty     '>'

public ref class Advent : public System::Web::UI::Page
{
private:

	int moves;
	int actfla;
	int drkfla;
	int darkflag1 = 0;
	FILE *F1;
	static FILE *cb;
	static char *bevents, *pevents;
	static char *SaveFile = new char[16];
	int clock = 0;

	int vocab(char *word) {
		register int _vocab, i;

		i = 1;
		for (;;) {
			_vocab = ktab(i);
			if (_vocab == 0) break;
			if (!strncmp(reinterpret_cast<char *>(&atab(i)), word, 4))  return _vocab;
			i = i + 1;
		}
		return -1;
	}

	char *conve(unsigned value, int length, int radix, char begins) {
		char *ptr;
		static char buf[16 + 1];
		int i;

		buf[16] = '\000';
		for (i = 0; i<16; ++i)  buf[i] = begins;
		ptr = &buf[16];

		do {
			*--ptr = (i = value%radix) < 10 ? i + '0' : i - 10 + 'A';
			value /= radix;
		} while (value);

		return(&buf[16 - length]);
	}

	void fatal(unsigned n)
	{
		//_write(1, "FATAL ERROR: ", 13);
		str_out += "FATAL ERROR: ";
		// _write(1, conve(n, 3, 10, ' '), 3);
		str_out += gcnew String(conve(n, 3, 10, ' '));
		//_write(1, "\n", 1);
		str_out += "\n";
		ready_out = true;
		Sleep(5000);
		exit(1);
	}

	void loadcm() {
		if ((F1 = fopen("common.adv", "rb")) == NULL) {
			if ((F1 = fopen("\\advent\\common.adv", "rb")) == NULL) fatal(2);
		}
		fread(_rtext, sizeof _rtext, 1, F1);
		fread(_ltext, sizeof _ltext, 1, F1);
		fread(_stext, sizeof _stext, 1, F1);
		fread(_ctext, sizeof _ctext, 1, F1);
		fread(_cval, sizeof _cval, 1, F1);
		fread(_ptext, sizeof _ptext, 1, F1);
		fread(_pstat, sizeof _pstat, 1, F1);
		fread(_trvkey, sizeof _trvkey, 1, F1);
		fread(_actkey, sizeof _actkey, 1, F1);
		fread(&nvoc, sizeof nvoc, 1, F1);
		fread(_ktab, sizeof _ktab, 1, F1);
		fread(_atab, sizeof _atab, 1, F1);
		fread(&tally, sizeof tally, 1, F1);
		fread(&treasr, sizeof treasr, 1, F1);
		fread(_place, sizeof _place, 1, F1);
		fread(_fixed, sizeof _fixed, 1, F1);
		fread(_prop, sizeof _prop, 1, F1);
		fread(&rndini, sizeof rndini, 1, F1);
		fread(&tevent, sizeof tevent, 1, F1);
		fread(&eevent, sizeof eevent, 1, F1);
		fread(&tiniti, sizeof tiniti, 1, F1);
		fclose(F1);
	}

	void iniget(unsigned adr) {
		if (cb == NULL) {
			if ((cb = fopen(adv_data, "rb")) == NULL) {
				if ((cb = fopen(adv_data2, "rb")) == NULL)  fatal(1);
			}
			bevents = (char*)malloc(eevent - tevent + 2);
			fseek(cb, (long)tevent, 0);
			fread(bevents, eevent - tevent, 1, cb);
		}

		if (tevent <= adr && adr<eevent) {
			pevents = bevents + (adr - tevent);
		}
		else {
			pevents = NULL;
			fseek(cb, (long)adr, 0);
		}
	}

	int get()
	{
		char c;
		if (pevents == NULL) {
			fread(&c, 1, 1, cb);
		}
		else {
			c = *pevents++;
		}
		return(c & 0377);
	}

	int at(int object) {
		int _at, p;
		p = place(object);
		_at = 0;
		if (p == loc) {
			_at = 1;
		}
		else if (p<0) {
			p = -p;
			while (fixed(p) != 0) {
				if ((fixed(p) & 0377) == loc) {
					_at = 1;
					return(_at);
				}
				p = p + 1;
			}
		}
		return(_at);
	}

	bool pct(int n) {
		register int r;

		r = (rand() >> 2) & 037777;
		return((r % 100) < n);
	}

	void mes(unsigned iadr) {
		FILE *cb = NULL;
		static char buf[blksiz + 1];
		register char *ptr, *zeroptr, *p;
		static long block = -1, nblock, adr;

		if (cb == NULL) {
			if ((cb = fopen(adv_text, "r")) == NULL)  {
				if ((cb = fopen(adv_text2, "r")) == NULL)  fatal(1);
			}
		}

		adr = (long)iadr * 2l;
		nblock = adr & mask;
		//if (block != nblock) {
		block = nblock;
		fseek(cb, block, 0);
		fread(buf, 1, blksiz, cb);
		//}

		ptr = &buf[(unsigned)(adr & (blksiz - 1l))];
		if (!*ptr)  ++ptr;

		if (*ptr != empty) {
			for (;;) {
				for (zeroptr = ptr; *zeroptr; ++zeroptr);
				//_write(1, ptr, zeroptr - ptr);
				//CString tmp(ptr);
				//tmp.OemToAnsi();
				str_out += gcnew String(ptr);
				if (zeroptr != &buf[blksiz]) break;
				fread(buf, 1, blksiz, cb);  ++block;
				ptr = buf;
			}
		}
	}

	void rspeak(int n) {
		register unsigned adr;
		if (!(adr = rtext(n)))  fatal(3);
		mes(adr);
	}

	void trim(char *s) {
		int i = 0;
		while ((s[i] == ' ') || (s[i] == '\t')) i++;
		strcpy(s, s + i);
	}

	int yes(int x) {
		char c, c1;
		char s[80];

	beg:
		rspeak(x);

		for (;;) {
			//gets(s);
			ready_out = true;
			while (!ready_in) {
				Sleep(1000);
				clock++;
				if (clock == 60)
					exit(0);
			}
			clock = 0;
			ready_in = false;
			str_out = "";
			CString tmp(str_in);
			//tmp.AnsiToOem();
			strcpy(s, tmp);
			trim(s);

			if (s[0] == 0) goto quit;

			c = s[0];

			if (c == 'y' || c == 'Y' || c == '¤' || c == '„') {
				x = 1;  break;
			}
			else if (c == 'n' || c == 'N' || c == '­' || c == 'Ќ') {
				x = 0;  break;
			}
			else {
				rspeak(40);
			}
		}
		return(x);

	quit:
		if (x != 22 && yes(22)) {
			ready_out = true;
			Sleep(5000);
			exit(0);
		}
		goto beg;
	}

	void mscore(int score, int maxscore) {
		//_write(1, "     ", 5);
		str_out += "     ";
		//_write(1, conve(score, 5, 10, ' '), 5);
		str_out += gcnew String(conve(score, 5, 10, ' '));
		//_write(1, "                   ", 19);
		str_out += "                   ";
		//_write(1, conve(maxscore, 5, 10, ' '), 5);
		str_out += gcnew String(conve(maxscore, 5, 10, ' '));
		//_write(1, "               ", 15);
		str_out += "               ";
		//_write(1, conve(moves, 5, 10, ' '), 5);
		str_out += gcnew String(conve(moves, 5, 10, ' '));
		//_write(1, "\n", 1);
		str_out += "\n";
	}

	void score() {
		int scor, maxsco, obj, i;

		scor = maxsco = 0;
		for (obj = treasr; obj <= objt; ++obj) {
			if (place(obj)>0) {
				maxsco = maxsco + 20;
				if (place(obj) == 3 && prop(obj) == 0) {    /* сокровище в доме */
					scor = scor + 20;
				}
				else if ((prop(obj) & 0377) != inipro) {   /* сокровище видел */
					scor = scor + 5;
				}
			}
		}
		rspeak(32);
		mscore(scor, maxsco);

		obj = 1;
		for (i = 1; i <= plcl; ++i)  if (cval(i) && scor >= cval(i)) obj = i;
		mes(ctext(obj));
	}

	bool here(int object) {
		return(place(object) == caried || at(object));
	}

	int dark() {
		static int lamp, light;

		if (lamp == 0) {                       /* и­нициализация */
			lamp = (vocab("lamp") % 1000);
			light = (vocab("!light") % 1000);
		}
		if (darkflag1)             /*@VG*/
			return(darkflag1 = 0);
		return(!at(light) && (!here(lamp) || prop(lamp) == 0));
	}

	void indobj() {
		int obj, kk, p;
		if (!dark()) {
			for (obj = 1; obj <= objt; ++obj) {
				if (ptext(obj) != 0) {
					kk = prop(obj) & 0377;
					if (kk == inipro)  kk = 0;
					kk = pstat(ptext(obj) + kk + 1);
					p = place(obj);

					if (p == loc) {                 /* подвиж­ый объект */
						if ((prop(obj) & 0377) == inipro) { /* впервые увидел */
							prop(obj) = 0     /*        сокровище */;
							tally = tally - 1;
						}
						mes(kk);

					}
					else if (p < 0) {                      /* ­не -"- -"- */
						p = -p;
						while (fixed(p) != 0) {
							if ((fixed(p) & 0377) == loc)  mes(kk);
							p = p + 1;
						}
					}
				}
			}
		}
	}

	void ds(bool n) {
		register int kk;

		if (n) goto L100;
		if (dark()) {
			rspeak(16);
		}
		else {
			kk = 0;
			if (abrev(loc) != 0) {
				kk = stext(loc);
			}
			else {
			L100:       kk = ltext(loc);
			}
			if (kk == 0)  kk = ltext(loc);
			mes(kk);
			if (abb != 0) {
				abrev(loc) = 2;
			}
			else {
				abrev(loc) = (abrev(loc) + 1) % 4;
			}
			indobj();
		}
	}

	void descr2() { ds(1); }

	void freeze() {
		F1 = fopen(SaveFile, "wb");
		fwrite(&abb, sizeof abb, 1, F1);
		fwrite(_abrev, sizeof _abrev, 1, F1);
		fwrite(&loc, sizeof loc, 1, F1);
		fwrite(&tally, sizeof tally, 1, F1);
		fwrite(_place, sizeof _place, 1, F1);
		fwrite(_prop, sizeof _prop, 1, F1);
		fwrite(&rndini, sizeof rndini, 1, F1);
		fwrite(&tevent, sizeof tevent, 1, F1);
		fwrite(&tiniti, sizeof tiniti, 1, F1);
		fclose(F1);
	}

	void specia(int n) {
		int flag, obj;

		switch (n) {
		case (2 - 1) : goto L1;
		case (3 - 1) : goto L2;
		case (4 - 1) : goto L3;
		case (5 - 1) : goto L4;
		case (6 - 1) : goto L5;
		case (7 - 1) : goto L6;
		case (8 - 1) : goto L7;
		case (9 - 1) : goto L8;
		case (10 - 1) : goto L9;
		}
		fatal(104);

	L1: str_out += "\nИгра окончена. Чтобы начать заново, перезагрузите страницу.";
		ready_out = true;
		Sleep(5000);
		exit(0);                                  /* ко­нец игры */

	L2: score();                             /* выдача счета игры */
		goto L100;

	L3: score();                             /* счет + кон­ец */
		str_out += "\nИгра окончена. Чтобы начать заново, перезагрузите страницу.";
		ready_out = true;
		Sleep(5000);
		exit(0);

	L4: abb = 1                                  /* только сокраще­ия */;
		goto L100;

	L5: flag = 0                                 /* invent */;
		for (obj = 1; obj <= objt; ++obj) {
			if (place(obj) == caried) {
				if (flag == 0)  { rspeak(99);  flag = 1; }
				mes(pstat(ptext(obj)));
			}
		}
		if (flag == 0)  rspeak(98);
		goto L100;

	L6: drkfla = 0                         /* look */;
		if (abrev(loc) == 1)  rspeak(15);
		descr2();
		abrev(loc) = 1;
		goto L100;

	L7: if (dark()) { darkflag1++; descr2();  /*@VG*/ }   /* lamp on */
		goto L100;

	L8: if (dark())  rspeak(16);           /* lamp off */
		goto L100;

	L9: freeze();                            /* freeze game */
		str_out += "\nИгра окончена. Чтобы начать заново, перезагрузите страницу.";
		ready_out = true;
		Sleep(5000);
		exit(0);

	L100:;
	}

	void descr()  { ds(0); }

	void chnloc(int newloc) {
		static int drkold;
		drkold = dark();
		loc = newloc;
		if (dark() && drkfla && drkold && pct(30)) {
			rspeak(23);                   /* свалился в колодец в темноте */
			score();
			str_out += "\nИгра окончена. Чтобы начать заново, перезагрузите страницу.\n";
			ready_out = true;
			Sleep(5000);
			exit(0);
		}
		drkfla = 1;
		descr();
	}

	bool act(int indx, int object) {
#   define not   128
#   define isobj 64
		int ntflag, obflag, condit, obimpl, cmnd, kod, obj, ncarry, i;

	begin:
		iniget(indx);

	L2000:
		for (;;) {                         /* проверка следующего условия */
			cmnd = get();
			if (cmnd == 0) break;
			obimpl = object;

			for (;;) {                              /* проверка условий */
				if (cmnd == 0)  goto L5000;     /*    условия выполн­ен­ы */

				kod = ((cmnd) % (isobj))              /*  загрузил код условия */;
				ntflag = (cmnd / not != 0)        /*  загрузил +/- флаг */;
				obj = obimpl                      /*  загрузил объект */;
				if ((cmnd%not) / isobj != 0)  obj = get();


				switch (kod) {
				case (2 - 1) : goto L1;
				case (3 - 1) : goto L2;
				case (4 - 1) : goto L3;
				case (5 - 1) : goto L4;
				case (6 - 1) : goto L5;
				case (7 - 1) : goto L6;
				case (8 - 1) : goto L7;
				case (9 - 1) : goto L8;
				case (10 - 1) : goto L9;
				case (11 - 1) : goto L10;
				case (12 - 1) : goto L11;
				case (13 - 1) : goto L12;
				}
				goto L13;

			L1:         obimpl = obj                           /* объект=заданному? */;
				condit = obj == object;
				goto L100;

			L2:         condit = at(obj)                     /* we are at object */;
				goto L100;

			L3:         condit = at(obj) || place(obj) == caried   /* object is here */;
				goto L100;

			L4:         condit = place(obj) == caried         /* we are toting object */;
				goto L100;

			L5:         condit = pct(obj)                   /* probability = n% */;
				goto L100;

			L6:         condit = (prop(obj) & 0377) == get() - 1    /* prop(obj)=n */;
				goto L100;

			L7:         condit = place(obj)<0               /* object is fixed */;
				goto L100;

			L8:         condit = 1                     /* 1 */;
				goto L100;

			L9:         condit = yes(obj)                   /* reply is "yes" */;
				goto L100;

			L10:        condit = loc == obj                   /* location = obj */;
				goto L100;

			L11:        ncarry = 0                            /* carry > obj objects */;
				for (i = 1; i <= objt; ++i) {
					if (place(i) == caried)  ncarry = ncarry + 1;
				}
				condit = ncarry>obj;
				goto L100;

			L12:        condit = (rndini >> 2) & 037777;   /* init. probability = obj% */
				condit = (rndini % 100) <obj;
				goto L100;

			L13:        condit = 0                      /* one of next words */;
				for (i = 1; i <= kod - 13; ++i) {
					if (get() == obj)  condit = 1;
				}
				goto L100;

			L100:
				if ((!condit) == (!ntflag)) break;
				cmnd = get();
			}

			do {                        /* условие не выполн­ен­о ==> */
			} while (get() != 0);          /*      скан­ировать условие */
			do {
			} while (get() != 0);             /*      сканировать действие */

		}
		return(0);                           /* подх. условие не ­найден­о */

	L5000:
		for (;;) {
			cmnd = get();
			if (cmnd == 0) break;
			kod = cmnd%isobj                     /*  загрузил код условия */;
			obj = obimpl                              /*  загрузил объект */;
			if ((cmnd%not) / isobj != 0)  obj = get();

			switch (kod) {
			case (15 - 14) : goto L101;
			case (16 - 14) : goto L102;
			case (17 - 14) : goto L103;
			case (18 - 14) : goto L104;
			case (19 - 14) : goto L105;
			case (20 - 14) : goto L106;
			case (21 - 14) : goto L107;
			case (22 - 14) : goto L108;
			case (23 - 14) : goto L109;
			case (24 - 14) : goto L110;
			case (25 - 14) : goto L111;
			case (26 - 14) : goto L112;
			}
			fatal(101);

		L101:   place(obj) = loc                           /* drop object here */;
			goto L200;

		L102:   place(obj) = 0                             /* destroy object */;
			goto L200;

		L103:   place(obj) = caried                        /* carry object */;
			goto L200;

		L104:   rspeak(obj);                         /* message #n (1<=n<256) */
			goto L200;
		L105:   rspeak(obj + 255);                     /* message #n ( 256<=n ) */
			goto L200;

		L106:   prop(obj) = get() - 1                        /* let prop(obj)=n */;
			goto L200;

		L107:   chnloc(obj);                         /* change location to #n */
			goto L200;

		L108:   specia(obj);                         /* special case #n */
			goto L200;

		L109:   place(obj) = get() - 1;                     /* throw to location #n */
			goto L200;

		L110:   prop(obj) = prop(obj) + 1                    /* add 1 to prop(obj) */;
			goto L200;

		L111:   indobj();                              /* indicate objects */
			goto L200;

		L112:   object = obj;                   /* указание объекта по умолчан­ию */
			goto begin;

		L200:;
		}
		if (actfla == 1)  goto L2000;              /* оце­нить след.условие */

		return(1);                                 /* ok. all is ready */
	}

	void events() {
		int actres;
		actfla = 1;
		actres = act(tevent, 0);
		actfla = 0;
	}

	void loadfr() {
		fread(&abb, sizeof abb, 1, F1);
		fread(_abrev, sizeof _abrev, 1, F1);
		fread(&loc, sizeof loc, 1, F1);
		fread(&tally, sizeof tally, 1, F1);
		fread(_place, sizeof _place, 1, F1);
		fread(_prop, sizeof _prop, 1, F1);
		fread(&rndini, sizeof rndini, 1, F1);
		fread(&tevent, sizeof tevent, 1, F1);
		fread(&tiniti, sizeof tiniti, 1, F1);
		fclose(F1);
		_unlink(SaveFile);
	}

	void ini() {
		int actres;

		loadcm();

		if ((F1 = fopen(SaveFile, "rb")) != NULL) {
			loadfr();

		}
		else {
			srand(time(NULL));
			loc = 1;
			rndini = rand();
			actfla = 1;
			actres = act(tiniti, 0);
			actfla = 0;
		}
		descr();

	}

	void tolower(register char *adr, int lgt) {
		register char *oldadr, c;

		oldadr = adr;
		while (lgt--) {
			c = *adr;
			if (c >= 'A' && c <= 'Z')   *adr |= 040;
			/*      else if( (c&0300)==0300 )  *adr &= ~040; */
			++adr;
		}

		return;
	}

	void inpans(char *wrd1, char *wrd2) {
		register int i;
		register char *ptr, *ptr0;
		char buf[80];

		for (i = 10; i--;) wrd1[i] = wrd2[i] = ' ';
		buf[80 - 1] = '\n';
		ready_out = true;
		while (!ready_in) {
			Sleep(1000);
			clock++;
			if (clock == 60)
				exit(0);
		}
		clock = 0;
		ready_in = false;
		str_out = "";
		int len = str_in->Length;
		CString tmp(str_in);
		//tmp.AnsiToOem();
		strcpy(buf, tmp);
		buf[len] = 10;
		if (!len)  strcpy(buf, "Є®­Ґж\n");
		tolower(buf, 80);
		for (ptr = buf; *ptr == ' ' || *ptr == '\t'; ++ptr);
		if (*ptr != '\n') {
			ptr0 = ptr;
			while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t') {
				if ((i = ptr - ptr0) < 10)  wrd1[i] = *ptr;
				++ptr;
			}
			for (; *ptr == ' ' || *ptr == '\t'; ++ptr);
			if (*ptr != '\n') {
				ptr0 = ptr;
				while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t') {
					if ((i = ptr - ptr0) < 10)  wrd2[i] = *ptr;
					++ptr;
				}
			}
		}
	}

	void getans(int *word1, int *word2, int *type1, int *type2) {
		int work, kk;
		char _wrd1[10], _wrd2[10];

		for (;;) {
			inpans(_wrd1, _wrd2);
			*word1 = *word2 = *type1 = *type2 = -1;
			if (wrd1(1) == '!' || wrd2(1) == '!')  goto L900;    /* спец-слова */

			work = vocab(_wrd1);
			if (work >= 1) {
				*word1 = work % 1000;
				*type1 = work / 1000;
			}

			work = vocab(_wrd2);
			if (work >= 1) {
				*word2 = work % 1000;
				*type2 = work / 1000;
			}

			if (*type1 >= 0) break;          /* все слова ­неизвестн­ы */
			if (wrd1(1) == ' ') {           /*  вообще нет ­и одн­ого */
				kk = 61;
			}
			else {
			L900:       kk = 60;
				if (pct(50))  kk = 61;
				if (pct(33))  kk = 13;
			}
			rspeak(kk);
		}
	}

	void action(int verb, int object) {
		int kk;

		kk = actkey(verb);
		if (kk == 0 && verb != 1)  fatal(102);
		if (!act(kk, object))  rspeak(12); /* как приме­ить слово? */
	}

	void motion(int verb) {
		int kk;

		kk = trvkey(loc);
		if (kk == 0 && loc != 1)  fatal(103);
		if (!act(kk, verb)) {
			if (pct(50)) {
				rspeak(9);                /* пути нет */
			}
			else {
				rspeak(12);              /* как приме­ить это слово здесь? */
			}
		}
	}

	void main() {
		int word1, word2, type1, type2;
		int oldob, oldobj;

		_chdir("D:\\home\\site\\wwwroot");

		CString tmp(ip);
		strcpy(SaveFile, tmp);
		strcat(SaveFile, ".adv");
		for (int i = 0; i < 15; i++)
			if (SaveFile[i] == ':')
				SaveFile[i] = '0';

		ini();                                /* initiate data base */
		
		for (;;) {
			events();                          /* случайные события */
			getans(&word1, &word2, &type1, &type2);
			++moves;
			oldob = 0;

			if (type1 == specwr) {      /* спец-слово */
				rspeak(word1);
			}
			else if (type2 == specwr) {
				rspeak(word2);

			}
			else if (type1 == movewr) {      /* передвиже­ия */
				motion(word1);

			}
			else if (type2 == movewr) {
				motion(word2);
			}
			else {

				if (type1 == objcwr) {            /* объекта нет рядом */
					if (!here(word1))  goto L80;
				}
				if (type2 == objcwr) {
					if (!here(word2)) {
					L80:				rspeak(203);
						goto L90;
					}
				}

				if (type1 == actnwr) {           /* действие + */
					if (type2 == objcwr) {         /*     + объект */
						action(word1, word2);
					}
					else if (oldobj != 0) {        /*     + старый объект */
						action(word1, oldobj);
					}
					else {                          /*     + нет объекта */
						action(word1, 255);
					}
				}
				else if (type2 == actnwr) {      /* объект + действие */
					action(word2, word1);

				}
				else if (type1 == objcwr) {      /* объект */
					rspeak(90);                     /*    что делать с ? */
					oldob = word1;
				}
			L90:;
			}
			oldobj = oldob;
		}
	}

protected:

	String ^ip;
	static String ^str_out;
	static String ^str_in;
	static bool ready_in;
	static bool ready_out;
	static Thread ^newThread;
	static bool rld = false;
	Button ^Button;
	TextBox ^Log;
	TextBox ^Command;
	Encoding^ from = System::Text::Encoding::GetEncoding(28591);
	Encoding^ to = System::Text::Encoding::GetEncoding(866);
	array<Byte>^ bytes;

public:

	void Page_LoadComplete(Object ^sender, EventArgs ^e) {
		if (rld) {
			ip = HttpContext::Current->Request->UserHostAddress;
			ThreadStart ^threadDelegate = gcnew ThreadStart(this, &Advent::main);
			newThread = gcnew Thread(threadDelegate);
			newThread->Start();
			str_out = "";
			while (!ready_out);
			ready_out = false;
			bytes = from->GetBytes(str_out);
			str_out = to->GetString(bytes);
			Log->Text = str_out;
			rld = true;
		}
	}

	void Send(Object ^sender, EventArgs ^e)
	{
		str_in = Command->Text;
		bytes = to->GetBytes(str_in);
		str_in = from->GetString(bytes);
		Log->Text += "> " + Command->Text + "\n";
		Command->Text = "";
		ready_in = true;
		while (!ready_out);
		ready_out = false;
		bytes = from->GetBytes(str_out);
		str_out = to->GetString(bytes);
		Log->Text += str_out;
	}
};