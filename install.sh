#!/usr/bin/env bash

# Clone coco
if ! [ -d "./coco" ]; then
	git clone -b release https://github.com/numbbo/coco.git
fi

# Install dependencies
pip3 install setuptools
pip3 install numpy
pip3 install --user scipy
pip3 install six
pip3 install --user matplotlib

# Build coco
cd coco
python do.py run-c
python do.py install-postprocessing
cd ..

SRC_PATH="./src/"
COCO_BUILD_PATH="coco/code-experiments/build/c"

# Copy example if not exists
if ! [ -f "${SRC_PATH}/example_experiment.c" ]; then
	cp ${COCO_BUILD_PATH}/example_experiment.c ${SRC_PATH}
	echo "example_experiment.c copied"
fi

# Remove coco.c and copy newer
if [ -f "${SRC_PATH}/coco.c" ]; then
	rm -f ${SRC_PATH}/coco.c
fi
cp ${COCO_BUILD_PATH}/coco.c ${SRC_PATH}
echo "coco.c copied"

# Remove coco.h and copy newer
if ! [ -f "${SRC_PATH}/coco.h" ]; then
	rm -f ${SRC_PATH}/coco.h
fi
cp ${COCO_BUILD_PATH}/coco.h ${SRC_PATH}
echo "coco.h copied"
