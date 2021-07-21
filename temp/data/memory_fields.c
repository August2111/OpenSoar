/**
* AUTHOR: August 2111
* Diese Datei wurde als Überbrückung zur Einbindung der Daten-Dateien erzeugt,
* die eigentlich mit PERL ins System übertragen werden
* Die ganzen hier erzeugten Felder sind quasi 'hohle Vögel
* **********************************************************************/

#include <stdint.h>
#include <stddef.h>


//-----------------------------------------
#define MEMORY_FIELD(field, field_size)     \
  const size_t field ## _size = field_size; \
uint8_t mem_ ## field[field_size] = {0};    \
const uint8_t * field  = mem_ ## field;
//-----------------------------------------

#define XCSOR_INFO_SIZE 0x1000
#define LANGUAGE_SIZE 0x1000

// MEMORY_FIELD(de_mo,LANGUAGE_SIZE) - de_mo ist echt erzeugt!


MEMORY_FIELD(NEWS_txt_gz, XCSOR_INFO_SIZE)
MEMORY_FIELD(AUTHORS_gz, XCSOR_INFO_SIZE)
MEMORY_FIELD(COPYING_gz, XCSOR_INFO_SIZE)
MEMORY_FIELD(ca_mo,LANGUAGE_SIZE)
MEMORY_FIELD(cs_mo,LANGUAGE_SIZE)
MEMORY_FIELD(da_mo,LANGUAGE_SIZE)

MEMORY_FIELD(egm96s_dem,LANGUAGE_SIZE)
MEMORY_FIELD(el_mo,LANGUAGE_SIZE)
MEMORY_FIELD(es_mo,LANGUAGE_SIZE)
MEMORY_FIELD(fi_mo,LANGUAGE_SIZE)
MEMORY_FIELD(fr_mo,LANGUAGE_SIZE)
MEMORY_FIELD(he_mo,LANGUAGE_SIZE)
MEMORY_FIELD(hr_mo,LANGUAGE_SIZE)
MEMORY_FIELD(hu_mo,LANGUAGE_SIZE)
MEMORY_FIELD(it_mo,LANGUAGE_SIZE)
MEMORY_FIELD(ja_mo,LANGUAGE_SIZE)
MEMORY_FIELD(ko_mo,LANGUAGE_SIZE)
MEMORY_FIELD(lt_mo,LANGUAGE_SIZE)
MEMORY_FIELD(nb_mo,LANGUAGE_SIZE)
MEMORY_FIELD(nl_mo,LANGUAGE_SIZE)
MEMORY_FIELD(pl_mo,LANGUAGE_SIZE)
MEMORY_FIELD(pt_mo,LANGUAGE_SIZE)
MEMORY_FIELD(pt_BR_mo,LANGUAGE_SIZE)
MEMORY_FIELD(ro_mo,LANGUAGE_SIZE)
MEMORY_FIELD(ru_mo,LANGUAGE_SIZE)
MEMORY_FIELD(sk_mo,LANGUAGE_SIZE)
MEMORY_FIELD(sl_mo,LANGUAGE_SIZE)
MEMORY_FIELD(sr_mo,LANGUAGE_SIZE)
MEMORY_FIELD(sv_mo,LANGUAGE_SIZE)
MEMORY_FIELD(tr_mo,LANGUAGE_SIZE)
MEMORY_FIELD(uk_mo,LANGUAGE_SIZE)
MEMORY_FIELD(vi_mo,LANGUAGE_SIZE)
MEMORY_FIELD(zh_CN_mo,LANGUAGE_SIZE)
MEMORY_FIELD(zh_Hant_mo,LANGUAGE_SIZE)

