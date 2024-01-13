OUTPUT_ARCH (mips)
SECTIONS
{
   __romPos = 0;
   _bootSegmentStart = ADDR(.boot); _bootSegmentRomStart = __romPos; .boot 0x04000000 : AT(__romPos)
   {
      build/asm/rom_header.o(.text);
      build/boot.6102.o(.data);
   }
   _bootSegmentEnd = ALIGN(ADDR(.boot) + SIZEOF(.boot),0x10); _bootSegmentRomEnd = _bootSegmentEnd - _bootSegmentStart + _bootSegmentRomStart; __romPos = _bootSegmentRomEnd; . = _bootSegmentEnd;
   _codeSegmentStart = ADDR(.code); _codeSegmentRomStart = __romPos; .code 0x80000400 : AT(__romPos) SUBALIGN(16)
   {
  build/asm/entry.o(.start);
      build/src/main*.o(.text);
      */libultra_d.a:*.o(.text);
      */libgcc.a:*.o(.text);
      /usr/lib/n64/PR/rspboot.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.fifo.o(.text);
      /usr/lib/n64/PR/aspMain.o(.text);
      build/src/main*.o(.data*);
      */libultra_d.a:*.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.fifo.o(.data*);
      /usr/lib/n64/PR/aspMain.o(.data*);
      build/src/main*.o(.*rodata*);
      */libultra_d.a:*.o(.*rodata*);
      */libgcc.a:*.o(.*rodata*);
   }
   _codeSegmentEnd = ALIGN(ADDR(.code) + SIZEOF(.code),0x10); _codeSegmentRomEnd = _codeSegmentEnd - _codeSegmentStart + _codeSegmentRomStart; __romPos = _codeSegmentRomEnd; . = _codeSegmentEnd;
   _codeSegmentBssStart = ADDR(.code.noload); .code.noload (NOLOAD) :
   {
      build/src/main*.o(.*bss*);
      */libultra_d.a:*.o(COMMON);
      */libultra_d.a:*.o(.scommon);
      */libultra_d.a:*.o(.*bss*);
   }
   _codeSegmentBssEnd = ALIGN(ADDR(.code.noload) + SIZEOF(.code.noload),0x10); _codeSegmentBssSize = _codeSegmentBssEnd - _codeSegmentBssStart; . = _codeSegmentBssEnd;
   _gfxdlistsSegmentBssStart = ADDR(.gfxdlists.noload); .gfxdlists.noload (NOLOAD) :
   {
      build/src/buffers/gfxdlists.o(.bss*);
   }
   _gfxdlistsSegmentBssEnd = ALIGN(ADDR(.gfxdlists.noload) + SIZEOF(.gfxdlists.noload),0x10); _gfxdlistsSegmentBssSize = _gfxdlistsSegmentBssEnd - _gfxdlistsSegmentBssStart; . = _gfxdlistsSegmentBssEnd;
   _staticSegmentStart = ADDR(.static); _staticSegmentRomStart = __romPos; .static 0x01000000 : AT(__romPos)
   {
      build/assets/gfxinit.o(.data);
      build/assets/gfxstatic.o(.data);
   }
   _staticSegmentEnd = ALIGN(ADDR(.static) + SIZEOF(.static),0x10); _staticSegmentRomEnd = _staticSegmentEnd - _staticSegmentStart + _staticSegmentRomStart; __romPos = _staticSegmentRomEnd; . = _staticSegmentEnd;
   _dynamicSegmentStart = ADDR(.dynamic); _dynamicSegmentRomStart = __romPos; .dynamic 0x02000000 : AT(__romPos)
   {
      build/src/buffers/gfxdynamic.o(.data);
   }
   _dynamicSegmentEnd = ALIGN(ADDR(.dynamic) + SIZEOF(.dynamic),0x10); _dynamicSegmentRomEnd = _dynamicSegmentEnd - _dynamicSegmentStart + _dynamicSegmentRomStart; __romPos = _dynamicSegmentRomEnd; . = _dynamicSegmentEnd;
   _sound_dataSegmentStart = ADDR(.sound_data); _sound_dataSegmentRomStart = __romPos; .sound_data __romPos : AT(__romPos)
   {
      build/assets/sound_data.o(.data);
   }
   _sound_dataSegmentEnd = ALIGN(ADDR(.sound_data) + SIZEOF(.sound_data),0x10); _sound_dataSegmentRomEnd = _sound_dataSegmentEnd - _sound_dataSegmentStart + _sound_dataSegmentRomStart; __romPos = _sound_dataSegmentRomEnd; . = _sound_dataSegmentEnd;
   . = 0x80200000;
   _zbufferSegmentBssStart = ADDR(.zbuffer.noload); .zbuffer.noload (NOLOAD) :
   {
      build/src/buffers/gfxzbuffer.o(.bss*);
   }
   _zbufferSegmentBssEnd = ALIGN(ADDR(.zbuffer.noload) + SIZEOF(.zbuffer.noload),0x10); _zbufferSegmentBssSize = _zbufferSegmentBssEnd - _zbufferSegmentBssStart; . = _zbufferSegmentBssEnd;
   . = 0x80600000;
   _cfbSegmentBssStart = ADDR(.cfb.noload); .cfb.noload (NOLOAD) :
   {
      build/src/buffers/gfxcfb.o(.bss*);
   }
   _cfbSegmentBssEnd = ALIGN(ADDR(.cfb.noload) + SIZEOF(.cfb.noload),0x10); _cfbSegmentBssSize = _cfbSegmentBssEnd - _cfbSegmentBssStart; . = _cfbSegmentBssEnd;
   /DISCARD/ :
   {
      *(*);
   }
}
