/* = common blocks for adventure === include file === */

/* - parameters --- current limits: */

                        /* === 5ƒ‹…5ε ‹γγ• === */
#define vocw 550        /* 5ƒ‹…5 γ5© */
#define loct 254        /* locations */
#define objt 127        /* objects */
#define fixt 200        /* locations for fixed objects */
#define sttt (objt*2)   /* different states of objects */
#define actw 60         /* "action" verbs */
#define ranm 400        /* random messages */
#define plcl 12         /* different player classifications. */

                        /* === ξ•ε γ === */
#define movewr 0        /* 5…•… „•”…•© */
#define objcwr 1        /* €'ekt */
#define actnwr 2        /* „…γξ•… c €'ektom */
#define specwr 3        /* γ…6-γ */


/* there are also limits which cannot be exceeded due to the structure */
/* of the database. these upper limits are: */
/*      1000 non-synonymous vocabulary words */
/*       255 locations */
/*       127 objects */
/*       511 random messages */
/*       255 states of objects */


/* --- messages --- */
extern int  abb;                      /* ΅–:  ” • … •γ•… */
extern unsigned  _rtext[ranm];        /* „5…γ 5ƒεζ γ€ª…• */
#define    rtext(x) _rtext[(x)-1]
extern unsigned  _ltext[loct];        /* - " -  „•εζ •γ• ƒ•6•• */
#define    ltext(x) _ltext[(x)-1]
extern unsigned  _stext[loct];        /* - " -  ‰5ξ‰•ζ •γ• ƒ•6••*/
#define    stext(x) _stext[(x)-1]
extern unsigned  _ctext[plcl];        /* - " -  ‰•΅•‰6• •–5‰ */
#define    ctext(x) _ctext[(x)-1]
extern unsigned  _cval[plcl];         /* 5–• ‰•΅•‰6•• •–5‰ */
#define    cval(x) _cval[(x)-1]
extern unsigned  _ptext[objt];        /* •„…‰γε γ€ª. o γγξ.€'ektob*/
#define    ptext(x) _ptext[(x)-1]
extern unsigned  _pstat[sttt];        /* •„…‰γε γ€ª. o γγξ.€'ektob*/
#define    pstat(x) _pstat[(x)-1]
extern char _abrev[loct];             /* ΅–•: „ γ‰5ª…… •γ•…*/
#define    abrev(x) _abrev[(x)-1]


/* --- travels + actions --- */
extern unsigned  _trvkey[loct];       /* ξ€•6 2 –5΅ „ ƒ. */
#define    trvkey(x) _trvkey[(x)-1]
extern unsigned  _actkey[actw];       /*  -"-    -"-  5…‰6•• ha „…γξ•… */
#define    actkey(x) _actkey[(x)-1]


/* --- vocabulary --- */
extern int  nvoc;                     /* 2•γ γ b γ5… */
extern int  _ktab[vocw];              /* 2•γ - ƒ2…•© γ */
#define    ktab(x) _ktab[(x)-1]
extern long _atab[vocw];              /* γ -  4 € ‰ε */
#define    atab(x) _atab[(x)-1]

/* --- state --- */
#define inipro  255           /* 2¬. γγξ©•… „© γ‰5•ª */
#define nulobj  255           /* ξγ ξγξ £ª• €"ekt */
#define caried  1000          /* mapkep - €'ekt …γ ξ */

extern int  rndini;
extern int  loc;                      /* ξ…‰ ª© ƒ•6•© */
extern int  tally;                    /* 2•γ …ª… … €5 ”. γ‰5•ª */
extern int  treasr;                   /* homep 1-– €'ekta-γ‰5•ª */
extern int  rndini;                   /* γ 2.2•γ „ 5ξ•• */
extern unsigned  tevent;              /*  ‰ƒξ…¬ ξ€•6ε γ 2.γ€εξ• */
extern unsigned  eevent;              /* ‰…6 ξ€•6ε γ 2εζ γ€εξ• */
extern unsigned  tiniti;              /* -"-  -"- ••6••ƒ6•• */
extern int  _place[loct];             /* ƒ•6•© €7…‰ξ •• -[•„…‰γ] */
#define    place(x) _place[(x)-1]
extern char _fixed[fixt];             /*    „© ΅•‰γ•5εζ €'ektob */
#define    fixed(x) _fixed[(x)-1]
extern char _prop[objt];              /* γγξ €'ekt */
#define    prop(x) _prop[(x)-1]
