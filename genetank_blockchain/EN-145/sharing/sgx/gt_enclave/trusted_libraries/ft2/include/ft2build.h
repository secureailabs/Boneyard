#ifndef FT2BUILD_H_
#define FT2BUILD_H_


#ifndef FTHEADER_H_
#define FTHEADER_H_

#ifdef __cplusplus
#define FT_BEGIN_HEADER  extern "C" {
#else
#define FT_BEGIN_HEADER  /* nothing */
#endif

#ifdef __cplusplus
#define FT_END_HEADER  }
#else
#define FT_END_HEADER  /* nothing */
#endif

#ifndef FT_CONFIG_CONFIG_H
#define FT_CONFIG_CONFIG_H  <freetype/config/ftconfig.h>
#endif

#ifndef FT_CONFIG_STANDARD_LIBRARY_H
#define FT_CONFIG_STANDARD_LIBRARY_H  <freetype/config/ftstdlib.h>
#endif

#ifndef FT_CONFIG_OPTIONS_H
#define FT_CONFIG_OPTIONS_H  <freetype/config/ftoption.h>
#endif

#ifndef FT_CONFIG_MODULES_H
#define FT_CONFIG_MODULES_H  <freetype/config/ftmodule.h>
#endif

#define FT_FREETYPE_H  <freetype/freetype.h>
#define FT_ERRORS_H  <freetype/fterrors.h>
#define FT_MODULE_ERRORS_H  <freetype/ftmoderr.h>
#define FT_SYSTEM_H  <freetype/ftsystem.h>
#define FT_IMAGE_H  <freetype/ftimage.h>
#define FT_TYPES_H  <freetype/fttypes.h>
#define FT_LIST_H  <freetype/ftlist.h>
#define FT_OUTLINE_H  <freetype/ftoutln.h>
#define FT_SIZES_H  <freetype/ftsizes.h>
#define FT_MODULE_H  <freetype/ftmodapi.h>
#define FT_RENDER_H  <freetype/ftrender.h>
#define FT_DRIVER_H  <freetype/ftdriver.h>
#define FT_AUTOHINTER_H  FT_DRIVER_H
#define FT_CFF_DRIVER_H  FT_DRIVER_H
#define FT_TRUETYPE_DRIVER_H  FT_DRIVER_H
#define FT_PCF_DRIVER_H  FT_DRIVER_H
#define FT_TYPE1_TABLES_H  <freetype/t1tables.h>
#define FT_TRUETYPE_IDS_H  <freetype/ttnameid.h>
#define FT_TRUETYPE_TABLES_H  <freetype/tttables.h>
#define FT_TRUETYPE_TAGS_H  <freetype/tttags.h>
#define FT_BDF_H  <freetype/ftbdf.h>
#define FT_CID_H  <freetype/ftcid.h>
#define FT_GZIP_H  <freetype/ftgzip.h>
#define FT_LZW_H  <freetype/ftlzw.h>
#define FT_BZIP2_H  <freetype/ftbzip2.h>
#define FT_WINFONTS_H   <freetype/ftwinfnt.h>
#define FT_GLYPH_H  <freetype/ftglyph.h>
#define FT_BITMAP_H  <freetype/ftbitmap.h>
#define FT_BBOX_H  <freetype/ftbbox.h>
#define FT_CACHE_H  <freetype/ftcache.h>
#define FT_MAC_H  <freetype/ftmac.h>
#define FT_MULTIPLE_MASTERS_H  <freetype/ftmm.h>
#define FT_SFNT_NAMES_H  <freetype/ftsnames.h>
#define FT_OPENTYPE_VALIDATE_H  <freetype/ftotval.h>
#define FT_GX_VALIDATE_H  <freetype/ftgxval.h>
#define FT_PFR_H  <freetype/ftpfr.h>
#define FT_STROKER_H  <freetype/ftstroke.h>
#define FT_SYNTHESIS_H  <freetype/ftsynth.h>
#define FT_FONT_FORMATS_H  <freetype/ftfntfmt.h>

  /* deprecated */
#define FT_XFREE86_H  FT_FONT_FORMATS_H
#define FT_TRIGONOMETRY_H  <freetype/fttrigon.h>
#define FT_LCD_FILTER_H  <freetype/ftlcdfil.h>
#define FT_INCREMENTAL_H  <freetype/ftincrem.h>
#define FT_GASP_H  <freetype/ftgasp.h>
#define FT_ADVANCES_H  <freetype/ftadvanc.h>
#define FT_COLOR_H  <freetype/ftcolor.h>


  /* */

  /* These header files don't need to be included by the user. */
#define FT_ERROR_DEFINITIONS_H  <freetype/fterrdef.h>
#define FT_PARAMETER_TAGS_H     <freetype/ftparams.h>

  /* Deprecated macros. */
#define FT_UNPATENTED_HINTING_H   <freetype/ftparams.h>
#define FT_TRUETYPE_UNPATENTED_H  <freetype/ftparams.h>

  /* `FT_CACHE_H` is the only header file needed for the cache subsystem. */
#define FT_CACHE_IMAGE_H          FT_CACHE_H
#define FT_CACHE_SMALL_BITMAPS_H  FT_CACHE_H
#define FT_CACHE_CHARMAP_H        FT_CACHE_H

  /* The internals of the cache sub-system are no longer exposed.  We */
  /* default to `FT_CACHE_H` at the moment just in case, but we know  */
  /* of no rogue client that uses them.                               */
  /*                                                                  */
#define FT_CACHE_MANAGER_H           FT_CACHE_H
#define FT_CACHE_INTERNAL_MRU_H      FT_CACHE_H
#define FT_CACHE_INTERNAL_MANAGER_H  FT_CACHE_H
#define FT_CACHE_INTERNAL_CACHE_H    FT_CACHE_H
#define FT_CACHE_INTERNAL_GLYPH_H    FT_CACHE_H
#define FT_CACHE_INTERNAL_IMAGE_H    FT_CACHE_H
#define FT_CACHE_INTERNAL_SBITS_H    FT_CACHE_H


  /*
   * Include internal headers definitions from `<internal/...>` only when
   * building the library.
   */
//#ifdef FT2_BUILD_LIBRARY
//#define  FT_INTERNAL_INTERNAL_H  <freetype/internal/internal.h>
//#include FT_INTERNAL_INTERNAL_H
#define FT_INTERNAL_OBJECTS_H             <freetype/internal/ftobjs.h>
#define FT_INTERNAL_STREAM_H              <freetype/internal/ftstream.h>
#define FT_INTERNAL_MEMORY_H              <freetype/internal/ftmemory.h>
#define FT_INTERNAL_DEBUG_H               <freetype/internal/ftdebug.h>
#define FT_INTERNAL_CALC_H                <freetype/internal/ftcalc.h>
#define FT_INTERNAL_HASH_H                <freetype/internal/fthash.h>
#define FT_INTERNAL_DRIVER_H              <freetype/internal/ftdrv.h>
#define FT_INTERNAL_TRACE_H               <freetype/internal/fttrace.h>
#define FT_INTERNAL_GLYPH_LOADER_H        <freetype/internal/ftgloadr.h>
#define FT_INTERNAL_SFNT_H                <freetype/internal/sfnt.h>
#define FT_INTERNAL_SERVICE_H             <freetype/internal/ftserv.h>
#define FT_INTERNAL_RFORK_H               <freetype/internal/ftrfork.h>
#define FT_INTERNAL_VALIDATE_H            <freetype/internal/ftvalid.h>

#define FT_INTERNAL_TRUETYPE_TYPES_H      <freetype/internal/tttypes.h>
#define FT_INTERNAL_TYPE1_TYPES_H         <freetype/internal/t1types.h>

#define FT_INTERNAL_POSTSCRIPT_AUX_H      <freetype/internal/psaux.h>
#define FT_INTERNAL_POSTSCRIPT_HINTS_H    <freetype/internal/pshints.h>
#define FT_INTERNAL_POSTSCRIPT_PROPS_H    <freetype/internal/ftpsprop.h>

#define FT_INTERNAL_AUTOHINT_H            <freetype/internal/autohint.h>

#define FT_INTERNAL_CFF_TYPES_H           <freetype/internal/cfftypes.h>
#define FT_INTERNAL_CFF_OBJECTS_TYPES_H   <freetype/internal/cffotypes.h>
//#endif /* FT2_BUILD_LIBRARY */


#endif /* FTHEADER_H_ */


/* END */

#endif /* FT2BUILD_H_ */


/* END */
