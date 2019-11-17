#!/bin/bash
#========================================================================
# 	@file:   configpath.sh
#   @brief:  在进行编译前，执行Makefile前请先执行这个脚本。将会把当前目录写到Makefile.depend中
#   @author: goof
#========================================================================

root_dir=`pwd`
echo ${root_dir}
sed "s;DIR_OF_ROOT_WILL_REPLACE_BY_SCRIPT;`pwd`;g" ./makerule/makefile.depend.template > ./makerule/makefile.depend
echo "DONE!"

