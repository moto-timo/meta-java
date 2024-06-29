#
# Lists of the ptest in meta-java, sorted into two sets by the time they take
# Please keep these sorted in alphabetical order
#
# A first pass at getting all meta-java recipes which inherit ptest
# meta_java_ptest_recipes=$(bitbake-layers show-recipes --recipes-only --layer meta-java --inherits ptest --bare | tr '\n' ' ' | pcregrep -o1 '^NOTE:.+===(.+)$')
#
# ptests which take less than ~30s each
PTESTS_FAST_META_JAVA = "\
"

PTESTS_SLOW_META_JAVA = "\
"

# ptest enabled recipes which do not pass
PTESTS_PROBLEMS_META_JAVA ="\
"
