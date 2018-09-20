#!/usr/bin/env bash

if ! [ -d "./coco" ]; then
	git clone -b release https://github.com/numbbo/coco.git
fi

pip3 install setuptools
pip3 install numpy
pip3 install scipy
pip3 install six
pip3 install matplotlib

cd coco
python do.py run-c
python do.py install-postprocessing

cd ..
PROJECT_PATH="."
if ! [ -d ${PROJECT_PATH} ]; then
	mkdir ${PROJECT_PATH}
fi

COCO_BUILD_PATH="coco/code-experiments/build/c"

if ! [ -f "${PROJECT_PATH}/example_experiment.c" ]; then
	cp ${COCO_BUILD_PATH}/example_experiment.c ${PROJECT_PATH}
	echo "example_experiment.c copied"
fi
if ! [ -f "${PROJECT_PATH}/coco.c" ]; then
	cp ${COCO_BUILD_PATH}/coco.c ${PROJECT_PATH}
	echo "coco.c copied"
fi
if ! [ -f "${PROJECT_PATH}/coco.h" ]; then
	cp ${COCO_BUILD_PATH}/coco.h ${PROJECT_PATH}
	echo "coco.h copied"
fi
if ! [ -f "${PROJECT_PATH}/Makefile" ]; then
	cp ${COCO_BUILD_PATH}/Makefile ${PROJECT_PATH}
	echo "Makefile copied"
fi
