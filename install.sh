#!/usr/bin/env bash

if ! [ -d "./coco" ]; then
	git clone -b release https://github.com/numbbo/coco.git
fi

pip3 install setuptools
pip3 install numpy
pip3 install --user scipy
pip3 install six
pip3 install --user matplotlib

cd coco
python do.py run-c
python do.py install-postprocessing

cd ..
SRC_PATH="./src/"

COCO_BUILD_PATH="coco/code-experiments/build/c"

if ! [ -f "${SRC_PATH}/example_experiment.c" ]; then
	cp ${COCO_BUILD_PATH}/example_experiment.c ${SRC_PATH}
	echo "example_experiment.c copied"
fi
if ! [ -f "${SRC_PATH}/coco.c" ]; then
	cp ${COCO_BUILD_PATH}/coco.c ${SRC_PATH}
	echo "coco.c copied"
fi
if ! [ -f "${SRC_PATH}/coco.h" ]; then
	cp ${COCO_BUILD_PATH}/coco.h ${SRC_PATH}
	echo "coco.h copied"
fi
