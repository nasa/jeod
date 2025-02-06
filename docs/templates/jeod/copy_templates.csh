# USAGE:   source copy_templates <MODEL_NAME> <MODEL_DOCS_PATH>

# PURPOSE:  copies all necessary document template files into <MODEL_DOCS_PATH>;
#          changes names of files from ModelName*  to <MODEL_NAME>*
#          replaces all occurrences of ModelName within files with <MODEL_NAME>

# REQUIREMENTS:  MODEL_DOCS_PATH can be absolute or relative, 
#                                and should end in  /tex
#               MODEL_NAME should contain letters only, no underscores.

# KLUDGES:  Fancy ls aliases may cause problems in origlist.temporary.  Solution
#           is to store the ls alias, unalias ls, run the script, then realias
#           ls.  This may not work for all users.  It could disable ls entirely.

# using sed -e {} rather than sed '' to allow use of input arguments

cp ModelName* $2
cp makefile $2
pushd $2

#RENAME and edit the files

#   list all of the Model files
which ls | awk '{print "xxxxx"$4,$5"yyyyy  ; echo $1"}' | sed -e {s/xxxxx/alias\ ls\ \'/} | sed -e {s/yyyyy/\'/} > sourcefile.temporary
unalias ls

ls ModelName* > origlist.temporary
#   make new names
sed -e {s/ModelName/$1/} origlist.temporary > newlist.temporary
#   combine names into one file
paste origlist.temporary newlist.temporary > comblist.temporary

#  add instructions
awk '{print "sed -e {s/ModelName/$1/g}",$1,">",$2}' comblist.temporary >>sourcefile.temporary

#source the command file, editing all ModelName* files
source sourcefile.temporary $1

#repeat for the makefile
sed -e {s/ModelName/$1/g} makefile > makefile.temporary
mv makefile.temporary makefile



#clean up and return
rm ModelName*
rm *.temporary
ls
popd
