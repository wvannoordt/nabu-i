# sudo apt-get install libsfml-dev

mkfile_path := $(abspath $(lastword ${MAKEFILE_LIST}))
mkfile_dir := $(dir $(mkfile_path))
bin_dir := ${mkfile_dir}/bin
front_end := nbi
launcher := nbilaunch.x


flags := 
flags += -g
flags += -std=c++20

sanny := 0
ifeq (${sanny},1)
flags += -fsanitize=undefined,address -fstack-protector-all
endif
main: setup
	g++ ${flags} -g -I./src -I${PTL}/include -I${NABU}/src main.cc -o ${bin_dir}/${launcher} -lsfml-graphics -lsfml-window -lsfml-system -L ${PTL}/lib -lPTL

setup:
	mkdir -p ${bin_dir}
	rm -f ${bin_dir}/${front_end}
	echo "${bin_dir}/${launcher} ${mkfile_dir} \$$1" >> ${bin_dir}/${front_end}
	chmod +x ${bin_dir}/${front_end}

clean:
	rm -rf ${bin_dir}

run: main
	${bin_dir}/${front_end}
