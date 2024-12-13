#define SHT_NULL            0        /* Section header table entry unused */
#define SHT_PROGBITS        1        /* Program data */
#define SHT_SYMTAB          2        /* Symbol table */
#define SHT_STRTAB          3        /* String table */
#define SHT_RELA            4        /* Relocation entries with addends */
#define SHT_HASH            5        /* Symbol hash table */
#define SHT_DYNAMIC         6        /* Dynamic linking information */
#define SHT_NOTE            7        /* Notes */
#define SHT_NOBITS          8        /* Program space with no data (bss) */
#define SHT_REL             9        /* Relocation entries, no addends */
#define SHT_SHLIB          10        /* Reserved */
#define SHT_DYNSYM         11        /* Dynamic linker symbol table */
#define SHT_INIT_ARRAY     14        /* Array of constructors */
#define SHT_FINI_ARRAY     15        /* Array of destructors */
#define SHT_PREINIT_ARRAY  16        /* Array of pre-constructors */
#define SHT_GROUP          17        /* Section group */
#define SHT_SYMTAB_SHNDX   18        /* Extended section indeces */
#define    SHT_NUM         19        /* Number of defined types.  */
#define SHT_LOOS           0x60000000    /* Start OS-specific.  */
#define SHT_GNU_ATTRIBUTES 0x6ffffff5    /* Object attributes.  */
#define SHT_GNU_HASH       0x6ffffff6    /* GNU-style hash table.  */
#define SHT_GNU_LIBLIST    0x6ffffff7    /* Prelink library list */
#define SHT_CHECKSUM       0x6ffffff8    /* Checksum for DSO content.  */
#define SHT_LOSUNW         0x6ffffffa    /* Sun-specific low bound.  */
#define SHT_SUNW_move      0x6ffffffa
#define SHT_SUNW_COMDAT    0x6ffffffb
#define SHT_SUNW_syminfo   0x6ffffffc
#define SHT_GNU_verdef     0x6ffffffd    /* Version definition section.  */
#define SHT_GNU_verneed    0x6ffffffe    /* Version needs section.  */
#define SHT_GNU_versym     0x6fffffff    /* Version symbol table.  */
#define SHT_HISUNW         0x6fffffff    /* Sun-specific high bound.  */
#define SHT_HIOS           0x6fffffff    /* End OS-specific type */
#define SHT_LOPROC         0x70000000    /* Start of processor-specific */
#define SHT_HIPROC         0x7fffffff    /* End of processor-specific */
#define SHT_LOUSER         0x80000000    /* Start of application-specific */
#define SHT_HIUSER         0x8fffffff    /* End of application-specific */