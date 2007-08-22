#ifndef __compiler_H
#define __compiler_H
/*
 * Copyright (c) 1999-2004 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */
#ifdef HAVE_CVS_IDENT
#ident "$Id: compiler.h,v 1.33 2007/04/19 02:52:53 steve Exp $"
#endif

# include  <list>
# include  <map>
# include  "netlist.h"
# include  "StringHeap.h"

/*
 * This defines constants and defaults for the compiler in general.
 */


/* The integer_width is the width of integer variables. */
extern unsigned integer_width;

/* The TIME_WIDTH is the width of time variables. */
#ifndef TIME_WIDTH
# define TIME_WIDTH 64
#endif

/*
 * When doing dynamic linking, we need a uniform way to identify the
 * symbol. Some compilers put leading _, some trailing _. The
 * configure script figures out which is the local convention and
 * defines NEED_LU and NEED_TU as required.
 */
#ifdef NEED_LU
#define LU "_"
#else
#define LU ""
#endif

#ifdef NEED_TU
#define TU "_"
#else
#define TU ""
#endif


/*
 * These are flags to enable various sorts of warnings. By default all
 * the warnings are off, the -W<list> parameter arranges for each to be
 * enabled.
 */

/* Implicit definitions of wires. */
extern bool warn_implicit;
extern bool error_implicit;

/* inherit timescales across files. */
extern bool warn_timescale;

/* Warn about legal but questionable module port bindings. */
extern bool warn_portbinding;

/* This is true if verbose output is requested. */
extern bool verbose_flag;

extern bool debug_scopes;
extern bool debug_eval_tree;
extern bool debug_elaborate;
extern bool debug_synth2;

/* Path to a directory useful for finding subcomponents. */
extern const char*basedir;

/* This is an ordered list of library suffixes to search. */
extern list<const char*>library_suff;
extern int build_library_index(const char*path, bool key_case_sensitive);

/* This is the generation of Verilog that the compiler is asked to
   support. Then there are also more detailed controls for more
   specific language features. */
enum generation_t {
      GN_VER1995  = 1,
      GN_VER2001  = 2,
      GN_VER2001X = 3,
      GN_DEFAULT  = 3
};

extern generation_t generation_flag;

extern bool gn_cadence_types_flag;

/* These functions test that specific features are enabled. */
inline bool gn_cadence_types_enabled()
{ return gn_cadence_types_flag && generation_flag==GN_VER2001X; }

/* If this flag is true, then elaborate specify blocks. If this flag
   is false, then skip elaboration of specify behavior. */
extern bool gn_specify_blocks_flag;

/* If this flag is false a warning is printed when the port declaration
   is scalar and the net/register definition is vectored. */
extern bool gn_io_range_error_flag;

  /* This is the string to use to invoke the preprocessor. */
extern char*ivlpp_string;

extern map<perm_string,unsigned> missing_modules;

  /* Files that are library files are in this map. The lexor compares
     file names as it processes `line directives, and if the file name
     matches an entry in this table, it will turn on the
     library_active_flag so that modules know that they are in a
     library. */
extern map<string,bool> library_file_map;

/*
 * the lex_strings are perm_strings made up of tokens from the source
 * file. Identifiers are so likely to be used many times that it makes
 * much sense to use a StringHeapLex to hold them.
 */
extern StringHeapLex lex_strings;
extern StringHeap misc_strings;

/*
 * system task/function listings.
 */
/*
 * This table describes all the return values of various system
 * functions. This table is used to elaborate expressions that are
 * system function calls.
 */
struct sfunc_return_type {
      const char*   name;
      ivl_variable_type_t type;
      unsigned      wid;
      int           signed_flag;
};

extern const struct sfunc_return_type* lookup_sys_func(const char*name);
extern int load_sys_func_table(const char*path);

/*
 * $Log: compiler.h,v $
 * Revision 1.33  2007/04/19 02:52:53  steve
 *  Add support for -v flag in command file.
 *
 * Revision 1.32  2007/03/07 04:24:59  steve
 *  Make integer width controllable.
 *
 * Revision 1.31  2006/09/28 04:35:18  steve
 *  Support selective control of specify and xtypes features.
 *
 * Revision 1.30  2005/07/11 16:56:50  steve
 *  Remove NetVariable and ivl_variable_t structures.
 *
 * Revision 1.29  2005/07/07 16:22:49  steve
 *  Generalize signals to carry types.
 *
 * Revision 1.28  2005/06/28 04:25:55  steve
 *  Remove reference to SystemVerilog.
 *
 * Revision 1.27  2005/04/24 23:44:01  steve
 *  Update DFF support to new data flow.
 *
 * Revision 1.26  2004/10/04 01:10:52  steve
 *  Clean up spurious trailing white space.
 *
 * Revision 1.25  2004/09/25 01:58:44  steve
 *  Add a debug_elaborate flag
 *
 * Revision 1.24  2004/09/10 23:51:42  steve
 *  Fix the evaluation of constant ternary expressions.
 *
 * Revision 1.23  2004/09/05 17:44:41  steve
 *  Add support for module instance arrays.
 *
 * Revision 1.22  2004/03/10 04:51:24  steve
 *  Add support for system function table files.
 *
 * Revision 1.21  2004/03/09 04:29:42  steve
 *  Separate out the lookup_sys_func table, for eventual
 *  support for function type tables.
 *
 *  Remove ipal compile flags.
 *
 * Revision 1.20  2004/02/18 17:11:54  steve
 *  Use perm_strings for named langiage items.
 *
 * Revision 1.19  2003/11/13 05:55:33  steve
 *  Move the DLL= flag to target config files.
 *
 * Revision 1.18  2003/11/08 20:06:21  steve
 *  Spelling fixes in comments.
 *
 * Revision 1.17  2003/09/25 00:25:14  steve
 *  Summary list of missing modules.
 *
 * Revision 1.16  2003/03/01 06:25:30  steve
 *  Add the lex_strings string handler, and put
 *  scope names and system task/function names
 *  into this table. Also, permallocate event
 *  names from the beginning.
 */
#endif
