#################################################
#
#  (C) 2010-2011 Calvin Morrison
#  mutantturkey@gmail.com
#
#  Improvements and feedback are welcome
#
#  This file is released under GPL >= 2
#
#################################################

message( AUTHOR_WARNING
		" \n"
		" ---------------------------------------------------------- \n"
		" ***  NOTE  *** \n"
		" Building kmrml is deprecated and should not be done. \n"
		" kmrml will be dropped starting from R14.2.0. \n"
		" Please contact the TDE development team on ML \n"
		" if you wish kmrml to be continued. \n"
		" ---------------------------------------------------------- \n")

include( ConfigureChecks.cmake )

add_subdirectory ( kmrml )
