ExtLibs

Purpose
  The ExtLibs folder contains third-party software as follows:

    boost_1_52_0  www.boost.org
    boost_1_57_0  www.boost.org
    pugixml_1_0   www.pugixml.org
    pugixml_1_4   www.pugixml.org
    sqlite_3_7_11 www.sqlite.org (sqlite-amalgamation-3_7_11)
    sqlite_3_8_7  www.sqlite.org (sqlite-amalgamation-3_8_7_1)

Modifications
  2014-11-17 (MM) Added boost_1_57_0, pugixml_1_4, and
                  sqlite_3_8_7.
  2012-11-15 (MM) Moved to boost_1_52_0; got rid of several
                  third-party libraries; updated these notes.
  2010-09-15 (MM) Added notes on boost and sqlite.
  2010-04-26 (MM) Created this readme file.


Boost Notes
  Project and property files reside at the ExtLibs level for
  building boost libraries. By keeping them at this level, a
  new boost release can be loaded into its own folder (e.g.,
  boost_1_57_0) without requiring additions to its contents.

  When a new boost release is acquired, corresponding property
  files must be created (e.g., "2010_boost_1_52_0.props" and
  "2013_boost_1_57_0.props") based on previous versions (e.g.,
  the "_1_52_0" files), with minor tweaks for name and folder
  settings.

  To build with a specific boost release, the boost project
  files (e.g., "2010_boost.vcxproj" and "2013_boost.vcxproj")
  must be changed to refer to that release's property files.
  In addition, the "*_Win32_Debug.*" and "*_Win32_Release.*"
  property sheets ("*.props") in the root source folder MUST
  have their AdditionalIncludeDirectories settings changed to
  match. Do not forget to do this! Also, a few projects can't
  use the property sheet settings and must be edited manually.
  In particular, "make.build" files for some Root3070 apps
  must be updated.

  The property files that reside at the ExtLibs level control
  which boost capabilities are built. Specifically, see the
  BoostBuildOpts user macro.

  Finally, add the new boost folder hierarchy to TFS before
  building it. Otherwise, boost-build binaries will go into
  TFS along with the original sources.


SQLite Notes
  When changing over to a new release, the project files in
  IntLibs/SQLite must be changed to match.

